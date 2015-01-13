#ifndef COLECCION_H
#define COLECCION_H
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#include "graficos.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class coleccion_objetos{
private:
/// tenemos un arreglos de imagenes de (#fichas)x(#colores + 1)
/// porque cada ficha tiene una imagen en cada color y una extra
/// en "color ceniza" cuando es afectada por una bomba
    int n_fichas;
    int n_colores;
    SDL_Surface*** ficha;
    SDL_Surface* fallo; /// ficha extra para errores
    SDL_Surface* vacio; /// internamente, el vacio es un objeto
/// ademas, para uso humano, cada ficha es referida por una letra
/// que en principio es estandar; la idea es que el tablero se
/// conforme de un arreglo de caracteres
/// la funcion busca_ficha toma el caracter y busca el indice en
/// el que se ha alojado la imagen
    char* nombre_ficha;
    int busca_ficha(char);
public:
/// el contructor toma una cadena con la ruta relativa [funcion
/// que Code::Blocks tiene por defecto], el numero y nombre de
/// las fichas y el numero de colores: en un estandar mio, la bomba
/// es 'B', y luego los archivos son "B#.png", la idea es que cargue
/// el grafico-color segun el caracter #
    coleccion_objetos(char*, ///nombre carpeta
              int,char*,   ///fichas
              int,char*);    ///colores
    coleccion_objetos(){}/// constructor para arreglos
/// la funcion de impresion es facil de usar: se llama a una ficha por
/// su caracter identificador y su color, y se dice donde se imprime
    void imprimir_ficha(char,int,SDL_Rect,SDL_Surface*);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
