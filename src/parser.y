%{
#define YYDEBUG 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declaração de funções do Flex
extern int yylex();
extern int yylineno;
extern FILE *yyin; // Entrada do Flex

// Variáveis globais para a geração do código MASPY
FILE *maspy_output_file;
int agent_class_counter = 0; // Para gerar Agente1, Agente2, etc.
// Armazenar nomes dos agentes para gerar o rodapé dinamicamente
char *agent_names[100]; // Suporta até 100 agentes
int num_agents = 0;

// ====================================================================
// VARIÁVEIS E FUNÇÕES PARA GERENCIAMENTO GLOBAL DE MEMÓRIA (Corrigido)
// ====================================================================
#define MAX_ALLOCATED_STRINGS 2000
char *global_allocated_strings[MAX_ALLOCATED_STRINGS];
int global_num_allocated_strings = 0;

void register_global_allocated_string(char *str) {
    if (str && global_num_allocated_strings < MAX_ALLOCATED_STRINGS) {
        global_allocated_strings[global_num_allocated_strings++] = str;
    } else if (str) {
        fprintf(stderr, "Aviso: Limite de strings alocadas globalmente atingido. Memória para '%s' pode vazar.\n", str);
    }
}

void cleanup_global_allocated_strings() {
    for (int i = 0; i < global_num_allocated_strings; i++) {
        if (global_allocated_strings[i] != NULL) {
            free(global_allocated_strings[i]);
            global_allocated_strings[i] = NULL;
        }
    }
    global_num_allocated_strings = 0;
}
// ====================================================================

// Declaração de funções de erro do Bison
void yyerror(const char *s);

// Funções auxiliares para gerar código MASPY
void generate_maspy_header();
void generate_agent_class_start(const char *agent_json_name);
void generate_agent_class_end();
void generate_belief_maspy(const char *belief_name);
void generate_goal_maspy(const char *goal_name);
void generate_plan_start_maspy(const char *trigger_name, const char *ctx_name, const char *func_name_in_maspy, int is_empty_body, const char *plan_body_str);
void generate_maspy_footer();

// Funções para manipulação de strings (Corrigido)
char* clean_for_python_id(const char* str) {
    if (!str) return NULL;
    char* cleaned = strdup(str); // ALOCA MEMÓRIA
    if (!cleaned) return NULL;

    register_global_allocated_string(cleaned); // REGISTRA ESTA ALOCAÇÃO

    for (int i = 0; cleaned[i] != '\0'; i++) {
        if (cleaned[i] == ' ') cleaned[i] = '_';
    }
    return cleaned;
}

%}

%union {
    char *str_val;
    // int is_empty_body_flag; // REMOVER: Não precisamos mais de uma flag separada aqui
    char *plan_body_content; // Mantém este para body_item_list e body_item
    struct {
        char *content; // O conteúdo acumulado do corpo
        int is_empty;  // 1 se o corpo está vazio, 0 se não está
    } body_result_t; // NOVO: Estrutura para o resultado do corpo
}

// Declaração de tokens (terminais)
%token OBRACE CBRACE OBRACKET CBRACKET COLON COMMA
%token AGENT_CODE BELIEFS GOAL PLANS TRIGGER CTX BODY
%token <str_val> STRING_LITERAL

// Declaração de tipos para símbolos não-terminais que retornam valores (Corrigido)
%type <str_val> agent_name trigger_val ctx_val plan_json_name
%type <plan_body_content> body_item_list body_item // Esses continuam retornando a string de conteúdo
%type <body_result_t> body_elements // ATUALIZADO: body_elements agora retorna a estrutura
%start program

%%

// Regra inicial da gramática: o programa JSON completo
program:
    OBRACE AGENT_CODE COLON OBRACE agent_definitions CBRACE CBRACE
    {
        // A liberação final de memória será feita por cleanup_global_allocated_strings()
        // generate_maspy_footer() é chamado antes do cleanup_global_allocated_strings
        // para que os nomes dos agentes ainda estejam disponíveis.
    }
