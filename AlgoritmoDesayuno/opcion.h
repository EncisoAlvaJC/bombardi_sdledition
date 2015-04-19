#ifndef EVENTOS_H
#define EVENTOS_H
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#include "uniendo.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/// el nucleo de este archivo es definir la clase opcion que
/// tiene el nombre de la opcion, una funcion que ejecuta el
/// movimiento y otra que indica -en botones- donde se puede realizar
/// esta accion
/// la idea final es definir el conjunto de reglas como un arreglo
/// opciones
typedef struct{
    char nombre;
    void (*botones)(uniendo*);
    void (*accion)(uniendo*,int,int);
    void (*simula)(uniendo*,int,int);
} jugada;
typedef struct{
    char nombre;
    void (*funcion)(uniendo*);
} paso;
/// un "glitch" de C++ es que no puedo declarar arreglos de
/// apuntadores a funciones... pero puedo hacer un struct con un
/// apuntador a funcion como unico elemento y luego arreglos
/// multidimensionales dinamicos de ese struct
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class opcion{
private:
    int n_opciones;
    jugada* opciona;
    jugada nula;

    int n_tests;
    paso* test; ///cosas como eliminacion por ahogado o detonacion
    /// automatica de bombas
    paso nulb;

    int n_roles;
    papel* rol;
    papel nulc;
public:
    opcion();
    int busca_opcion(char);
    void ejecuta_botones(char,uniendo*);
    void ejecuta_opciona(char,uniendo*,int,int);
    void ejecuta_simula(char,uniendo*,int,int);

    papel da_el_rol(char);

    void eliminacion_por_ahogado(uniendo*);
    ///esta es importante y unica [hasta enero de 2014 :D]
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
int opcion::busca_opcion(char K){
///hay una razon epecial para esta funcion: definir el vector ventaja
int ans=-1;
    for(int i=0;i<n_opciones;i++){
        if(opciona[i].nombre==K){
            ans=i;
        }
    }
return ans;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void opcion::ejecuta_botones(char K,uniendo* U){
int temp=busca_opcion(K);
    if(temp!=-1){
        opciona[temp].botones(U);
    }
    else{
        nula.botones(U);
    }
}
void opcion::ejecuta_opciona(char K,uniendo* U,int y,int x){
int temp=busca_opcion(K);
    if(temp!=-1){
        opciona[temp].accion(U,y,x);
    }
    else{
        nula.botones(U);
    }
}
void opcion::ejecuta_simula(char K,uniendo* U,int y,int x){
int temp=busca_opcion(K);
    if(temp!=-1){
        opciona[temp].simula(U,y,x);
    }
    else{
        nula.botones(U);
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
papel opcion::da_el_rol(char K){
papel returner=nulc;
    for(int i=0;i<n_opciones;i++){
        if(rol[i].nombre==K){
            returner=rol[i];
        }
    }
return returner;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#endif