coleccion_objetos::coleccion_objetos(char* ruta,
                     int num_fichas,char* nom_fichas,
                     int num_colores,char* nom_colores){
/// generando una cadena de caracteres con el nombre del archivo
char archivo[25]; int lugar=strlen(ruta);
archivo[0]='\0';
strcat(archivo,ruta); strcat(archivo,"  .png");
/// generar el arreglo de SDL_Surface*'s y cargando las imagenes
    n_fichas=num_fichas;
    n_colores=num_colores;
    ficha=new SDL_Surface**[n_fichas];
    nombre_ficha=new char[n_fichas];
    for(int i=0;i<n_fichas;i++){
        ficha[i]=new SDL_Surface*[n_colores+1];
        nombre_ficha[i]=nom_fichas[i];
            archivo[lugar]=nombre_ficha[i];
        for(int j=0;j<n_colores;j++){
                archivo[lugar+1]=nom_colores[j];
            ficha[i][j+1]=IMG_Load(archivo);
            if(!ficha[i][j+1]){
                cout<<"Error al cargar "<<archivo<<endl;
            }
        }
        ///otro estandar mio es nombrar a los archivos
        ///de imagen ceniza (ie. de una bomba) BA.png
            archivo[lugar+1]='A';
        ficha[i][0]=IMG_Load(archivo);
        if(!ficha[i][0]){
            cout<<"Error al cargar "<<archivo<<endl;
        }
    }
    /// finalmente, cargar la imagen-error
    archivo[0]='\0'; strcat(archivo,ruta); strcat(archivo,"U.png");
    fallo=IMG_Load(archivo);
    if(!fallo){
        cout<<"Error al cargar imagen-fallo."<<endl;
    }
    archivo[0]='\0'; strcat(archivo,ruta); strcat(archivo,"V.png");
    vacio=IMG_Load(archivo);
    if(!vacio){
        cout<<"Error al cargar "<<archivo<<endl;
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
int coleccion_objetos::busca_ficha(char K){
/// recorre el arreglo con los identificadores de las fichas hasta
/// alcanzar la buscada... o regresa -1
/// un estandar mio es llamar al vacio "_"
int ans=-1;
    for(int i=0;i<n_fichas;i++){
       if(nombre_ficha[i]==K){
            ans=i;
       }
    }
    if(K=='V'){
        ans=-2;
    }
return ans;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void coleccion_objetos::imprimir_ficha(char K,int N,
                                       SDL_Rect R,SDL_Surface* S){
/// una preferenca personal es que los objetos esten centrados
/// dentro de sus casillas, asi que declaro un SDL_Rect auxiliar
int i=busca_ficha(K); SDL_Rect aux=R;
    if(K!=0){
        if(i==-1){///si hubo algun error
            aux.x+=(( 64 - fallo->w )/2);
            aux.y+=(( 64 - fallo->h )/2);
            SDL_BlitSurface(fallo,0,S,&aux);
        }
        else{
            if(i==-2){///imprimiendo el vacio
                aux.x+=(( 64 - vacio->w )/2);
                aux.y+=(( 64 - vacio->h )/2);
                SDL_BlitSurface(vacio,0,S,&aux);
            }
            else{
                aux.x+=(( 64 - ficha[i][N]->w )/2);
                aux.y+=(( 64 - ficha[i][N]->h )/2);
                SDL_BlitSurface(ficha[i][N],0,S,&aux);
            }
        }
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class fondo_boton{
/// una coleccion de imagenes esteticas: los cuadros del tablero,
/// el boton verde, el pico que pone que jugador tiene el turno
private:
    SDL_Surface* fondo[3];
    SDL_Surface* boton;
    SDL_Surface* ram[5];
public:
    fondo_boton(char*,        ///ruta
                char*,char*, ///nombres en archivo
                char*,int);      ///espacio interportal de colores
    fondo_boton(){}///constructor para arreglos
    void imprimir_boton(bool,SDL_Rect,SDL_Surface*);
    void imprimir_fondo(bool,SDL_Rect,SDL_Surface*);
    void imprimir_rojos(bool,SDL_Rect,SDL_Surface*);
    void imprimir_raum(bool,SDL_Rect,SDL_Surface*,int);///tambien portal
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
fondo_boton::fondo_boton(char* ruta,
                         char* nom_fondo,char* nom_boton,
                         char* nom_ram,int num_ram){
char archivo[50]; int aux;
    archivo[0]='\0';
    strcat(archivo,ruta); strcat(archivo,nom_fondo);
    /// estandar mio. buscare dos copias de la imagen-cuadro con el
    /// mismo nombre: uno con 0 y otro con 1
    aux=strlen(archivo); archivo[aux]='N'; archivo[aux+1]='\0';
    strcat(archivo,(char*)".png");///N de normal
        fondo[0]=IMG_Load(archivo);
        if(!fondo[0]){
            cout<<"Error al cargar "<<archivo<<endl;
        }
    archivo[aux]='C';///de ceniza
        fondo[1]=IMG_Load(archivo);
        if(!fondo[1]){
            cout<<"Error al cargar "<<archivo<<endl;
        }
    archivo[aux]='A';///de ataque
        fondo[2]=IMG_Load(archivo);
        if(!fondo[2]){
            cout<<"Error al cargar "<<archivo<<endl;
        }

    aux=strlen(ruta);
    archivo[0]='\0';
    strcat(archivo,ruta); strcat(archivo,"raum_.png");
    for(int i=0;i<5;i++){
        if(i<num_ram){
            archivo[aux+4]=nom_ram[i];
        }
        else{
            archivo[aux+4]='_';
        }
        ram[i]=IMG_Load(archivo);
        if(!ram[i]){
            cout<<"Error al cargar "<<archivo<<endl;
        }
    }

    archivo[0]='\0';
    strcat(archivo,ruta); strcat(archivo,nom_boton);///boton
    strcat(archivo,(char*)".png");
        boton=IMG_Load(archivo);
        if(!boton){
            cout<<"Error al cargar "<<archivo<<endl;
        }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void fondo_boton::imprimir_boton(bool B,SDL_Rect R,SDL_Surface* S){
SDL_Rect aux=R;
    if(B==true){
        aux.x+=(64-boton->w)/2;
        aux.y+=(64-boton->h)/2;
        SDL_BlitSurface(boton,0,S,&aux);
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void fondo_boton::imprimir_rojos(bool B,SDL_Rect R,SDL_Surface* S){
    if(B==true){///actualmente es atacado
        SDL_BlitSurface(fondo[2],0,S,&R);
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void fondo_boton::imprimir_fondo(bool B,SDL_Rect R,SDL_Surface* S){
    if(B==false){///casilla desocupada
        SDL_BlitSurface(fondo[0],0,S,&R);
    }
    else{///casilla previamente atacada
        SDL_BlitSurface(fondo[1],0,S,&R);
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void fondo_boton::imprimir_raum(bool B,SDL_Rect R,SDL_Surface* S,
                                int cual){
    if(B==false){///ES vacio
        SDL_BlitSurface(ram[cual],0,S,&R);
    }
    else{///el raum fue previamente atacado
        SDL_BlitSurface(fondo[1],0,S,&R);
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class coleccion_movimientos{
/// la dinamica con esta clase es muy similar a coleccion_objetos,
/// la diferencia es que esta se usa unicamente para los iconos de
/// las opciones
private:
    int n_opciones;
    char* nombre_opcion;
    SDL_Surface** opcion;
    SDL_Surface* mini_menu[3];///parche
public:
    coleccion_movimientos(char*, /// ruta
                          int,char*); ///numero y nombre de opciones
    coleccion_movimientos(){}///constructor para arreglos
    void imprimir_opcion(int,SDL_Rect,SDL_Surface*);
    /// las demas funciones necesitan saber cuantas opciones son
    /// posibles
    int cuantas_opciones();
    int busca_movimiento(char);
    /// con el fin de difundir el nombre estandar y la permutabilidad
    /// de las opciones, vale la pena tener a la mano una funcion
    /// que recupera tal nombre
    char nombre_de(int);

    void imprimir_menu(SDL_Surface*,SDL_Rect);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
coleccion_movimientos::coleccion_movimientos(char* ruta,
                      int num_opciones,char* nom_opciones){
char archivo[50]; int aux;
    archivo[0]='\0'; strcat(archivo,ruta);
    /// otro estandar inutil mio es que las imagenes-opcion se
    /// llamen CON_#, con # el nombre de la opcion
    strcat(archivo,(char*)"CON__"); aux=strlen(archivo)-1;
    strcat(archivo,(char*)".png");
    n_opciones=num_opciones+1;
    opcion=new SDL_Surface*[n_opciones];
    nombre_opcion=new char[n_opciones];
        nombre_opcion[0]='_'; /// la opcion volver se llama _
    /// estandar sujeto a cambios: el  simbolito de volver se
    /// llama CON__.png
    {
        opcion[0]=IMG_Load(archivo);
        if(!opcion[0]){
            cout<<"Error al cargar "<<archivo<<endl;
        }
    }
    for(int i=1;i<n_opciones;i++){
        nombre_opcion[i]=nom_opciones[i-1];
            archivo[aux]=nom_opciones[i-1];
        opcion[i]=IMG_Load(archivo);
        if(!opcion[i]){
            cout<<"Error al cargar "<<archivo<<endl;
        }
    }
    mini_menu[0]=IMG_Load((char*)"./bild/menu_texto.png");
    mini_menu[1]=IMG_Load((char*)"./bild/menu_otra.png");
    mini_menu[2]=IMG_Load((char*)"./bild/menu_menu.png");
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void coleccion_movimientos::imprimir_opcion(int N,
                                            SDL_Rect R,
                                            SDL_Surface* S){
SDL_Rect aux=R;
    if(N!=-1){
    aux.x+=(( 64 - opcion[N]->w )/2);
    aux.y+=(( 64 - opcion[N]->h )/2);
    SDL_BlitSurface(opcion[N],0,S,&aux);
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
int coleccion_movimientos::cuantas_opciones(){
    int n=n_opciones-1; return n;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
char coleccion_movimientos::nombre_de(int N){
char ans=-1;
    if(N>-1&&N<n_opciones){
        ans=nombre_opcion[N];
    }
return ans;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
int coleccion_movimientos::busca_movimiento(char K){
int ans=-1;
    for(int i=0;i<n_opciones;i++){
        if(nombre_opcion[i]==K){
            ans=i;
        }
    }
return ans;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void coleccion_movimientos::imprimir_menu(SDL_Surface* S,SDL_Rect R){
SDL_Rect aux=R;
    SDL_BlitSurface(mini_menu[0],0,S,&aux);
        aux.y+=60;
    SDL_BlitSurface(mini_menu[0],0,S,&aux);
        aux=R;
        aux.x+=(256-mini_menu[1]->w)/2;
        aux.y+=(60-mini_menu[1]->h)/2;
    SDL_BlitSurface(mini_menu[1],0,S,&aux);
        aux=R; aux.y+=60;
        aux.x+=(256-mini_menu[2]->w)/2;
        aux.y+=(60-mini_menu[2]->h)/2;
    SDL_BlitSurface(mini_menu[2],0,S,&aux);
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#endif
