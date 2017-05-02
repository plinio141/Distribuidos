////////////////////////////////////
// Clase: cliente.cpp
////////////////////////////////////

#include "cliente.h"

/*
* Constructores
*/
Cliente::Cliente(){
	this->puertoServidor=9000;
	this->ipServidor="192.168.1.10";
	this->estado=true;
}

Cliente::Cliente(int puerto, string ip,int val){
	this->puertoServidor=puerto;
	this->ipServidor=ip;
	this->estado=true;
	this->tipoCliente=val;
}
/*
* destructor
*/
Cliente::~Cliente(){
}

/*
* Metodo escuchar servidor
*/
void * Cliente::escucharServidor(void *cli){
	Cliente* cliente=(Cliente *) cli;
	char mensajeDeServidor[60];
	char key[] = "1"; // Opcion de recibir archivo
	char key2[] = "2"; // Opcion de listar archivos
	char key3[] = "3"; // Opcion de contar archivos
	while(1){
		int i = recv(cliente->getDescriptor(), (void *)&mensajeDeServidor,60,0);
		if(i!=0){
			if(strcmp (key,mensajeDeServidor) != 0){
				cliente->recibirArchivo((void *)cliente);
			}else if(strcmp (key2,mensajeDeServidor) != 0){
				cliente->listarArchivos((void *)cliente);
			}else if(strcmp (key3,mensajeDeServidor) != 0){
				cliente->contarArchivos((void *)cliente);
			}
			
		}else{
			cout<<"Se desconecto el cliente con IP: "<<inet_ntoa(cliente->getClienteInfor().sin_addr)<<" con error "<<endl;
			cliente->setEstado(false);
			close(cliente->getDescriptorCliente());
		}
		
	}
}

/*
* Metodo escribir servidor
*/
void * Cliente::escribirServidor(void * cli, char msg[]){
	Cliente* cliente =(Cliente *) cli;
	
	int conectado=1;
	
	int i=send(cliente->getDescriptor(),(void *)msg,sizeof(msg),0);
	sleep(1);

	if(i==-1){
		conectado=0;
		cout<<"Se desconecto del servidor"<<endl;
		close(cliente->getDescriptor());
		exit(EXIT_SUCCESS);
	}
}
/*
*Metodo de opciones de cliente
*/
void * Cliente::opciones(void * cli){
	int opcion;
	cout<<"Opciones Cliente"<<endl;
	cout<<"1. Subir Archivo"<<endl;
	cout<<"2. Listado de archivos por maquina"<<endl;
	cout<<"3. Eliminar archivo"<<endl;
	cout<<"4. Balanceo de carga"<<endl;
	cin>>opcion;
	cin.get();

	switch(opcion){
		case 1: enviarArchivo(cli);
		break;
		case 2: char[] msg="2"; 
				send(cliente->getDescriptor(),(void *)msg,sizeof(msg),0);
		break;
		case 3: eliminarArchivo(cli);
		break;
		default: balancearCarga(cli);
	}
}
/*
* Metodo de enviar archivo
*/
void * Cliente::enviarArchivo(void * cli){
	Cliente * cliente = (Cliente *) cli;
	char url[200];
	char buffer[BUFFSIZE];
	char name[200];
	cout<<"ingrese la ruta del archivo a enviar"<<endl;
	cin>>url;
	cout<<"ingrese el nombre del archivo junto a la extensión"<<endl;
	cin>>name;
	cin.get();
	FILE * archivo;
	archivo = fopen(url,"rb");
	//definir el envio del archivo
	char msg[] = "1";//esto indica que se va a enviar el archivo
	escribirServidor((void *)cli,msg);

	escribirServidor((void *)cli,name);		

	while(!feof(archivo)){
		fread(buffer,sizeof(char),BUFFSIZE, archivo);
		if(send(cliente->getDescriptor(),buffer,BUFFSIZE,0)==-1){
			cout<<"Error al enviar el archivo"<<endl;
		}	 
	}
	char mensaje[80];
	read(cliente->getDescriptor(),mensaje,sizeof(mensaje));
	printf("\nConfirmación recibida:\n%s\n",mensaje);
	
	read(cliente->getDescriptor(),mensaje,sizeof(mensaje));
	printf("\nMD5SUM:\n%s\n",mensaje);
	
	fclose(archivo);
	opciones(cli);	
}

/*
* Metodo de listar archivos
*/
void * Cliente::listarArchivos(void * cli){

	Cliente * cliente = (Cliente *) cli;

	vector<char> listaArchivos;
	/* Con un puntero a DIR abriremos el directorio */
	DIR *dir;
  /* en *ent habrá información sobre el archivo que se está "sacando" a cada momento */
	struct dirent *ent;

  /* Empezaremos a leer en el directorio actual */
	dir = opendir ("./Archivos");

  /* Miramos que no haya error */
	if (dir == NULL) 
		error("No puedo abrir el directorio");

  /* Una vez nos aseguramos de que no hay error, ¡vamos a jugar! */
  /* Leyendo uno a uno todos los archivos que hay */
	cout<<"Listar archivos"<<endl;
	while ((ent = readdir (dir)) != NULL) 
	{
      /* Nos devolverá el directorio actual (.) y el anterior (..), como hace ls */
		if ( (strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) )
		{
      /* Una vez tenemos el archivo, lo pasamos a una función para procesarlo. */
			cout<<ent->d_name<<endl;
			listaArchivos.push_back(ent->d_name);
		}
	}
	closedir (dir);	
	for(int i=0; i<listaArchivos.size(); i++){
		char[] msg=listaArchivos[i]; 
		send(cliente->getDescriptor(),(void *)msg,sizeof(msg),0);
	}

	char msg [] ="end"; 
	send(cliente->getDescriptor(),(void *)msg,sizeof(msg),0);
}

