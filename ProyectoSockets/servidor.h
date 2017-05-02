///////////////////////////////////////////
// Clase: servidor.h
///////////////////////////////////////////

#include "clienteInfo.h"
#include <string.h>
#define maxClientes 100
#define BUFFSIZE 1

class Servidor{
	
	private:
		int descriptorSer;
		int idBind;
		int puerto;
		struct sockaddr_in servidorInfo;
		vector<ClienteInfo *> clientesDescriptor;
		vector<ClienteInfo *> clientesDescriptorClientes;
		vector<ClienteInfo *> clientesDescriptorAlmacenamiento;
		int contClientes; //numero de clientes
		int contAlmacenamiento; //numero de cientes de almacenamiento

		bool state;
		
	public:
		//****** Constructores ******
		Servidor(void);
		Servidor(int);
		
		//****** Destructor ******
		~Servidor();	
			
		//****** Metodos ******
		void inicializarServidor(void);
		void aceptarClientes(void);
		void cerrarServidor(void);
		static void * comenzarServidor(void *);
		void ejecutarServidor(void);
		void seleccionarAlmacenmiento();
		
		//****** Metodos para el envio de archivo******
		void  recibirArchivo(void*, void *, void *);
		void enviarConfirmacion(void*);
		void enviarMD5SUM(void*);

		
		
		//****** Setters ******
		void setDescriptorServidor(int);
		void setIdBind(int);
		void setPuerto(int);
		void setServidorInfo(struct sockaddr_in);
		
		//****** Getters ******
		int getDescriptorServidor(void);
		int getIdBind(void);
		int getPuerto(void);
		struct sockaddr_in getServidorInfo(void);
		vector<ClienteInfo *> getClientes(void);
		
};//fin de la clase
