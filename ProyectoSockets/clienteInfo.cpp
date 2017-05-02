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
