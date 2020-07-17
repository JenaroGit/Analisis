#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fstream>
#include <ctype.h>
#include <map>
#include <math.h>

using namespace std;


//Variables globales
int nodosLista = 0; //Variable para llevar un conteo de la cantidad de nodos de la lista
int ready = 0; //Variable apra determinar si una palabra existe en el diccionario
int input; //Determinara cual sera la entrada del texto
int sensitive; //Determina si la entrada sera case sensitive

int chars = 0;
int palabras = 0;
int unicas = 0;
int comparaciones = 0;
int ejecuciones = 0;
map <string,int> caracteres;



//Estructuras
struct Nodo{
	int prioridad;
	int posicion;
	int linea;
	string dato;
	Nodo *der;
	Nodo *izq;
	Nodo *padre;
};
Nodo *arbol = NULL;


struct Lista{
	int uso;
	int posc;
	int line;
	string palabra;
	Lista *siguiente;
};
Lista *primero, *ultimo;

struct Delimitadores{//Nodo para la lista enlazada
	char delimitador;
	struct Delimitadores* siguiente;
};


typedef struct Delimitadores nodo;

nodo *first, *last;//Puntero a primero y ultimo nodo vacios

//PROTOTIPOS
Nodo *crearNodo(string, Nodo *, int, int);
Lista *crearLista(string, int, int, int);
void insertarNodo(Nodo *&, string, Nodo *, int, int, int);
void recorrer (Nodo *&);
void rotIzq(Nodo *&);
void rotDer(Nodo *&);
void buscar(Nodo *, string);
void insertarLista(string, int, int, int);
void aumentar(Nodo *, string);
void ordenar (Nodo *&);
void inOrden(Nodo *);
void crearArchivo(Lista *);
void crearArchivo2(Lista *);
void evaluar(string, int, int, int);
void menu();
void buscarPalabra(Nodo *, string);

void mostrar (Lista *);
void mostrar2 (Lista *);

bool buscar_map(string caracter);
void vaciar_map();
void agregar_delimitador (char del);
void eliminar_delimitador(char del);
bool buscar_delimitador(char del);
void crear_delimitadores();
multimap <int,string> sort(map <string,int> caracteres);
void histograma(string caracter);
void leer_estandar_s();
void leer_estandar_n();
void leer_archivo_s();
void leer_archivo_n();
void graficar ();
void menu_sensible();
void elegir_sensibilidad(char entrada);
void menu_entrada();
void elegir_entrada();

int main (){
	crear_delimitadores();

	elegir_entrada();//Seleccion del usuario

	crearArchivo(primero); //Llama a la función crearArchivo

	return 0;

}

bool buscar_map(string caracter){
	map <string,int>::iterator it = caracteres.find(caracter);
	if (it==caracteres.end())
		return false;
	else
		return true;
}

void vaciar_map(){
	while(!caracteres.empty())
		caracteres.erase(caracteres.begin());		
}

void agregar_delimitador (char del){//Inserta un nodo a la lista
	nodo *nuevo;
	nuevo = (nodo*) malloc(sizeof(nodo));//Reserva espacio en memoria
	nuevo->delimitador = del;//Ingresa la palabra a la lista
	nuevo->siguiente = NULL;//El puntro siguiente es NULL
	if (first == NULL){
		first = nuevo;//Primero es el nuevo nodo
		last = nuevo;//Ultimo es el nuevo nodo
	}
	else{
		last->siguiente = nuevo;//Agrega el nuevo nodo al final de la lista
		last = nuevo;// Ultimo es el nuevo nodo
	}
}

void eliminar_delimitador(char del){
	nodo *auxiliar;
	nodo *temp;//Se ubica en el nodo anterior
	auxiliar=first;
	temp=auxiliar;
	if (auxiliar==NULL)
		cout << "La lista esta vacía" << endl;
	//Borra el primer digito si lo piden
	else{
		//Se ubica en el nodo correcto
		while(auxiliar->delimitador != del){
			if((auxiliar->siguiente)!=NULL){
				temp=auxiliar;
				auxiliar=auxiliar->siguiente;
			}
			else{
				break;
			}
		}
		//Borra el nodo
		if ((auxiliar->siguiente)!=NULL){
			temp->siguiente=auxiliar->siguiente;
			free(auxiliar);
		}
		//Borra el ultimo nodo
		else{
			temp->siguiente=NULL;
			free(auxiliar);
		}	
	}
}