;

// Lista de definições de agentes (pode ter um ou vários agentes separados por vírgula)
agent_definitions:
    agent_definition
|   agent_definition COMMA agent_definitions
;

// Definição de um único agente
agent_definition:
    agent_name COLON agent_details
    {
        // REMOVIDO: free($<str_val>1);
        // Gerenciamento de memória agora é feito globalmente.
    }
;

// Captura o nome do agente e inicia a classe MASPY correspondente (Corrigido)
agent_name:
    STRING_LITERAL
    {
        $$ = $1; // Passa a string literal para o símbolo. O gerenciador global a liberará.
        generate_agent_class_start($1);
        if (num_agents < 100) {
            char *agent_name_copy = strdup($1); // Cria uma CÓPIA para a lista agent_names
            register_global_allocated_string(agent_name_copy); // REGISTRA A CÓPIA
            agent_names[num_agents] = agent_name_copy;
            num_agents++;
        } else {
            fprintf(stderr, "Aviso: Limite de agentes no rodapé atingido (100). Não será possível adicionar %s.\n", $1);
        }
    }
;

// Detalhes de um agente (beliefs, goal, plans)
agent_details:
    OBRACE
        BELIEFS COLON belief_list COMMA
        GOAL COLON goal_definition COMMA
        PLANS COLON plan_object
    CBRACE
    {
        generate_agent_class_end(); // Termina a definição da classe do agente
    }
;

// Lista de crenças (array JSON)
belief_list:
    OBRACKET string_list CBRACKET
;

// Definição do objetivo (Corrigido)
goal_definition:
    STRING_LITERAL { generate_goal_maspy($1); /* REMOVIDO: free($1); */ }
;

// Lista genérica de strings (usada para beliefs, por exemplo) (Corrigido)
string_list:
    /* empty */ // Permite lista vazia
|   STRING_LITERAL { generate_belief_maspy($1); /* REMOVIDO: free($1); */ }
|   STRING_LITERAL COMMA string_list { generate_belief_maspy($1); /* REMOVIDO: free($1); */ }
;

// Objeto que contém as definições dos planos
plan_object:
    OBRACE plan_definitions CBRACE
;

// Lista de definições de planos (pode ter um ou vários planos separados por vírgula)
plan_definitions:
    plan_definition
|   plan_definition COMMA plan_definitions
;

// Definição de um único plano (Corrigido)
plan_definition:
    plan_json_name COLON plan_details
    {
        /* REMOVIDO: free($<str_val>1); */
    }
;

// Captura o nome do plano (string literal)
plan_json_name:
    STRING_LITERAL { $$ = $1; } // Gerenciador global liberará $1
;

// Detalhes de um plano (trigger, ctx, body) (ATUALIZADO)
plan_details:
    OBRACE
        TRIGGER COLON trigger_val COMMA
        CTX COLON ctx_val COMMA
        BODY COLON body_elements // ATUALIZADO: Apenas body_elements
    CBRACE
    {
        // $12 agora é o resultado da regra body_elements (a estrutura body_result_t)
        generate_plan_start_maspy(
            $<str_val>4,                        // CORRIGIDO: trigger_val é o 4º símbolo da regra
            $<str_val>8,                        // CORRIGIDO: ctx_val é o 8º símbolo da regra
            $<str_val>4,                        // func_name_in_maspy (nome do trigger) é o 4º símbolo
            $<body_result_t>12.is_empty,       // Flag se o corpo está vazio
            $<body_result_t>12.content         // A string com o conteúdo do corpo
        );
    }
;

// Captura o valor do trigger
trigger_val:
    STRING_LITERAL { $$ = $1; } // Gerenciador global liberará $1
;

// Captura o valor do contexto
ctx_val:
    STRING_LITERAL { $$ = $1; } // Gerenciador global liberará $1
;

