///////////////////////////////////////////
// Clase: servidor.h
///////////////////////////////////////////

#include "clienteInfo.h"
#define maxClientes 100

class Servidor{
	
	private:
		int descriptorSer;
		int idBind;
		int puerto;
		struct sockaddr_in servidorInfo;
		vector<ClienteInfo *> clientesDescriptor;
		
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
