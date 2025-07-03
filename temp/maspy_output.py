#
# Trabalho de Compiladores
# tradutor: json2maspy
#
from maspy import *

class Agente1(Agent):
    def __init__(self, agt_name):
        super().__init__(agt_name)
        # Original JSON Agent Name: charlie
        self.add(Belief("estaComFome"))
        self.add(Belief("geladeiraVazia"))
        self.add(Belief("diaDeSol"))
        self.add(Goal("fazerCompras"))
    @pl(gain, Goal("comprar_comida"), Belief("pedirComida"))
    def comprar_comida_(self, src):
        self.print("Executando Ação: ir_ao_mercado")
        self.add(Goal("pagarConta"))
        self.add(Belief("comprasFeitas"))
    @pl(gain, Goal("pedir_pizza"), Belief("fomeRapida"))
    def pedir_pizza_(self, src):
        self.print("Executando Ação: ligar_pizzaria")
        self.add(Belief("pizzaACaminho"))
    @pl(gain, Goal("esperar_entrega"), Belief("pizzaACaminho"))
    def esperar_entrega_(self, src):
        pass
    @pl(gain, Goal("ir_para_praia"), Belief("fimDeSemana"))
    def ir_para_praia_(self, src):
        self.print("Executando Ação: pegar_toalha")
        self.print("Executando Ação: passar_protetor")
        self.add(Belief("na_praia"))
    @pl(gain, Goal("fazerCompras"), Belief("estaComFome"))
    def fazerCompras_(self, src):
        self.print("Executando Ação: verificar_geladeira")
        self.add(Belief("pedirComida"))
        self.add(Goal("comprar_comida"))


# Configuração do Ambiente e Agentes
my_env = Environment("MeuAmbienteGeral")

charlie = Agente1("charlie")
charlie.connect_to(my_env)

Admin().start_system()