// Elementos do corpo de um plano (array JSON) (ATUALIZADO e UNIFICADO)
body_elements:
    OBRACKET CBRACKET // Caso de corpo vazio: []
    {
        $$.content = strdup(""); // Conteúdo é uma string vazia
        register_global_allocated_string($$.content);
        $$.is_empty = 1; // Flag indica que é vazio
    }
|   OBRACKET body_item_list CBRACKET // Caso de corpo não-vazio: [item, item, ...]
    {
        $$.content = $2; // $2 é o conteúdo acumulado de body_item_list
        $$.is_empty = 0; // Flag indica que NÃO é vazio
    }
;

// Lista de itens do corpo (AGORA NÃO PODE SER VAZIA)
body_item_list:
    // REMOVER: /* empty */ {
    // REMOVER:     $$ = strdup(""); // Retorna uma string vazia para corpo vazio
    // REMOVER:     register_global_allocated_string($$);
    // REMOVER: }
    body_item {
        $$ = $1; // Passa a string do único item
    }
|   body_item COMMA body_item_list {
        // Concatena a string do item atual com o restante da lista
        char *combined_str = (char *)malloc(strlen($1) + strlen($3) + 1);
        if (combined_str) {
            strcpy(combined_str, $1);
            strcat(combined_str, $3);
            register_global_allocated_string(combined_str);
        }
        $$ = combined_str;
    }
;

body_item:
    STRING_LITERAL {
        char buffer[512]; // Buffer temporário para formatar a string
        int len;
        char *generated_line = NULL;

        if (strncmp($1, "A_", 2) == 0) {
            char *action_name = $1 + 2; // Extrai o nome da ação (removendo "A_")
            // AGORA GERA UMA LINHA DE PRINT ANTES DA EXECUÇÃO DA AÇÃO
            len = snprintf(buffer, sizeof(buffer),"        self.print(\"Executando Ação: %s\")\n", //"        self.do(Action(\"%s\"))\n",
                           action_name); // action_name é usado para os dois %s.

        } else if (strncmp($1, "G_", 2) == 0) {
            // Pode adicionar um print similar para Goal se quiser ver quando um Goal é adicionado
            len = snprintf(buffer, sizeof(buffer), "        self.add(Goal(\"%s\"))\n", $1 + 2);
        } else if (strncmp($1, "B_", 2) == 0) {
            // Pode adicionar um print similar para Belief se quiser ver quando uma Belief é adicionada
            len = snprintf(buffer, sizeof(buffer), "        self.add(Belief(\"%s\"))\n", $1 + 2);
        } else {
            fprintf(stderr, "Erro na linha %d: Elemento de body inesperado: '%s'\n", yylineno, $1);
            len = snprintf(buffer, sizeof(buffer), "        # Erro: Elemento de body inesperado: \"%s\"\n", $1);
        }

        generated_line = strdup(buffer); // Aloca memória para a linha combinada
        register_global_allocated_string(generated_line); // Registra para limpeza
        $$ = generated_line; // Passa a string gerada para o símbolo
    }
;

%%

// Implementações das funções auxiliares para geração de código MASPY

void generate_maspy_header() {
    fprintf(maspy_output_file, "#\n");
    fprintf(maspy_output_file, "# Trabalho de Compiladores\n");
    fprintf(maspy_output_file, "# tradutor: json2maspy\n");
    fprintf(maspy_output_file, "#\n");
    fprintf(maspy_output_file, "from maspy import *\n\n");
}

void generate_agent_class_start(const char *agent_json_name) {
    agent_class_counter++;
    fprintf(maspy_output_file, "class Agente%d(Agent):\n", agent_class_counter);
    fprintf(maspy_output_file, "    def __init__(self, agt_name):\n");
    fprintf(maspy_output_file, "        super().__init__(agt_name)\n");
    fprintf(maspy_output_file, "        # Original JSON Agent Name: %s\n", agent_json_name);
}

void generate_agent_class_end() {
    fprintf(maspy_output_file, "\n"); // Adiciona uma linha em branco para espaçamento
}

