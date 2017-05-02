////////////////////////////////////////////////
// Clase: cliente.h
/////////////////////////////////////////////////

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <iostream>
#include <time.h>
#include <cstdio>
#include <stdlib.h>
#include <pthread.h>
#include <sstream>
#include <vector>


#define BUFFSIZE 1

using namespace std;

class Cliente{
	
	private:
		//****** Atributos ********
		int descriptorCliente;
		struct sockaddr_in servidorInfo;
		int puertoServidor;
		string ipServidor;
		bool estado;
		int tipoCliente; //cliente o almacenamiento
	
	public:
		//******* Constructores ******
		Cliente(void);
		Cliente(int, string, int);
			
		//******* destructor ****
		~Cliente();

		//******* Metodos ******
		void conectarServidor(void);
		static void * escucharServidor(void*);
		static void * escribirServidor(void*,char *);
		static void * opciones(void *);
		static void * enviarArchivo(void *);
		static void * listarArchivos(void *);
		static void * eliminarArchivo(void *);
		static void * balancearCarga(void *);

		void  recibirArchivo(void*);
		void enviarConfirmacion(void*);
		void enviarMD5SUM(void*);
		
		//******* Getters ******
		int getDescriptor(void);
		bool getEstado(void);
		
		//******* Setters ******
		void setDescriptor(int);
		void setEstado(bool);

};//fin de la clase
