#include "tradutor_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// CORREÇÃO ESSENCIAL: Declaração externa de yyerror (função definida no parser.y), 
// que substitui a inclusão problemática de "parser.tab.h"
extern int yyerror(const char *s); 

// ====================================================================
// VARIÁVEIS GLOBAIS
// ====================================================================

#define MAX_ALLOCATED_STRINGS 2000
char *global_allocated_strings[MAX_ALLOCATED_STRINGS];
int global_num_allocated_strings = 0;

char *agent_ids[MAX_AGENTS];
char *agent_names[MAX_AGENTS];
int num_agents = 0;

char *channels[MAX_CHANNELS];
int num_channels = 0;

FILE *maspy_output_file = NULL; 


// ====================================================================
// FUNÇÕES DE GERENCIAMENTO DE MEMÓRIA E UTILIDADE
// ====================================================================

void register_global_allocated_string(char *str) {
    if (str && global_num_allocated_strings < MAX_ALLOCATED_STRINGS) {
        global_allocated_strings[global_num_allocated_strings++] = str;
    } else if (str) {
        fprintf(stderr, "Aviso: Limite de strings alocadas globalmente atingido. Memória para '%s' pode vazar.\n", str);
    }
}

void cleanup_global_allocated_strings() {
    for (int i = 0; i < global_num_allocated_strings; i++) {
        if (global_allocated_strings[i]) {
            free(global_allocated_strings[i]);
            global_allocated_strings[i] = NULL;
        }
    }
    global_num_allocated_strings = 0;
}

int validate_identifier_for_spaces(const char *str, const char *type) {
    if (str && strchr(str, ' ')) {
        char err_msg[512];
        snprintf(err_msg, 512, "Identificador de %s inválido: '%s' contém espaços. O projeto não permite espaços em identificadores.", type, str);
        yyerror(err_msg); 
        return 0; 
    }
    return 1;
}

void add_agent(char* id, char* name) {
    if (num_agents < MAX_AGENTS) {
        // CORREÇÃO CRÍTICA: Faz a cópia persistente (strdup) AQUI
        // E registra as cópias para serem liberadas no final (cleanup_global_allocated_strings).
        
        char *id_copy = strdup(id);
        char *name_copy = strdup(name);

        // Verifica se a alocação foi bem-sucedida
        if (id_copy == NULL || name_copy == NULL) {
            fprintf(stderr, "Erro de alocação de memória ao adicionar agente.\n");
            // Tenta liberar o que foi alocado, se houver
            if (id_copy) free(id_copy);
            if (name_copy) free(name_copy);
            return;
        }

        register_global_allocated_string(id_copy);
        register_global_allocated_string(name_copy);
        
        // Armazena os ponteiros para as cópias persistentes
        agent_ids[num_agents] = id_copy; 
        agent_names[num_agents] = name_copy;
        num_agents++;
    }
}

void add_channel(char* name) {
    for (int i = 0; i < num_channels; i++) {
        if (strcmp(channels[i], name) == 0) {
            // Se já existe, não precisa adicionar novamente
            return;
        }
    }
    if (num_channels < MAX_CHANNELS) {
        // Como o nome já deve ser uma cópia persistente (strdup) de quem chamou,
        // apenas armazenamos o ponteiro.
        channels[num_channels++] = name; 
    }
}


// ====================================================================
// GERAÇÃO DE CÓDIGO MASPY
// ====================================================================

void generate_maspy_header() {
    if (!maspy_output_file) return;

    fprintf(maspy_output_file, "#!/usr/bin/env python\n");
    fprintf(maspy_output_file, "#\n# Código gerado pelo tradutor JSON -> MASPY\n#\n\n");
    fprintf(maspy_output_file, "from maspy import *\n");

}

