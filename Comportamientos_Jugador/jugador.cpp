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
			switch (current_state.brujula){
				case norte: 
					current_state.fil-2; 
				break;
				case noreste: 
					current_state.fil-2;
					current_state.col+2;
				break;
				case este: 
					current_state.col+2; 
				break;
				case sureste: 
					current_state.fil+2;
					current_state.col+2;
				break;
				case sur: 
					current_state.fil+2; 
				break;
				case suroeste: 
					current_state.fil+2;
					current_state.col-2;
				break;
				case oeste: 
					current_state.col-2; 
				break;
				case noroeste: 
					current_state.fil-2;
					current_state.col-2;
				break;
			}
		break;

		case actTURN_SR: //girar 45º a la derecha
			a = current_state.brujula;
			a = (a+1)%8; //modulo 8 ya que hay 8 posibles sentidos
			current_state.brujula = static_cast<Orientacion>(a);
		break;

		case actTURN_L: //girar 90º a la izquierda
			a = current_state.brujula;
			a = (a+6)%8; //equivalente a (a-1+8)%8 y (a-1)%8 pero no usamos num neg
			current_state.brujula = static_cast<Orientacion>(a);
		break;
	}


	//Definicion de reglas:
	

		//CASILLA EN LA QUE NOS ENCONTRAMOS:
		if( sensores.terreno[0] == 'G' && !bien_situado){ 
			current_state.fil=sensores.posF;
			current_state.col=sensores.posC;
			current_state.brujula=sensores.sentido;
			bien_situado=true;
		}
		
		//Si está en una cas de pos estará bien situado, podemos registrar lo que ve el jugador en nuestro mapaResultado
		if(bien_situado){ 
			PonerTerrenoEnMatriz(sensores.terreno, current_state, mapaResultado);	
		}

		if(sensores.terreno[0]='K') bikini=true;
		else if(sensores.terreno[0]='D') zapatillas=true;
		else if(sensores.terreno[0]='X') recarga(sensores);

		//RESETEO:
		if(sensores.bateria==0) reset(sensores);

		//PROXIMO MOVIMIENTO
		//ir a casillas directas si hay objeto o necesitamos recarga
	
			int i=1;
			bool prioridad=false;
			while(i<16 && prioridad==false){
				if((sensores.terreno[i]=='K' && !bikini)||(sensores.terreno[i]=='D' && !zapatillas) || (sensores.terreno[i]=='X')){
					if(transitable(i,sensores)){
						moverHacia(i);
						prioridad=true;
					}
				}
				i++;
			}
			if(!prioridad || last_action==actIDLE){
				if(transitable(2,sensores)) accion=actWALK;
				else if(transitable(6,sensores)) accion=actRUN;
				
				if(!transitable(2,sensores)){
					if(girar_derecha){
						accion=actTURN_SR;
						girar_derecha=(rand()%2==0);
					}else{	
						accion=actTURN_L;
						girar_derecha=(rand()%2==0);
					} 
				}

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

void ComportamientoJugador::reset(Sensores sensores){
	bikini = false;
	zapatillas = false;
	sensores.bateria = 5000;
}

void ComportamientoJugador::recarga(Sensores sensores){
	if(sensores.bateria<=4990){
		sensores.bateria +=10;
	}
}


int ComportamientoJugador::casPos(Sensores sensores){
	int pos=-1, i=1;

	while(pos!=-1 || i==16){
		if(sensores.terreno[i]=='G' && i!=16) pos=i;
		i++;
	}

	return pos;
}

Action ComportamientoJugador::moverHacia(int a){
	Action accion = actIDLE;

	if(a==2) accion=actWALK;
	else if(a==6||a==12) accion=actRUN;
	else if(a==1||a==4||a==5||a==10||a==11) accion=actTURN_L;
	else if(a==3||a==7||a==8||a==14||a==15) accion=actTURN_SR;
	
	return accion;
}



Action ComportamientoJugador::siguienteAccion(vector<char> linea){
	int a=0;
	Action accion = actIDLE;
	vector<int> posibles;
	for(int j=0; j<linea.size(); j++){
		if(linea[j]!='_') posibles.push_back(j+1);
	}
		
	a = 1+rand() % (posibles.size()+1);
	accion = moverHacia(a);
		
	return accion;
}



bool ComportamientoJugador::transitable(int num, Sensores sensores){
	if((sensores.terreno[num]=='T' || sensores.terreno[num]=='S' || sensores.terreno[num]=='G' ||sensores.terreno[num]=='K' || sensores.terreno[num]=='D' || sensores.terreno[num]=='X' ) && (sensores.agentes[num]='_' )){ 
		return true;
	}else return false;
}



// extiende esta version inicial donde solo se pone la componente 0 
// en la matriz; la nueva version debe poner todas las componentes 
// de terreno en funcion de la orientacion del agente
void ComportamientoJugador::PonerTerrenoEnMatriz(const vector<unsigned char> & terreno, const state &st, vector<vector<unsigned char>> &matriz){
	//sensores.terreno -> terreno
	//current_state -> st 
	//mapaResultado -> matriz

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

//PROXIMO MOVIMIENTO:

		/*
		vector<char> linea1;
		for(int i=0; i<3; i++) linea1.push_back('_');
		vector<char> linea2;
		vector<char> linea3;
		

		for(int i=1; i<4; i++){
			if(sensores.terreno[i]=='K' && !bikini) linea1.push_back('K');
			else if(sensores.terreno[i]=='D' && !zapatillas) linea1.push_back('D');
			else if(sensores.terreno[i]=='X' ) linea1.push_back('X');
		}

		if(linea1.empty()){
			for(int i=0; i<5; i++) linea2.push_back('_');
			for(int i=4; i<9; i++){
				if(sensores.terreno[i]=='K' && !bikini) linea2.push_back('K');
				else if(sensores.terreno[i]=='D' && !zapatillas)  linea2.push_back('D');
				else if(sensores.terreno[i]=='X' )  linea2.push_back('X');
			}
		}else{
			accion=siguienteAccion(linea1);
		}

		if(linea1.empty() && linea2.empty()){
			for(int i=0; i<6; i++) linea3.push_back('_');
			for(int i=9; i<16; i++){
				if(sensores.terreno[i]=='K' && !bikini) linea3.push_back('K');
				else if(sensores.terreno[i]=='D' && !zapatillas)  linea3.push_back('D');
				else if(sensores.terreno[i]=='X' )  linea3.push_back('X');
			}
		}else{
			accion=siguienteAccion(linea2);
		}

		if(linea1.empty() && linea2.empty() && !linea3.empty() ){
			accion=siguienteAccion(linea3);
		}
		else if(linea1.empty() && linea2.empty() && linea3.empty() && casPos(sensores)!=-1){
			accion=moverHacia(casPos(sensores));
		}


		if(accion==actIDLE){
			if(transitable(2,sensores)) accion=actWALK;
		}else if(!girar_derecha){
			accion=actTURN_L;
			girar_derecha=(rand()%2==0);
		}else{
			accion=actTURN_SR;
			girar_derecha=(rand()%2==0);
		}*/

int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}