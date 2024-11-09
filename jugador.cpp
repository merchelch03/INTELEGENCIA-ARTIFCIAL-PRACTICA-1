#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
#include <cstdlib> //para los numeros aleatorios
using namespace std;

Action ComportamientoJugador::think(Sensores sensores)
{

	Action accion = actIDLE;

    cout << contador << endl;
    cout << sensores.vida << endl;
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

        //Actualizar al mundo con la ultima accion
        switch (last_action){
            case actWALK:
                switch (current_state.brujula) {
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
            case actRUN: //Para avanzar dos casillas seguidas
                switch (current_state.brujula) {
                    case norte:
                        current_state.fil=current_state.fil-2;
                        break;
                    case noreste:
                        current_state.fil=current_state.fil-2;
                        current_state.col=current_state.col+2;
                        break;
                    case este:
                        current_state.col=current_state.col+2;
                        break;
                    case sureste:
                        current_state.fil++;
                        current_state.col=current_state.col+2;
                        break;
                    case sur:
                        current_state.fil=current_state.fil+2;
                        break;
                    case suroeste:
                        current_state.fil=current_state.fil+2;
                        current_state.col=current_state.col-2;
                        break;
                    case oeste:
                        current_state.col=current_state.col-2;
                        break;
                    case noroeste:
                        current_state.fil=current_state.fil-2;
                        current_state.col=current_state.col-2;
                        break;
                }
                break;
            case actTURN_SR: //GIRA 45 GRADOS
                a=current_state.brujula;
                a=(a+1)%8;
                current_state.brujula=static_cast<Orientacion>(a);
                break;
            case actTURN_L: //GIRA 45 IZQUIERDA
                a=current_state.brujula;
                a=(a+6)%8;
                current_state.brujula=static_cast<Orientacion>(a);
                break;
        }


    //CON ESTO SABEMOS LA POSICION QUE TENEMOS EN CADA MOMENTO EN EL NIVEL O
    if(sensores.nivel==0){
        current_state.fil = sensores.posF;
        current_state.col = sensores.posC;
        current_state.brujula = sensores.sentido;
        bien_situado = true;
    }
    //LO QUE PASA SI MORIMOS, ES DECIR NOS LLEVA UN LOBO O UN PRECIPICIO
    if(sensores.reset){
        reiniciamos();
        contador++;
        cout << "REINICIAR" <<endl;
    }



    //VEMOS QUE PASA EN LA POSICION ACTUAL DEL JUGADOR
    //CON ESTO YA SABEMOS LA POSICION QUE TENEMOS EN EL NIVEL POSTERIOR A 0
    if (sensores.terreno[0] == 'G' and !bien_situado and (sensores.sentido==norte or sensores.sentido==sur or sensores.sentido==este or sensores.sentido==oeste)) {
        current_state.fil = sensores.posF;
        current_state.col = sensores.posC;
        current_state.brujula = sensores.sentido;
        bien_situado = true;
        cout << current_state.fil << " " << current_state.col << " " << current_state.brujula << endl;
    }
    if(sensores.terreno[0]=='K' and !bikini){
        bikini=true;
    }
    if(sensores.terreno[0]=='D' and !zapatillas){
        zapatillas=true;
    }
    if(sensores.terreno[0]=='X' and sensores.bateria<max_bateria_pre and sensores.vida>sensores.bateria){
        recargando=true;
    }

    //FUNCION PARA ACTUALIZAR MAPA
    if(bien_situado){
        actualizar_mapa(sensores);
    }

    //COMO NOS MOVEMOS Y LO VAMOS HA HACER CON PRIORIDAD PAR AENCONTRAR OBJETOS
    //primero si no estamos en el nivel 0 hay que encontrar lo antes posible G
    if(sensores.nivel>0 and !bien_situado and !prioridad and !recargando){
        pos_g=encontrar_G(sensores);
        if(pos_g>=0){
            prioridad=true;
            accion=ir(pos_g, sensores);
        }
    }
    if(!prioridad and !recargando and rodeado_muros(sensores)){
        accion= roderas_muros_a(sensores);
        prioridad=true;
    }
    if(!bikini and !prioridad and !recargando){
        pos_k=encontrar_bikini(sensores);
        if(pos_k>=0){
            prioridad=true;
            accion=ir(pos_k, sensores);
            cout << "BIKINI" << endl;
        }
    }
    //hay que encontrar bikinis y zapatilla (primero bikini que agua gasta mas que bosque)
    if(!zapatillas and !prioridad and !recargando){
        pos_d=encontrar_zapatillas(sensores);
        if(pos_d>=0){
            prioridad=true;
            accion=ir(pos_d, sensores);
            cout << "ZAPATILLAS" << endl;
        }
    }


    if(!prioridad and !recargando and bien_situado){
        //IR POR MUROS
        if(mirar_salida_iz and hay_salida_muro_iz(sensores)){
            accion=actTURN_L;
            cout << "GIRAR IZQUIERDA" << endl;
            mirar_salida_iz=false;
            prioridad=true;
        }
        if(mirar_salida_de and hay_salida_muro_de(sensores)){
            accion=actTURN_SR;
            cout << "GIRAR DERECHA" << endl;
            mirar_salida_de=false;
            prioridad=true;
        }
        if(hay_camino_muro_de(sensores)){
            accion=actWALK;
            cout << "CAMINAR" << endl;
            prioridad=true;
            mirar_salida_de=true;
        }
        if(hay_camino_muro_iz(sensores)){
            accion=actWALK;
            cout << "CAMINAR" << endl;
            prioridad=true;
            mirar_salida_iz=true;
        }
       /* if(rand() % 10 == 5){
            accion=aleatorio_i_d(sensores);
        }*/
    }
    if(recargando){//esta en una casilla para recargar bateria
        accion=actIDLE;
        cout << "RECARGANDO" << endl;
    }



    if (!prioridad and !recargando){
        accion= mover(sensores);
        cout << "MOVER" << endl;
    }

    prioridad=false;
    if(sensores.bateria==max_bateria_pre or sensores.vida<=sensores.bateria){
        recargando=false;
    }

    last_action=accion;
	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}

//
//OTRAS FUNCIONES
//

//estas dos funciones nos van a servir tanto para rodear muros como precipicios
Action ComportamientoJugador::roderas_muros_a(Sensores sensores) {
    Action accion= mover(sensores);
    if(sensores.terreno[1]=='M' and sensores.terreno[5]=='M' and sensores.terreno[6]=='M' and sensores.terreno[3]!='M'){
        accion=actTURN_SR;
    }
    if(sensores.terreno[1]!='M' and sensores.terreno[3]=='M' and sensores.terreno[7]=='M' and sensores.terreno[5]=='M'){
        accion=actTURN_L;
    }
    if(sensores.terreno[1]=='M' and sensores.terreno[3]=='M' and sensores.terreno[2]=='M'){
        accion= aleatorio_i_d(sensores);
    }
    if(sensores.terreno[1]=='M' and comprobar_posicion(sensores, 3) and sensores.terreno[6]=='P' and sensores.terreno[5]=='P'){
        accion=actTURN_SR;
    }
    if(sensores.terreno[3]=='M' and comprobar_posicion(sensores, 1) and sensores.terreno[7]=='P' and sensores.terreno[6]=='P'){
        accion=actTURN_L;
    }
    /*if(comprobar_posicion(sensores, 1) and comprobar_posicion(sensores, 2) and comprobar_posicion(sensores, 3) and comprobar_posicion(sensores, 7) and sensores.terreno[5]=='P' and sensores.terreno[11]=='P'){
        accion=actWALK;
    }
    if(sensores.terreno[1]=='P' and comprobar_posicion(sensores, 2) and comprobar_posicion(sensores, 3) and comprobar_posicion(sensores, 7) and sensores.terreno[5]=='P' and sensores.terreno[11]=='P'){
        accion=actWALK;
    }
    if(comprobar_posicion(sensores, 1) and comprobar_posicion(sensores, 2) and comprobar_posicion(sensores, 3) and comprobar_posicion(sensores, 7) and comprobar_posicion(sensores, 5) and sensores.terreno[11]=='P'){
        accion=actWALK;
    }*/
    return accion;
}

bool ComportamientoJugador::rodeado_muros(Sensores sensores) {
    bool muros=false;
    if(sensores.terreno[1]=='M' and sensores.terreno[5]=='M' and sensores.terreno[6]=='M' and comprobar_posicion(sensores, 3)){
        muros=true;
    }
    if(comprobar_posicion(sensores, 1) and sensores.terreno[3]=='M' and sensores.terreno[7]=='M' and sensores.terreno[5]=='M'){
        muros=true;
    }
    if(sensores.terreno[1]=='M' and sensores.terreno[3]=='M' and sensores.terreno[2]=='M'){
        muros=true;
        cout << "HACIENDO" << endl;
    }
    //los dos siguientes para cuando esta el muro y el precipio, para que se ponga a recorrer el muro todo el rato
    if(sensores.terreno[1]=='M' and comprobar_posicion(sensores, 3) and sensores.terreno[6]=='P' and sensores.terreno[5]=='P'){
        muros=true;
    }
    if(sensores.terreno[3]=='M' and comprobar_posicion(sensores, 1) and sensores.terreno[7]=='P' and sensores.terreno[6]=='P'){
        muros=true;
    }
    /*para precipicios
    if(comprobar_posicion(sensores, 1) and comprobar_posicion(sensores, 2) and comprobar_posicion(sensores, 3) and comprobar_posicion(sensores, 7) and sensores.terreno[5]=='P' and sensores.terreno[11]=='P'){
        muros=true;
    }
    if(sensores.terreno[1]=='P' and comprobar_posicion(sensores, 2) and comprobar_posicion(sensores, 3) and comprobar_posicion(sensores, 7) and sensores.terreno[5]=='P' and sensores.terreno[11]=='P'){
        muros=true;
    }
    if(comprobar_posicion(sensores, 1) and comprobar_posicion(sensores, 2) and comprobar_posicion(sensores, 3) and comprobar_posicion(sensores, 7) and comprobar_posicion(sensores, 5) and sensores.terreno[11]=='P'){
        muros=true;
    }*/
    return muros;
}

bool ComportamientoJugador::hay_salida_muro_de(Sensores sensores){
    bool camino=false;
    if(comprobar_posicion(sensores, 3) and sensores.terreno[7]=='M' and comprobar_posicion(sensores, 2)){
        camino=true;
    }
    return camino;
}

bool ComportamientoJugador::hay_salida_muro_iz(Sensores sensores){
    bool camino=false;
    if(comprobar_posicion(sensores, 1) and sensores.terreno[5]=='M' and comprobar_posicion(sensores, 2)){
        camino=true;
    }
    return camino;
}

bool ComportamientoJugador::hay_camino_muro_iz(Sensores sensores){
    bool camino=false;
    if(sensores.terreno[1]=='M' and sensores.terreno[3]!='M' and comprobar_posicion(sensores, 2) and sensores.terreno[6]!='M'){
        camino=true;
    }
    return camino;
}

bool ComportamientoJugador::hay_camino_muro_de(Sensores sensores){
    bool camino=false;
    if(sensores.terreno[3]=='M' and sensores.terreno[1]!='M' and comprobar_posicion(sensores, 2) and sensores.terreno[6]!='M'){
        camino=true;
    }
    return camino;
}

Action ComportamientoJugador::aleatorio_i_d(Sensores sensores){
    Action accion=actIDLE;
    int n=rand() % 10;
    if (n>=0 and n<=5){
        accion=actTURN_L;
    }else if (n>=5 and n<=10){
        accion=actTURN_SR;
    }
    return accion;
}

//Movimiento normal
Action ComportamientoJugador::mover(Sensores sensores){
    Action accion=actIDLE;
    int n=rand() % 10;
    if (n==5){
        accion= aleatorio_i_d(sensores);
    }else{
        if(seguir_alfrente(sensores)){
            accion=actWALK;
        }
        if(!seguir_alfrente(sensores) and !comprobar_posicion(sensores,1) and comprobar_posicion(sensores, 3)){
            accion=actTURN_SR;
        }
        if(!seguir_alfrente(sensores) and comprobar_posicion(sensores,1) and !comprobar_posicion(sensores, 3)){
            accion=actTURN_L;
        }
        if(!seguir_alfrente(sensores) and !comprobar_posicion(sensores,1) and !comprobar_posicion(sensores, 3)){
            accion=actTURN_L;
        }
        //aleatorio si girar izquierda o derecha si en frente no se puede
        if(!seguir_alfrente(sensores) and comprobar_posicion(sensores,1) and comprobar_posicion(sensores, 3)){
            accion=aleatorio_i_d(sensores);
        }

    }

    //PARTE DEL MAPA QUE SIEMPRE ES IGUAL (preciucios de los bordes)
    for(int i=0;i<3;i++){
        for (int j = 0; j < tam_mapa; ++j) {
            mapaResultado[i][j]='P';
        }
    }
    for(int i=0;i<tam_mapa;i++){
        for (int j = 0; j < 3; ++j) {
            mapaResultado[i][j]='P';
        }
    }
    for(int i=0;i<3;i++){
        for (int j = 0; j < tam_mapa; ++j) {
            mapaResultado[i][j]='P';
        }
    }
    for(int i=0;i<tam_mapa;i++){
        for (int j = 0; j < 3; ++j) {
            mapaResultado[i][j]='P';
        }
    }   for(int i=tam_mapa-3;i<tam_mapa;i++){
        for (int j = 0; j < tam_mapa; ++j) {
            mapaResultado[i][j]='P';
        }
    }
    for(int j = 0; j < tam_mapa; ++j){
        for (int i=tam_mapa-3;i<tam_mapa;i++){
            mapaResultado[j][i]='P';
        }
    }


    return accion;
}


//Mover a una posicion determinada para objetos
Action ComportamientoJugador::ir(int i, Sensores sensores) {
    Action accion;
    switch (i) {
        case 2: case 5: case 7: case 10: case 11: case 14:
            if(comprobar_posicion(sensores, 2)){
                accion=actWALK;
            }else{
                accion=mover(sensores);
            }
            break;
        case 6: case 12: case 13:
            if(avanzar_actrun(sensores)){
                accion=actRUN;
            }else if(comprobar_posicion(sensores, 2)){
                accion=actWALK;
            }else{
                accion=mover(sensores);
            }
            break;
        case 1: case 4: case 9:
            if(comprobar_posicion(sensores, 1)){
                accion=actTURN_L;
            }else{
                accion=mover(sensores);
            }
            break;
        case 3: case 8: case 15:
            if(comprobar_posicion(sensores, 3)){
                accion=actTURN_SR;
            }else{
                accion=mover(sensores);
            }
            break;
        default:
            accion=mover(sensores);
            break;
    }
    return accion;
}

int ComportamientoJugador::encontrar_G(Sensores sensores) {
    int posi=-1;//quiere decir que no hay
    for(int i=0;i<tam_sterreno;i++){
        if(sensores.terreno[i]=='G'){
            posi=i;//esta es la posicion i
        }
    }
    return posi;
}
int ComportamientoJugador::encontrar_bikini(Sensores sensores){
    int posi=-1;//quiere decir que no hay
    for(int i=0;i<tam_sterreno;i++){
        if(sensores.terreno[i]=='K'){
            posi=i;//esta es la posicion i
        }
    }
    return posi;
}
int ComportamientoJugador::encontrar_zapatillas(Sensores sensores){
    int posi=-1;//quiere decir que no hay
    for(int i=0;i<tam_sterreno;i++){
        if(sensores.terreno[i]=='D'){
            posi=i;//esta es la posicion i
        }
    }
    return posi;
}

bool ComportamientoJugador::seguir_alfrente(Sensores sensores){
    bool seguir=false;
    if (!bikini and !zapatillas){
        if(sensores.agentes[2]=='_' and (sensores.terreno[2]=='S' or sensores.terreno[2]=='T' or sensores.terreno[2]=='G' or sensores.terreno[2]=='X' or sensores.terreno[2]=='K' or  sensores.terreno[2]=='D')){
            seguir=true;
        }
    }
    if (!bikini and zapatillas){
        if(sensores.agentes[2]=='_' and (sensores.terreno[2]=='S' or sensores.terreno[2]=='T' or sensores.terreno[2]=='G' or sensores.terreno[2]=='X' or sensores.terreno[2]=='K' or  sensores.terreno[2]=='D' or sensores.terreno[2]=='B')){
            seguir=true;
        }
    }
    if (bikini and !zapatillas){
        if(sensores.agentes[2]=='_' and (sensores.terreno[2]=='S' or sensores.terreno[2]=='T' or sensores.terreno[2]=='G' or sensores.terreno[2]=='X' or sensores.terreno[2]=='K' or  sensores.terreno[2]=='D' or sensores.terreno[2]=='A')){
            seguir=true;
        }
    }
    if (bikini and zapatillas){
        if(sensores.agentes[2]=='_' and (sensores.terreno[2]=='S' or sensores.terreno[2]=='T' or sensores.terreno[2]=='G' or sensores.terreno[2]=='X' or sensores.terreno[2]=='K' or  sensores.terreno[2]=='D' or sensores.terreno[2]=='A' or sensores.terreno[2]=='B')){
            seguir=true;
        }
    }
    //situaciones extrañas
    if (!zapatillas){
        if(sensores.agentes[2]=='_' and (sensores.terreno[2]=='B' and sensores.terreno[1]=='B' and sensores.terreno[3]=='B')){
            seguir=true;
        }
    }
    if (!bikini){
        if(sensores.agentes[2]=='_' and (sensores.terreno[2]=='A' and sensores.terreno[6]=='A' and comprobar_posicion(sensores, 12))){
            seguir=true;
        }
    }
    if (!bikini){
        if(sensores.agentes[2]=='_' and (sensores.terreno[2]=='A' and  comprobar_posicion(sensores, 6))){
            seguir=true;
        }
    }
    if(sensores.agentes[2]=='_' and (sensores.terreno[1]=='P' and sensores.terreno[5]=='P' and sensores.terreno[3]=='M' and sensores.terreno[7]=='M' and  comprobar_posicion(sensores, 2))){
        seguir=true;
    }
    if(sensores.agentes[2]=='_' and (sensores.terreno[3]=='P' and sensores.terreno[7]=='P' and sensores.terreno[1]=='M' and sensores.terreno[5]=='M' and  comprobar_posicion(sensores, 2))){
        seguir=true;
    }
    /*if (!bikini){//esto me jode para otro mapa
        if(sensores.agentes[2]=='_' and (sensores.terreno[2]=='A' and sensores.terreno[1]=='P' and sensores.terreno[3]=='A')){
            seguir=true;
        }
        if(sensores.agentes[2]=='_' and (sensores.terreno[2]=='A' and sensores.terreno[1]=='P' and sensores.terreno[3]=='T')){
            seguir=true;
        }
        if(sensores.agentes[2]=='_' and (sensores.terreno[2]=='A' and sensores.terreno[1]=='A' and sensores.terreno[3]=='P')){
            seguir=true;
        }
    }*/
    return seguir;
}

bool ComportamientoJugador::comprobar_posicion(Sensores sensores, int posicion) {
    bool seg=false;
    if (!bikini and !zapatillas){
        if(sensores.agentes[posicion]=='_' and (sensores.terreno[posicion]=='S' or sensores.terreno[posicion]=='T' or sensores.terreno[posicion]=='G' or sensores.terreno[posicion]=='X' or sensores.terreno[2]=='K' or  sensores.terreno[2]=='D')){
            seg=true;
        }
    }
    if (!bikini and zapatillas){
        if(sensores.agentes[posicion]=='_' and (sensores.terreno[posicion]=='S' or sensores.terreno[posicion]=='T' or sensores.terreno[posicion]=='G' or sensores.terreno[posicion]=='X' or sensores.terreno[2]=='K' or  sensores.terreno[2]=='D' or sensores.terreno[2]=='B')){
            seg=true;
        }
    }
    if (bikini and !zapatillas){
        if(sensores.agentes[posicion]=='_' and (sensores.terreno[posicion]=='S' or sensores.terreno[posicion]=='T' or sensores.terreno[posicion]=='G' or sensores.terreno[posicion]=='X' or sensores.terreno[2]=='K' or  sensores.terreno[2]=='D' or sensores.terreno[2]=='A')){
            seg=true;
        }
    }
    if (bikini and zapatillas){
        if(sensores.agentes[posicion]=='_' and (sensores.terreno[posicion]=='S' or sensores.terreno[posicion]=='T' or sensores.terreno[posicion]=='G' or sensores.terreno[posicion]=='X' or sensores.terreno[2]=='K' or  sensores.terreno[2]=='D' or sensores.terreno[2]=='A' or sensores.terreno[2]=='B')){
            seg=true;
        }
    }

    return seg;
}

bool ComportamientoJugador::avanzar_actrun(Sensores sensores){
    bool dos_casillas=false;
    int p=6;
    if(seguir_alfrente(sensores) and comprobar_posicion(sensores, p)){
        dos_casillas=true;
    }
    return dos_casillas;
}

void ComportamientoJugador::reiniciamos(){
    current_state.brujula=norte;
    current_state.fil=22;
    current_state.col=10;
    bikini=false;
    zapatillas=false;
    bien_situado=false;
}

void ComportamientoJugador::actualizar_mapa(Sensores sensores){
    switch (current_state.brujula) {
        case norte:
            mapaResultado[current_state.fil][current_state.col] = sensores.terreno[0];
            mapaResultado[current_state.fil-1][current_state.col-1] = sensores.terreno[1];
            mapaResultado[current_state.fil-1][current_state.col] = sensores.terreno[2];
            mapaResultado[current_state.fil-1][current_state.col+1] = sensores.terreno[3];
            mapaResultado[current_state.fil-2][current_state.col-2] = sensores.terreno[4];
            mapaResultado[current_state.fil-2][current_state.col-1] = sensores.terreno[5];
            mapaResultado[current_state.fil-2][current_state.col] = sensores.terreno[6];
            mapaResultado[current_state.fil-2][current_state.col+1] = sensores.terreno[7];
            mapaResultado[current_state.fil-2][current_state.col+2] = sensores.terreno[8];
            mapaResultado[current_state.fil-3][current_state.col-3] = sensores.terreno[9];
            mapaResultado[current_state.fil-3][current_state.col-2] = sensores.terreno[10];
            mapaResultado[current_state.fil-3][current_state.col-1] = sensores.terreno[11];
            mapaResultado[current_state.fil-3][current_state.col] = sensores.terreno[12];
            mapaResultado[current_state.fil-3][current_state.col+1] = sensores.terreno[13];
            mapaResultado[current_state.fil-3][current_state.col+2] = sensores.terreno[14];
            mapaResultado[current_state.fil-3][current_state.col+3] = sensores.terreno[15];
            break;
        case sur:
            mapaResultado[current_state.fil][current_state.col] = sensores.terreno[0];
            mapaResultado[current_state.fil+1][current_state.col+1] = sensores.terreno[1];
            mapaResultado[current_state.fil+1][current_state.col] = sensores.terreno[2];
            mapaResultado[current_state.fil+1][current_state.col-1] = sensores.terreno[3];
            mapaResultado[current_state.fil+2][current_state.col+2] = sensores.terreno[4];
            mapaResultado[current_state.fil+2][current_state.col+1] = sensores.terreno[5];
            mapaResultado[current_state.fil+2][current_state.col] = sensores.terreno[6];
            mapaResultado[current_state.fil+2][current_state.col-1] = sensores.terreno[7];
            mapaResultado[current_state.fil+2][current_state.col-2] = sensores.terreno[8];
            mapaResultado[current_state.fil+3][current_state.col+3] = sensores.terreno[9];
            mapaResultado[current_state.fil+3][current_state.col+2] = sensores.terreno[10];
            mapaResultado[current_state.fil+3][current_state.col+1] = sensores.terreno[11];
            mapaResultado[current_state.fil+3][current_state.col] = sensores.terreno[12];
            mapaResultado[current_state.fil+3][current_state.col-1] = sensores.terreno[13];
            mapaResultado[current_state.fil+3][current_state.col-2] = sensores.terreno[14];
            mapaResultado[current_state.fil+3][current_state.col-3] = sensores.terreno[15];
            break;
        case este:
            mapaResultado[current_state.fil][current_state.col] = sensores.terreno[0];
            mapaResultado[current_state.fil-1][current_state.col+1] = sensores.terreno[1];
            mapaResultado[current_state.fil][current_state.col+1] = sensores.terreno[2];
            mapaResultado[current_state.fil+1][current_state.col+1] = sensores.terreno[3];
            mapaResultado[current_state.fil-2][current_state.col+2] = sensores.terreno[4];
            mapaResultado[current_state.fil-1][current_state.col+2] = sensores.terreno[5];
            mapaResultado[current_state.fil][current_state.col+2] = sensores.terreno[6];
            mapaResultado[current_state.fil+1][current_state.col+2] = sensores.terreno[7];
            mapaResultado[current_state.fil+2][current_state.col+2] = sensores.terreno[8];
            mapaResultado[current_state.fil-3][current_state.col+3] = sensores.terreno[9];
            mapaResultado[current_state.fil-2][current_state.col+3] = sensores.terreno[10];
            mapaResultado[current_state.fil-1][current_state.col+3] = sensores.terreno[11];
            mapaResultado[current_state.fil][current_state.col+3] = sensores.terreno[12];
            mapaResultado[current_state.fil+1][current_state.col+3] = sensores.terreno[13];
            mapaResultado[current_state.fil+2][current_state.col+3] = sensores.terreno[14];
            mapaResultado[current_state.fil+3][current_state.col+3] = sensores.terreno[15];
            break;
        case oeste:
            mapaResultado[current_state.fil][current_state.col] = sensores.terreno[0];
            mapaResultado[current_state.fil+1][current_state.col-1] = sensores.terreno[1];
            mapaResultado[current_state.fil][current_state.col-1] = sensores.terreno[2];
            mapaResultado[current_state.fil-1][current_state.col-1] = sensores.terreno[3];
            mapaResultado[current_state.fil+2][current_state.col-2] = sensores.terreno[4];
            mapaResultado[current_state.fil+1][current_state.col-2] = sensores.terreno[5];
            mapaResultado[current_state.fil][current_state.col-2] = sensores.terreno[6];
            mapaResultado[current_state.fil-1][current_state.col-2] = sensores.terreno[7];
            mapaResultado[current_state.fil-2][current_state.col-2] = sensores.terreno[8];
            mapaResultado[current_state.fil+3][current_state.col-3] = sensores.terreno[9];
            mapaResultado[current_state.fil+2][current_state.col-3] = sensores.terreno[10];
            mapaResultado[current_state.fil+1][current_state.col-3] = sensores.terreno[11];
            mapaResultado[current_state.fil][current_state.col-3] = sensores.terreno[12];
            mapaResultado[current_state.fil-1][current_state.col-3] = sensores.terreno[13];
            mapaResultado[current_state.fil-2][current_state.col-3] = sensores.terreno[14];
            mapaResultado[current_state.fil-3][current_state.col-3] = sensores.terreno[15];
            break;
        case noreste:
            mapaResultado[current_state.fil][current_state.col] = sensores.terreno[0];
            mapaResultado[current_state.fil-1][current_state.col] = sensores.terreno[1];
            mapaResultado[current_state.fil-1][current_state.col+1] = sensores.terreno[2];
            mapaResultado[current_state.fil][current_state.col+1] = sensores.terreno[3];
            mapaResultado[current_state.fil-2][current_state.col] = sensores.terreno[4];
            mapaResultado[current_state.fil-2][current_state.col+1] = sensores.terreno[5];
            mapaResultado[current_state.fil-2][current_state.col+2] = sensores.terreno[6];
            mapaResultado[current_state.fil-1][current_state.col+2] = sensores.terreno[7];
            mapaResultado[current_state.fil][current_state.col+2] = sensores.terreno[8];
            mapaResultado[current_state.fil-3][current_state.col] = sensores.terreno[9];
            mapaResultado[current_state.fil-3][current_state.col+1] = sensores.terreno[10];
            mapaResultado[current_state.fil-3][current_state.col+2] = sensores.terreno[11];
            mapaResultado[current_state.fil-3][current_state.col+3] = sensores.terreno[12];
            mapaResultado[current_state.fil-2][current_state.col+3] = sensores.terreno[13];
            mapaResultado[current_state.fil-1][current_state.col+3] = sensores.terreno[14];
            mapaResultado[current_state.fil][current_state.col+3] = sensores.terreno[15];
            break;
        case sureste:
            mapaResultado[current_state.fil][current_state.col] = sensores.terreno[0];
            mapaResultado[current_state.fil][current_state.col+1] = sensores.terreno[1];
            mapaResultado[current_state.fil+1][current_state.col+1] = sensores.terreno[2];
            mapaResultado[current_state.fil+1][current_state.col] = sensores.terreno[3];
            mapaResultado[current_state.fil][current_state.col+2] = sensores.terreno[4];
            mapaResultado[current_state.fil+1][current_state.col+2] = sensores.terreno[5];
            mapaResultado[current_state.fil+2][current_state.col+2] = sensores.terreno[6];
            mapaResultado[current_state.fil+2][current_state.col+1] = sensores.terreno[7];
            mapaResultado[current_state.fil+2][current_state.col] = sensores.terreno[8];
            mapaResultado[current_state.fil][current_state.col+3] = sensores.terreno[9];
            mapaResultado[current_state.fil+1][current_state.col+3] = sensores.terreno[10];
            mapaResultado[current_state.fil+2][current_state.col+3] = sensores.terreno[11];
            mapaResultado[current_state.fil+3][current_state.col+3] = sensores.terreno[12];
            mapaResultado[current_state.fil+3][current_state.col+2] = sensores.terreno[13];
            mapaResultado[current_state.fil+3][current_state.col+1] = sensores.terreno[14];
            mapaResultado[current_state.fil+3][current_state.col] = sensores.terreno[15];
            break;
        case noroeste:
            mapaResultado[current_state.fil][current_state.col] = sensores.terreno[0];
            mapaResultado[current_state.fil][current_state.col-1] = sensores.terreno[1];
            mapaResultado[current_state.fil-1][current_state.col-1] = sensores.terreno[2];
            mapaResultado[current_state.fil-1][current_state.col] = sensores.terreno[3];
            mapaResultado[current_state.fil][current_state.col-2] = sensores.terreno[4];
            mapaResultado[current_state.fil-1][current_state.col-2] = sensores.terreno[5];
            mapaResultado[current_state.fil-2][current_state.col-2] = sensores.terreno[6];
            mapaResultado[current_state.fil-2][current_state.col-1] = sensores.terreno[7];
            mapaResultado[current_state.fil-2][current_state.col] = sensores.terreno[8];
            mapaResultado[current_state.fil][current_state.col-3] = sensores.terreno[9];
            mapaResultado[current_state.fil-1][current_state.col-3] = sensores.terreno[10];
            mapaResultado[current_state.fil-2][current_state.col-3] = sensores.terreno[11];
            mapaResultado[current_state.fil-3][current_state.col-3] = sensores.terreno[12];
            mapaResultado[current_state.fil-3][current_state.col-2] = sensores.terreno[13];
            mapaResultado[current_state.fil-3][current_state.col-1] = sensores.terreno[14];
            mapaResultado[current_state.fil-3][current_state.col] = sensores.terreno[15];
            break;
        case suroeste:
            mapaResultado[current_state.fil][current_state.col] = sensores.terreno[0];
            mapaResultado[current_state.fil+1][current_state.col] = sensores.terreno[1];
            mapaResultado[current_state.fil+1][current_state.col-1] = sensores.terreno[2];
            mapaResultado[current_state.fil][current_state.col-1] = sensores.terreno[3];
            mapaResultado[current_state.fil+2][current_state.col] = sensores.terreno[4];
            mapaResultado[current_state.fil+2][current_state.col-1] = sensores.terreno[5];
            mapaResultado[current_state.fil+2][current_state.col-2] = sensores.terreno[6];
            mapaResultado[current_state.fil+1][current_state.col-2] = sensores.terreno[7];
            mapaResultado[current_state.fil][current_state.col-2] = sensores.terreno[8];
            mapaResultado[current_state.fil+3][current_state.col] = sensores.terreno[9];
            mapaResultado[current_state.fil+3][current_state.col-1] = sensores.terreno[10];
            mapaResultado[current_state.fil+3][current_state.col-2] = sensores.terreno[11];
            mapaResultado[current_state.fil+3][current_state.col-3] = sensores.terreno[12];
            mapaResultado[current_state.fil+2][current_state.col-3] = sensores.terreno[13];
            mapaResultado[current_state.fil+1][current_state.col-3] = sensores.terreno[14];
            mapaResultado[current_state.fil][current_state.col-3] = sensores.terreno[15];
            break;
    }

}


