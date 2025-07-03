from maspy import *

class AgenteCafe(Agent):
    @pl(gain, Belief("preparar_cafe"))
    def fazer_cafe(self, src):
        self.print("Preparar café!")
        self.stop_cycle()

cafe = AgenteCafe("cafe")
cafe.add(Belief("preparar_cafe"))
Admin().start_system()