bool buscar_delimitador(char del){
	nodo *auxiliar;
	if (first==NULL)
		cout << "La lista esta vacía" << endl;
	auxiliar=first;
	//Busca el nodo por la cedula cuando la encuentra muestra el nodo
	while (auxiliar!=NULL){
		if (del==(auxiliar->delimitador)){
			return true;
		}
		else
			auxiliar=auxiliar->siguiente;
	}
	if (auxiliar==NULL)
		return false;
}

void crear_delimitadores(){
	agregar_delimitador(' ');
	agregar_delimitador('\n');
	agregar_delimitador('\t');	
}

multimap <int,string> sort(map <string,int> caracteres){
	multimap <int,string> ordenado;
	for (map <string,int>::iterator it = caracteres.begin(); it != caracteres.end(); ++it){
		ordenado.insert(pair <int,string>(it->second,it->first));	
	}
	return ordenado;
}

void histograma(string caracter){
	if (caracter == " " || caracter == "\n"){
		return;
	}
	if (!buscar_map(caracter))
		caracteres.insert(pair <string,int>(caracter,1));
	else
		caracteres[caracter] = caracteres[caracter] + 1;
}

void leer_estandar_s(){	
	char c;
	string mensaje;
	int posicion = 1; //variable que guardara la posicion de la palabra en el diccionario
	int prioridad = 1; //variable que guardara la priordad
	
	int linea = 1;
	while ((c = getchar()) != EOF){
		char car[1] = {c};
		histograma((string) car);
		if ((string) car == " " || (string) car == "\n"){ //Si el caracter es un delimitador
			if (mensaje != ""){ //Si el mensaje no esta vacio
				evaluar(mensaje, prioridad, posicion, linea); //Se llama a la funcion que evalua lo que entra
				mensaje = ""; //Se reinicia mensjae	
				posicion++; //Se aumenta la posicion de la palabra
			}
		}

		else{ //Si el caracter no es un delimitador
			mensaje += car; //Se le almacena el caracter dentro de mensaje
		}

		if ((string) car == "\n")
			linea++;
		chars++;
	}
	chars--;
	cout << endl;
}

void leer_estandar_n(){	
	char c;
	string mensaje;
	int posicion = 1; //variable que guardara la posicion de la palabra en el diccionario
	int prioridad = 1; //variable que guardara la priordad
	
	int linea = 1;
	while ((c = getchar()) != EOF){
		char car[1] = {c};
		if (isupper(car[0])){
			char minus[1] = {(char) tolower(car[0])};
			histograma((string) minus);
			if ((string) minus == " " || (string) minus == "\n"){ //Si el caracter es un delimitador
				if (mensaje != ""){ //Si el mensaje no esta vacio
					evaluar(mensaje, prioridad, posicion, linea); //Se llama a la funcion que evalua lo que entra
					mensaje = ""; //Se reinicia mensjae	
					posicion++; //Se aumenta la posicion de la palabra
				}
			}


			else{ //Si el caracter no es un delimitador
				mensaje += minus; //Se le almacena el caracter dentro de mensaje
			}

			if ((string) minus == "\n")
				linea++;
			chars++;
		}
		else{
			char minus[1] = {car[0]};
			histograma((string) minus);
			if ((string) minus == " " || (string) minus == "\n"){ //Si el caracter es un delimitador
				if (mensaje != ""){ //Si el mensaje no esta vacio
					evaluar(mensaje, prioridad, posicion, linea); //Se llama a la funcion que evalua lo que entra
					mensaje = ""; //Se reinicia mensjae	
					posicion++; //Se aumenta la posicion de la palabra
				}
			}

			else{ //Si el caracter no es un delimitador
				mensaje += minus; //Se le almacena el caracter dentro de mensaje
			}

			if ((string) minus == "\n")
				linea++;
			chars++;
		}
	}
	chars--;
	cout << endl;
}