void generate_agent_class(char* id, char* name, char* beliefs, char* goal, char* plans) {
    if (!maspy_output_file) return;

    fprintf(maspy_output_file, "class Agente_%s(Agent):\n", id);
    fprintf(maspy_output_file, "    def __init__(self):\n");
    fprintf(maspy_output_file, "        super().__init__('%s')\n\n", name);
    
    // Crenças Iniciais
    if (beliefs && strlen(beliefs) > 0) {
        fprintf(maspy_output_file, "        # Crenças Iniciais\n");
        fprintf(maspy_output_file, "%s\n\n", beliefs); 
    }
    
    // Objetivo Inicial
    if (goal && strlen(goal) > 0) {
        fprintf(maspy_output_file, "        # Objetivo Inicial\n");
        char *goal_call = translate_add_goal(goal); 
        fprintf(maspy_output_file, "%s\n\n", goal_call);
    }
    
    // Planos
    if (plans && strlen(plans) > 0) {
        fprintf(maspy_output_file, "    # Planos\n");
        fprintf(maspy_output_file, "%s\n", plans); 
    }
    fprintf(maspy_output_file, "\n");
}


void generate_maspy_footer() {
    if (!maspy_output_file) return;

    fprintf(maspy_output_file, "\n# ====================================================================\n");
    fprintf(maspy_output_file, "# CONFIGURAÇÃO E INICIALIZAÇÃO DO SISTEMA\n");
    fprintf(maspy_output_file, "# ====================================================================\n\n");
    
    // 1. Instanciação dos Agentes
    fprintf(maspy_output_file, "# Instanciação dos Agentes\n");
    for (int i = 0; i < num_agents; i++) {
        // CORREÇÃO: Usa agent_ids[i] para o nome da variável Python no lado esquerdo.
        fprintf(maspy_output_file, "%s = Agente_%s()\n", agent_ids[i], agent_ids[i]);
    }

    // 2. Instanciação dos Canais
    if (num_channels > 0) {
        fprintf(maspy_output_file, "\n# Instanciação dos Canais\n");
        for (int i = 0; i < num_channels; i++) {
            // Usa o ID do canal (armazenado em channels[i]) para instanciar a classe Channel
            fprintf(maspy_output_file, "%s = Channel('%s')\n", channels[i], channels[i]);
        }
    }

    // 3. Conexão e Inicialização
    fprintf(maspy_output_file, "\n# Conexão e Inicialização\n");

    char agents_list[1024] = "";
    if (num_agents > 0) {
        // CORREÇÃO: Usa agent_ids[i] aqui também
        strcat(agents_list, agent_ids[0]);
        for (int i = 1; i < num_agents; i++) {
            strcat(agents_list, ", ");
            strcat(agents_list, agent_ids[i]);
        }
    }

    char channels_list[1024] = "";
    if (num_channels > 0) {
        strcat(channels_list, channels[0]);
        for (int i = 1; i < num_channels; i++) {
            strcat(channels_list, ", ");
            strcat(channels_list, channels[i]);
        }
    }

    if (num_agents > 0 && num_channels > 0) {
        // Conexão com Agentes E Canais
        fprintf(maspy_output_file, "Admin().connect_to([%s], [%s])\n", agents_list, channels_list);
    } else if (num_agents > 0) {
        // Conexão apenas com Agentes
        fprintf(maspy_output_file, "Admin().connect_to([%s])\n", agents_list);
    }

    fprintf(maspy_output_file, "Admin().start_system()\n");
}


// ====================================================================
// TRADUÇÃO DE ELEMENTOS DO BODY E PLANOS
// ====================================================================

char* translate_action(char* action_name) {
    // Identação de 8 espaços (dentro do método do plano)
    char *result;
    asprintf(&result, "        self.%s()", action_name); 
    register_global_allocated_string(result);
    return result;
}

char* translate_add_belief(char* belief_name) {
    // CORREÇÃO: Identação de 8 espaços (dentro do __init__)
    char *result;
    asprintf(&result, "        self.add(Belief('%s'))", belief_name); 
    register_global_allocated_string(result);
    return result;
}

char* translate_add_goal(char* goal_name) {
    // CORREÇÃO: Identação de 8 espaços (dentro do __init__)
    char *result;
    asprintf(&result, "        self.add(Goal('%s'))", goal_name); 
    register_global_allocated_string(result);
    return result;
}


