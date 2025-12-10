%{
#define YYDEBUG 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tradutor_utils.h"

extern int yylex();
extern int yylineno;
extern FILE *yyin; 

int yyerror(const char *s); 

// Declaração de funções auxiliares (implementadas em src/tradutor_utils.c)
extern char* process_body_element(char *body_line); 
extern int validate_identifier_for_spaces(const char *str, const char *type);
extern void register_global_allocated_string(char *str); 

%}

/* --- União para o Valor dos Tokens --- */
%union {
    char *str_val;
}

/* --- Declaração de Tokens --- */
%token OBRACE CBRACE OBRACKET CBRACKET COMMA COLON
%token AGENT_CODE BELIEFS GOAL PLANS TRIGGER CTX BODY NAME 

%token <str_val> IDENTIFIER STRING_LITERAL NUMBER 

/* --- Tipos para os Não-Terminais --- */
%type <str_val> json_object agent_list agent_definition beliefs_list
%type <str_val> plans_map plan_list plan_definition plan_body 
%type <str_val> body_element 
%type <str_val> initial_belief_item
%type <str_val> beliefs_array_elements

/* --- Início da Gramática --- */
%start program

%%

/* ----------------------------------------------------
 * Regras de Estrutura do Programa
 * ----------------------------------------------------
 */
program: json_object
         {
             cleanup_global_allocated_strings();
         }
         ;

json_object: OBRACE AGENT_CODE COLON OBRACE agent_list CBRACE CBRACE
             {
                 $$ = strdup(""); 
             }
             ;

agent_definition: STRING_LITERAL COLON STRING_LITERAL COMMA // $1: ID, $3: Nome
                  beliefs_list COMMA                        // $5: beliefs content
                  GOAL COLON STRING_LITERAL COMMA           // $9: goal string
                  PLANS COLON plans_map                     // $13: plans map
                  {
                      // VALIDAÇÃO: ID, Nome e Meta não podem ter espaços.
                      if (!validate_identifier_for_spaces($1, "ID do Agente") ||
                          !validate_identifier_for_spaces($3, "Nome do Agente") ||
                          !validate_identifier_for_spaces($9, "Meta Inicial")) {
                          YYABORT; 
                      }

                      char *ag_id = $1;
                      char *ag_name = $3;
                      char *beliefs_str = $5; 
                      char *goal_str = $9;
                      char *plans_str = $13;

                      add_agent(ag_id, ag_name); // ESTE CHAMA strdup(ag_id) E REGISTRA O NOVO PONTEIRO
                      generate_agent_class(ag_id, ag_name, beliefs_str, goal_str, plans_str); 
                      
                      $$ = strdup("");
                      // Apenas o valor de retorno da regra ($$) é registrado
                      register_global_allocated_string($$); 
                  }
                  ;

agent_list: agent_definition
          | agent_list COMMA agent_definition // Permite múltiplos agentes separados por vírgula
          | /* Vazio */
            {
                $$ = strdup(""); 
            }
          ;


/* ----------------------------------------------------
 * Regras de Beliefs (Inicialização)
 * ----------------------------------------------------
 */
beliefs_list: BELIEFS COLON OBRACKET beliefs_array_elements CBRACKET
            {
                $$ = $4; 
            }
            ;

beliefs_array_elements: initial_belief_item 
                      | beliefs_array_elements COMMA initial_belief_item
                        {
                            asprintf(&$$, "%s\n%s", $1, $3); 
                            register_global_allocated_string($$); 
                        }
                      ;

initial_belief_item: STRING_LITERAL
              { 
                  // VALIDAÇÃO: Crenças não podem ter espaços.
                  if (!validate_identifier_for_spaces($1, "Crença Inicial")) {
                      YYABORT;
                  }
                  $$ = translate_add_belief($1); 
              }
              ;

/* ----------------------------------------------------
 * Regras de Planos e Corpo
 * ----------------------------------------------------
 */

plans_map: OBRACE plan_list CBRACE
           {
               $$ = $2; 
           }
         | OBRACE CBRACE // Permite plans: {}
           {
               $$ = strdup(""); 
               register_global_allocated_string($$);
           }
         ;

plan_list: plan_definition
           {
               $$ = $1;
           }
         | plan_list COMMA plan_definition
           {
               asprintf(&$$, "%s\n%s", $1, $3); 
               register_global_allocated_string($$); 
           }
         ;


plan_definition: STRING_LITERAL COLON OBRACE 
                 TRIGGER COLON STRING_LITERAL COMMA  // $6 é o Trigger (ex: "B_crenca")
                 CTX COLON STRING_LITERAL COMMA 
                 BODY COLON OBRACKET plan_body CBRACKET CBRACE
               {
                   char *trigger_val = $6;

                   // VALIDAÇÃO: ID, Trigger e Contexto não podem ter espaços.
                   if (!validate_identifier_for_spaces($1, "ID do Plano") ||
                       !validate_identifier_for_spaces($10, "Contexto do Plano")) {
                       // O trigger tem validação de prefixo e espaços internos mais complexa,
                       // que é parcialmente delegada ao tradutor ou validada abaixo.
                       YYABORT;
                   }

                   // VALIDAÇÃO CRÍTICA: O trigger deve começar com B_, G_, M_ ou A_
                   if (strncmp(trigger_val, "B_", 2) != 0 && 
                       strncmp(trigger_val, "G_", 2) != 0 &&
                       strncmp(trigger_val, "M_", 2) != 0 &&
                       strncmp(trigger_val, "A_", 2) != 0) {
                       yyerror("Trigger do plano inválido. Deve começar com 'B_', 'G_', 'M_' ou 'A_'.");
                       YYABORT;
                   }

                   char *plan_id = $1;
                   char *ctx = $10;
                   char *body = $15;
                   
                   $$ = translate_plan(plan_id, trigger_val, ctx, body); 
               }
               ;

plan_body: body_element
         | plan_body COMMA body_element
           {
               asprintf(&$$, "%s\n%s", $1, $3);
               register_global_allocated_string($$);
           }
         | /* Vazio (Plano vazio) */ 
           { 
               $$ = strdup(""); 
               register_global_allocated_string($$); 
           }
         ;

/* ----------------------------------------------------
 * Regras do Corpo (Unificada)
 * ----------------------------------------------------
 */
body_element: STRING_LITERAL
            {
                // A validação de espaços no conteúdo (após o prefixo A_, B_, G_, M_)
                // é feita dentro do process_body_element. Se falhar, yyerror é chamado
                // e o Bison aborta no próximo ciclo.
                $$ = process_body_element($1); 
            }
            ;

%%

/* ----------------------------------------------------
 * Função de Suporte (Erro)
 * ----------------------------------------------------
 */

int yyerror(const char *s) {
    fprintf(stderr, "Erro de parsing na linha %d: %s\n", yylineno, s);
    return 0;
}