void leer_archivo_n(){
	FILE * flujo = fopen("texto.txt", "rb");//Abre el archivo en modo lectura
	char caracter[1];
	string mensaje;
	int posicion = 1; //variable que guardara la posicion de la palabra en el diccionario
	int prioridad = 1; //variable que guardara la priordad
	
	int linea = 1;
	while (!feof(flujo)){//Mientras no sea el final del archivo
		fscanf(flujo,"%c",caracter);//Escanea el archivo y guarda un caracter en caracter
		if (isupper(caracter[0])){
			char minus[1] = {(char) tolower(caracter[0])};
			histograma((string) minus);
			if ((string) minus == " " || (string) minus == "\n"){ //Si el caracter es un delimitador
				if (mensaje != ""){ //Si el mensaje no esta vacio
					evaluar(mensaje, prioridad, posicion, linea); //Se llama a la funcion que evalua lo que entra
					mensaje = ""; //Se reinicia mensjae	
					posicion++; //Se aumenta la posicion de la palabra
				}
			}

			else{ //Si el caracter no es un delimitador
				mensaje += minus; //Se le almacena el caracter dentro de mensaje
			}

			if ((string) minus == "\n")
				linea++;
			chars++;
		}
		
		else{
			char minus[1] = {caracter[0]};
			histograma((string) minus);
			if ((string) minus == " " || (string) minus == "\n"){ //Si el caracter es un delimitador
				if (mensaje != ""){ //Si el mensaje no esta vacio
					evaluar(mensaje, prioridad, posicion, linea); //Se llama a la funcion que evalua lo que entra
					mensaje = ""; //Se reinicia mensjae	
					posicion++; //Se aumenta la posicion de la palabra
				}
			}

			else{ //Si el caracter no es un delimitador
				mensaje += minus; //Se le almacena el caracter dentro de mensaje
			}

			if ((string) minus == "\n")
				linea++;
			chars++;
		}
	}
	chars--;	
}

void graficar (){
	multimap <int,string> ordenado = sort(caracteres);
	double maximo = ordenado.rbegin()->first;
	double minimo = ordenado.begin()->first;
	double media = maximo - minimo;
	for (map <int,string>::iterator it = ordenado.begin(); it != ordenado.end(); ++it){
		cout << "Llave" << " " << it->second << " ";
		for (int i = 1; i <= ((double) (it->first - minimo)/media) * 72 + 1  ; i++)
			cout << "*";				
		cout << it->first << endl;
	}
	int i;
	for (i = 0; i < 8; i++)
      		cout << " ";
 	cout << "|";
 	for (i = 0; i <= 34; i++)
     		cout << " ";
 	cout << "|";
 	for (i = 0; i <= 34; i++)
     		cout << " ";
 	cout << "|" << endl;
	for (i = 0; i < 8; i++)
      		cout << " ";
 	cout << ceil(minimo);
 	for (i = 0; i <= 34; i++)
     		cout << " ";
 	cout << ceil((media/2)+minimo);
 	for (i = 0; i <= 34; i++)
      		cout << " ";
 	cout << ceil(maximo) << endl;
}

void menu_sensible(){
	cout << "¿Quiere que sea sensible a mayusculas?" << endl;
	cout << "SI, presione la tecla s" << endl;
	cout << "NO, presione la tecla n" << endl;
	cout << "Salir, presione 0" << endl;
}



void elegir_sensibilidad(char entrada){
	char opcion;
	do{
		menu_sensible();
		cout << "Digite la opcion que desee realizar" << endl;
		cin >> opcion;
		if (opcion == 's'){
			if (entrada == '1'){
				leer_estandar_s();
				ordenar(arbol); //llama a la funciona que ordena el arbol alfabeticamente
				inOrden(arbol); //llama a la funcion que recorre el arbol en orden
			}
			else{
				leer_archivo_s();
				ordenar(arbol); //llama a la funciona que ordena el arbol alfabeticamente
				inOrden(arbol); //llama a la funcion que recorre el arbol en orden
			}
			menu();
		}
		else if (opcion == 'n'){
			if (entrada == '1'){
				leer_estandar_n();
				ordenar(arbol); //llama a la funciona que ordena el arbol alfabeticamente
				inOrden(arbol); //llama a la funcion que recorre el arbol en orden
			}
			else{
				cout << entrada << endl;
				leer_archivo_n();
				ordenar(arbol); //llama a la funciona que ordena el arbol alfabeticamente
				inOrden(arbol); //llama a la funcion que recorre el arbol en orden
			}
			menu();
		}
		else if (opcion == '0')
			break;
		else
			cout << "Opcion invalida" << endl;
	}while (true);
}

