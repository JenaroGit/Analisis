#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <map>
#include <math.h>

using namespace std;

map <string,int> caracteres;
int num_char;

struct Nodo{//Nodo para la lista enlazada
	char delimitador;
	struct Nodo* siguiente;
};


typedef struct Nodo nodo;

nodo *primero, *ultimo;//Puntero a primero y ultimo nodo vacios

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
	if (primero == NULL){
		primero = nuevo;//Primero es el nuevo nodo
		ultimo = nuevo;//Ultimo es el nuevo nodo
	}
	else{
		ultimo->siguiente = nuevo;//Agrega el nuevo nodo al final de la lista
		ultimo = nuevo;// Ultimo es el nuevo nodo
	}
}

void eliminar_delimitador(char del){
	nodo *auxiliar;
	nodo *temp;//Se ubica en el nodo anterior
	auxiliar=primero;
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
	if (primero==NULL)
		cout << "La lista esta vacía" << endl;
	auxiliar=primero;
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
	while ((c = getchar()) != EOF){
		char car[1] = {c};
		histograma((string) car);
	}
	cout << endl;
}
void leer_estandar_n(){	
	char c;
	while ((c = getchar()) != EOF){
		char car[1] = {c};
		if (isupper(car[0])){
			char minus[1] = {(char) tolower(car[0])};
			histograma((string) minus);
		}
		else{
			char minus[1] = {car[0]};
			histograma((string) minus);
		}
	}
	cout << endl;
}

void leer_archivo_s(){
	FILE * flujo = fopen("texto.txt", "rb");//Abre el archivo en modo lectura
	fseek (flujo, 0, SEEK_END);//Mueve flujo al final del archivo
	num_char = ftell(flujo); //cantidad total de caracteres que tiene el archivo
	rewind (flujo); //mueve el flujo al inicio
	char caracter[1];
	string mensaje;
	while (!feof(flujo)){//Mientras no sea el final del archivo
		fscanf(flujo,"%c",caracter);//Escanea el archivo y guarda un caracter en caracter
		histograma((string) caracter);
	}
}

void leer_archivo_n(){
	FILE * flujo = fopen("texto.txt", "rb");//Abre el archivo en modo lectura
	fseek (flujo, 0, SEEK_END);//Mueve flujo al final del archivo
	num_char = ftell(flujo); //cantidad total de caracteres que tiene el archivo
	rewind (flujo); //mueve el flujo al inicio
	char caracter[1];
	string mensaje;
	while (!feof(flujo)){//Mientras no sea el final del archivo
		fscanf(flujo,"%c",caracter);//Escanea el archivo y guarda un caracter en caracter
		if (isupper(caracter[0])){
			char minus[1] = {(char) tolower(caracter[0])};
			histograma((string) minus);
		}
		else{
			char minus[1] = {caracter[0]};
			histograma((string) minus);
		}
	}	
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
	while (true){
		menu_sensible();
		cout << "Digite la opcion que desee realizar" << endl;
		cin >> opcion;
		if (opcion == 's'){
			if (entrada == '1'){
				leer_estandar_s();
			}
			else{
				leer_archivo_s();
			}
			graficar();
			vaciar_map();
		}
		else if (opcion == 'n'){
			if (entrada == '1'){
				leer_estandar_n();
			}
			else{
				leer_archivo_n();
			}
			graficar();
			vaciar_map();
		}
		else if (opcion == '0')
			break;
		else
			cout << "Opcion invalida" << endl;
	}
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

int main(){
	crear_delimitadores();
	elegir_entrada();
	cout << "Numero de caracteres: " << num_char << endl;
	return 0;
}
