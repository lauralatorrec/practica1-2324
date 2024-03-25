#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
using namespace std;

Action ComportamientoJugador::think(Sensores sensores)
{
	Action accion = actIDLE;
	int a;

	//Actualización del mundo:
	switch(last_action){
		case actWALK: //avanzar
			switch (current_state.brujula){
				case norte: 
					current_state.fil--; 
				break;
				case noreste: 
					current_state.fil--;
					current_state.col++;
				break;
				case este: 
					current_state.col++; 
				break;
				case sureste: 
					current_state.fil++;
					current_state.col++;
				break;
				case sur: 
					current_state.fil++; 
				break;
				case suroeste: 
					current_state.fil++;
					current_state.col--;
				break;
				case oeste: 
					current_state.col--; 
				break;
				case noroeste: 
					current_state.fil--;
					current_state.col--;
				break;
			}
		break;

		case actRUN: //correr

		break;

		case actTURN_SR: //girar 45º a la derecha
			a = current_state.brujula;
			a = (a+1)%8; //modulo 8 ya que hay 8 posibles sentidos
			current_state.brujula = static_cast<Orientacion>(a);
		break;

		case actTURN_L: //girar 90º a la izquierda
			a = current_state.brujula;
			a = (a+7)%8; //equivalente a (a-1+8)%8 y (a-1)%8 pero no usamos num neg
			current_state.brujula = static_cast<Orientacion>(a);
		break;
	}

	//Definicion de reglas:

	//-Captura valores fil, col y orient
	//sustituir cond por (sensores.terreno[0]=='G' && !bien_situado)
	if( sensores.posF != -1 && !bien_situado){ 
		current_state.fil=sensores.posF;
		current_state.col=sensores.posC;
		current_state.brujula=sensores.sentido;
		bien_situado=true;
	}
	
	//Si está en una cas de pos estará bien situado, podemos registrar lo que ve el jugador en nuestro mapaResultado
	if(bien_situado){ 
		PonerTerrenoEnMatriz(sensores.terreno, current_state, mapaResultado);	
	}

	//Decision de la nueva accion
	if ((sensores.terreno[2]=='T' || sensores.terreno[2]=='S' || sensores.terreno[2]=='G') && sensores.agentes[2]=='_'){
		accion = actWALK;	//avanzar solo si casilla destino es terreno arenoso o pedregroso o es cas de posicionamiento
	} //SINO PUDE AVANZAR:
	else if(!girar_derecha){ //girar_derecha es un booleano inicializado a false por lo que entrará aqui primero
		accion = actTURN_L; //girará a la izq
		girar_derecha = (rand()%2==0); //NUEVO VALOR ALEATORIO PARA girar_derecha
	}else{
		accion = actTURN_SR; //girará a la der
		girar_derecha = (rand()%2==0);
	}

	// Actualización de acción realizada
	last_action = accion; 
	
	// Mostrar el valor de los sensores
	cout << "Posicion: fila " << sensores.posF << " columna " << sensores.posC;
	switch (sensores.sentido)
	{
		case norte:	  cout << " Norte\n";	break;
		case noreste: cout << " Noreste\n";	break;
		case este:    cout << " Este\n";	break;
		case sureste: cout << " Sureste\n";	break;
		case sur:     cout << " Sur\n";	break;
		case suroeste:cout << " Suroeste\n";	break;
		case oeste:   cout << " Oeste\n";	break;
		case noroeste:cout << " Noroeste\n";	break;
	}
	cout << "Terreno: ";
	for (int i=0; i<sensores.terreno.size(); i++)
		cout << sensores.terreno[i];

	cout << "  Agentes: ";
	for (int i=0; i<sensores.agentes.size(); i++)
		cout << sensores.agentes[i];

	cout << "\nColision: " << sensores.colision;
	cout << "  Reset: " << sensores.reset;
	cout << "  Vida: " << sensores.vida << endl<< endl;

	//Devuelve acción a realizar
	return accion;
}

