#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tradutor_utils.h"

// Variáveis e funções externas do Flex/Bison
extern int yyparse();
extern FILE *yyin;

/**
 * Função principal que abre os arquivos e chama o parser.
 */
int main(int argc, char **argv) {

    if (argc != 2) {
        fprintf(stderr, "Uso: %s <arquivo_json_entrada>\n", argv[0]);
        return 1;
    }

    // 1. Abre o arquivo de entrada JSON (para o Flex)
    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("Erro ao abrir arquivo JSON de entrada");
        return 1;
    }

    // 2. Abre o arquivo de saída MASPY (.py) (Usado pelas funções auxiliares)
    maspy_output_file = fopen("maspy_output.py", "w");
    if (!maspy_output_file) {
        perror("Erro ao criar arquivo MASPY de saída (maspy_output.py)");
        fclose(yyin);
        return 1;
    }

    // 3. Geração do cabeçalho, parsing e rodapé
    generate_maspy_header(); // Gera a seção de imports
    
    yyparse(); // Inicia a análise sintática (chamará as regras do parser.y)

    generate_maspy_footer(); // Gera a configuração do sistema (agentes e canais)

    // 4. Fechamento e limpeza
    fclose(yyin);
    fclose(maspy_output_file);
    cleanup_global_allocated_strings();
    
    return 0;
}