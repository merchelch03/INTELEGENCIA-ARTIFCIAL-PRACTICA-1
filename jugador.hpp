#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
using namespace std;

struct state{
    int fil;
    int col;
    Orientacion brujula;
};

class ComportamientoJugador : public Comportamiento{

  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
        last_action=actIDLE;
        current_state.fil=99;
        current_state.col=99;
        current_state.brujula=norte;
        bien_situado=false;
        bikini=false;
        zapatillas=false;
        tam_mapa=size;
        tam_sterreno=16;
        prioridad=false;
        max_bateria_pre=4000;
        recargando=false;
        mirar_salida_de=false;
        mirar_salida_iz=false;
        contador=0;
    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);
    void actualizar_mapa(Sensores sensores);
    void reiniciamos();
    bool avanzar_actrun(Sensores sensores);
    bool seguir_alfrente(Sensores sensores);
    bool comprobar_posicion(Sensores sensores, int posicion);
    int encontrar_G(Sensores sensores);
    int encontrar_bikini(Sensores sensores);
    int encontrar_zapatillas(Sensores sensores);
    Action ir(int i, Sensores sensores);
    Action mover(Sensores sensores);
    Action aleatorio_i_d(Sensores sensores);
    bool hay_camino_muro_de(Sensores sensores);
    bool hay_camino_muro_iz(Sensores sensores);
    bool hay_salida_muro_de(Sensores sensores);
    bool hay_salida_muro_iz(Sensores sensores);
    bool rodeado_muros(Sensores sensores);
    Action roderas_muros_a(Sensores sensores);

  private:
  Action last_action;
  state current_state;
  Orientacion brujula;
  int a; //giro siempre positivo
  bool bien_situado;//cuando sepamos la posicion
  //objetos
  bool bikini;
  bool zapatillas;
  int tam_mapa;
  int tam_sterreno;
  int pos_g;
  int pos_k;
  int pos_d;
  bool prioridad;//ya tenemos a donde ir, no entorpezer
  int max_bateria_pre;
  bool recargando;
  bool mirar_salida_de;
  bool mirar_salida_iz;
  int contador;
};
#endif
