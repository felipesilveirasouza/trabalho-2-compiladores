#ifndef TRADUTOR_UTILS_H
#define TRADUTOR_UTILS_H

#include <stdio.h>

#define MAX_AGENTS 100
#define MAX_CHANNELS 100

// ====================================================================
// GLOBAIS
// ====================================================================

// Variáveis de Gerenciamento de Memória (para Flex/Bison)
extern void register_global_allocated_string(char *str);
extern void cleanup_global_allocated_strings();

// Variáveis de Configuração do Sistema
extern char *agent_ids[MAX_AGENTS];
extern char *agent_names[MAX_AGENTS];
extern int num_agents;

extern char *channels[MAX_CHANNELS];
extern int num_channels;

extern FILE *maspy_output_file; 

// ====================================================================
// FUNÇÕES DE UTILIDADE E CONFIGURAÇÃO
// ====================================================================
void add_agent(char* id, char* name);
void add_channel(char* name);

// Declaração da função para validar identificadores (sem espaços)
int validate_identifier_for_spaces(const char *str, const char *type);

// ====================================================================
// FUNÇÕES DE GERAÇÃO DE CÓDIGO
// ====================================================================
void generate_maspy_header();
void generate_agent_class(char* id, char* name, char* beliefs, char* goal, char* plans);
void generate_maspy_footer();

// ====================================================================
// FUNÇÕES DE TRADUÇÃO
// ====================================================================
char* translate_action(char* action_name);
char* translate_add_belief(char* belief_name);
char* translate_add_goal(char* goal_name);
char* translate_plan(char* plan_id, char* trigger, char* ctx, char* body);
char* translate_message(char* receiver, char* knowledge_str, char* channel_str);
char* process_body_element(char *body_line);

#endif // TRADUTOR_UTILS_H