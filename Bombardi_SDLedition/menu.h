#ifndef MENU_H
#define MENU_H
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#include "bombardi_engine.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class menu{
/// la idea del menu -en forma interna- es declarar y personalizar
/// las cadenas de caracteres y enteros que rigen el juego -es decir,
/// las partes en que se puede elegir de las reglas
private:
    /// guarda un apuntador a la pantalla por compacidad del codigo
    SDL_Surface** pantalla;
    SDL_Event* evento;

    /// graficos de los elementos de control y el fondo
    SDL_Surface* fondo;
    SDL_Surface* flecha[3];
    SDL_Surface* cuadro_unido;
    SDL_Surface* numero[5];

    SDL_Surface* X;

    /// informacion sobre los "colores" de ficha disponibles
    int no_colores;
    char* colores;
    /// las fichas de "colores" en si
    SDL_Surface** ficha;

    /// la idea de estas dos funciones es hacer mas compacto y
    /// legible el codigo:

    /// cuando se anade un jugador, el seleccionador debe
    /// elegir por defecto un color que no este repetido
    void inicializa(int);
    /// el color aleatorio [?] no es un color sino ordenar al
    /// programa elegir uno aleatoriamente: eso es lo que hace
    /// esta funcion
    void elige_aleatorios();

    /// mas tarde implementare los juegos multiples; por lo
    /// mientras hay un unico "juego" funcional
    SDL_Surface* cuadro_texto;
    //int n_juegos;
    //SDL_Surface** nombre;
        SDL_Surface* nombre;
    SDL_Surface* boton_jugar;

    bool salida_prematura;

    bool hay_salvavidas;
    int n_jugadores;
    char* colores_elegidos;
    int* indices;