/*
* Metodo de listar archivos
*/
void * Cliente::contarArchivos(void * cli){
	Cliente * cliente = (Cliente *) cli;
	int file_count = 0;
	DIR * dirp;
	struct dirent * entry;

	dirp = opendir("./Archivos"); /* There should be error handling after this */
	while ((entry = readdir(dirp)) != NULL) {
	    if (entry->d_type == DT_REG) { /* If the entry is a regular file */
	         file_count++;
	    }
	}
	closedir(dirp);
	char msg[] =""+file_count; 
	send(cliente->getDescriptor(),(void *)msg,sizeof(msg),0);
}

/*
* Metodo eliminar archivo
*/
void * Cliente::eliminarArchivo(void * cli){
}
/*
*Metodo balancear carga
*/
void * Cliente::balancearCarga(void * cli){

}
/*
* Metodo conectar servidor
*/
void Cliente::conectarServidor(){
	
	descriptorCliente = socket(AF_INET,SOCK_STREAM,0);
	servidorInfo.sin_family=AF_INET;
	inet_pton(AF_INET,ipServidor.c_str(),&servidorInfo.sin_addr);
	servidorInfo.sin_port=htons(puertoServidor);
	
	int conn=connect(descriptorCliente,(struct sockaddr *)&servidorInfo,sizeof(servidorInfo));
	cout<<tipoCliente<<endl;
	if(conn!=-1){
		if(tipoCliente==2){
			char msg[] = "1";//esto indica que es usuario
			escribirServidor((void *)this,msg);
			pthread_t hiloEscucha;
			pthread_create(&hiloEscucha,NULL,escucharServidor,(void *)this);
			cout<<"Usuario"<<endl;
			opciones((void *)this);
		}else {
			char msg[] = "2";//esto indica que es de almacenamiento
			escribirServidor((void *)this,msg);
			pthread_t hiloEscucha;
			pthread_create(&hiloEscucha,NULL,escucharServidor,(void *)this);
			cout<<"almacenamiento"<<endl;
		}
		
		
	//	pthread_t hiloEscribe;
	//	pthread_create(&hiloEscribe,NULL,escribirServidor,(void *)this);
		
		
		while(1);
	}else{
		cout<<"No se pudo conectar con el servidor"<<endl;
	}
}

/*
*Recibir Archivo
*/
void Cliente::recibirArchivo(void * cli){
	Cliente * cliente = (Cliente *) cli;
	char buffer[BUFFSIZE];
	int recibido = -1;

	/*Se abre el archivo para escritura*/
	FILE * file;
	file = fopen("Archivos/archivoRecibido","wb");
	
	while((recibido = recv(cliente->getDescriptor(), buffer, BUFFSIZE, 0)) > 0){
		printf("%s",buffer);
		fwrite(buffer,sizeof(char),1,file);
	}//Termina la recepción del archivo
	enviarConfirmacion((void *)cliente);
	enviarMD5SUM((void *)cliente);
	fclose(file);
}
void Cliente::enviarConfirmacion(void * cli){
	Cliente * cliente = (Cliente *) cli;
	char mensaje[80] = "Paquete Recibido";
	int lenMensaje = strlen(mensaje);
	printf("\nConfirmación enviada\n");
	if(write(cliente->getDescriptor(),mensaje,sizeof(mensaje)) == -1)
		perror("Error al enviar la confirmación:");
}//End enviarConfirmacion

void Cliente::enviarMD5SUM(void * cli){
	Cliente * cliente = (Cliente *) cli;
	FILE *tmp;//Apuntador al archivo temporal que guarda el MD5SUM del archivo.
	char fileName[] = "verificacion";
	char md5sum[80];
	system("md5sum archivoRecibido >> verificacion");
	
	tmp = fopen(fileName,"r");
	fscanf(tmp,"%s",md5sum);	
	printf("\nMD5SUM:%s\n",md5sum);	
	write(cliente->getDescriptor(),md5sum,sizeof(md5sum));
	fclose(tmp);

}//End enviarMD5DUM




/*
* Getters
*/
int Cliente::getDescriptor(){
	return this->descriptorCliente;
}

bool Cliente::getEstado(){
	return this->estado;
}

/*
* Setters
*/
void Cliente::setDescriptor(int descriptor){
	this->descriptorCliente=descriptor;
}

void Cliente::setEstado(bool estado){
	this->estado=estado;
}

//fin de la clase
