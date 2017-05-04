///////////////////////////////////////////
// Clase: servidor.cpp
///////////////////////////////////////////

#include "servidor.h"


// ************ Constructores ***************

Servidor::Servidor(){
	this->puerto=9000;
	this->contClientes=0;
	this->contAlmacenamiento=0;

}

Servidor::Servidor(int puerto){
	this->puerto=puerto;
	this->contClientes=0;
	this->contAlmacenamiento=0;
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
void * Servidor::recibirCliente(void *ser){
	Servidor * server = (Servidor *) ser;

	cout<<"cont"<<endl;
	cout<<server->getContClientes()<<endl;
	cout<<"tamano list"<<endl;
	cout<<server->getClientesDescriptorClientes().[server->getContClientes()]<<endl;
	

	ClienteInfo * cliente = (ClienteInfo *) server->getClientesDescriptorClientes();

	char mensajeDeCliente[128];
	char key[]= "1";
	

	while(cliente->getEstado()){
		cout<<"escuchando" <<endl;
		int i=recv(cliente->getDescriptorCliente(), (void *)&mensajeDeCliente,128,0);
		cout<<"escuchando2" <<endl;
		sleep(1);
		if(i!=0){

			if(strcmp (key,mensajeDeCliente) != 0){
				recv(cliente->getDescriptorCliente(), (void *)&mensajeDeCliente,128,0);
				cout<<mensajeDeCliente<<endl;
				vector <ClienteInfo *> seleccion = server->seleccionarAlmacenmiento();
				server->recibirArchivo((void *)cliente,(void *) seleccion[0],(void *)seleccion[1], (void *) mensajeDeCliente);
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
				cout<<mensajeDeCliente<<endl;
				if(strcmp (cliente,mensajeDeCliente) == 0){
					
					clientesDescriptorClientes.push_back(new ClienteInfo(descriptorCliente,clienteInfor));
					clientesDescriptorClientes[contClientes]->setId(contClientes);
					pthread_t clientesHilos;
					pthread_create(&clientesHilos,NULL,&recibirCliente,(void *)this);
					contClientes++;
					cout<<"Cliente"<<endl;
				}else{
					clientesDescriptorAlmacenamiento.push_back(new ClienteInfo(descriptorCliente,clienteInfor));
					//pthread_t clientesHilos;
					//pthread_create(&clientesHilos,NULL,&recibirCliente,(void *) clientesDescriptorAlmacenamiento[contAlmacenamiento]);
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

vector<ClienteInfo *> Servidor::seleccionarAlmacenmiento(){

	vector<ClienteInfo *> seleccionAlmacenamiento;
	char mensajeACliente[]= "3";
	for(int i=0; i<clientesDescriptorAlmacenamiento.size(); i++){
		ClienteInfo * cliente = clientesDescriptorAlmacenamiento[i];
		if(send(cliente->getDescriptorCliente(), (void *)mensajeACliente, sizeof(mensajeACliente),0)!=-1){
			sleep(1);
			char mensajeDeCliente[128];
			cout<<"inicio seleccion"<<endl;
			int i=recv(cliente->getDescriptorCliente(), (void *)&mensajeDeCliente,128,0);
			cout<<"fin seleccion"<<endl;
			cliente->setNumeroArchivos(*(int*)mensajeDeCliente);
		}
	}

	for(int i=0; i<clientesDescriptorAlmacenamiento.size(); i++){
		for(int j=i+1; j<clientesDescriptorAlmacenamiento.size(); j++){
			if(clientesDescriptorAlmacenamiento[i]>clientesDescriptorAlmacenamiento[j]){
				ClienteInfo * clienteAux= clientesDescriptorAlmacenamiento[i];
				clientesDescriptorAlmacenamiento[i]=clientesDescriptorAlmacenamiento[j];
				clientesDescriptorAlmacenamiento[j]= clienteAux;
			}
		}
	}
	for(int i=0; i<clientesDescriptorAlmacenamiento.size(); i++){
		cout<<"Orden"+i<<endl;
		cout<<clientesDescriptorAlmacenamiento[i]<<endl;
	}
	seleccionAlmacenamiento.push_back(clientesDescriptorAlmacenamiento[0]);
	seleccionAlmacenamiento.push_back(clientesDescriptorAlmacenamiento[1]);
	return seleccionAlmacenamiento;
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
*Recibir Archivo
*/
void Servidor::recibirArchivo(void * cli, void * sel1, void * sel2, void * fileName){
	ClienteInfo * cliente = (ClienteInfo *) cli;
	ClienteInfo * seleccionAlmacenamiento1 = (ClienteInfo *) sel1;
	ClienteInfo * seleccionAlmacenamiento2 = (ClienteInfo *) sel2;
	char * nameFile = (char *)fileName;
	char buffer[BUFFSIZE];
	int recibido = -1;

	/*Se abre el archivo para escritura*/
	//FILE * file;
	//file = fopen("Archivos/archivoRecibido","wb");
	if(send(seleccionAlmacenamiento1->getDescriptorCliente(),(void *)nameFile,sizeof(nameFile),0)==-1){
		cout<<"Error al enviar el archivo"<<endl;
	}
	if(send(seleccionAlmacenamiento2->getDescriptorCliente(),(void *)nameFile,sizeof(nameFile),0)==-1){
	  cout<<"Error al enviar el archivo"<<endl;
	}

	while((recibido = recv(cliente->getDescriptorCliente(), buffer, BUFFSIZE, 0)) > 0){
		printf("%s",buffer);
		//fwrite(buffer,sizeof(char),1,file);
		if(send(seleccionAlmacenamiento1->getDescriptorCliente(),buffer,BUFFSIZE,0)==-1){
		  cout<<"Error al enviar el archivo"<<endl;
		}
		if(send(seleccionAlmacenamiento2->getDescriptorCliente(),buffer,BUFFSIZE,0)==-1){
		  cout<<"Error al enviar el archivo"<<endl;
		}
	}//Termina la recepción del archivo
	enviarConfirmacion((void *)cliente);
	enviarMD5SUM((void *)cliente);
	//fclose(file);
}

void Servidor::enviarConfirmacion(void * cli){
	ClienteInfo * cliente = (ClienteInfo *) cli;
	char mensaje[80] = "Paquete Recibido";
	int lenMensaje = strlen(mensaje);
	printf("\nConfirmación enviada\n");
	if(write(cliente->getDescriptorCliente(),mensaje,sizeof(mensaje)) == -1)
			perror("Error al enviar la confirmación:");

	
}//End enviarConfirmacion

void Servidor::enviarMD5SUM(void * cli){
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

int Servidor::getContClientes(){
	return this->contClientes;
}


struct sockaddr_in Servidor::getServidorInfo(){
	return this->servidorInfo;
}

vector<ClienteInfo *> Servidor::getClientes(){
	return this->clientesDescriptor;
}

vector<ClienteInfo *> Servidor::getClientesDescriptorClientes(){
	return this->clientesDescriptorClientes;
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
