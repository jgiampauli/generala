#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <time.h>
#include <iomanip> 
#include <stdio.h>
 
using namespace std;

typedef char str21[21];


struct partido{
    int idPartida;
    str21 nomJugador;
    int fecha;
    int puntaje;
};

struct nodo{
    partido info;
    nodo* sgte;
};

///////Prototipo de funciones///////

void inicializarMatriz(int[][11], int);
int tirar(int[]);
void numeros(int[], int[][11], int);
void jugar(int[][11], int, bool&);
bool generala(int[]);
bool poker(int[]);
bool full(int[]);
bool escalera(int[]);
void puntajeMax(int[][11], int);
void mostrarResultados(int[][11], int, str21[], str21&, int&);
nodo* insertarOrdenado(nodo*&, partido);
int cantRegistros (FILE*);
partido pop(nodo*&);

 
/////////// INT MAIN /////////////

int main(){

FILE* archivo=fopen("partidoGenerala.txt", "r+");
	if(archivo==NULL){
    	archivo=fopen("partidoGenerala.txt", "w+");
    }

int cantjugadores, i, j, fecha;
bool genSer = false;
int puntajeGanador;
partido partido;
int idJugada=cantRegistros(archivo)+1;
nodo* lista=NULL;


cout<<"Ingrese la fecha del partido: "<<endl;
cin>>fecha;
cout<<"Ingrese la cantidad de jugadores: "<<endl;
cin>>cantjugadores;
  
int puntuaciones [cantjugadores][11]={{-1}};
str21 nomJug[cantjugadores];

inicializarMatriz(puntuaciones, cantjugadores);
fseek(archivo, 0, SEEK_END);
srand(time(NULL));

 
for(i=0 ; i<cantjugadores ; i++){
    cout<<"\nNombre del jugador "<<i+1<<" : ";
    cin>>nomJug[i];
}
 
for(i=0 ; i<11 && genSer==false ; i++){
    cout<<"\nRONDA: "<<i+1<<"\n---------------------------------"<<endl;
for(j=0 ; j<cantjugadores && genSer==false ; j++){
    cout<<"\nJugador: "<<nomJug[j]<<endl;
jugar(puntuaciones, j, genSer);
    }
}


cout<<endl;
cout<<endl;
cout<<endl;

//Mostramos por pantalla la tabla de puntuacion:

cout<<"Nombre               1     2     3     4     5     6    esca  full  pokr  gral gdob"<<endl;
cout<<"                     -------------------------------------------------------------"<<endl;


for(i=0 ; i<cantjugadores ; i++){
    
        cout
        << setw(21)
        << left
        << nomJug[i];
        
        for(j=0 ; j<11 ; j++){
            cout
                << setw(6)
                << left
                << puntuaciones[i][j];
    }
    cout<<endl;
    }

mostrarResultados(puntuaciones, cantjugadores, nomJug, partido.nomJugador, partido.puntaje);
partido.idPartida=idJugada;
partido.fecha = fecha;

fwrite(&partido, sizeof(partido), 1, archivo);
    
fseek(archivo, 0, SEEK_SET);
       while(fread(&partido, sizeof(partido), 1, archivo)){
        insertarOrdenado(lista, partido);
    }
fclose(archivo);
i=1;
cout<<"\nRANKING DE LOS 3 MEJORES\n\nPuesto  Nombre               Puntaje   Fecha      idJugada\n";
while(i<4 && lista!=NULL){
        partido = pop(lista);
        cout<<	setw(8)<<left<<i<<
				setw(21)<<left<<partido.nomJugador<<
				setw(7)<<left<<partido.puntaje<<"   "<<
				setw(8)<<left<<partido.fecha<<"   "<<
				setw(9)<<left<<partido.idPartida<<
								endl;
        i++;
    }


  return 0;
}


////////Funciones////////
 
 
void inicializarMatriz(int puntuaciones[][11], int cantjugadores){
  int i , j;
  
 for(i=0 ; i<cantjugadores ; i++){
    for(int j=0 ; j<11 ; j++){
      puntuaciones [i][j]=-1;      
    }
  }
}
 