public:
    menu(SDL_Surface*,SDL_Event,char*,int,char*);
    void elegir();

    bool dime_hay_salvavidas(){bool b=hay_salvavidas; return b;}
    int dime_n_jugadores(){int n=n_jugadores; return n;}
    char* dime_colores_elegidos(){char* n=colores_elegidos; return n;}
    bool dime_salida_prematura(){bool b=salida_prematura; return b;}
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
menu::menu(SDL_Surface* screen,SDL_Event E,
           char* ruta,int n_colores,char* nom_colores){
hay_salvavidas=0; n_jugadores=2; salida_prematura=false;
pantalla=&screen; no_colores=n_colores; evento=&E;
    colores=new char[no_colores+1];
    ficha=new SDL_Surface*[no_colores+1];
        for(int i=0;i<no_colores;i++){
            colores[i+1]=nom_colores[i];
        }
    colores_elegidos=new char[5];
    indices=new int[5];
        colores_elegidos[0]=colores[1];
        colores_elegidos[1]=colores[2];
        indices[0]=1; indices[1]=2;

/// como siempre, todas las imagenes son png
char archivo[50]; char extension[]=".png"; int salto;
    archivo[0]='\0'; strcat(archivo,ruta);
    strcat(archivo,(char*)"menu_");
    salto=strlen(archivo);
/// se procede a cargar el fondo
    strcat(archivo,(char*)"fondo");strcat(archivo,extension);
    fondo=IMG_Load(archivo);
    if(!fondo){
        cout<<"Error al cargar "<<archivo<<endl;
    }
    archivo[salto]='\0';
/// se cargan los elemtos de control (flechas y todo eso)
    strcat(archivo,(char*)"i");strcat(archivo,extension);
    flecha[0]=IMG_Load(archivo);
    if(!flecha[0]){
        cout<<"Error al cargar "<<archivo<<endl;
    }
    archivo[salto]='\0';

    strcat(archivo,(char*)"cuadro");strcat(archivo,extension);
    flecha[1]=IMG_Load(archivo);
    if(!flecha[1]){
        cout<<"Error al cargar "<<archivo<<endl;
    }
    archivo[salto]='\0';

    strcat(archivo,(char*)"d");strcat(archivo,extension);
    flecha[2]=IMG_Load(archivo);
    if(!flecha[2]){
        cout<<"Error al cargar "<<archivo<<endl;
    }
    archivo[salto]='\0';

    strcat(archivo,(char*)"select");strcat(archivo,extension);
    cuadro_unido=IMG_Load(archivo);
    if(!cuadro_unido){
        cout<<"Error al cargar "<<archivo<<endl;
    }
    archivo[salto]='\0';

    strcat(archivo,(char*)"x");strcat(archivo,extension);
    X=IMG_Load(archivo);
    if(!X){
        cout<<"Error al cargar "<<archivo<<endl;
    }
    archivo[salto]='\0';
/// ahora los numeros
    for(int i=0;i<5;i++){
        archivo[salto]=i+'1';archivo[salto+1]='\0';
        strcat(archivo,extension);
        numero[i]=IMG_Load(archivo);
        if(!numero[i]){
            cout<<"Error al cargar "<<archivo<<endl;
        }
        archivo[salto]='\0';
    }
/// ahora se cargan los letreros con letras
    strcat(archivo,(char*)"texto");strcat(archivo,extension);
    cuadro_texto=IMG_Load(archivo);
    if(!cuadro_texto){
        cout<<"Error al cargar "<<archivo<<endl;
    }
    archivo[salto]='\0';

    strcat(archivo,(char*)"normal");strcat(archivo,extension);
    nombre=IMG_Load(archivo);
    if(!nombre){
        cout<<"Error al cargar "<<archivo<<endl;
    }
    archivo[salto]='\0';

    strcat(archivo,(char*)"start");strcat(archivo,extension);
    boton_jugar=IMG_Load(archivo);
    if(!boton_jugar){
        cout<<"Error al cargar "<<archivo<<endl;
    }
    archivo[salto]='\0';
/// finalmente se cargan las fichas como muestra
    strcat(archivo,(char*)"rand");strcat(archivo,extension);
    ficha[0]=IMG_Load(archivo);
    if(!ficha[0]){
        cout<<"Error al cargar "<<archivo<<endl;
    }
    colores[0]=-1;


    archivo[0]='\0'; strcat(archivo,ruta);
    salto=strlen(archivo);
    archivo[salto]='F';archivo[salto+2]='\0';
    strcat(archivo,extension);

    for(int i=0;i<no_colores;i++){
        archivo[salto+1]=nom_colores[i];
        ficha[i+1]=IMG_Load(archivo);
        if(!ficha[i+1]){
            cout<<"Error al cargar "<<archivo<<endl;
            colores[i+1]=-1;
        }
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void menu::inicializa(int quien){
/// recorreremos los primeros colores; si alguno no
/// esta ocupado, lo tomaremos, asi que recorremos los quien-1
/// jugadores anteriores buscando repetidos
bool esta_ocupado;
    for(int i=1; ;i++){
        esta_ocupado=false;
        for(int j=0;j<quien;j++){
            if(indices[j]==i){
                esta_ocupado=true;
                break;
            }
        }
        if(esta_ocupado==false){
            colores_elegidos[quien]=colores[i];
            indices[quien]=i;
            break;
        }
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void menu::elige_aleatorios(){
int elect=0; bool esta_ocupado;
    for(int i=0;i<n_jugadores;i++){
        if(colores_elegidos[i]==-1){
            while(colores_elegidos[i]==-1){
            ///esto es redundante pero busco que NO haya repetidos
                elect=rand()%(no_colores)+1;
                esta_ocupado=false;
                for(int j=0;j<i;j++){
                    if(indices[j]==elect){
                        esta_ocupado=true;
                    }
                }
                if(esta_ocupado==false){
                    colores_elegidos[i]=colores[elect];
                    indices[i]=elect;
                }
            }
        }
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void menu::elegir(){
int auxint,x,y; bool seleccionado;

SDL_Rect cero,cuadros[20],aux;
    cero.x=0;cero.y=0;

    cuadros[0].x=32;cuadros[0].y=96;
    cuadros[1].x=192;cuadros[1].y=224;
    cuadros[3].x=32;cuadros[3].y=224;
    cuadros[2].x=192;cuadros[2].y=96;
    cuadros[4].x=112;cuadros[4].y=160;

    cuadros[5].x=112;cuadros[5].y=32;
    cuadros[6].x=160;cuadros[6].y=32;
    cuadros[7].x=224;cuadros[7].y=32;

    cuadros[8].x=124;cuadros[8].y=352;

    cuadros[9].x=512; cuadros[9].y=158;
    cuadros[10].x=512;cuadros[10].y=218;

bool start=false;
    while(salida_prematura==false && start==false){
        while(SDL_PollEvent(evento)){
            switch(evento->type){
                case SDL_QUIT:
                    salida_prematura=true;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    x=evento->button.x;
                    y=evento->button.y;
                    if(x>cuadros[10].x){
                    /// esta del lado del boton START
                        if(x<(cuadros[10].x+256) &&
                           y>cuadros[10].y &&
                           y<(cuadros[10].y+60)){
                                start=true;
                           }
                    }
                    else if(y>cuadros[8].y){
                    /// esta debajo del boton "hay_salvavidas"
                        if(x>cuadros[8].x &&
                           x<(cuadros[8].x+96) &&
                           y<(cuadros[8].y+96)){
                                hay_salvavidas=!hay_salvavidas;
                           }
                    }
                    else if(y<cuadros[0].y){
                    /// esta arriba de los selectores de color
                        if(y>cuadros[5].y){
                        /// recordando que las dos flechas estan
                        /// a la misma altura
                            if(x>cuadros[5].x &&
                               x<(cuadros[5].x+48) &&
                               n_jugadores>1){
                                    n_jugadores--;
                            }
                            else if(x>cuadros[7].x &&
                                    x<(cuadros[7].x+48) &&
                                    n_jugadores<5){
                                        inicializa(n_jugadores);
                                        n_jugadores++;
                            }
                        }
                    }
                    else{
                        for(int j=0;j<n_jugadores;j++){
                            if(x>cuadros[j].x && x<(cuadros[j].x)+48 &&
                            y>cuadros[j].y && y<(cuadros[j].y)+64){
                                seleccionado=false;
                                while(seleccionado==false){
                                    indices[j]=(indices[j]+no_colores)%(no_colores+1);
                                    seleccionado=true;
                                    for(int i=0;i<n_jugadores;i++){
                                        if(i!=j && indices[i]!=0 &&
                                           indices[j]==indices[i]){
                                            seleccionado=false;
                                            break;
                                        }
                                    }
                                }
                                colores_elegidos[j]=colores[indices[j]];
                            }
                            else if(x>(cuadros[j].x+112) && x<(cuadros[j].x+160) &&
                                y>cuadros[j].y && y<(cuadros[j].y)+64){
                                    seleccionado=false;
                                while(seleccionado==false){
                                    indices[j]=(indices[j]+no_colores+2)%(no_colores+1);
                                    seleccionado=true;
                                    for(int i=0;i<n_jugadores;i++){
                                        if(i!=j && indices[i]!=0 &&
                                           indices[j]==indices[i]){
                                            seleccionado=false;
                                            break;
                                        }
                                    }
                                }
                                colores_elegidos[j]=colores[indices[j]];
                            }
                        }
                    }
                    break;
                default:
                    break;
            }
        }
        SDL_FillRect(*pantalla,0,
                     SDL_MapRGB((*pantalla)->format,114,142,177));
        SDL_BlitSurface(fondo,0,*pantalla,&cero);

    /// imprimir slector de numero de jugadores
        SDL_BlitSurface(flecha[1],0,*pantalla,&cuadros[6]);
            aux=cuadros[6];
            aux.x+=(64-numero[n_jugadores-1]->w)/2;
            aux.y+=(64-numero[n_jugadores-1]->h)/2;
        SDL_BlitSurface(numero[n_jugadores-1],0,*pantalla,&aux);
        if(n_jugadores>1){
            SDL_BlitSurface(flecha[0],0,*pantalla,&cuadros[5]);
        }
        if(n_jugadores<5){
            SDL_BlitSurface(flecha[2],0,*pantalla,&cuadros[7]);
        }
    /// imprimir los selectores de color de los jugadores
        switch(n_jugadores){
            case 5:
                SDL_BlitSurface(cuadro_unido,0,*pantalla,&cuadros[4]);
                    aux=cuadros[4];aux.x+=48;
                    auxint=indices[4];
                    aux.x+=(64-ficha[auxint]->w)/2;
                    aux.y+=(64-ficha[auxint]->h)/2;
                SDL_BlitSurface(ficha[auxint],0,*pantalla,&aux);
            case 4:
                SDL_BlitSurface(cuadro_unido,0,*pantalla,&cuadros[3]);
                    aux=cuadros[3];aux.x+=48;
                    auxint=indices[3];
                    aux.x+=(64-ficha[auxint]->w)/2;
                    aux.y+=(64-ficha[auxint]->h)/2;
                SDL_BlitSurface(ficha[auxint],0,*pantalla,&aux);
            case 3:
                SDL_BlitSurface(cuadro_unido,0,*pantalla,&cuadros[2]);
                    aux=cuadros[2];aux.x+=48;
                    auxint=indices[2];
                    aux.x+=(64-ficha[auxint]->w)/2;
                    aux.y+=(64-ficha[auxint]->h)/2;
                SDL_BlitSurface(ficha[auxint],0,*pantalla,&aux);
            case 2:
                SDL_BlitSurface(cuadro_unido,0,*pantalla,&cuadros[1]);
                    aux=cuadros[1];aux.x+=48;
                    auxint=indices[1];
                    aux.x+=(64-ficha[auxint]->w)/2;
                    aux.y+=(64-ficha[auxint]->h)/2;
                SDL_BlitSurface(ficha[auxint],0,*pantalla,&aux);
            case 1:
                SDL_BlitSurface(cuadro_unido,0,*pantalla,&cuadros[0]);
                    aux=cuadros[0];aux.x+=48;
                    auxint=indices[0];
                    aux.x+=(64-ficha[auxint]->w)/2;
                    aux.y+=(64-ficha[auxint]->h)/2;
                SDL_BlitSurface(ficha[auxint],0,*pantalla,&aux);
                break;
            default:
                break;
        }
    /// imprimir el tache para cuando hay o no salvavidas
        if(hay_salvavidas==false){
            SDL_BlitSurface(X,0,*pantalla,&cuadros[8]);
        }
    /// imprimir el boton de inicio y el selector de juego
        SDL_BlitSurface(cuadro_texto,0,*pantalla,&cuadros[9]);
            aux=cuadros[9];
            aux.x+=(256-nombre->w)/2;
            aux.y+=(60-nombre->h)/2;
        SDL_BlitSurface(nombre,0,*pantalla,&aux);

        SDL_BlitSurface(cuadro_texto,0,*pantalla,&cuadros[10]);
            aux=cuadros[10];
            aux.x+=(256-boton_jugar->w)/2;
            aux.y+=(60-boton_jugar->h)/2;
        SDL_BlitSurface(boton_jugar,0,*pantalla,&aux);
    /// fin de la impresion
        SDL_Flip(*pantalla);
    }
elige_aleatorios();
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#endif
