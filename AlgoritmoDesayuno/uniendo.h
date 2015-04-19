#ifndef UNIENDO_H
#define UNIENDO_H
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#include "control.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class uniendo:public profesiones{
/// tendre un apuntador a SDL_Surface* para evitar tener problemas
/// con el original
/// este no es estandar, pero me confunde manejar nombres
/// planeados para una funcion especifica y "entendible":
/// es por eso que los objetos en si tienen nombres en ingles
private:
    SDL_Surface** pantalla;
    SDL_Surface *logo,*credit,*arrow;
    fondo_boton Button;
    int num_players;
protected:
    int dran;
    SDL_Rect A,B,C,D,E,F;
public:
    //
    /// estas funciones forman parte de un parche
        uniendo(uniendo&);
        void reasimila_elementos(uniendo&);
        void set_dran(int D){dran=D;}
        void genera_polvoras(tablero_booleano*,tablero_booleano*);
    //
    uniendo(){}
    coleccion_movimientos Coptions;
    coleccion_objetos Cobjects;
    tablero_objetos Bobjects;
    tablero_booleano Bbackground,Bbuttons,Beffects;
    jugador* player;
/// advertencia: el constructor del uniendo es particularmente largo
/// y tedioso de manejar, pero ordenadamente es la suma de los
/// constructores anteriores y algun dato mas
    uniendo(SDL_Surface**, /// la pantalla a la cual apuntar
            char*,char*,char*,char*,/// ruta y nombres de logo...
            char*,int,char*,/// de coleccion_movimientos
            char*,int,char*,int,char*,///coleccion_objetos
            char*,char*,char*,///fondo_boton
            int,///numero de jugadores
            char*,int*,int*,bool,bool*);
                /// un arreglo de argumentos inicializadores
    /// el bool define el estado esperando-opcion/opcion-int-elegida
    uniendo(SDL_Surface**, /// la pantalla a la cual apuntar
            char*,char*,char*,char*,/// ruta y nombres de logo...
            char*,int,char*,/// de coleccion_movimientos
            char*,int,char*,int,char*,///coleccion_objetos
            char*,char*,char*,///fondo_boton
            int,///numero de jugadores
            char*,int*,int*,bool*);
                /// un arreglo de argumentos inicializadores
    /// el bool define el estado esperando-opcion/opcion-int-elegida
    void imprimir();
    void poner_opciones(bool,/// 0=no elegido; 1=opcion elegida
                        int);/// que opcion se ha elegido
    void incluir_ficha_extra(char,int,SDL_Rect);
    void voltear_pantalla();
    int wo_ist_dran();///en aleman "[de] quien es el turno"
    int cuantos_jugadores();
    int indice_de_la_opcion(char); ///otro regalo de privacidad
    uniendo* regalo_final(); /// el ultimo, lo juro
/// por si no se lo han preguntado, la idea de hacer publica la
/// funcion de impresion es que las funciones especificas de las
/// opciones puedan tener animaciones mientras realizan el turno
    void imprimir_mini_menu(){Coptions.imprimir_menu(*pantalla,C);}
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
uniendo::uniendo(SDL_Surface** S,
                 char* ruta_detalles,
                 char* nom_logo,char* nom_credit,char* nom_arrow,
                 //
                 char* ruta_opciones,
                 int num_opciones,char* nom_opciones,
                 //
                 char* ruta_objetos,
                 int num_fichas,char* nom_fichas,
                 int num_colores,char* nom_colores,
                 //
                 char* ruta_fondoboton,
                 char* nom_fondo,char* nom_boton,
                 //
                 int num_jugadores,
                 //
                 char* rol_de,
                 int* X0,int* Y0,
                 bool hay_salvavidas,
                 bool* es_humano)
