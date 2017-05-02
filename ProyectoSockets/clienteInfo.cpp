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
	this->numeroArchivos=0;
}
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
int ClienteInfo::getNumeroArchivos(){
	return this->numeroArchivos;
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

void ClienteInfo::setNumeroArchivos(int numeroArchivos){
	this->numeroArchivos=numeroArchivos;
}

//fin de la clase
