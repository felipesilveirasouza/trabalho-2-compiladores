from maspy import *
class Agente_sensor_unit(Agent):
    def __init__(self):
        super().__init__('Unidade_Sensor_Veicular')
        self.add(Belief('B_sistema_operacional'))
        self.add(Goal('G_monitorar_trafego'))

    @pl(gain, Goal('monitorar_trafego'), Belief('sistema_operacional'))
    def plan_pl_detectar_e_enviar_(self):
        self.processar_dados_sensor()
        self.send('traffic_manager', tell, Belief('veiculo_passando'), 'ch_alerta')

class Agente_traffic_manager(Agent):
    def __init__(self):
        super().__init__('Gerente_de_Trafego')
        self.add(Belief('B_canal_conectado'))
        self.add(Goal('G_aguardar_alerta'))


    @pl(gain, Belief('veiculo_passando', Any), Goal('aguardar_alerta'))
    def plan_pl_receber_alerta_(self, src, msg):
        self.add(Belief('alerta_recebido_do_sensor'))
        self.add(Goal('iniciar_ajuste_semaforo'))

agente1 = Agente_sensor_unit()
agente2 = Agente_traffic_manager()

ch1 = Channel("ch_alerta")

Admin().connect_to([agente1, agente2], [ch1])
Admin().start_system()