// CORREÇÃO: Função translate_plan reescrita para aceitar triggers MASPY de Conhecimento, Mensagem e Ação
char* translate_plan(char* plan_id, char* trigger, char* ctx, char* body) {
    char *result = NULL;
    char *trigger_args = NULL; // Argumentos dentro do @pl() antes do Ctx

    // 1. Processa o trigger baseado no prefixo
    if (strncmp(trigger, "B_", 2) == 0 || strncmp(trigger, "G_", 2) == 0) {
        // Trigger de Conhecimento (Belief ou Goal) -> Sintaxe: @pl(gain, Belief/Goal('...'), Ctx('...'))
        char *knowledge_class = (trigger[0] == 'B') ? "Belief" : "Goal";
        char *knowledge_content = trigger + 2; 
        // Assume 'gain' como a ação padrão para novos itens de conhecimento.
        asprintf(&trigger_args, "gain, %s('%s')", knowledge_class, knowledge_content);

    } else if (strncmp(trigger, "M_", 2) == 0) {
        // Trigger de Mensagem -> Sintaxe: @pl(Msg('tipo', 'canal'), Ctx('...'))
        // Assume que a string é no formato M_tipo:canal (Ex: "M_tell:ch_status")
        char *msg_params = trigger + 2; 

        // CRIA UMA CÓPIA para manipular strtok de forma segura (embora usemos strchr para evitar strtok)
        char *msg_params_copy = strdup(msg_params);
        char *msg_type = msg_params_copy;
        char *msg_channel = NULL;
        char *colon_pos = strchr(msg_type, ':');

        if (colon_pos) {
            *colon_pos = '\0'; // Termina a string 'tipo'
            msg_channel = colon_pos + 1; // 'canal'
            
            // Validação de segurança (mantida)
            if (!validate_identifier_for_spaces(msg_type, "Tipo de Mensagem") ||
                !validate_identifier_for_spaces(msg_channel, "Canal de Mensagem")) {
                free(msg_params_copy);
                goto error_handling;
            }

            // >> CORREÇÃO 1: REGISTRAR O CANAL <<
            // add_channel exige uma cópia persistente, pois msg_params_copy será liberado.
            add_channel(strdup(msg_channel)); 

            asprintf(&trigger_args, "Msg('%s', '%s')", msg_type, msg_channel);
        } else {
            // Se não houver ':' (canal)
            asprintf(&trigger_args, "Msg('%s')", msg_type);
        }
        free(msg_params_copy);
        
    } else if (strncmp(trigger, "A_", 2) == 0) {
        // Trigger de Ação (Ação Planejada) -> Sintaxe: @pl(act, Action('...'), Ctx('...'))
        char *action_content = trigger + 2;

        if (!validate_identifier_for_spaces(action_content, "Ação")) {
            goto error_handling;
        }

        asprintf(&trigger_args, "act, Action('%s')", action_content);
        
    } else {
        // Prefixo Desconhecido
        char err_msg[512];
        snprintf(err_msg, 512, "Trigger de plano inválido: '%s'. Deve começar com B_, G_, M_, ou A_.", trigger);
        yyerror(err_msg);
        goto error_handling;
    }
    
    if (!trigger_args) {
        goto error_handling; 
    }

    // 2. Monta a string de saída final: @pl(TRIGGERS..., Ctx('...'))
    asprintf(&result, "\n    @pl(%s, Ctx('%s'))\n    def plan_%s_(self):\n%s", 
             trigger_args,       
             ctx, 
             plan_id, 
             body);

    register_global_allocated_string(result);
    free(trigger_args);
    return result;

error_handling:
    if (trigger_args) free(trigger_args);
    result = strdup("    # ERRO DE GERAÇÃO DE PLANO\n");
    register_global_allocated_string(result);
    return result;
}


