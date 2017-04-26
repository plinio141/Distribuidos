///////////////////////////////////////////
// Clase: servidor.cpp
///////////////////////////////////////////

#include "servidor.h"

int cont;

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
	char key[] = "1";
	char mensajeDeCliente[128];
	
	while(cliente->getEstado()){
		int i=recv(cliente->getDescriptorCliente(), (void *)&mensajeDeCliente,128,0);
		sleep(1);
		
		if(i!=0){
			if(strcmp (key,mensajeDeCliente) != 0){
				
				recibirArchivo(cli);

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
	cont=0;
	
	while(cont<maxClientes){
		struct sockaddr_in clienteInfor;
		int tamano=sizeof(struct sockaddr_in);
		cout<<"Aceptando cliente.."<<endl;
		descriptorCliente=accept(this->descriptorSer, (struct sockaddr *)&clienteInfor, (socklen_t*) &tamano);
		
		// recibiendo clientes
		if(descriptorCliente!=-1){
			cout<<"Cliente CONECTADO" <<endl;
			clientesDescriptor.push_back(new ClienteInfo(descriptorCliente,clienteInfor));
			pthread_t clientesHilos;
			
			pthread_create(&clientesHilos,NULL,&recibirCliente,(void *) clientesDescriptor[cont]);
			
			clientesDescriptor[cont]->setId(cont);
			cont++;
		}
	}
}

/*
*Recibir Archivo
*/
void * Servidor::recibirArchivo(void * cli){
	ClienteInfo * cliente = (ClienteInfo *) cli;
	char buffer[BUFFSIZE];
	int recibido = -1;

	/*Se abre el archivo para escritura*/
	FILE * file;
	file = fopen("archivoRecibido","wb");
	enviarConfirmacion((void *)cli);
	enviarMD5SUM((void *) cli);
	while((recibido = recv(cliente->getDescriptorCliente(), buffer, BUFFSIZE, 0)) > 0){
		printf("%s",buffer);
		fwrite(buffer,sizeof(char),1,file);
	}//Termina la recepción del archivo

	fclose(file);
	

}
void * Servidor::enviarConfirmacion(void * cli){
	ClienteInfo * cliente = (ClienteInfo *) cli;
	char mensaje[80] = "Paquete Recibido";
	int lenMensaje = strlen(mensaje);
	printf("\nConfirmación enviada\n");
	if(write(cliente->getDescriptorCliente(),mensaje,sizeof(mensaje)) == -1)
			perror("Error al enviar la confirmación:");

	
}//End enviarConfirmacion

void * Servidor::enviarMD5SUM(void * cli){
	ClienteInfo * cliente = (ClienteInfo *) cli;
	FILE *tmp;//Apuntador al archivo temporal que guarda el MD5SUM del archivo.
	char fileName[] = "verificacion";
	char md5sum[80];
	system("md5sum archivoRecibido >> verificacion");
	
	tmp = fopen(fileName,"r");
	fscanf(tmp,"%s",md5sum);	
	printf("\nMD5SUM:%s\n",md5sum);	
	write(cliente->getDescriptorCliente(),md5sum,sizeof(md5sum));
	fclose(tmp);

}//End enviarMD5DUM

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
