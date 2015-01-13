#ifndef CONTROL_H
#define CONTROL_H
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#include "jugadores.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/// la idea de control.h es poner dos funciones para el
/// reconocimiento de un click (SDL_Event) como un boton...
/// bueno, se le pasan como argumentos las coordenadas del click
int leer_movimiento(int X,int Y,SDL_Rect R){
SDL_Rect aux=R; int i; int returner=-1;
    for(i=0;i<4;i++,aux.x+=64){
        if(aux.x<X && X<(aux.x+64) && aux.y<Y && Y<(aux.y+64)){
            returner=i;
        }
    }
    aux=R; aux.y+=64;
    for(i=0;i<4;i++,aux.x+=64){
        if(X>aux.x && X<(aux.x+64) && Y>aux.y && Y<aux.y+64){
            returner=i+4;
        }
    }
return returner;
}
/// la razon por la que esta funcion necesita recibir un SDL_Rect es
/// que TAL VEZ quiera cambiar su ubicacion
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
int leer_casilla(int X,int Y,SDL_Rect R){
SDL_Rect aux1,aux2; int returner=-1;
    aux1=R;
    for(int i=0;i<8;i++,aux1.y+=64){
        aux2=aux1;
        for(int j=0;j<7;j++,aux2.x+=64){
            if(aux2.x<X && X<(aux2.x+64) && aux2.y<Y && Y<(aux2.y+64)){
                if(i<7){
                    returner=7*i+j;
                }
                else{
                    if(0<j && j<6){
                        returner=50+j-1;
                    }
                }
            }
        }
    }
return returner;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
int leer_de_cuadricula(int X,int Y,SDL_Rect R,int limy,int limx){
SDL_Rect aux1,aux2; int returner=-1;
    aux1=R;
    for(int i=0;i<limy;i++,aux1.y+=64){
        aux2=aux1;
        for(int j=0;j<limx;j++,aux2.x+=64){
            if(aux2.x<X && X<(aux2.x+64) && aux2.y<Y && Y<(aux2.y+64)){
                returner=limx*i+j;
            }
        }
    }
return returner;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#endif