// extiende esta version inicial donde solo se pone la componente 0 
// en la matriz; la nueva version debe poner todas las componentes 
// de terreno en funcion de la orientacion del agente
void ComportamientoJugador::PonerTerrenoEnMatriz(const vector<unsigned char> & terreno, const state &st, vector<vector<unsigned char>> &matriz){
	//sensores.terreno -> terreno
	//current_state -> st 
	//mapaResultado -> matriz

	matriz[st.fil][st.col] = terreno[0]; 

	switch (st.brujula){
		case norte: 
			matriz[st.fil-1][st.col-1] = terreno[1]; 
			matriz[st.fil-1][st.col] = terreno[2]; 
			matriz[st.fil-1][st.col+1] = terreno[3]; 

			matriz[st.fil-2][st.col-2] = terreno[4]; 
			matriz[st.fil-2][st.col-1] = terreno[5]; 
			matriz[st.fil-2][st.col] = terreno[6]; 
			matriz[st.fil-2][st.col+1] = terreno[7]; 
			matriz[st.fil-2][st.col+2] = terreno[8]; 

			matriz[st.fil-3][st.col-3] = terreno[9]; 
			matriz[st.fil-3][st.col-2] = terreno[10]; 
			matriz[st.fil-3][st.col-1] = terreno[11]; 
			matriz[st.fil-3][st.col] = terreno[12]; 
			matriz[st.fil-3][st.col+1] = terreno[13]; 
			matriz[st.fil-3][st.col+2] = terreno[14]; 
			matriz[st.fil-3][st.col+3] = terreno[15]; 
		break;
		case noreste: 
			matriz[st.fil-1][st.col] = terreno[1]; 
			matriz[st.fil-2][st.col] = terreno[4]; 
			matriz[st.fil-3][st.col] = terreno[9]; 

			matriz[st.fil][st.col+1] = terreno[3]; 
			matriz[st.fil-1][st.col+1] = terreno[2]; 
			matriz[st.fil-2][st.col+1] = terreno[5]; 
			matriz[st.fil-3][st.col+1] = terreno[10]; 

			matriz[st.fil][st.col+2] = terreno[8]; 
			matriz[st.fil-1][st.col+2] = terreno[7]; 
			matriz[st.fil-2][st.col+2] = terreno[6]; 
			matriz[st.fil-3][st.col+2] = terreno[11]; 

			matriz[st.fil][st.col+3] = terreno[15]; 
			matriz[st.fil-1][st.col+3] = terreno[14]; 
			matriz[st.fil-2][st.col+3] = terreno[13]; 
			matriz[st.fil-3][st.col+3] = terreno[12]; 
		break;
		case este: 
			matriz[st.fil-1][st.col+1] = terreno[1]; 
			matriz[st.fil][st.col+1] = terreno[2]; 
			matriz[st.fil+1][st.col+1] = terreno[3]; 

			matriz[st.fil-2][st.col+2] = terreno[4]; 
			matriz[st.fil-1][st.col+2] = terreno[5]; 
			matriz[st.fil][st.col+2] = terreno[6]; 
			matriz[st.fil+1][st.col+2] = terreno[7]; 
			matriz[st.fil+2][st.col+2] = terreno[8]; 

			matriz[st.fil-3][st.col+3] = terreno[9]; 
			matriz[st.fil-2][st.col+3] = terreno[10]; 
			matriz[st.fil-1][st.col+3] = terreno[11]; 
			matriz[st.fil][st.col+3] = terreno[12]; 
			matriz[st.fil+1][st.col+3] = terreno[13]; 
			matriz[st.fil+2][st.col+3] = terreno[14]; 
			matriz[st.fil+3][st.col+3] = terreno[15]; 
		break;
		case sureste: 
			matriz[st.fil][st.col+1] = terreno[1]; 
			matriz[st.fil][st.col+2] = terreno[4]; 
			matriz[st.fil][st.col+3] = terreno[9]; 

			matriz[st.fil+1][st.col] = terreno[3]; 
			matriz[st.fil+1][st.col+1] = terreno[2]; 
			matriz[st.fil+1][st.col+2] = terreno[5]; 
			matriz[st.fil+1][st.col+3] = terreno[10]; 

			matriz[st.fil+2][st.col] = terreno[8]; 
			matriz[st.fil+2][st.col+1] = terreno[7]; 
			matriz[st.fil+2][st.col+2] = terreno[6]; 
			matriz[st.fil+2][st.col+3] = terreno[11]; 

			matriz[st.fil+3][st.col] = terreno[15]; 
			matriz[st.fil+3][st.col+1] = terreno[14]; 
			matriz[st.fil+3][st.col+2] = terreno[13]; 
			matriz[st.fil+3][st.col+3] = terreno[12];
		break;
		case sur: 
			matriz[st.fil+1][st.col+1] = terreno[1]; 
			matriz[st.fil+1][st.col] = terreno[2]; 
			matriz[st.fil+1][st.col-1] = terreno[3]; 

			matriz[st.fil+2][st.col+2] = terreno[4]; 
			matriz[st.fil+2][st.col+1] = terreno[5]; 
			matriz[st.fil+2][st.col] = terreno[6]; 
			matriz[st.fil+2][st.col-1] = terreno[7]; 
			matriz[st.fil+2][st.col-2] = terreno[8]; 

			matriz[st.fil+3][st.col+3] = terreno[9]; 
			matriz[st.fil+3][st.col+2] = terreno[10]; 
			matriz[st.fil+3][st.col+1] = terreno[11]; 
			matriz[st.fil+3][st.col] = terreno[12]; 
			matriz[st.fil+3][st.col-1] = terreno[13]; 
			matriz[st.fil+3][st.col-2] = terreno[14]; 
			matriz[st.fil+3][st.col-3] = terreno[15]; 
		break;
		case suroeste: 
			matriz[st.fil+1][st.col] = terreno[1]; 
			matriz[st.fil+2][st.col] = terreno[4]; 
			matriz[st.fil+3][st.col] = terreno[9]; 

			matriz[st.fil][st.col-1] = terreno[3]; 
			matriz[st.fil+1][st.col-1] = terreno[2]; 
			matriz[st.fil+2][st.col-1] = terreno[5]; 
			matriz[st.fil+3][st.col-1] = terreno[10]; 

			matriz[st.fil][st.col-2] = terreno[8]; 
			matriz[st.fil+1][st.col-2] = terreno[7]; 
			matriz[st.fil+2][st.col-2] = terreno[6]; 
			matriz[st.fil+3][st.col-2] = terreno[11]; 

			matriz[st.fil][st.col-3] = terreno[15]; 
			matriz[st.fil+1][st.col-3] = terreno[14]; 
			matriz[st.fil+2][st.col-3] = terreno[13]; 
			matriz[st.fil+3][st.col-3] = terreno[12];
		break;
		case oeste: 
			matriz[st.fil+1][st.col-1] = terreno[1]; 
			matriz[st.fil][st.col-1] = terreno[2]; 
			matriz[st.fil-1][st.col-1] = terreno[3]; 

			matriz[st.fil+2][st.col-2] = terreno[4]; 
			matriz[st.fil+1][st.col-2] = terreno[5]; 
			matriz[st.fil][st.col-2] = terreno[6]; 
			matriz[st.fil-1][st.col-2] = terreno[7]; 
			matriz[st.fil-2][st.col-2] = terreno[8]; 

			matriz[st.fil+3][st.col-3] = terreno[9]; 
			matriz[st.fil+2][st.col-3] = terreno[10]; 
			matriz[st.fil+1][st.col-3] = terreno[11]; 
			matriz[st.fil][st.col-3] = terreno[12]; 
			matriz[st.fil-1][st.col-3] = terreno[13]; 
			matriz[st.fil-2][st.col-3] = terreno[14]; 
			matriz[st.fil-3][st.col-3] = terreno[15]; 
		break;
		case noroeste: 
			matriz[st.fil][st.col-1] = terreno[1]; 
			matriz[st.fil][st.col-2] = terreno[4]; 
			matriz[st.fil][st.col-3] = terreno[9]; 

			matriz[st.fil-1][st.col] = terreno[3]; 
			matriz[st.fil-1][st.col-1] = terreno[2]; 
			matriz[st.fil-1][st.col-2] = terreno[5]; 
			matriz[st.fil-1][st.col-3] = terreno[10]; 

			matriz[st.fil-2][st.col] = terreno[8]; 
			matriz[st.fil-2][st.col-1] = terreno[7]; 
			matriz[st.fil-2][st.col-2] = terreno[6]; 
			matriz[st.fil-2][st.col-3] = terreno[11]; 

			matriz[st.fil-3][st.col] = terreno[15]; 
			matriz[st.fil-3][st.col-1] = terreno[14]; 
			matriz[st.fil-3][st.col-2] = terreno[13]; 
			matriz[st.fil-3][st.col-3] = terreno[12];

		break;
	}
}

int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}