int tirar(int juego[]){
 
int tiradas,i;
 
 
 for(i=0 ; i<5 ; i++){
    
 juego[i] = (1 + rand()%(6));
}
 
tiradas = rand()%(3) + 1;
 
return tiradas;
}
 
void numeros(int cantidad[], int puntuaciones[][11], int j){
    str21 columnas[11]={"1", "2", "3", "4", "5", "6", "Escalera", "Full", "Poker", "Generala", "Generala doble"};
    int max=0,maxPosicion=0,i;
    
    for (i=0 ; i<6 ; i++) {
    if (cantidad[i]>max && puntuaciones[j][i]==-1) {
    maxPosicion=i;
    max=cantidad[i];
    }}
        if(max==0){
            for(i=10 ; i>=0 ; i--){
                if(puntuaciones[j][i]==-1){
                    puntuaciones[j][i]=0;
                    cout<<"Tachamos la columna: "<<columnas[i]<<endl;
 
                break;
                }
            }
        }
        else{
            puntuaciones[j][maxPosicion]=max*(maxPosicion+1);
            cout<<"Sumamos "<<max*(maxPosicion+1)<<" al "<<maxPosicion+1<<endl;
 
        }
 

}
 
 
void jugar(int puntuaciones[][11], int j, bool& genSer){
int contador, i, posDados, Dados[5], cantidad[6]={0};
int canttiradas = tirar(Dados);
 
 cout<<"La cantidad de tiros fue: "<<canttiradas<<endl;

  for(contador=1 ; contador<7 ; contador++){ 
    for(posDados=0 ; posDados<5 ; posDados++){
     if(Dados[posDados] == contador){
       cantidad[contador-1]+=1;
     }
    }
  }

  cout<<"Los dados son: "<<endl; 
  for(i=0 ; i<5 ; i++){
 
   cout<<Dados[i]<<endl;   
   
}
 
    if(canttiradas == 1){
        if(generala(cantidad) && puntuaciones[j][9] == -1){//Generala servida
            genSer == true;
            cout<<"Anoto Generala servida"<<endl;
            puntajeMax(puntuaciones , j);
        }
            else{ 
            if((poker(cantidad) && puntuaciones[j][8] == -1)){//Poker servida
            puntuaciones[j][8]=45;
            cout<<"Anoto Poker servido"<<endl;
            }
                else{
                    if((full(cantidad) && puntuaciones[j][7] == -1)){//Full servido
                    puntuaciones[j][7]=35;
                    cout<<"Anoto Full servido"<<endl;
                    }
                        else{
                            if(escalera(cantidad) && puntuaciones[j][6]==-1){//Escalera servida
                            puntuaciones[j][6]=25;
                            cout<<"Anoto Escalera servida"<<endl;
                            }
                            else{
                                        numeros (cantidad, puntuaciones, j);
                                    } 
        }}}}
    else{
        if(generala(cantidad) && puntuaciones[j][9] == -1){//Generala 
            cout<<"Anoto Generala"<<endl;
            puntuaciones[j][9]=50;
        }
            else{   
                if(generala(cantidad) && puntuaciones[j][9]>0 && puntuaciones[j][10] == -1){ //Generala Doble
                    cout<<"Anoto Doble generala"<<endl;
                    puntuaciones[j][10]=100;
                    }
                    else{ 
                        if((poker(cantidad) && puntuaciones[j][8] == -1)){//Poker
                        puntuaciones[j][8]=40;
                        cout<<"Anoto Poker"<<endl;       
                        }
                        else{
                            if((full(cantidad) && puntuaciones[j][7] == -1)){//Full
                                puntuaciones[j][7]=30;
                                cout<<"Anoto Full"<<endl;
                                }
                                else{
                                    if(escalera(cantidad) && puntuaciones[j][6]==-1){//Escalera
                                        puntuaciones[j][6]=20;
                                        cout<<"Anoto Escalera"<<endl;
                                    }
                                    else{//Si no sale ningun juego:
                                        numeros (cantidad, puntuaciones, j);
                                    } 
        }}}}}}
        
 
 