void menu_entrada(){
	cout << "Tipo de entrada" << endl;
	cout << "1.Estandar" << endl;
	cout << "2.Archivo" << endl;
	cout << "0.Salir" << endl;
}

void elegir_entrada(){
	char opcion;
	while (true){
		menu_entrada();
		cout << "Digite la opcion que desee realizar" << endl;
		cin >> opcion;
		if (opcion == '1'){
			elegir_sensibilidad(opcion);
			vaciar_map();
		}
		else if (opcion == '2'){
			elegir_sensibilidad(opcion);
			vaciar_map();
		}
		else if (opcion == '0')
			break;
		else
			cout << "Opcion invalida" << endl;
	}
}

//Funcion que imprime un menu
///NO recibe nada
//NO retorna nada
void menu(){
	int opcion;
	string busqueda;
	
	do{
		cout<<"\t.:MENU:."<<endl;
		cout<<"1. Ver histograma"<<endl;
		cout<<"2. Mostrar el diccionario de palabras"<<endl;
		cout<<"3. Mostrar el diccionario de palabras con posicicones"<<endl;
		cout<<"4. Buscar palabra"<<endl;
		cout<<"5. Buscar prefijo"<<endl;
		cout<<"6. Ver estadísticas"<<endl;
		cout<<"7. Salir"<<endl;
		cout<<"\nOpcion: ";
		cin>>opcion;

		switch(opcion){

			case 1: cout<<"\nMostrando el histograma\n\n";
				graficar();
				vaciar_map();
				cout<<"\n";
				break;

			case 2: cout<<"\nMostrando el diccionario de palabras\n\n";
				mostrar (primero);
				cout<<"\n";
				break;

			case 3: cout<<"\nMostrando el diccionario de palabras con posiciones\n\n";
				mostrar2(primero);
				cout<<"\n";
				break;

			case 4: cout<<"\nBuscar palabra\n\n";				
				cout<<"Ingrese la palabra a buscar: ";
				cin>>busqueda;
				buscarPalabra(arbol, busqueda);
				if (ready == 0){
					cout << "\nPalabra no encontrada!!!";
				}
				else{
					ready = 0;
				}
				cout<<"\n\n";
				break;

			case 5: cout<<"\nBuscar prefijo\n\n";
				cout<<"Ingrese el prefijo a buscar: ";
				cin>>busqueda;
				cout<<"\nPalabras encontradas:\n\n";
				buscar(arbol, busqueda);
				cout<<"\n\n";
				break;

			case 6: cout<<"\nMostrando estadísticas\n\n";
				cout << "Caracteres leidos: " << chars << endl;
				cout << "Palabras totales: " << palabras << endl;
				cout << "Palabras unicas: " << unicas << endl;
				cout << "Comparciones realizadas: " << comparaciones << endl;
				cout<<"\n";
				break;

			case 7: system("clear");
				break;
	
			default:cout << "\nOpción inválida!!!\n";				
				break;

		}
	}while(opcion != 7);//Mientras la opcion no sea salir
}



//Funcion para leer un archivo.
//No recibe nada
//NO retorna nada
void leer_archivo_s(){
	FILE * flujo = fopen("texto.txt", "rb");//Abre el archivo en modo lectura
	char caracter[1]; //variable que guaradara el valor del caracter leido
	string mensaje; //Variable que guardara la palabra ingresada
	int posicion = 1; //variable que guardara la posicion de la palabra en el diccionario
	int prioridad = 1; //variable que guardara la priordad
	
	int linea = 1;

	while (!feof(flujo)){//Mientras no sea el final del archivo
		fscanf(flujo,"%c",caracter);//Escanea el archivo y guarda un caracter en caracter
		histograma((string) caracter);

		if ((string) caracter == " " || (string) caracter == "\n"){ //Si el caracter es un delimitador
			if (mensaje != ""){ //Si el mensaje no esta vacio
				evaluar(mensaje, prioridad, posicion, linea); //Se llama a la funcion que evalua lo que entra
				mensaje = ""; //Se reinicia mensjae	
				posicion++; //Se aumenta la posicion de la palabra
			}
		}

		else{ //Si el caracter no es un delimitador
			mensaje += caracter; //Se le almacena el caracter dentro de mensaje
		}

		if ((string) caracter == "\n")
			linea++;
		chars++;
	}
	chars--;
}


