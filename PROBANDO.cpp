#include <iostream>
#include <math.h> 
//#include <string>
#include <iomanip>
//#include <time.h>     VERIFICAR SI LOS UTILIZO
//#include <sstream>
#include <stdio.h>
#include <stdlib.h> 
#include <fstream>

using namespace std;

#define N 5							//cantidad de trafico que atraviesa un semaforo en un turno.
#define cantNODOS 16					//Cantidad de esquinas que tiene el plano de la ciudad
#define cantSemaforos 36    			//AJUSTAR valor 

#include "Nodo.cpp"
#include "ColaVertices.cpp"
#include "HeapMinVehiculos.cpp"
#include "listasemaforos.cpp" 
#include "GrafoCiudad.cpp"

void generarTrafico(Grafo *city, int nAutos, int nPatrulleros, int nAmbulancias,Listasemaforos* S);
void verde(Semaforo *,Listasemaforos *);	//Metodo que permite el paso de vehiculos en la cantidad n por carril y envía, de ser posible,
											//los autos al siguiente semaforo destino. 


 

int main(void){
	Grafo *ciudad=new Grafo();
	Listasemaforos *Semaforos=new Listasemaforos();
	Semaforo *semaforo;
	
	ciudad->launch(Semaforos);
	
	//~ ciudad->print();
	
	//generarTrafico(ciudad,2,0,0,Semaforos);

	//~ Semaforos->print();
	
	//~ Vehiculo *v1=new Vehiculo();
	//~ Vehiculo *v2=new Vehiculo();
	//~ Vehiculo *v3=new Vehiculo();
	Vehiculo *miAuto = new Vehiculo("Auto",9999,0,10);      //Creo el vehiculo que pretendo conducir
	//Vehiculo *miAuto=new Vehiculo();
	
	//~ v1->print_DatosVehiculo();
	//~ v2->print_DatosVehiculo();
	//~ v3->print_DatosVehiculo();
	miAuto->print_DatosVehiculo();
	
	//~ Semaforos->ubicarAleatorio(v1);
	//~ Semaforos->ubicarAleatorio(v2);
	Semaforos->ubicarAleatorio(miAuto); 
	//~ cout<<endl<<"Semaforos->buscar(1,0)->rojo(miAuto); :"<<Semaforos->buscar(1,0)->rojo(miAuto)<<endl;
	
	//Calculo el camino a recorrer de cada uno
	//~ v1->set_camino(ciudad->dijkstra(v1->get_origen(),v1->get_destino()));
	//~ v2->set_camino(ciudad->dijkstra(v2->get_origen(),v2->get_destino()));
	//~ v3->set_camino(ciudad->dijkstra(v3->get_origen(),v3->get_destino()));
	miAuto->set_camino(ciudad->dijkstra(miAuto->get_origen(),miAuto->get_destino()));
	
	Semaforos->ordenar();
	Semaforos->print_TODO(miAuto);
	
	
	int iter=0;		
	Nodosemaforo *nodo;
	Semaforo *semAux;
		
	nodo = Semaforos->get_czo();
	int i=0;
	
	while(nodo!=NULL){
		semAux = nodo->get_dato();
		cout<<++i<<endl;
		verde(semAux,Semaforos);
		cout<<i<<" DSP"<<endl;
		nodo = nodo->get_next();
		//Semaforos->ordenar();
	}

	Semaforos->print_TODO(miAuto);

	
	//~ while(miAuto->get_posicionActual()!=miAuto->get_destino())
	//~ {
//~ 
		//~ nodo = Semaforos->get_czo();
		//~ while(nodo!=NULL){
			//~ semAux = nodo->get_dato();
			//~ verde(semAux,Semaforos);
			//~ nodo = nodo->get_next();
		//~ }
		//~ Semaforos->ordenar();
		//~ 
		//~ iter++;
		//~ cout<<endl<<"-----------------------ITERACION: "<<iter<<"-------------------------------------------"<<endl;
		//~ miAuto->print_DatosVehiculo3();		
	//~ }

	return 0;
}

void generarTrafico(Grafo *city, int nAutos, int nPatrulleros, int nAmbulancias,Listasemaforos* S)
{
	///Verificar que la suma de todos los vehiculos sea menor a la capacidad acumulada de todos los semaforos.
	
	for(int i=0;i<nAutos;i++){
		Vehiculo *a=new Vehiculo("Auto");
		S->ubicarAleatorio(a);
		a->set_camino(city->dijkstra(a->get_origen(),a->get_destino()));
	}
	
	for(int i=0;i<nPatrulleros;i++){
		Vehiculo *p=new Vehiculo("Patrullero");
		S->ubicarAleatorio(p);
		p->set_camino(city->dijkstra(p->get_origen(),p->get_destino()));
	}
	
	for(int i=0;i<nAmbulancias;i++){
		Vehiculo *am=new Vehiculo("Ambulancia");
		S->ubicarAleatorio(am);
		am->set_camino(city->dijkstra(am->get_origen(),am->get_destino()));
	}
	
	S->ordenar();
	
	///Verificar que la cantidad de vehiculos en el heap sea igual a la cantidad de vehiculos ingresados
}


