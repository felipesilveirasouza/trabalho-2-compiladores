#include "tradutor_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
        char *id_copy = strdup(id);
        char *name_copy = strdup(name);

        if (id_copy == NULL || name_copy == NULL) {
            fprintf(stderr, "Erro de alocação de memória ao adicionar agente.\n");
            if (id_copy) free(id_copy);
            if (name_copy) free(name_copy);
            return;
        }

        register_global_allocated_string(id_copy);
        register_global_allocated_string(name_copy);
        
        agent_ids[num_agents] = id_copy; 
        agent_names[num_agents] = name_copy;
        num_agents++;
    }
}

void add_channel(char* name) {
    for (int i = 0; i < num_channels; i++) {
        if (strcmp(channels[i], name) == 0) {
            return;
        }
    }
    if (num_channels < MAX_CHANNELS) {
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
        fprintf(maspy_output_file, "%s = Agente_%s()\n", agent_ids[i], agent_ids[i]);
    }

    // 2. Instanciação dos Canais
    if (num_channels > 0) {
        fprintf(maspy_output_file, "\n# Instanciação dos Canais\n");
        for (int i = 0; i < num_channels; i++) {
            fprintf(maspy_output_file, "%s = Channel('%s')\n", channels[i], channels[i]);
        }
    }

    // 3. Conexão e Inicialização
    fprintf(maspy_output_file, "\n# Conexão e Inicialização\n");

    char agents_list[1024] = "";
    if (num_agents > 0) {
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
        fprintf(maspy_output_file, "Admin().connect_to([%s], [%s])\n", agents_list, channels_list);
    } else if (num_agents > 0) {
        fprintf(maspy_output_file, "Admin().connect_to([%s])\n", agents_list);
    }

    fprintf(maspy_output_file, "Admin().start_system()\n");
}


// ====================================================================
// TRADUÇÃO DE ELEMENTOS DO BODY E PLANOS
// ====================================================================

char* translate_action(char* action_name) {
    char *result;
    asprintf(&result, "        self.%s()", action_name); 
    register_global_allocated_string(result);
    return result;
}

char* translate_add_belief(char* belief_name) {
    char *result;
    asprintf(&result, "        self.add(Belief('%s'))", belief_name); 
    register_global_allocated_string(result);
    return result;
}

char* translate_add_goal(char* goal_name) {
    char *result;
    asprintf(&result, "        self.add(Goal('%s'))", goal_name); 
    register_global_allocated_string(result);
    return result;
}