//Funcion para evaluar el texto de entrada
//Recibe un string
//No retotna nada
void evaluar(string palabra, int prioridad, int posicion, int linea){
	palabras++;
	aumentar(arbol, palabra); //Llama a la funcion aumentar
	if (ready == 0){ //Si la palabra no se encuentra dentro del arbol
		insertarNodo(arbol, palabra, NULL, prioridad, posicion, linea); //Llama a insertar el nodo en el árbol	
		unicas++;
	}
	else{ //Si la palabra se encontro en el arbol
		ready = 0; //Reinicia la variable global ready
		unicas--;
	}
}



//Funcion para crear un nuevo nodo.
//Recibe la raíz del árbol, la palabra a insertar, el padre del nodo actual, la prioridad de la palabra, la linea y la columna donde se encuentra
//Retorna el nodo creado
Nodo *crearNodo(string dato, Nodo *padre, int prioridad, int posicion, int linea){ 
	Nodo *nuevo_nodo = new Nodo(); //Crea un nuevo nodo

	nuevo_nodo->prioridad = prioridad; //Prioridad pasa a ser la prioridad leída
	
	nuevo_nodo->posicion = posicion;
	nuevo_nodo->linea = linea;

	nuevo_nodo->dato = dato; //Dato pasa a ser la palabra leída
	nuevo_nodo->der = NULL; //Hijo derecho = NULL, pues no se sabe si tendrá
	nuevo_nodo->izq = NULL; //HIjo derecho = NULL, pues no se sabe si tendrá
	nuevo_nodo->padre =  padre; //Padre pasa a ser el padre dado

	return nuevo_nodo; //retorna el nodo creado
}


//Funcion para insertar elementos en el arbol.
//Recibe la raíz del árbol, la palabra a insertar, el padre del nodo actual, la prioridad de la palabra, la linea y la columna donde se encuentra
void insertarNodo(Nodo *&arbol, string dato, Nodo *padre, int prioridad, int posicion, int linea){ 
	if(arbol == NULL){ //Si el arbol esta vacio
		Nodo *nuevo_nodo = crearNodo(dato, padre, prioridad, posicion, linea); //Llama a la función para crear el nuevo nodo
		arbol = nuevo_nodo; //Nodo actual = nuevo nodo.
	}
	else{ //Si el arbol tiene uno o mas nodos
		string valorRaiz = arbol->dato; //Obtenemos el valor de la raiz
		if(dato < valorRaiz){ //Si el elemento es menor que la raiz, insertamos en izq
			insertarNodo(arbol->izq, dato, arbol, prioridad, posicion, linea); //Llama a la función para insertar los nodos
			if (arbol->izq != nullptr && arbol->izq->prioridad > arbol->prioridad){ //Si el hijo izq tiene una prioridad mayor al nodo actual
				rotDer(arbol); //Llama a rotarDer
			}
		}
		else{ //Si el elemento es mayor que la raiz, insertamos en der
			insertarNodo(arbol->der, dato, arbol, prioridad, posicion, linea);//Llama a la función para insertar los nodos
			if(arbol->der != nullptr && arbol->der->prioridad > arbol->prioridad, linea){//Si el hijo der tiene una prioridad mayor al nodo actual
				rotIzq(arbol);//Llama a rotarIzq
			}
		}
	}
}


//Funcion para recorrer y rotar el arbol.
//Recibe la raíz
void recorrer (Nodo *&arbol){
	comparaciones++;
	if (arbol == NULL){ //Si el árbol está vacío
		return; //NO retorna nada
	}
	if(arbol->izq != nullptr && arbol->izq->prioridad > arbol->prioridad){//Si el hijo izq tiene una prioridad mayor al nodo actual
		rotDer(arbol);//Llama a rotarDer
	}
	if(arbol->der != nullptr && arbol->der->prioridad > arbol->prioridad){//Si el hijo der tiene una prioridad mayor al nodo actual
		rotIzq(arbol);//Llama a rotarIzq
	}
	recorrer(arbol->izq); //Llama a recorrer el hijo izq
	recorrer(arbol->der); //Llama a recorrer el hijo der
}


