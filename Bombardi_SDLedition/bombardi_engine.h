#ifndef BOMBARDI_ENGINE_H
#define BOMBARDI_ENGINE_H
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#include "eventos.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class bombardi_engine: public uniendo,public opcion{
private:
    SDL_Event* e;
    int n_sobrevivientes();///si hay sobrevivientes, el juego sigue
    int trofeo;/// el ultimo jugador en completar un turno

    bool es_posible(int,int);///verifica que la casilla haya sido
    ///marcada como posible para realizar el movimiento

    bool salida_prematura;///planeado para cuando se cierre el juego

    void siguiente_turno();
    int felicita_al_ganador();
    void ciclo_turno();
public:
    bombardi_engine(){salida_prematura=false;}
    void s_turno(){siguiente_turno();}
    bombardi_engine(SDL_Event,SDL_Surface**,
                    char*,char*,char*,char*,
                    int,char*,
                    int,char*,int,char*,
                    char*,char*,
                    int,
                    char*,int*,int*,bool);
    int per_se(); ///repite el ciclo turno hasta decarar un ganador
    bool dime_salida_prematura(){bool b=salida_prematura; return b;}
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
bombardi_engine::bombardi_engine(SDL_Event E,SDL_Surface** S,
                                 char* ruta,
                                 char* nom_logo,char* nom_credit,char* nom_arrow,
                                 //
                                 int num_opciones,char* nom_opciones,
                                 //
                                 int num_fichas,char* nom_fichas,
                                 int num_colores,char* nom_colores,
                                 //
                                 char* nom_fondo,char* nom_boton,
                                 //
                                 int num_jugadores,
                                 //
                                 char* rol_de,
                                 int* X0,int* Y0,
                                 bool hay_salvavidas)
