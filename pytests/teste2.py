from maspy import *

class Agente1(Agent):
    def __init__(self, agt_name):
        super().__init__(agt_name)
        self.add(Belief("estaChovendo"))
        self.add(Belief("naoTenhoGuardaChuva"))
        self.add(Belief("tenhoProva"))
        self.add(Goal("comprarGuardaChuva"))

    @pl(gain, Goal("comprarGuardaChuva"), Belief("estaChovendo"))
    def comprarGuardaChuva(self, src):
        self.print("sair para comprar guardachuva")
        self.print("procurar loja")
        self.print("comprar guardachuva")
        self.add(Goal("estudar"))

    @pl(gain, Goal("estudar"), Belief("tenhoProva"))
    def estudar(self, src):
        self.print("convidar alice")
        self.print("estudar na biblioteca")

class Agente2(Agent):
    def __init__(self, agt_name):
        super().__init__(agt_name)
        self.add(Belief("estaChovendo"))
        self.add(Belief("tenhoGuardaChuva"))
        self.add(Belief("tenhoProva"))

    @pl(gain, Belief("tenhoGuardaChuva"), Belief("estaChovendo"))
    def tenhoGuardaChuva(self, src):
        self.print("posso estudar")
        self.wait(10)
        self.print("recebi convite de bob")
        self.add(Goal("estudarNaBiblioteca"))

    @pl(gain, Goal("estudarNaBiblioteca"), Belief("tenhoProva"))
    def estudarNaBiblioteca(self, src):
        self.print("escolher caminho")
        self.print("usar guardachuva")
        self.print("ir para biblioteca")
        self.print("estudar!")

Agente1("bob")
Agente2("alice")
Admin().start_system()