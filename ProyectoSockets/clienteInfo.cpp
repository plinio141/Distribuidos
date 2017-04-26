///////////////////////////
// Clase: clienteInfo.cpp
///////////////////////////

#include "clienteInfo.h"

/*
* Constructor
*/
ClienteInfo::ClienteInfo(int desc, struct sockaddr_in infor){
	this->descriptorCliente=desc;
	this->clienteInfor=infor;
	this->estado=true;
}
/*
*Recibir Archivo
*/
void ClienteInfo::recibirArchivo(void * cli){
	ClienteInfo * cliente = (ClienteInfo *) cli;
	char buffer[BUFFSIZE];
	int recibido = -1;

	/*Se abre el archivo para escritura*/
	FILE * file;
	file = fopen("archivoRecibido","wb");
	enviarConfirmacion((void *)cliente);
	enviarMD5SUM((void *)cliente);
	while((recibido = recv(cliente->getDescriptorCliente(), buffer, BUFFSIZE, 0)) > 0){
		printf("%s",buffer);
		fwrite(buffer,sizeof(char),1,file);
	}//Termina la recepción del archivo

	fclose(file);
	

}
void ClienteInfo::enviarConfirmacion(void * cli){
	ClienteInfo * cliente = (ClienteInfo *) cli;
	char mensaje[80] = "Paquete Recibido";
	int lenMensaje = strlen(mensaje);
	printf("\nConfirmación enviada\n");
	if(write(cliente->getDescriptorCliente(),mensaje,sizeof(mensaje)) == -1)
			perror("Error al enviar la confirmación:");

	
}//End enviarConfirmacion

void ClienteInfo::enviarMD5SUM(void * cli){
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
* destructor
*/
ClienteInfo::~ClienteInfo(){
}

/*
* Getters
*/
int ClienteInfo::getId(){
	return this->id;
}

int ClienteInfo::getDescriptorCliente(void){
	return this->descriptorCliente;	
}

struct sockaddr_in ClienteInfo::getClienteInfor(){
	return this->clienteInfor;
}

bool ClienteInfo::getEstado(){
	return this->estado;
}

/*
* Setters
*/
void ClienteInfo::setId(int id){
	this->id=id;
}

void ClienteInfo::setEstado(bool estado){
	this->estado=estado;
}

//fin de la clase
