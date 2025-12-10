all: temp/json2maspy_translator
	@echo "--- Compilacao do tradutor concluida. ---"

temp/json2maspy_translator: src/parser.y src/lexer.l
	mkdir -p temp
	bison -d src/parser.y -o temp/parser.tab.c
	flex -o temp/lex.yy.c src/lexer.l
	gcc -Wall -g temp/lex.yy.c temp/parser.tab.c -o temp/json2maspy_translator
	@echo "Execute o comando ./temp/json2maspy_translator tests/input/1.json"
	@echo "Depois execute python maspy_output.py"

setup:
	@echo "Configurando ambiente virtual Python..."
	test -d venv || python3 -m venv venv
	@echo "Ambiente virtual criado/existente em venv"
	@echo "Tentando ativar o ambiente virtual para esta execucao do make setup..."
	source venv/bin/activate || { echo "AVISO: Falha ao ativar o ambiente virtual no subshell do make. Isso pode acontecer em algumas configuracoes."; }
	@echo "AVISO IMPORTANTE: A ativacao acima e APENAS para o processo 'make setup'."
	@echo "Para usar o ambiente virtual em seu terminal, voce DEVE ativa-lo manualmente:"
	@echo "  source venv/bin/activate"
	@echo "Certifique-se de instalar as dependencias Python (como 'maspy') dentro do ambiente virtual, se ainda nao o fez:"
	@echo "  venv/bin/pip install maspy"

clean:
	rm -rf temp/* temp
	rm -f tests/output/*.py
	rm -f src/parser.tab.c src/parser.tab.h src/lex.yy.c