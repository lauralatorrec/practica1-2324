#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
using namespace std;

struct state{
  int fil;
  int col;
  Orientacion brujula;
}

class ComportamientoJugador : public Comportamiento{

  public:

    // Constructor de la clase
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
      // Dar el valor inicial a las variables de estado
      current_state.fil = current_satte.col = 99;
      current_satte.brujula = norte;
      last_action = actIDLE;
      girar_derecha = false;
      bien_situado=false;
    }

    //Constructor de copia
    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    
    //Destructor
    ~ComportamientoJugador(){}

    //Funciones
    Action think(Sensores sensores);
    int interact(Action accion, int valor);

  private:
  // Declarar aquí las variables de estado
  state current_state; //dónde estoy y hacia donde voy a dar prox paso
  Action last_action; //última acción que hice
  bool girar_derecha;
  bool bien_situado;
  
};
#endif
