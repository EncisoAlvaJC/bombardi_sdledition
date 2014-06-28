#ifndef JUGADORES_H
#define JUGADORES_H
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#include "tablero.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
typedef struct{
    char nombre;
    int n_turnos;
    int n_jugadas;
    char* jugada;
} papel;
class profesiones{
private:
    int n_roles;
    papel* rol;
    papel nulo;
public:
    profesiones();
    papel da_el_rol(char);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
profesiones::profesiones(){
    n_roles=4;
    rol=new papel[n_roles];

    nulo=(papel){'_',1,4,(char*)"MBPD"};
    rol[0]=(papel){'_',1,4,(char*)"MBPD"};
    rol[1]=(papel){'N',2,4,(char*)"NBPD"};
    rol[2]=(papel){'S',1,5,(char*)"MBPD*"};
    rol[3]=(papel){'f',1,3,(char*)"bPD"};
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
papel profesiones::da_el_rol(char K){
papel returner=nulo;
    for(int i=0;i<n_roles;i++){
        if(rol[i].nombre==K){
            returner=rol[i];
        }
    }
return returner;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class jugador{
private:
    bool humano;
    bool vivo;
    int n_turnos,n_opciones;
    int X,Y;
        /// ¿donde esta?
    int n_svidas,n_sbombas;
    char* opcionn;
        /// que opciones tiene permitidas
public:
    jugador(profesiones,char, ///numero de turnos, de opcios, las opciones
            int,int, /// posicion inicial
            bool); /// hay salvavidas (3)
    /// la clase jugador es muy importante en cuanto a privacidad:
    /// hay muchas funciones relativas al manejo de estos datos
    jugador(){}///constructor para arreglos
    ///
        bool es_humano(){return humano;}
    ///
    bool vive();
    void muere();
    bool tiene_salvavidas();
    bool tiene_salvabombas();
    void usa_salvavidas();
    void usa_salvabomba();
    void carga();
    int coordx();
    int coordy();
    int cuantos_turnos();
    int cuantas_jugadas();
    char da_la_opcion(int);
    void mover_a(int,int);
    /// las opciones se imprimen con los iconos y
    void imprimir_opciones(coleccion_movimientos,fondo_boton,
                           SDL_Rect,SDL_Surface*);
    void imprimir_regreso(int, ///imprime la opcion que tienes y el regreso
                          coleccion_movimientos,fondo_boton,
                          SDL_Rect,SDL_Surface*);
    void imprimir_jugador(int,coleccion_objetos,
                          SDL_Rect,SDL_Surface*);
    /// para variar, funciones utiles
    void impresion_condicionada(coleccion_objetos,tablero_objetos);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
jugador::jugador(profesiones elige,char R,
                 int y0,int x0,bool hay_salvavidas){
papel rol=elige.da_el_rol(R);
/// implementacion para 14.05.27
    humano=true;
/// fin de la actualizacion
    vivo=true; X=x0;Y=y0;
    n_turnos=rol.n_turnos;
    n_opciones=rol.n_jugadas;
/// hay num_opciones para NO tener coherencia con la clase
/// colecccion_opciones: estoy ignorando la opcion 0 (retorno)
    opcionn=new char[n_opciones+3];
    for(int i=0;i<n_opciones;i++){
        opcionn[i]=rol.jugada[i];
    }
    if(hay_salvavidas==true){
        n_svidas=3;
        opcionn[n_opciones]='S';
        opcionn[n_opciones+1]='T';
        opcionn[n_opciones+2]='K';
        n_opciones+=3;
    }
    else{
        n_svidas=0;
    }
    n_sbombas=0;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
bool jugador::vive(){return (vivo==true);}
void jugador::muere(){vivo=false;}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
bool jugador::tiene_salvavidas(){
    int n=n_svidas; return (bool)n;
}
bool jugador::tiene_salvabombas(){
    int n=n_sbombas; return (bool)n;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void jugador::usa_salvavidas(){ n_svidas-=1; }
void jugador::usa_salvabomba(){ n_sbombas-=1; }
void jugador::carga(){ n_svidas-=1; n_sbombas+=1; }
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
int jugador::coordx(){ int n=X; return n; }
int jugador::coordy(){ int n=Y; return n; }
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
int jugador::cuantos_turnos(){ int n=n_turnos; return n;}
int jugador::cuantas_jugadas(){ int n= n_opciones; return n; }
char jugador::da_la_opcion(int N){ char c=opcionn[N]; return c; }
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void jugador::mover_a(int y,int x){ X=x; Y=y; }
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void jugador::imprimir_jugador(int n_player,coleccion_objetos C,
                               SDL_Rect R,SDL_Surface* S){
SDL_Rect aux=R;
    if(vivo==true){
        /// esto no es un estandar sino una regla general que puede
        /// dejar de aplicar: las fichas miden 32x32 pixeles
        C.imprimir_ficha('F',n_player+1,aux,S);
        aux.x+=32;
        for(int i=0;i<n_svidas;i++,aux.x+=32){
            C.imprimir_ficha('S',n_player+1,aux,S);
        }
        for(int i=0;i<n_sbombas;i++,aux.x+=32){
            C.imprimir_ficha('T',n_player+1,aux,S);
        }
    }
    else{
        C.imprimir_ficha('F',0,aux,S);
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void jugador::imprimir_opciones(coleccion_movimientos M,
                                fondo_boton FB,
                                SDL_Rect R,SDL_Surface* S){
SDL_Rect aux=R; int i=0; int t=n_opciones;
/// lamentablemnte, esta version solo soporta un maximo de 8 opciones
/// y EXACTAMENTE 8 bloques
    for(;i<4;i++,aux.x+=64){
        FB.imprimir_fondo(false,aux,S);
    }
    aux=R; aux.y+=64;
    for(;i<8;i++,aux.x+=64){
        FB.imprimir_fondo(false,aux,S);
    }
    aux=R; int contador=0;
    for(i=0;i<t && i<8;i++,aux.x+=64,contador++){
        if(contador==4){
            aux=R; aux.y+=64;
        }
        M.imprimir_opcion(M.busca_movimiento(opcionn[i]),aux,S);
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void jugador::imprimir_regreso(int K,
                               coleccion_movimientos M,
                               fondo_boton FB,
                               SDL_Rect R,SDL_Surface* S){
SDL_Rect aux=R; aux.y+=64;
    FB.imprimir_fondo(false,R,S);
    M.imprimir_opcion(0,R,S);
    FB.imprimir_fondo(true,aux,S);
    M.imprimir_opcion(K,aux,S);
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#endif