:profesiones(){
char archivo[50]; int aux;
    archivo[0]='\0'; strcat(archivo,ruta_detalles);
    aux=strlen(archivo);
        strcat(archivo,nom_logo); strcat(archivo,(char*)".png");
        logo=IMG_Load(archivo);
        if(!logo){
            cout<<"Error al cargar "<<archivo<<endl;
        }
    archivo[aux]='\0';
        strcat(archivo,nom_credit); strcat(archivo,(char*)".png");
        credit=IMG_Load(archivo);
        if(!credit){
            cout<<"Error al cargar "<<archivo<<endl;
        }
    archivo[aux]='\0';
        strcat(archivo,nom_arrow); strcat(archivo,(char*)".png");
        arrow=IMG_Load(archivo);
        if(!arrow){
            cout<<"Error al cargar "<<archivo<<endl;
        }
    pantalla=S; dran=0;
    Coptions=coleccion_movimientos(ruta_opciones,
                                   num_opciones,nom_opciones);
    Cobjects=coleccion_objetos(ruta_objetos,
                               num_fichas,nom_fichas,
                               num_colores,nom_colores);
    Button=fondo_boton(ruta_fondoboton,
                       nom_fondo,nom_boton,nom_colores,num_colores);
    num_players=num_jugadores;
        player=new jugador[num_players];
    for(int i=0;i<num_players;i++){
        player[i]=jugador(*this,rol_de[i],
                          Y0[i],X0[i],
                          hay_salvavidas,
                          es_humano[i]);
        Bobjects.cambiar(Y0[i],X0[i],'F',i+1);
    }
    A.x=512;A.y=16; B.x=160;B.y=490;
    C.x=512;C.y=380;
    D.x=0;D.y=0;E.x=0;E.y=0;
    F.x=32; F.y=32;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
uniendo::uniendo(SDL_Surface** S,
                 char* ruta_detalles,
                 char* nom_logo,char* nom_credit,char* nom_arrow,
                 //
                 char* ruta_opciones,
                 int num_opciones,char* nom_opciones,
                 //
                 char* ruta_objetos,
                 int num_fichas,char* nom_fichas,
                 int num_colores,char* nom_colores,
                 //
                 char* ruta_fondoboton,
                 char* nom_fondo,char* nom_boton,
                 //
                 int num_jugadores,
                 //
                 char* rol_de,
                 int* X0,int* Y0,
                 bool* es_humano)
:profesiones(){
char archivo[50]; int aux;
    archivo[0]='\0'; strcat(archivo,ruta_detalles);
    aux=strlen(archivo);
        strcat(archivo,nom_logo); strcat(archivo,(char*)".png");
        logo=IMG_Load(archivo);
        if(!logo){
            cout<<"Error al cargar "<<archivo<<endl;
        }
    archivo[aux]='\0';
        strcat(archivo,nom_credit); strcat(archivo,(char*)".png");
        credit=IMG_Load(archivo);
        if(!credit){
            cout<<"Error al cargar "<<archivo<<endl;
        }
    archivo[aux]='\0';
        strcat(archivo,nom_arrow); strcat(archivo,(char*)".png");
        arrow=IMG_Load(archivo);
        if(!arrow){
            cout<<"Error al cargar "<<archivo<<endl;
        }
    pantalla=S; dran=0;
    Coptions=coleccion_movimientos(ruta_opciones,
                                   num_opciones,nom_opciones);
    Cobjects=coleccion_objetos(ruta_objetos,
                               num_fichas,nom_fichas,
                               num_colores,nom_colores);
    Button=fondo_boton(ruta_fondoboton,
                       nom_fondo,nom_boton,nom_colores,num_colores);
    num_players=num_jugadores;
        player=new jugador[num_players];
    for(int i=0;i<num_players;i++){
        player[i]=jugador(*this,rol_de[i],
                          Y0[i],X0[i],
                          es_humano[i]);
        Bobjects.cambiar(Y0[i],X0[i],'F',i+1);
    }
    A.x=512;A.y=16; B.x=160;B.y=8;
    C.x=512;C.y=380;
    D.x=0;D.y=0;E.x=0;E.y=0;
    F.x=32; F.y=32;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
uniendo::uniendo(uniendo& V){
    pantalla=V.pantalla;
    logo=V.logo;
    credit=V.credit;
    arrow=V.arrow;
    Button=V.Button;

    num_players=V.num_players;
    dran=V.dran;

    A=V.A; B=V.B; C=V.C; D=V.D; E=V.E; F=V.F;

    Coptions=V.Coptions;
    Cobjects=V.Cobjects;

    Bobjects=tablero_objetos(V.Bobjects);
    Bbackground=tablero_booleano(V.Bbackground);
    Bbuttons=tablero_booleano(V.Bbuttons);
    Beffects=tablero_booleano(V.Beffects);

    player=new jugador[num_players];
    for(int i=0;i<num_players;i++){
        player[i]=jugador(V.player[i]);
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void uniendo::reasimila_elementos(uniendo& V){
    dran=V.dran;
    Bobjects=tablero_objetos(V.Bobjects);
    Bbackground=tablero_booleano(V.Bbackground);
    Bbuttons=tablero_booleano(V.Bbuttons);
    Beffects=tablero_booleano(V.Beffects);
    for(int i=0;i<num_players;i++){
        player[i]=jugador(V.player[i]);
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void uniendo::imprimir(){
        D.x=544;D.y=128;
        E.x=512;E.y=130+48*dran;
        /// otra regla general es el alto de la ficha: 48 pixles
    SDL_FillRect(*pantalla,0,
                 SDL_MapRGB((*pantalla)->format,114,142,177));
    SDL_BlitSurface(logo,0,*pantalla,&A);
    SDL_BlitSurface(credit,0,*pantalla,&B);
    Bbackground.imprimir_fondo(Button,*pantalla);
    Beffects.imprimir_rojos(Button,*pantalla);
    Bobjects.imprimir(Cobjects,*pantalla);
    Bbuttons.imprimir_boton(Button,*pantalla);
    for(int i=0;i<num_players;i++,D.y+=48){
        player[i].imprimir_jugador(i,Cobjects,D,*pantalla);
    }
    E.x+=(64-arrow->w)/2; E.y+=(64-arrow->h)/2;
    SDL_BlitSurface(arrow,0,*pantalla,&E);
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void uniendo::poner_opciones(bool b,int N){
    if(b==true){
        player[dran].imprimir_opciones(Coptions,Button,C,*pantalla);
    }
    else{
        player[dran].imprimir_regreso(N,Coptions,Button,C,*pantalla);
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void uniendo::voltear_pantalla(){
    SDL_Flip(*pantalla);
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void uniendo::incluir_ficha_extra(char K,int N,SDL_Rect R){
    Cobjects.imprimir_ficha(K,N,R,*pantalla);
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/// el objetivo de esta funcion muy tonta es que se SEPA de quien es
/// el turno pero no se pueda modificar
int uniendo::wo_ist_dran(){
    int a=dran; return a;
}
int uniendo::cuantos_jugadores(){
    int n=num_players; return n;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
int uniendo::indice_de_la_opcion(char K){
    return Coptions.busca_movimiento(K);
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
uniendo* uniendo::regalo_final(){
    return this;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#endif