// CORREÇÃO ESSENCIAL: translate_plan para eliminar Ctx() e ajustar M_ para gain, Knowledge(..., Any)
char* translate_plan(char* plan_id, char* trigger, char* ctx, char* body) {
    char *result = NULL;
    char *trigger_args = NULL; 
    
    char knowledge_type[10] = "";
    char knowledge_name[100] = "";
    char *ctx_output = NULL;

    // 1. Processa o trigger baseado no prefixo
    if (strncmp(trigger, "B_", 2) == 0 || strncmp(trigger, "G_", 2) == 0) {
        // Trigger de Conhecimento (Belief ou Goal) -> Sintaxe: @pl(gain, Belief/Goal('...'), Belief('...'))
        char *knowledge_class = (trigger[0] == 'B') ? "Belief" : "Goal";
        char *knowledge_content = trigger + 2; 
        asprintf(&trigger_args, "gain, %s('%s')", knowledge_class, knowledge_content);

    } else if (strncmp(trigger, "M_", 2) == 0) {
        // Trigger de Mensagem (CORREÇÃO: Traduzido como gain, Knowledge(..., Any) para simular Meta/Crença induzida)
        
        char *msg_params = trigger + 2; 
        char *msg_params_copy = strdup(msg_params);
        char *msg_content = msg_params_copy; 
        
        char *colon_pos = strchr(msg_content, ':');
        char *msg_channel = NULL;
        
        if (colon_pos) {
            *colon_pos = '\0'; 
            msg_channel = colon_pos + 1;
        }

        // Extrai tipo e nome do conhecimento (removendo prefixo B_/G_)
        if (strncmp(msg_content, "B_", 2) == 0) {
            strcpy(knowledge_type, "Belief");
            strcpy(knowledge_name, msg_content + 2);
        } else if (strncmp(msg_content, "G_", 2) == 0) {
            strcpy(knowledge_type, "Goal");
            strcpy(knowledge_name, msg_content + 2);
        } else {
            // Se o prefixo for omitido, assume Goal (padrão para comunicação com achieve)
            strcpy(knowledge_type, "Goal"); 
            strcpy(knowledge_name, msg_content);
        }
        
        // O canal é registrado (CORRIGIDO: se for usado, é registrado)
        if (msg_channel && strlen(msg_channel) > 0) {
            char *channel_copy = strdup(msg_channel); 
            register_global_allocated_string(channel_copy); 
            add_channel(channel_copy); 
        }

        // Sintaxe: gain, Knowledge('nome', Any)
        asprintf(&trigger_args, "gain, %s('%s', Any)", 
                 knowledge_type, knowledge_name);
        
        free(msg_params_copy);
        
    } else if (strncmp(trigger, "A_", 2) == 0) {
        // Trigger de Ação
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
    
    // 2. Processa o Contexto (Ctx) (CORREÇÃO: Traduz diretamente para Belief() ou Any)
    if (strlen(ctx) == 0 || strcmp(ctx, "") == 0) {
        // Contexto Vazio (Traduz para Any)
        ctx_output = strdup("Any");
        register_global_allocated_string(ctx_output);
    } else if (strncmp(ctx, "G_", 2) == 0) {
        // Se o contexto é Goal (Ex: G_aguardar_alerta)
        char *goal_content = ctx + 2;
        asprintf(&ctx_output, "Goal('%s')", goal_content); 
        register_global_allocated_string(ctx_output);
    } else {
        // Assume Belief (Ex: B_sistema_operacional ou apenas 'crenca')
        char *belief_content = (strncmp(ctx, "B_", 2) == 0) ? (ctx + 2) : ctx;
        asprintf(&ctx_output, "Belief('%s')", belief_content); 
        register_global_allocated_string(ctx_output);
    }


    // 3. Monta a string de saída final: @pl(TRIGGER_ARGS, CONTEXTO_BELIEF_OU_ANY)
    
    char *plan_func_signature;
    // O plano precisa de 'src, msg' se o trigger usar Any (ou seja, se for M_ ou se o trigger_args tiver 'Any')
    if (strstr(trigger_args, "Any") != NULL) {
        asprintf(&plan_func_signature, "def plan_%s_(self, src, msg):", plan_id);
    } else {
        asprintf(&plan_func_signature, "def plan_%s_(self):", plan_id);
    }
    register_global_allocated_string(plan_func_signature);


    asprintf(&result, "\n    @pl(%s, %s)\n    %s\n%s", 
             trigger_args,       
             ctx_output, 
             plan_func_signature,
             body);

    register_global_allocated_string(result);
    free(trigger_args);
    // ctx_output e plan_func_signature são liberados globalmente
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
    char *performative; 

    // O knowledge_str deve conter B_ ou G_
    if (strncmp(knowledge_str, "B_", 2) == 0) {
        strcpy(knowledge_type, "Belief");
        strcpy(knowledge_name, knowledge_str + 2);
        performative = "tell"; 
    } else if (strncmp(knowledge_str, "G_", 2) == 0) {
        strcpy(knowledge_type, "Goal");
        strcpy(knowledge_name, knowledge_str + 2);
        performative = "achieve"; 
    } else {
        // Se não houver prefixo (não deve acontecer com validação estrita, mas como fallback)
        strcpy(knowledge_type, "Belief"); 
        strcpy(knowledge_name, knowledge_str);
        performative = "tell";
    }

    // Sintaxe self.send com achieve/tell e canal opcional.
    if (channel_str && strlen(channel_str) > 0) {
        // Com canal (4 argumentos)
        char *channel_copy = strdup(channel_str); 
        register_global_allocated_string(channel_copy); 
        add_channel(channel_copy); 
        
        // self.send('receiver', achieve/tell, Knowledge('name'), 'channel_name')
        asprintf(&result, "        self.send('%s', %s, %s('%s'), '%s')",
             receiver, performative, knowledge_type, knowledge_name, channel_str);
    } else {
        // Sem canal (3 argumentos)
        // self.send('receiver', achieve/tell, Knowledge('name'))
        asprintf(&result, "        self.send('%s', %s, %s('%s'))",
             receiver, performative, knowledge_type, knowledge_name);
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
        // Esperado M_receptor:conhecimento:canal (canal opcional)
        char *message_params = body_line + 2; 

        char *temp_copy = strdup(message_params);
        char *receptor_p = temp_copy;
        char *knowledge_p = NULL;
        char *channel_p = NULL;

        knowledge_p = strchr(receptor_p, ':');
        if (knowledge_p) {
            *knowledge_p = '\0'; 
            knowledge_p++;
            
            channel_p = strchr(knowledge_p, ':');
            if (channel_p) {
                *channel_p = '\0'; 
                channel_p++;
            }
            
            if (!validate_identifier_for_spaces(receptor_p, "Receptor")) {
                free(temp_copy);
                return strdup("");
            }
            
            char *result = translate_message(receptor_p, knowledge_p, channel_p);
            free(temp_copy);
            return result;
        } else {
            // Sintaxe inválida
            yyerror("Sintaxe M_ no corpo inválida. Esperado M_receptor:conhecimento[:canal]");
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