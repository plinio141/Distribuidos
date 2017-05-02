/////////////////////////////////
// Clase: clienteInfo.h
/////////////////////////////////

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <pthread.h>
#include <time.h>
#include <cstdio>
#include <string.h>

using namespace std;

#define BUFFSIZE 1

class ClienteInfo{
	
	//****** atributos *****
	private:
		int descriptorCliente;
		struct sockaddr_in clienteInfor;
		bool estado;
		int id;
		int numeroArchivos;
		
	
	public:
		//***** Constructor ******
		ClienteInfo(int ,struct sockaddr_in);

		// ******* destructor *****
		~ClienteInfo();

		// ***** Metodos
		//void  recibirArchivo(void*);
		void enviarConfirmacion(void*);
		void enviarMD5SUM(void*);

		//***** Getters ******
		int getId(void);
		int getDescriptorCliente(void);

		bool getEstado(void);
		struct sockaddr_in getClienteInfor();
		
		//***** Setters ******
		void setId(int);
		void setEstado(bool);

};// fin de la clase
