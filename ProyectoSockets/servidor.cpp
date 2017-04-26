///////////////////////////////////////////
// Clase: servidor.cpp
///////////////////////////////////////////

#include "servidor.h"

int contClientes; //numero de clientes
int contAlmacenamiento; //numero de cientes de almacenamiento
// ************ Constructores ***************

Servidor::Servidor(){
	this->puerto=9000;
}

Servidor::Servidor(int puerto){
	this->puerto=puerto;
}

// ************ Destructor **************
Servidor::~Servidor(){
}

// ************** Metodos ***************

/**
* Metodo inicializar servidor
*/
void Servidor::inicializarServidor(){
	
	descriptorSer=socket(AF_INET, SOCK_STREAM, 0);
	servidorInfo.sin_family=AF_INET;
	servidorInfo.sin_addr.s_addr=htonl(INADDR_ANY);
	servidorInfo.sin_port=htons(puerto);
	idBind=bind(descriptorSer, (struct sockaddr *)&servidorInfo, sizeof(servidorInfo));
	
	listen(descriptorSer, maxClientes);
	
	if(descriptorSer==-1 && idBind==-1)
		cout<<"Error iniciando el servidor: "<<endl;
}

/**
* Metodo cerrar servidor
*/
void Servidor::cerrarServidor(){
	exit(EXIT_SUCCESS);
}

/**
* Metodo recibir cliente (no declarado en el *.h)
*/
void * recibirCliente(void *cli){
	ClienteInfo * cliente = (ClienteInfo *) cli;
	char key[] = "1"; // Opcion de recibir archivo
	char mensajeDeCliente[128];
	
	while(cliente->getEstado()){
		int i=recv(cliente->getDescriptorCliente(), (void *)&mensajeDeCliente,128,0);
		sleep(1);
		
		if(i!=0){
			if(strcmp (key,mensajeDeCliente) != 0){
				
				cliente->recibirArchivo((void *)cliente);

			}
			
		}else{
			cout<<"Se desconecto el cliente con IP: "<<inet_ntoa(cliente->getClienteInfor().sin_addr)<<" con error "<<endl;
			cliente->setEstado(false);
			close(cliente->getDescriptorCliente());
		}
	}
}

/**
* Metodo aceptar clientes
*/	
void Servidor::aceptarClientes(){

	int descriptorCliente;
	contClientes=0;
	contAlmacenamiento=0;
	char cliente[] = "1"; 
	char almacenamiento[] = "2";
	
	while((contClientes+contAlmacenamiento)<maxClientes){ 

		struct sockaddr_in clienteInfor;
		int tamano=sizeof(struct sockaddr_in);
		cout<<"Aceptando cliente.."<<endl;
		descriptorCliente=accept(this->descriptorSer, (struct sockaddr *)&clienteInfor, (socklen_t*) &tamano);
		
		
		// recibiendo clientes
		if(descriptorCliente!=-1){
			cout<<"Cliente CONECTADO" <<endl;
			char mensajeDeCliente[128];
			int i=recv(descriptorCliente, (void *)&mensajeDeCliente,128,0);

			if(i!=0){
				if(strcmp (cliente,mensajeDeCliente) != 0){
					
					clientesDescriptorClientes.push_back(new ClienteInfo(descriptorCliente,clienteInfor));
					pthread_t clientesHilos;
					
					pthread_create(&clientesHilos,NULL,&recibirCliente,(void *) clientesDescriptorClientes[contClientes]);
					
					clientesDescriptorClientes[contClientes]->setId(contClientes);
					contClientes++;
					cout<<"Cliente"<<endl;
				}else{
					clientesDescriptorAlmacenamiento.push_back(new ClienteInfo(descriptorCliente,clienteInfor));
					pthread_t clientesHilos;
					
					pthread_create(&clientesHilos,NULL,&recibirCliente,(void *) clientesDescriptorAlmacenamiento[contClientes]);
					
					clientesDescriptorAlmacenamiento[contAlmacenamiento]->setId(contAlmacenamiento);
					contAlmacenamiento++;
					cout<<"Almacenamiento"<<endl;
				}
			}else{
				cout<<"Se desconecto el cliente con IP: con error "<<endl;
				close(descriptorCliente);
			}
		}
	}
}


/*
* Metodo comenzar servidor
*/
void * Servidor::comenzarServidor(void * servidor){
	Servidor * server=(Servidor *) servidor;
	server->aceptarClientes();
}

/*
* Metodo ejecutar servidor
*/
void Servidor::ejecutarServidor(){
	pthread_t hilo;
	pthread_create(&hilo,NULL,&comenzarServidor,(void *) this);
}


/*
* Getters
*/
int Servidor::getDescriptorServidor(){
	return this->descriptorSer;
}

int Servidor::getIdBind(){
	return this->idBind;
}
	
int Servidor::getPuerto(){
	return this->puerto;
}

struct sockaddr_in Servidor::getServidorInfo(){
	return this->servidorInfo;
}

vector<ClienteInfo *> Servidor::getClientes(){
	return this->clientesDescriptor;
}
/*
* Setters
*/

void Servidor::setDescriptorServidor(int descriptor){
	this->descriptorSer=descriptor;
}
void Servidor::setIdBind(int bind){
	this->idBind=bind;
}

void Servidor::setPuerto(int puerto){
	this->puerto=puerto;
}

void Servidor::setServidorInfo(struct sockaddr_in info){
	this->servidorInfo=info;
}

//fin de la clase
