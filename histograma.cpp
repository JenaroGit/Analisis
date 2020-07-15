#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <map>
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

void insertar (char del){//Inserta un nodo a la lista
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

void borrar(char del){
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

bool buscar(char del){
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

void leer_archivo(){
	FILE * flujo = fopen("texto.txt", "rb");//Abre el archivo en modo lectura
	fseek (flujo, 0, SEEK_END);//Mueve flujo al final del archivo
	num_char = ftell(flujo); //cantidad total de caracteres que tiene el archivo
	rewind (flujo); //mueve el flujo al inicio
	char caracter[1];
	string mensaje;
	while (!feof(flujo)){//Mientras no sea el final del archivo
		fscanf(flujo,"%c",caracter);//Escanea el archivo y guarda un caracter en caracter
		cout << caracter <<endl;
		if ((string) caracter == " " || (string) caracter == "\n")
			continue;
		if (!buscar_map((string) caracter))
			caracteres.insert(pair <string,int>( (string) caracter,1));
		else
			caracteres[(string) caracter] = caracteres[ (string) caracter] + 1;
	}
}

int main(){
	leer_archivo();
	for (map <string,int>::iterator it = caracteres.begin(); it != caracteres.end(); ++it)
		cout << it->first << "=" << it->second << endl;
	cout << "Numero de caracteres: " << num_char << endl;
	return 0;
}