//Despacha n vehículos hacia sus respectivos destinos si las colas en destino no están llenas. 
//Si así lo fuere los autos permanecen en su actual ubicación. 
void verde(Semaforo *semActual,Listasemaforos *S)
{	
	Vehiculo *vehiculo;				//Puntero al con el que se manejan los diferentes vehiculos 
	
	Vehiculo *aux = new Vehiculo("Auto",8888,0,15);	
	
	Semaforo *nextSemaforo;			//Puntero al semaforo al que intenta dirigirse "vehiculo"
	HeapMin *colaSem;				//Variable para almacenar un puntereo a la cola del Semaforo actual.
	
	int nextNodo;					//Entero de referencia al próximo nodo del grafo al que quiero moverme
	int contVehiculos=0;			//contador para controlar la cantidad de vehículos que 'cruzan' un semaforo
	semActual->desbloquear();		//Si el semaforo quedó bloaqueado, se desbloquea aca.
	
	colaSem = semActual->get_ColaDelSemaforo();
	
	if(colaSem->heap_vacio()){ 	//Si la cola del semaforo no existe o está vacia:
	semActual->bloquear();							//el semaforo se bloquea para que la simulación continue.
	cout<<"Se bloqueo el semaforo por cola_vacia"<<endl; 
	}
	if(colaSem == NULL){ 	//Si la cola del semaforo no existe o está vacia:
	semActual->bloquear();							//el semaforo se bloquea para que la simulación continue.
	cout<<"Se bloqueo el semaforo por cola_NULL"<<endl; 
	}    						

	while(contVehiculos<N*(semActual->get_carriles())&&(!semActual->get_estado())) //Deja pasar n*carriles vehiculos
	{
		vehiculo=colaSem->first(); //Tomo el primer vehiculo de la cola del semaforo
		//vehiculo->print_DatosVehiculo();
		if(vehiculo!=NULL){
			
			if(vehiculo->get_posicionActual()==vehiculo->get_destino()){ //Si ya llegó al destino
				colaSem->extraer(); //Extraigo el vehiculo de la cola del semaforo, y no lo ubico en ningún otro lugar. 
				semActual->bloquear(); //Bloqueo el semaforo.
				//continue;
			}
			if(vehiculo->get_posicionActual()==vehiculo->get_origen()){ //Si todavía no se movió del origen: Primer movimiento
				vehiculo->put_recorrido(vehiculo->get_siguiente()); 	//Cargo el primer elemento (cabeza) en la cola de vertices recorrido
				vehiculo->borrar_cabeza(); 								//Si no borro el primer elemento, pierdo una iteracion moviendome a donde ya estoy
			}
			
			nextNodo=vehiculo->get_siguiente();
			nextSemaforo=S->buscar(nextNodo,semActual->get_ubicacion()); //S->buscar(NodoUbicacion, nodoProcedencia)

			if((!nextSemaforo->is_full())&&(nextSemaforo!=NULL)){ //Si el proximo semaforo NO está lleno y si existe:
				vehiculo = colaSem->extraer(); //Extraigo el vehiculo del semaforo actual
				nextSemaforo->rojo(vehiculo);  //Pongo en cola de 'nextSemaforo' a 'vehiculo'
				vehiculo->set_posicionActual(nextSemaforo->get_ubicacion());
				vehiculo->put_recorrido(vehiculo->get_siguiente()); //Cargo el elemento en la cola de vertices recorridos
				vehiculo->borrar_cabeza(); //Borro el salto que ya hizo el vehiculo en su camino.
				contVehiculos++;
				aux = vehiculo; //BORRAR ESTA LINEA!!!!!!!!!!!!!!
			}
			else semActual->bloquear(); //El semaforo al que se dirigía estaba lleno o no existía
		}
		else semActual->bloquear(); //El vehiculo no existe, seguramente la cola del semaforo esta vacía. 
	}

	cout<<"Estado del semaforo al salir del ciclo: ";
	if(semActual->get_estado() == true) cout<<"Bloqueado"<<endl;
	else cout<<"Desbloqueado"<<endl;
	cout<<"Cantidad de vehiculos que pasaron el semaforo: "<<contVehiculos<<endl;
	cout<<"El último vehiculo que pasó: "<<endl;
	aux->print_DatosVehiculo3();
}



