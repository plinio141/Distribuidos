/////////////////////////////////////
// Clase: proyectoSocket.cpp
// Funcion: main
/////////////////////////////////////

#include <iostream>
#include "servidor.h"
#include "cliente.h"

using namespace std;

int main(int argc, char *argv[]) {
	int tipo =0;
	int puerto=0;
	
	cout<<"Seleccione el tipo: 1=Servidor ; 2=Cliente; 3= Almacenamiento. Cual?:   ";
	cin>>tipo;
	cin.get();
	
	cout <<"Puerto: ";
	cin >> puerto;
	cin.get();
	
	int val=tipo;
	
		if(val==1){
			
			Servidor servidor;
			
			servidor.setPuerto(puerto);
			servidor.inicializarServidor();
			servidor.ejecutarServidor();
			
			while(1);
		}
		
		if(val==2 || val == 3){
			
			cout<< "Nombre del host o IP: ";
				string host;
				getline(cin,host);

			
			Cliente * cliente = new Cliente(puerto,host, val);
				cliente->conectarServidor();
		}
		return 0;
		
}// fin de la clase
