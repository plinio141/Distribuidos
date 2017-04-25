////////////////////////////////////
// Clase: cliente.cpp
////////////////////////////////////

#include "cliente.h"

/*
* Constructores
*/
Cliente::Cliente(){
	this->puertoServidor=9000;
	this->ipServidor="192.168.1.10";
	this->estado=true;
}

Cliente::Cliente(int puerto, string ip){
	this->puertoServidor=puerto;
	this->ipServidor=ip;
	this->estado=true;
}
/*
* destructor
*/
Cliente::~Cliente(){
}

/*
* Metodo escuchar servidor
*/
void * Cliente::escucharServidor(void *cli){
	Cliente* cliente=(Cliente *) cli;
	char mensajeDeServidor[60];
	
	while(1){
		recv(cliente->getDescriptor(), (void *)&mensajeDeServidor,60,0);
		cout<<mensajeDeServidor<<endl;
	}
}

/*
* Metodo escribir servidor
*/
void * Cliente::escribirServidor(void * cli){
	Cliente* cliente =(Cliente *) cli;
	
	int conectado=1;
	while(conectado){
		
		char msg[128];
		sprintf(msg,"Hola ");
		
		int i=send(cliente->getDescriptor(),(void *)msg,sizeof(msg),0);
		sleep(1);
		
		if(i==-1){
			conectado=0;
			cout<<"Se desconecto del servidor"<<endl;
			close(cliente->getDescriptor());
			exit(EXIT_SUCCESS);
		}
	}
}
/*
*Metodo de opciones de cliente
*/
void * Cliente::opciones(void * cli){
	int opcion;
	cout<<"Opciones Cliente"<<endl;
	cout<<"1. Subir Archivo"<<endl;
	cout<<"2. Listado de archivos por maquina"<<endl;
	cout<<"3. Eliminar archivo"<<endl;
	cout<<"4. Balanceo de carga"<<endl;
	cin>>opcion;
	cin.get();

	switch(opcion){
		case 1: enviarArchivo(cli);
		break;
		case 2: listarArchivos(cli);
		break;
		case 3: eliminarArhivo(cli);
		break;
		default: balancearCarga(cli);
	}
}
/*
* Metodo de enviar archivo
*/
void * Cliente::enviarArchivo(void * cli){
	Cliente * cliente = (Cliente *) cli;
	char url[];
	char buffer[BUFFSIZE]
	cout<<"ingrese la ruta del archivo a enviar"<<endl;
	cin>>url;
	cin.get();
	FILE * archivo;
	archivo = fopen(utl,"rb");
	while(!feof(archivo)){
		fread(buffer,sizeof(char),BUFFSIZE, archivo);
		if(send(cliente->getDescriptor(),buffer,BUFFERSIZE,0)){
		  cout<<Error al enviar el archivo<<endl;
		}	 
	}
	opciones(cli);	
}

/*
* Metodo de listar archivos
*/
void * Cliente::listarArchivos(){
}

/*
* Metodo eliminar archivo
*/
void * Cliente::eliminarArchivo(){
}
/*
*Metodo balancear carga
*/
void * Cliente::balancearCarga(){

}
/*
* Metodo conectar servidor
*/
void Cliente::conectarServidor(){
	
	descriptorCliente = socket(AF_INET,SOCK_STREAM,0);
	servidorInfo.sin_family=AF_INET;
	inet_pton(AF_INET,ipServidor.c_str(),&servidorInfo.sin_addr);
	servidorInfo.sin_port=htons(puertoServidor);
	
	int conn=connect(descriptorCliente,(struct sockaddr *)&servidorInfo,sizeof(servidorInfo));
	
	if(conn!=-1){
		pthread_t hiloEscucha;
		pthread_create(&hiloEscucha,NULL,escucharServidor,(void *)this);
		
		pthread_t hiloEscribe;
		pthread_create(&hiloEscribe,NULL,escribirServidor,(void *)this);
		
		opciones();
		while(1);
	}else{
		cout<<"No se pudo conectar con el servidor"<<endl;
	}
}

/*
* Getters
*/
int Cliente::getDescriptor(){
	return this->descriptorCliente;
}

bool Cliente::getEstado(){
	return this->estado;
}

/*
* Setters
*/
void Cliente::setDescriptor(int descriptor){
	this->descriptorCliente=descriptor;
}

void Cliente::setEstado(bool estado){
	this->estado=estado;
}

//fin de la clase