void generate_belief_maspy(const char *belief_name) {
    fprintf(maspy_output_file, "        self.add(Belief(\"%s\"))\n", belief_name);
}

void generate_goal_maspy(const char *goal_name) {
    fprintf(maspy_output_file, "        self.add(Goal(\"%s\"))\n", goal_name);
}

// DEFINIÇÃO ATUALIZADA: Agora recebe a string do corpo do plano
void generate_plan_start_maspy(const char *trigger_name, const char *ctx_name, const char *func_name_in_maspy, int is_empty_body_flag, const char *plan_body_str) {
    char *clean_func_name = clean_for_python_id(func_name_in_maspy);

    fprintf(maspy_output_file, "    @pl(gain, Goal(\"%s\"), Belief(\"%s\"))\n", trigger_name, ctx_name);
    fprintf(maspy_output_file, "    def %s_(self, src):\n", clean_func_name);

    if (is_empty_body_flag) { // Se o corpo estiver vazio
        fprintf(maspy_output_file, "        pass\n");
    } else {
        // Imprime a string do corpo acumulado, que já contém a indentação
        fprintf(maspy_output_file, "%s", plan_body_str);
    }
    // clean_func_name é gerenciado globalmente pela função clean_for_python_id
}

void generate_maspy_footer() {
    fprintf(maspy_output_file, "\n# Configuração do Ambiente e Agentes\n");
    // Cria uma instância de um ambiente genérico. O nome "MeuAmbienteGeral" é arbitrário.
    fprintf(maspy_output_file, "my_env = Environment(\"MeuAmbienteGeral\")\n\n");

    // Itera sobre os agentes registrados e gera o código para instanciá-los e conectá-los ao ambiente
    for (int i = 0; i < num_agents; i++) {
        // Gera um nome de variável Python limpo para o agente (ex: agent_bob, agent_alice)
        char *python_agent_var_name = clean_for_python_id(agent_names[i]);
        
        // Instancia o agente (ex: agent_bob = Agente1("bob"))
        fprintf(maspy_output_file, "%s = Agente%d(\"%s\")\n", python_agent_var_name, i + 1, agent_names[i]);
        
        // Conecta o agente ao ambiente (ex: agent_bob.connect_env(my_env))
        fprintf(maspy_output_file, "%s.connect_to(my_env)\n\n", python_agent_var_name); // CORRIGIDO: método 'connect_to'
    }
    // Inicia o sistema MASPY
    fprintf(maspy_output_file, "Admin().start_system()\n");
}

// Função principal que abre os arquivos e chama o parser
int main(int argc, char **argv) {

    //#ifdef YYDEBUG
    //yydebug = 1;
    //#endif

    if (argc != 2) {
        fprintf(stderr, "Uso: %s <arquivo_json_entrada>\n", argv[0]);
        return 1;
    }

    // Abre o arquivo de entrada JSON
    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("Erro ao abrir arquivo JSON de entrada");
        return 1;
    }

    // Abre o arquivo de saída MASPY (.py)
    maspy_output_file = fopen("maspy_output.py", "w");
    if (!maspy_output_file) {
        perror("Erro ao criar arquivo MASPY de saída (maspy_output.py)");
        fclose(yyin);
        return 1;
    }

    generate_maspy_header(); // Gera o cabeçalho do arquivo MASPY

    yyparse(); // Inicia a análise sintática

    generate_maspy_footer(); // Gera o rodapé do arquivo MASPY (precisa dos nomes dos agentes)

    cleanup_global_allocated_strings(); // NOVO E CRUCIAL: Libera TODA a memória alocada

    fclose(yyin); // Fecha o arquivo de entrada
    fclose(maspy_output_file); // Fecha o arquivo de saída
    return 0;
}

// Função de tratamento de erros do Bison
void yyerror(const char *s) {
    fprintf(stderr, "Erro sintático na linha %d: %s\n", yylineno, s);
}