char* translate_message(char* receiver, char* knowledge_str, char* channel_str) {
    char knowledge_type[10]; 
    char knowledge_name[100]; 
    char *result;

    if (strncmp(knowledge_str, "B_", 2) == 0) {
        strcpy(knowledge_type, "Belief");
        strcpy(knowledge_name, knowledge_str + 2); 
    } else if (strncmp(knowledge_str, "G_", 2) == 0) {
        strcpy(knowledge_type, "Goal");
        strcpy(knowledge_name, knowledge_str + 2); 
    } else {
        result = strdup("        # ERRO: Tipo de conhecimento da mensagem desconhecido\n");
        register_global_allocated_string(result);
        return result;
    }
    
    // >> CORREÇÃO 3: REGISTRAR O CANAL E MONTAR STRING <<
    if (channel_str && strlen(channel_str) > 0) {
        // add_channel exige uma cópia persistente, pois channel_str é temporário (apontando para temp_params)
        add_channel(strdup(channel_str)); 
        
        // Com canal
        asprintf(&result, "        self.send('%s', tell, %s('%s'), '%s')",
             receiver, knowledge_type, knowledge_name, channel_str);
    } else {
        // Sem canal (opcional em MASPY, mas a sintaxe deve ser simplificada)
        asprintf(&result, "        self.send('%s', tell, %s('%s')) # Aviso: Canal omitido",
             receiver, knowledge_type, knowledge_name);
    }

    register_global_allocated_string(result);
    return result;
}

char* process_body_element(char *body_line) {
    if (!body_line || strlen(body_line) < 3) {
        fprintf(stderr, "Erro de corpo de plano: linha inválida ou muito curta: '%s'\n", body_line ? body_line : "(null)");
        char *err = strdup("");
        register_global_allocated_string(err);
        return err;
    }
    
    // Validação de espaços no conteúdo útil (após o prefixo)
    if (strchr(body_line + 2, ' ')) {
        char err_msg[512];
        snprintf(err_msg, 512, "Identificador de elemento de plano inválido: '%s' contém espaços após o prefixo.", body_line);
        yyerror(err_msg);
        char *err = strdup("");
        register_global_allocated_string(err);
        return err;
    }

    // Delega a tradução com base no prefixo
    if (strncmp(body_line, "A_", 2) == 0) {
        return translate_action(body_line + 2);

    } else if (strncmp(body_line, "G_", 2) == 0) {
        return translate_add_goal(body_line + 2);

    } else if (strncmp(body_line, "B_", 2) == 0) {
        return translate_add_belief(body_line + 2);
        
    } else if (strncmp(body_line, "M_", 2) == 0) {
        // CORREÇÃO 2: Implementação da lógica de extração para Ações M_
        // Esperado M_receptor:conhecimento:canal
        char *message_params = body_line + 2; 

        // Cria uma cópia persistente para manipulação, pois message_params é temporário
        char *temp_copy = strdup(message_params);
        char *receptor_p = temp_copy;
        char *knowledge_p = NULL;
        char *channel_p = NULL;

        // Encontra o primeiro ':' (separa receptor e conhecimento)
        knowledge_p = strchr(receptor_p, ':');
        if (knowledge_p) {
            *knowledge_p = '\0'; // Termina o receptor
            knowledge_p++;
            
            // Encontra o segundo ':' (separa conhecimento e canal)
            channel_p = strchr(knowledge_p, ':');
            if (channel_p) {
                *channel_p = '\0'; // Termina o knowledge
                channel_p++;
            }
            
            // Validações de segurança (se necessário)
            if (!validate_identifier_for_spaces(receptor_p, "Receptor") ||
                !validate_identifier_for_spaces(knowledge_p, "Conhecimento")) {
                // A validação do canal ocorre em translate_message
                free(temp_copy);
                return strdup("");
            }
            
            // Chama o tradutor de mensagem
            char *result = translate_message(receptor_p, knowledge_p, channel_p);
            free(temp_copy);
            return result;
        } else {
            // Sintaxe inválida: Não tem pelo menos 1 ':' (receptor:conhecimento)
            yyerror("Sintaxe M_ no corpo inválida. Esperado M_receptor:conhecimento:canal");
            free(temp_copy);
            return strdup("");
        }

    } else {
        fprintf(stderr, "Erro de corpo de plano: prefixo desconhecido em '%s'. Ignorando linha.\n", body_line);
        char *err = strdup("");
        register_global_allocated_string(err);
        return err;
    }
}