:uniendo(S,ruta,nom_logo,nom_credit,nom_arrow,
         ruta,num_opciones,nom_opciones,
         ruta,num_fichas,nom_fichas,
         num_colores,nom_colores,
         ruta,nom_fondo,nom_boton,
         num_jugadores,rol_de,X0,Y0,hay_salvavidas)
{trofeo=0; e=&E; salida_prematura=false;}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void bombardi_engine::ciclo_turno(){
int turnos_trascurridos=0;
int OPT,CAS; ///variables de control y captura
int XX=0,YY=0; /// variable temporal del lugar de la jugada
char temp='_'; ///este cargara el nombre de la jugada
int abrev1=player[wo_ist_dran()].cuantos_turnos();
    while(salida_prematura==false && turnos_trascurridos<abrev1){
        eliminacion_por_ahogado(this);
        if(player[dran].vive()==false){
            break;///si el jugador muere, se sale del while
        }
        imprimir();
        poner_opciones(true,0);
        voltear_pantalla();
            OPT=-1,CAS=-2;
        while(salida_prematura==false && OPT==-1){
            while(SDL_PollEvent(e)){
                switch(e->type){
                    case SDL_QUIT:
                        salida_prematura=true;
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        OPT=leer_movimiento(e->button.x,
                                            e->button.y,C);
                        if(OPT<0 || OPT>=player[dran].cuantas_jugadas()){
                            OPT=-1;
                        }
                        else{
                            temp=player[dran].da_la_opcion(OPT);
                        }
                        break;
                    default:
                        break;
                }
            }
            imprimir();
            poner_opciones(true,0);
            voltear_pantalla();
        }
        ejecuta_botones(temp,this);
        imprimir();
        poner_opciones(false,OPT+1);
        voltear_pantalla();
        while(salida_prematura==false && CAS==-2){
        ///CAS==-1 significa que presionaron volver
            while(SDL_PollEvent(e)){
                switch(e->type){
                    case SDL_QUIT:
                        salida_prematura=true;
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        if(e->button.x<C.x){
                            CAS=leer_casilla(e->button.x,
                                             e->button.y,F);
                            if(CAS==-1){CAS=-2;}
                            else{
                                if(Bbuttons.valor(CAS/7,CAS%7)==false){
                                    CAS=-2;
                                }
                                else{
                                    XX=CAS%7; YY=CAS/7;
                                    Bbuttons.reinicia();
                                    ejecuta_opciona(temp,this,YY,XX);
                                    turnos_trascurridos++;
                                    trofeo=dran;
                                    cout<<"["<<(dran+1)<<"]"
                                        <<temp
                                        <<"("<<YY<<","
                                        <<XX<<")"<<endl;
                                }
                            }
                        ///mi error al coordinar valores-error
                        }
                        else{
                            CAS=leer_movimiento(e->button.x,
                                                e->button.y,C);
                            if(CAS==0){CAS=-1;}
                            else{CAS=-2;}
                        }
                        break;
                    default:
                        break;
                }
            }
            imprimir();
            poner_opciones(false,OPT+1);
            voltear_pantalla();
        }
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
int bombardi_engine::n_sobrevivientes(){
int ans=0;
    for(int i=0;i<cuantos_jugadores();i++){
        if(player[i].vive()==true){
            ans+=1;
        }
    }
return ans;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
int bombardi_engine::per_se(){
int ans=-1;
    while(salida_prematura==false && n_sobrevivientes()>1){
        ciclo_turno();
        siguiente_turno();
    }
    if(salida_prematura==false){
        ans=felicita_al_ganador();
    }
return ans;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void bombardi_engine::siguiente_turno(){
int n;
    for(int i=1;i<=cuantos_jugadores();i++){
        n=(dran+i)%cuantos_jugadores();
        if(player[n].vive()){
            dran=n;
            break;
        }
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
int bombardi_engine::felicita_al_ganador(){
bool esperando=false;
int aleatoriox=0,aleatorioy=0;
int especial; int regresando=-1;
tablero_booleano completez;
///busquemos al ultimo superviviente
    if(player[trofeo].vive()==false){
        for(int i=0;i<cuantos_jugadores();i++){
            if(player[i].vive()==true){
                trofeo=i;
            }
        }
    }
    ///una victoria limpia tiene como final tu fichita
    ///recorriendo el tablero, dejando bombas
    /// notese que esta funcocn es una total perdida de tiempo,
    /// en especial si al publico no le gusta
    if(player[trofeo].vive()){
        for(int i=0;i<7;i++){
            for(int j=0;j<7;j++){
                if(Bobjects.comparar(i,j,0)==false){
                    completez.haz1(i,j);
                }
            }
        }
        dran=trofeo;
        while(salida_prematura==false &&
              player[trofeo].vive()==true &&
              regresando==-1){
            while(SDL_PollEvent(e)){
                switch(e->type){
                    case SDL_QUIT:
                        salida_prematura=true;
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        if(e->button.x>A.x){
                            if(e->button.x<(A.x+256) &&
                                e->button.y>A.y && e->button.y<(A.y+96)){
                                esperando=false;
                                if(completez.cuenta()<49){
                                    while(esperando==false){
                                        aleatoriox=rand()%7;
                                        aleatorioy=rand()%7;
                                        if(completez.valor(aleatorioy,
                                                           aleatoriox)==false){
                                            ejecuta_opciona('b',this,
                                                            aleatorioy,
                                                            aleatoriox);
                                            completez.haz1(aleatorioy,
                                                           aleatoriox);
                                            esperando=true;
                                        }
                                    }
                                }
                            }
                            else{
                                if(e->button.x>C.x && e->button.x<(C.x+256) &&
                                   e->button.y>C.y && e->button.y<(C.y+60)){
                                        regresando=1;
                                        cout<<endl<<"AGAIN"<<endl<<endl;
                                }
                                else if(e->button.x>C.x && e->button.x<(C.x+256) &&
                                        e->button.y>C.y+60 && e->button.y<(C.y+120)){
                                            regresando=0;
                                            cout<<endl<<"MENU"<<endl<<endl;
                                }
                            }
                        }
                        else{
                            especial=leer_casilla(e->button.x,e->button.y,F);
                            if(especial!=-1){
                                if(completez.valor(especial/7,especial%7)==true &&
                                   Bobjects.comparar(especial/7,especial%7,'B')){
                                    ejecuta_opciona('D',this,especial/7,especial%7);
                                    completez.reinicia();
                                    for(int i=0;i<7;i++){
                                        for(int j=0;j<7;j++){
                                            if(Bobjects.comparar(i,j,0)==false){
                                                completez.haz1(i,j);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        break;
                    default:
                        break;
                }
            }
            imprimir();
            imprimir_mini_menu();
            voltear_pantalla();
        }
    }
/// en caso de empate, el ultimo jugador en completar un turno
/// tiene una media victoria, que DEBE sentirse menos
    if(player[trofeo].vive()==false && regresando==-1){
        for(int i=0;i<7;i++){
            for(int j=0;j<7;j++){
                Bbackground.haz1(i,j);
                Bobjects.cambiar(i,j,'F',0);
            }
        }
        Bobjects.cambiar(3,3,'F',trofeo+1);
        while(salida_prematura==false && regresando==-1){
            while(SDL_PollEvent(e)){
                switch(e->type){
                    case SDL_QUIT:
                        salida_prematura=true;
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        if(e->button.x>C.x && e->button.x<(C.x+256) &&
                           e->button.y>C.y && e->button.y<(C.y+60)){
                                regresando=1;
                                cout<<endl<<"AGAIN"<<endl<<endl;
                        }
                        else if(e->button.x>C.x && e->button.x<(C.x+256) &&
                                e->button.y>C.y+60 && e->button.y<(C.y+120)){
                                    regresando=0;
                                    cout<<endl<<"MENU"<<endl<<endl;
                        }
                        break;
                    default:
                        break;
                }
            }
            imprimir();
            imprimir_mini_menu();
            voltear_pantalla();
        }
    }
return regresando;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#endif