//Funcion para rotar a la izq.
//Recibe un nodo del árbol
void rotIzq(Nodo *&arbol) {
	Nodo* R = arbol->der; //Hijo der
	Nodo* I = arbol->der->izq; //Hijo izq del hijo der
	R->izq = arbol; //Hijo izq del hijo der pasa a ser el nodo actual
	arbol->der = I; //hijo der pasa a ser el hijo izq del hijo der
	arbol = R; //nodo actual pasa a ser el hijo der
}


//Funcion para rotar a la der.
//Recibe un nodo del árbol
void rotDer(Nodo *&arbol) {
	Nodo* L = arbol->izq; //Hijo izq
	Nodo* R = arbol->izq->der; //Hijo der del hijo izq
	L->der = arbol; //Hijo der del hijo izq pasa a ser el nodo actual
	arbol->izq = R; //Hijo izq pasa a ser el gijo izq del hijo der
	arbol = L; //nodo actual pasa a ser el hijo izq
}



//Funcion para buscar las posibles palabras.
//Recibe la raíz del árbol y el dato a buscar
void buscar(Nodo *arbol, string c){
	comparaciones++;
	int value = 0;  //variable para saber si se encuentra una palabra
	int largo = c.length(); //determina el largo del dato
	if (arbol == NULL){ //Si el árbol está vacío
		return; //no retorna nada
	}
	else{ //Si hay nodos en el árbol
		for (int i=0; i < largo;i++){ //ciclo de largo = largo del dato
			if (arbol->dato[i] != c[i]){ //Si un caracter del dato no es igual al de la entrada
				value=1; //Cambia el valor de value a 1
				break; //Rompe el ciclo
			}
		}

		if (value == 0){ //Si el valor de value = 0
			cout << arbol->dato << "\n";
		}
		buscar(arbol->izq, c); //se llama la funcion para buscar otras palabras
		buscar(arbol->der, c); //se llama la funcion para buscar otras palabras
	}
}


//Funcion para crear un nuevo nodo de la lista.
//Recibe la palabra a insertar, el uso y la linea y columna de la misma
//Retorna el nodo creado
Lista *crearLista(string palabra, int uso, int posc, int line){
	Lista *nuevo_nodo = new Lista(); //Crea un nuevo nodo tipo lista

	nuevo_nodo->uso = uso; //uso = cantidad de veces que se ha usado la palabra

	nuevo_nodo->posc = posc;
	nuevo_nodo->line = line;

	nuevo_nodo->palabra = palabra; //palabra del nodo actual = a la palabra leída
	nuevo_nodo->siguiente = NULL; //nodo siguiente = NULL, pues no se sabe si habrá

	return nuevo_nodo; //retorna el nodo creado
}


//Funcion para insertar elementos en la lista.
//Recibe la palabra a insertar, el uso y la linea y columna de la misma
void insertarLista(string palabra, int uso, int posc, int line){
	if(primero == NULL){ //Si la lista esta vacia
		Lista *nuevo_nodo = crearLista(palabra, uso, posc, line); //Llama a la función encargada de crear un nuevo nodo de la lista
		primero = nuevo_nodo; //El nuevo nodo pasa a ser el primero
		ultimo = nuevo_nodo; //El nuevo nodo pasa a ser el último
		nodosLista++; //Se incrementa en 1 el contador de nodos de la lista
	}
	else{ //Si la lista tiene uno o mas nodos
		Lista *nuevo_nodo = crearLista(palabra, uso, posc, line); //Llama a la función encargada de crear un nuevo nodo de la lista
		ultimo->siguiente = nuevo_nodo; //Inserta el nuevo nodo al final de la lista
		ultimo = nuevo_nodo; //Determina al nuevo nodo como el último
		nodosLista++; //Incrementa en 1 la cantidad de nodos en la lista
	}
}


//Funcion para aumentar el conteo de veces de la palabra.
//Recibe la raíz del árbol y la palabra a buscar
void aumentar(Nodo *arbol, string palabra){
	comparaciones++;
	if (arbol == NULL){//Si el arbol es vacio
		return; //No retorna nada
	}
	else if (arbol->dato == palabra){ //Si la palabra = al dato del nodo actual
		arbol->prioridad++;//aumenta la cantidad de veces que se ha usado la palabra
		ready = 1;
		return;
	}
	aumentar(arbol->izq, palabra); //sigue recorriendo el árbol
	aumentar(arbol->der, palabra); //sigue recorriendo el árbol
}