bool generala(int cantidad[]){
    bool gen = false;
 
    if(cantidad[0] == 5 || cantidad[1] == 5 || cantidad[2] == 5 || cantidad[3] == 5 || cantidad[4] == 5 || cantidad[5] == 5){
            gen = true;
 
    }
    return gen;
 
}
 
bool poker(int cantidad[]){
    bool pok = false;
    if(cantidad[0] == 4 || cantidad[1] == 4 || cantidad[2] == 4 || cantidad[3] == 4 || cantidad[4] == 4 || cantidad[5] == 4){
        pok = true;
        
    }
    return pok;
 
}
 
bool full(int cantidad[]){
  bool ful=false;
  if((cantidad[0]==3 || cantidad[1]==3 || cantidad[2]==3   || cantidad[3] ==3 || cantidad[4]==3 
    || cantidad[5] ==3) && (cantidad[0]==2 || cantidad[1]==2 || cantidad[2]==2   || cantidad[3] ==2 
    || cantidad[4]==2 || cantidad[5] ==2)){
        ful=true;
    }
    return ful;

}
 
bool escalera(int cantidad[]){
    bool esca = false;
    if ((cantidad[2]==1 && cantidad[3]==1 && cantidad[4]==1)
    &&((cantidad[0]==1 && cantidad[1]==1)||(cantidad[1]==1 && cantidad[5]==1)
    ||(cantidad[5]==1 && cantidad[0]==1))){
        esca = true;
 
    }
    
    return esca;
}
 
void puntajeMax(int puntuaciones[][11], int j){
    puntuaciones[j][0]=6;
    puntuaciones[j][1]=12;
    puntuaciones[j][2]=18;
    puntuaciones[j][3]=24;
    puntuaciones[j][4]=30;
    puntuaciones[j][5]=36;
    puntuaciones[j][6]=25;
    puntuaciones[j][7]=35;
    puntuaciones[j][8]=45;
    puntuaciones[j][9]=50;
    puntuaciones[j][10]=100;
}


void mostrarResultados(int puntuaciones[][11], int cantjugadores, str21 nomJug[], str21& nomJugador, int& puntajeGanador){
    int sumas[cantjugadores]={0};
    int i, k, max=0, maxPos=0;

    for(i=0 ; i<cantjugadores ; i++){
         for(k=0 ; k<11 ; k++){
             sumas[i]+=puntuaciones[i][k];
         }
     }

    for(i=0 ; i<cantjugadores ; i++){
        cout<<"\nJugador: "<<nomJug[i]<<"  Puntaje: "<<sumas[i]<<endl;
         if(sumas[i]>max || i==0){
             max=sumas[i];
             maxPos=i;
         }
     }
    cout<<"\n\nGANADOR: "<<nomJug[maxPos]<<endl<<endl;
    cout<<"----------------------------------------------------------------------------------"<<endl;
    strcpy(nomJugador, nomJug[maxPos]);
    puntajeGanador=sumas[maxPos];
}

nodo* insertarOrdenado(nodo*& lista, partido ingreso){
    nodo* p=new nodo();
    strcpy(p->info.nomJugador,ingreso.nomJugador);
    p->info.idPartida=ingreso.idPartida;
    p->info.fecha=ingreso.fecha;
    p->info.puntaje=ingreso.puntaje;
    if(lista==NULL || ingreso.puntaje>lista->info.puntaje){
        p->sgte=lista;
        lista=p;
    }
    else{
        nodo* aux=lista;
        while(aux->sgte!=NULL && ingreso.puntaje<aux->sgte->info.puntaje){
            aux=aux->sgte;
        }
        p->sgte=aux->sgte;
        aux->sgte=p;
    }
    return p;
}

int cantRegistros (FILE* f) {
    int actual=ftell(f);
    fseek(f, 0, SEEK_END);
    int cantidad=ftell(f)/sizeof(partido);
    fseek(f, actual, SEEK_SET);
    return cantidad;
}

partido pop(nodo*& lista){
    partido n=lista->info;
    nodo* aux=lista;
    lista=lista->sgte;
    delete aux;
    return n;
}


