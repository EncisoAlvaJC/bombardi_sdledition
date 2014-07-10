#ifndef TABLERO_H
#define TABLERO_H
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#include "coleccion.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class tablero_objetos{
private:
/// el tablero objetos unicamente tiene un arreglo de caracteres con la
/// letra que identifica a cada objeto, y el color del objeto
    char casilla[7][7];
    char color[7][7];
public:
/// las funciones son simples: "acceder" a datos privados sin que dejen
/// de ser privados: la idea es que haya una capa de seguridad
    tablero_objetos();
    tablero_objetos(tablero_objetos&);

    void cambiar(int,int,char,int);
    void elimina(int,int); /// caso particular de la anterior a 0,0
    void haz_ceniza(int,int); /// como el anterior, pero solo de color
    bool comparar(int,int,char);
    int el_color_de(int,int);
    void imprimir(coleccion_objetos,SDL_Surface*);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
tablero_objetos::tablero_objetos(){
/// inicializar a 0's
    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++){
            casilla[i][j]=0;
            color[i][j]=0;
        }
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
tablero_objetos::tablero_objetos(tablero_objetos& T){
    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++){
            casilla[i][j]=T.casilla[i][j];
            color[i][j]=T.color[i][j];
        }
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void tablero_objetos::cambiar(int y,int x,char k,int n){
    casilla[y][x]=k; color[y][x]=n;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void tablero_objetos::elimina(int y,int x){
    casilla[y][x]=0; color[y][x]=0;
}
void tablero_objetos::haz_ceniza(int y,int x){
    color[y][x]=0;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
bool tablero_objetos::comparar(int y,int x,char k){
    return (casilla[y][x]==k);
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
int tablero_objetos::el_color_de(int y,int x){
    int ans=color[y][x]; return ans;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void tablero_objetos::imprimir(coleccion_objetos C,SDL_Surface* S){
/// la idea es sencilla, lugar recorre la primera columna y, en cada
/// paso, auxiliar lo copia y luego recoore tal renglon
SDL_Rect lugar,auxiliar; lugar.x=32;lugar.y=32;
    for(int i=0;i<7;i++,lugar.y+=64){
        auxiliar=lugar;
        for(int j=0;j<7;j++,auxiliar.x+=64){
            C.imprimir_ficha(casilla[i][j],color[i][j],auxiliar,S);
        }
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class tablero_booleano{
/// para acelerar las cosas, los tableros con el fondo y los botones
/// manejan bool en vez de char
private:
    bool casilla[7][7];
public:
    tablero_booleano();
    tablero_booleano(tablero_booleano&);

    void reinicia();
        // todos sean false
    void haz0(int,int);
    void haz1(int,int);
    bool valor(int,int);
/// las siguientes funciones expresan la funcion del tablero_booleano
    void imprimir_fondo(fondo_boton,SDL_Surface*);
    void imprimir_boton(fondo_boton,SDL_Surface*);
    void imprimir_rojos(fondo_boton,SDL_Surface*);
    int cuenta();

    void parche_impresion();
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void tablero_booleano::parche_impresion(){
    cout<<endl<<endl;
    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++){
            cout<<" ";
            if(casilla[i][j]){
                cout<<"1";
            }
            else{
                cout<<"_";
            }
        }
        cout<<endl;
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
tablero_booleano::tablero_booleano(){
    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++){
            casilla[i][j]=false;
        }
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
tablero_booleano::tablero_booleano(tablero_booleano& T){
    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++){
            casilla[i][j]=T.casilla[i][j];
        }
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void tablero_booleano::reinicia(){
    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++){
            casilla[i][j]=false;
        }
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void tablero_booleano::haz0(int y,int x){casilla[y][x]=false;}
void tablero_booleano::haz1(int y,int x){casilla[y][x]=true;}
bool tablero_booleano::valor(int y,int x){
    return (casilla[y][x]==true);}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void tablero_booleano::imprimir_boton(fondo_boton B,
                                      SDL_Surface* S){
SDL_Rect lugar,auxiliar; lugar.x=32;lugar.y=32;
    for(int i=0;i<7;i++,lugar.y+=64){
        auxiliar=lugar;
        for(int j=0;j<7;j++,auxiliar.x+=64){
            B.imprimir_boton(casilla[i][j],auxiliar,S);
        }
    }
}
void tablero_booleano::imprimir_rojos(fondo_boton B,
                                      SDL_Surface* S){
SDL_Rect lugar,auxiliar; lugar.x=32;lugar.y=32;
    for(int i=0;i<7;i++,lugar.y+=64){
        auxiliar=lugar;
        for(int j=0;j<7;j++,auxiliar.x+=64){
            B.imprimir_rojos(casilla[i][j],auxiliar,S);
        }
    }
}
void tablero_booleano::imprimir_fondo(fondo_boton F,
                                      SDL_Surface* S){
SDL_Rect lugar,auxiliar; lugar.x=32;lugar.y=32;
    for(int i=0;i<7;i++,lugar.y+=64){
        auxiliar=lugar;
        for(int j=0;j<7;j++,auxiliar.x+=64){
            F.imprimir_fondo(casilla[i][j],auxiliar,S);
        }
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
int tablero_booleano::cuenta(){
int ans=0;
    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++){
            if(casilla[i][j]==true){
                ans+=1;
            }
        }
    }
return ans;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#endif