//Funcion para recorrer y rotar el arbol.
//Recibe la raíz
void ordenar (Nodo *&arbol){
	comparaciones++;
	if (arbol == NULL){ //Si el árbol está vacío
		return; //NO retorna nada
	}
	if(arbol->izq != nullptr && arbol->izq->dato > arbol->dato){//Si el hijo izq tiene una prioridad mayor al nodo actual
		rotDer(arbol);//Llama a rotarDer
	}
	if(arbol->der != nullptr && arbol->der->dato > arbol->dato){//Si el hijo der tiene una prioridad mayor al nodo actual
		rotIzq(arbol);//Llama a rotarIzq
	}
	ordenar(arbol->izq); //Llama a recorrer el hijo izq
	ordenar(arbol->der); //Llama a recorrer el hijo der
}


//Funcion para recorrer el arbol en orden y ordenar la lista de a - z.
//Recibe la raíz del árbol
void inOrden(Nodo *arbol){
	if (arbol == NULL){ //Si el arbol está vacío
		return; //No retorna nada
	}
	else{ //Si hay nodos
		inOrden(arbol->izq); //Recorre el árbol por la izquierda
		insertarLista (arbol->dato, arbol->prioridad, arbol->posicion, arbol->linea); //Llama a insertar el nodo en la lista
		inOrden(arbol->der); //Recorre el árbol por la derecha
	}
}


//Funcion para crear el archivo antes de salir.
//Recibe el primer nodo de la lista
void crearArchivo(Lista *nodo){
	FILE *flujo = fopen ("diccionario.txt","w"); //Abre el archivo a trabajar
	if (flujo == NULL){ //Si NO se abrió el archivo
		cout << "Error con la apertura del archivo!!!" << endl; //Imprime
	}
	fflush(flujo);//limpia el canal de información que se usa
	fclose(flujo); //Cierra el archivo creado
	crearArchivo2(nodo); //Llama a la funcion CrearArchivo2
}


//Funcion para escribir en el archivo antes de salir.
//Recibe el primer nodo de la lista
void crearArchivo2(Lista *nodo){
	ofstream file("diccionario.txt"); //determina el archivo a trabajar
	for(int i=0; i<nodosLista; i++){ //Ciclo que cuenta la cantidad de nodos que posee la lista
		file << nodo->palabra << " " << nodo->uso << " " << nodo->line << " " << nodo->posc << "\n"; //agrega los datos necesarios al archivo creado
		nodo = nodo->siguiente; //Nodo actual pasa a ser el nodo siguiente
	}
}


//Funcion para buscar una palabra.
//Recibe la raíz del árbol y la palabra a buscar
void buscarPalabra(Nodo *arbol, string palabra){
	comparaciones++;
	if (arbol == NULL){//Si el arbol es vacio
		return; //No retorna nada
	}
	else if (arbol->dato == palabra){ //Si la palabra = al dato del nodo actual
		cout << "\nPalabra encontrada: " << arbol->dato << "\nUso: " << arbol->prioridad << "\nNumero de linea: " << arbol->linea << "\nPosicion en el texto: " << arbol->posicion;
		ready = 1;
		return;
	}
	
	buscarPalabra(arbol->izq, palabra); //sigue recorriendo el árbol
	buscarPalabra(arbol->der, palabra); //sigue recorriendo el árbol
}


//Funcion muestra la lista ordenada de las palabras del diccionario y su <<prioridad>>
//Recibe la raíz del árbol y la palabra a buscar
void mostrar (Lista *primero){
	Lista *aux = primero;
	for (int i=0; i < nodosLista; i++){
		cout<<"\nPalabra: "<<aux->palabra << "\nUso: " << aux->uso << endl;
		aux = aux->siguiente;
	}
}


//Funcion muestra la lista ordenada de las palabras del diccionario y su <<prioridad>>
//Recibe la raíz del árbol y la palabra a buscar
void mostrar2 (Lista *primero){
	Lista *aux = primero;
	for (int i=0; i < nodosLista; i++){
		cout<<"\nPalabra: "<<aux->palabra << "\nUso: " << aux->uso << "\nNumero de linea: " << aux->line << "\nPosicion: " << aux->posc << "\n";
		aux = aux->siguiente;
	}
}
