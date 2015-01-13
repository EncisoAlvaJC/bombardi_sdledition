#ifndef OPCION_H
#define OPCION_H
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#include "opcion.h"
/// basicamenta, aqui estan definidas
//TODAS las jugadas permitidas
//// en Bombardi, al menos hasta el 31/diciembre/2013
/// algunas de los movimientos de Kalium al 31/diciembre/2014
//// planeo que, en el futuro, este sea el unico archivo que tenga
//// que actualizarse publicamente
/// este prototipo solo existe para probar IA's y exhibirlas
///
/// Si alguien lee esta nota en el futuro, pues las cosas estan chidas
/// en el 2015, esperando el regreso de Marty McFly
/// Termine el diciembre pasado lo que llamo el "Algoritmo Bombardi"
/// con un tipo especial de cadenas de Markov que no se como se
/// llaman, y tengo una gran fe en ellas
/// El mejor anime que he visto es Madoka Magica: Rebellion, y el
/// mejor libro fue 100 anhos de soledad, superando a La mecanica del
/// corazon
/// Como proyecto alterno a Kalium tengo uno sobre un duelo de magos
/// con fichas elementales y cartas. No se olviden Rover Run, Las
/// leyendas de Savant-Hill
/// Con TMH lo nuevo es una historia sobre drogas y el plano astral
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void mover(uniendo* U,int y,int x){
/// la idea es simple a pesar de lo complicado que se ve:
/// -se obtinen las coordenadas de la ficha a mover
/// -se obtiene la "direccion" del movimiento
/// -se preparan unas coordenadas temporales que se "moveran" desde
///  las coordenadas actuales de la ficha hacia las finales
/// -se borra la ficha
/// -se imprime la ficha en los espacios intermedios
/// -se coloca la ficha en su posicion final
/// en resumen, es un quita-mueve-pon
int X=0,Y=0,dir_x,dir_y,en_turno;
SDL_Rect temp;
    en_turno=U->wo_ist_dran();
    if(U->player[en_turno].coordy()==-1){
        X=U->player[en_turno].coordx();
        Y=U->player[en_turno].coordy();
        U->Bobjects.elimina(Y,X);
        /// una parte no trivial de salir de un portal es decidir de cual
        bool hallado=false;
        /// AA
        coordenada C=U->Bobjects.colindante_a(y,x,'A','A');
        if(C.valido && U->Bobjects.comparar(C.yy,C.xx,'R')){
            hallado=true;
            X=C.xx; Y=C.yy;
        }
        /// <A
        if(hallado==false){
            C=U->Bobjects.colindante_a(y,x,'A','<');
        }
        if(hallado==false && C.valido && U->Bobjects.comparar(C.yy,C.xx,'R')){
            hallado=true;
            X=C.xx; Y=C.yy;
        }
        /// <<
        if(hallado==false){
            C=U->Bobjects.colindante_a(y,x,'<','<');
        }
        if(hallado==false && C.valido && U->Bobjects.comparar(C.yy,C.xx,'R')){
            hallado=true;
            X=C.xx; Y=C.yy;
        }
        /// <V
        if(hallado==false){
            C=U->Bobjects.colindante_a(y,x,'V','<');
        }
        if(hallado==false && C.valido && U->Bobjects.comparar(C.yy,C.xx,'R')){
            hallado=true;
            X=C.xx; Y=C.yy;
        }
        /// VV
        if(hallado==false){
            C=U->Bobjects.colindante_a(y,x,'V','V');
        }
        if(hallado==false && C.valido && U->Bobjects.comparar(C.yy,C.xx,'R')){
            hallado=true;
            X=C.xx; Y=C.yy;
        }
        /// V>
        if(hallado==false){
            C=U->Bobjects.colindante_a(y,x,'V','>');
        }
        if(hallado==false && C.valido && U->Bobjects.comparar(C.yy,C.xx,'R')){
            hallado=true;
            X=C.xx; Y=C.yy;
        }
        /// >>
        if(hallado==false){
            C=U->Bobjects.colindante_a(y,x,'>','>');
        }
        if(hallado==false && C.valido && U->Bobjects.comparar(C.yy,C.xx,'R')){
            hallado=true;
            X=C.xx; Y=C.yy;
        }
        /// >A
        if(hallado==false){
            C=U->Bobjects.colindante_a(y,x,'A','>');
        }
        if(hallado==false && C.valido && U->Bobjects.comparar(C.yy,C.xx,'R')){
            hallado=true;
            X=C.xx; Y=C.yy;
        }
    }
    else{
        X=U->player[en_turno].coordx();
        Y=U->player[en_turno].coordy();
        U->Bobjects.elimina(Y,X);
    }
    dir_x=x-X; dir_y=y-Y;
    temp.x=32+64*X; temp.y=32+64*Y;

    if(U->player[en_turno].coordy()!=-1 || y!=-1){
        for(int i=0;i<64;i++){
            U->imprimir();
            U->incluir_ficha_extra('F',en_turno+1,temp);
            U->voltear_pantalla();
            temp.x+=dir_x; temp.y+=dir_y;
        }
    }

    if(U->Bobjects.comparar(y,x,'R')){
        int color=U->Bobjects.el_color_de(y,x)-1;
        U->player[en_turno].mover_a(-1,color);
        U->Bobjects.cambiar(-1,color,'F',en_turno+1);
    }
    else{
        U->player[en_turno].mover_a(y,x);
        U->Bobjects.cambiar(y,x,'F',en_turno+1);
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void mover_simula(uniendo* U,int y,int x){
int X,Y,en_turno;
    en_turno=U->wo_ist_dran();
    X=U->player[en_turno].coordx();
    Y=U->player[en_turno].coordy();
    U->Bobjects.elimina(Y,X);
    U->player[en_turno].mover_a(y,x);
    U->Bobjects.cambiar(y,x,'F',en_turno+1);
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void bomba(uniendo* U,int y, int x){
    U->Bobjects.cambiar(y,x,'B',U->wo_ist_dran()+1);
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void pared(uniendo* U,int y, int x){
    U->Bobjects.cambiar(y,x,'P',U->wo_ist_dran()+1);
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
///la funcion de detonacion es la mas compleja de todas, pero a la
///vez es la mas importante del juego
///para facilitar la sintaxis se divide en tres funciones:
/// -una detona una casilla en particular, danando el objeto en caso
///  de que aplique. funciona como "tabla de conversion por dano"
/// -PARCHE una redirige la explosion de un portal
/// -una detona una bomba en particular, invocando la funcion
///  anterior para danar segun el camino predeterminado. funciona
///  "ruta de detonacion". no dira nada respecto a la bomba en si
/// -la definitiva declara un tablero_booleano donde anota las
///  bombas que se activaron durante la detonacion, y luego lo
///  recorre para detonarlas
void detona1(uniendo* U,int y,int x,tablero_booleano* pendientes,
             bool* inmune){
int color=U->Bobjects.el_color_de(y,x);
    U->Bbackground.haz1(y,x);
    if(U->Bobjects.comparar(y,x,0) || color==0
       || U->Bobjects.comparar(y,x,'V')){
    ///para ahorrar tiempo, verifica y no hace nada si haay un 0
    /// si tiene color 0 y no es vacio, es una ficha-ceniza
    }
    else if(U->Bobjects.comparar(y,x,'F')){
        if(inmune[color-1]==false){
            if(U->player[color-1].tiene_salvavidas()){
                U->player[color-1].carga();
                inmune[color-1]=true;
            }
            else{
                U->player[color-1].muere();
                U->Bobjects.haz_ceniza(y,x);
            }
        }
    }
    else if(U->Bobjects.comparar(y,x,'B') ||
            U->Bobjects.comparar(y,x,'T')){
                pendientes->haz1(y,x);
            ///aqui hay algo que aclara y es que a este punto
            ///el color de la bomba NO es 0; de ahi tomo como un
            ///estadar que una bomba pendiente conserva su color,
            ///y al detonar su color sera 0
    }
    else{
        U->Bobjects.haz_ceniza(y,x);
    }
}
void detona1_2(uniendo* U,int y,int x,tablero_booleano* pendientes,bool* inmune){
    /// si atacas un portal, redirige la explosion
    if(U->Bobjects.comparar(y,x,'R')){
        int color=U->Bobjects.el_color_de(y,x)-1;
        detona1(U,-1,color,pendientes,inmune);
        U->Bbackground.haz1(y,x);
        U->Beffects.haz1(y,x);
    }
    else{
        detona1(U,y,x,pendientes,inmune);
    }
}
void detona2(uniendo* U,int y,int x,tablero_booleano* pendientes,bool* inmune){
int wich_option=U->indice_de_la_opcion('D');
        U->Beffects.haz1(y,x);
        U->imprimir();
        U->poner_opciones(true,wich_option);
        U->voltear_pantalla();
        SDL_Delay(400);
    if(U->Bobjects.comparar(y,x,'F')==false){
        U->Bobjects.haz_ceniza(y,x);
    }
    coordenada col[4];
    char dir[4];///esta idea se me ocurrio al programar en python
    dir[0]='>';dir[1]='A';dir[2]='<';dir[3]='V';
    ///para las 4 direcciones de detonacion: >,A,<,V
    for(int z=0;z<4;z++){
        col[z]=U->Bobjects.colindante_a(y,x,dir[z],dir[z]);
        if(col[z].valido){
            detona1_2(U,col[z].yy,col[z].xx,pendientes,inmune);
            U->Beffects.haz1(col[z].yy,col[z].xx);
        }
    }
    U->imprimir();
    U->poner_opciones(true,wich_option);
    U->voltear_pantalla();
    SDL_Delay(400);

    coordenada col2[4];
    for(int z=0;z<4;z++){
        col2[z]=U->Bobjects.colindante_a(col[z].yy,col[z].xx,dir[z],dir[z]);
        if(col2[z].valido && U->Bobjects.comparar(col[z].yy,col[z].xx,0)){
            detona1_2(U,col2[z].yy,col2[z].xx,pendientes,inmune);
            U->Beffects.haz1(col2[z].yy,col2[z].xx);
        }
    }
    U->imprimir();
    U->poner_opciones(true,wich_option);
    U->voltear_pantalla();
    SDL_Delay(400);
    U->Beffects.reinicia();
}
void detona3(uniendo* U,int y,int x){
tablero_booleano pendientes;
int respaldo_tablero[7][7];///parche
bool inmune[5];
    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++){
            respaldo_tablero[i][j]=U->Bobjects.el_color_de(i,j);
        }
    }
    for(int i=0;i<5;i++){
        inmune[i]=false;
    }
    ///inicializaciones de rutina
    U->Bbackground.haz1(y,x);
    U->Beffects.haz1(y,x);
    detona2(U,y,x,&pendientes,inmune);
    if(U->Bobjects.comparar(y,x,'F')){
        U->player[U->wo_ist_dran()].usa_salvabomba();
    }
    else{
        U->Bobjects.haz_ceniza(y,x);
    }
    ///la detonacion es ANTES de borrar el color contemplando la
    ///posibilidad de que se este detonando un salvabomba
    while(pendientes.cuenta()>0){
        for(int i=0;i<7;i++){
            for(int j=0;j<7;j++){
                if(pendientes.valor(i,j)){
                    detona2(U,i,j,&pendientes,inmune);
                    U->Bobjects.haz_ceniza(i,j);
                    pendientes.haz0(i,j);
                }
            }
        }
    }
    ///mientras haya pendientes: buscalos y detonalos
    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++){
            if(U->Bobjects.comparar(i,j,0)==false){
                if(U->Bobjects.el_color_de(i,j)==0){
                    if(U->Bobjects.comparar(i,j,'S')){
                        U->Bobjects.cambiar(i,j,'T',respaldo_tablero[i][j]);
                    }
                    else{
                        if(U->Bobjects.comparar(i,j,'V')==false){
                            U->Bobjects.elimina(i,j);
                        }
                    }
                }
            }
        }
    }
    U->Bbackground.reinicia();
    ///limpia las cenizas y el fondo rojo
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void detona_agujeronegro(uniendo* U,int y,int x){
int wich_option=U->indice_de_la_opcion('D');
        U->Bbackground.haz1(y,x);
        U->Bobjects.haz_ceniza(y,x);
        U->imprimir();
        U->poner_opciones(true,wich_option);
        U->voltear_pantalla();
        SDL_Delay(400);
    coordenada col[4];
    char dir[4];///esta idea se me ocurrio al programar en python
    dir[0]='>';dir[1]='A';dir[2]='<';dir[3]='V';
    ///para las 4 direcciones de detonacion: >,A,<,V
    for(int z=0;z<4;z++){
        col[z]=U->Bobjects.colindante_a(y,x,dir[z],dir[z]);
        if(col[z].valido){
            U->Bbackground.haz1(col[z].yy,col[z].xx);
            if(U->Bobjects.comparar(col[z].yy,col[z].xx,'F')){
                int color=U->Bobjects.el_color_de(col[z].yy,col[z].xx);
                if(U->player[color-1].tiene_salvavidas()){
                    U->player[color-1].usa_salvavidas();
                    ///pierde el salvavidas pero no muere
                }
                else{
                    U->player[color-1].muere();
                    U->Bobjects.haz_ceniza(col[z].yy,col[z].xx);
                }
            }
            else{
                U->Bobjects.haz_ceniza(col[z].yy,col[z].xx);
            }
        }
    }
    U->imprimir();
    U->poner_opciones(true,wich_option);
    U->voltear_pantalla();
    SDL_Delay(400);

    coordenada col2[4];
    for(int z=0;z<4;z++){
        col2[z]=U->Bobjects.colindante_a(col[z].yy,col[z].xx,dir[z],dir[z]);
        if(col2[z].valido && U->Bobjects.comparar(col[z].yy,col[z].xx,0)){
            U->Bbackground.haz1(col2[z].yy,col2[z].xx);
            if(U->Bobjects.comparar(col2[z].yy,col2[z].xx,'F')){
                int color=U->Bobjects.el_color_de(col2[z].yy,col2[z].xx);
                if(U->player[color-1].tiene_salvavidas()){
                    U->player[color-1].usa_salvavidas();
                    ///pierde el salvavidas pero no muere
                }
                else{
                    U->player[color-1].muere();
                    U->Bobjects.haz_ceniza(col2[z].yy,col2[z].xx);
                }
            }
            else{
                U->Bobjects.haz_ceniza(col2[z].yy,col2[z].xx);
            }
        }
    }
    U->imprimir();
    U->poner_opciones(true,wich_option);
    U->voltear_pantalla();
    SDL_Delay(400);
    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++){
            if(U->Bobjects.comparar(i,j,0)==false){
                if(U->Bobjects.el_color_de(i,j)==0){
                    if(U->Bobjects.comparar(i,j,'V')==false){
                        U->Bobjects.elimina(i,j);
                    }
                }
            }
        }
    }
    U->Bbackground.reinicia();
    U->Bobjects.cambiar(y,x,'V',0);
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void detona4(uniendo* U,int y,int x){
    if(U->Bobjects.comparar(y,x,'H')){
        detona_agujeronegro(U,y,x);
    }
    else{
        detona3(U,y,x);
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void salvavidas(uniendo* U,int y, int x){
    U->Bobjects.cambiar(y,x,'S',U->wo_ist_dran()+1);
    U->player[U->wo_ist_dran()].usa_salvavidas();
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void salvabombas(uniendo* U,int y, int x){
    U->Bobjects.cambiar(y,x,'T',U->wo_ist_dran()+1);
    U->player[U->wo_ist_dran()].usa_salvabomba();
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void cargar(uniendo* U,int y, int x){
    U->player[U->wo_ist_dran()].carga();
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void portal(uniendo* U,int y, int x){
    U->Bobjects.cambiar(y,x,'R',U->wo_ist_dran()+1);
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void agujeronegro(uniendo* U,int y, int x){
    U->Bobjects.cambiar(y,x,'H',U->wo_ist_dran()+1);
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void casilla_colindante_movible(uniendo* U){
/// condicion: una casilla que sea colindante y este vacia
/// el paradigma ha cambiado ligeramente con las fisicas de vacio
int X,Y;
char direccion[4];
coordenada cor;
    direccion[0]='>';direccion[1]='A';direccion[2]='<';direccion[3]='V';
    X=U->player[U->wo_ist_dran()].coordx();
    Y=U->player[U->wo_ist_dran()].coordy();
    if(Y>-1){///si no estas en un portal
        for(int a=0;a<4;a++){
            for(int b=0;b<4;b++){
                cor=U->Bobjects.colindante_a(Y,X,direccion[a],direccion[b]);
                if(cor.valido){
                    if(U->Bobjects.comparar(cor.yy,cor.xx,0)){
                        U->Bbuttons.haz1(cor.yy,cor.xx);
                    }
                    if(U->Bobjects.comparar(cor.yy,cor.xx,'R')){
                        int color=U->Bobjects.el_color_de(cor.yy,cor.xx)-1;
                        if(U->Bobjects.comparar(-1,color,0) &&
                           U->player[color].vive()){
                            U->Bbuttons.haz1(cor.yy,cor.xx);
                        }
                    }
                }
            }
        }
    }
    else{///si efectivamente estas en un portal
        int color=X;///en que portal estas?
        ///puedes moverte a los otros espacios inter-portal
        for(int z=0;z<U->cuantos_jugadores();z++){
            if(z!=color && U->Bobjects.comparar(-1,z,0)){
                U->Bbuttons.haz1(-1,z);
            }
        }
        ///tambien puedes poner cosas desde dentro de un portal
        color++;
        for(int i=0;i<7;i++){
            for(int j=0;j<7;j++){
                if(U->Bobjects.comparar(i,j,'R')
                   && U->Bobjects.el_color_de(i,j)==color){
                    for(int a=0;a<4;a++){
                        for(int b=0;b<4;b++){
                            cor=U->Bobjects.colindante_a(i,j,direccion[a],direccion[b]);
                            if(cor.valido){
                                if(U->Bobjects.comparar(cor.yy,cor.xx,0)){
                                    U->Bbuttons.haz1(cor.yy,cor.xx);
                                }
        ///no se puede salir de un portal y justo entrar en otro
                            }
                        }
                    }
                }
            }
        }
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void casilla_colindante_vacia(uniendo* U){
/// condicion: una casilla que sea colindante y este vacia
/// el paradigma ha cambiado ligeramente con las fisicas de vacio
    casilla_colindante_movible(U);
    for(int y=0;y<7;y++){
        for(int x=0;x<7;x++){
            if(U->Bobjects.comparar(y,x,'R')){
                U->Bbuttons.haz0(y,x);
            }
        }
    }
    for(int z=0;z<5;z++){
        U->Bbuttons.haz0(-1,z);
    }
/// para economizar usare la anterior; este truco se me acaba de
/// ocurrir y quiza lo use mas a menudo
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void casilla_colindante_vacia_salvavidas(uniendo* U){
/// variacio: && tiene salvavidas
    if(U->player[U->wo_ist_dran()].tiene_salvavidas()==true){
        casilla_colindante_vacia(U);
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void casilla_colindante_vacia_salvabombas(uniendo* U){
/// variacio: && tiene salvabombas
    if(U->player[U->wo_ist_dran()].tiene_salvabombas()==true){
        casilla_colindante_vacia(U);
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void bomba_tuya_o_salvabomba(uniendo* U){
int X,Y,d;
    d=U->wo_ist_dran();
    if(U->player[d].tiene_salvabombas()==true){
        X=U->player[d].coordx();
        Y=U->player[d].coordy();
        if(Y>-1){
            U->Bbuttons.haz1(Y,X);
        }
    }
    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++){
            if(U->Bobjects.comparar(i,j,'B')
               ||U->Bobjects.comparar(i,j,'T')
               ||U->Bobjects.comparar(i,j,'H')){
                if(U->Bobjects.el_color_de(i,j)==d+1){
                    U->Bbuttons.haz1(i,j);
                }
            }
        }
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void tiene_salvavidas(uniendo* U){
/// el test es para cargar
int X,Y,d;
    d=U->wo_ist_dran();
    if(U->player[d].tiene_salvavidas()==true){
        X=U->player[d].coordx();
        Y=U->player[d].coordy();
        U->Bbuttons.haz1(Y,X);
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void opcion::eliminacion_por_ahogado(uniendo* U){
/// la idea es simple (aunque la burocracia de datos privados haga
/// parecer otra cosa): cuenta cuantas opciones puede realizar el
/// jugador [en turno] en su turno, y si no es capaz de realizar
/// ninguna entonces es eliminado
int acumulado=0,lim1,d;
char aux; /// este evitara confusiones
    d=U->wo_ist_dran();
    lim1=U->player[d].cuantas_jugadas();
    for(int i=0;i<lim1;i++){
        aux=U->player[d].da_la_opcion(i);
        ejecuta_botones(aux,U);
        acumulado+=(*U).Bbuttons.cuenta();
        U->Bbuttons.reinicia();
    }
/// ahora bien, lo que se hace REALMENTE es colocar los botones -que
/// corresponden a las jugadas posibles- y luego los cuenta;
/// 0 botones representan, naturalmente, que el jugador no puede
/// realizar ninguna jugada permitida
    if(acumulado==0){
    int X,Y;
        X=U->player[d].coordx();
        Y=U->player[d].coordy();
        U->player[d].muere();
        U->Bobjects.elimina(Y,X); /// y se elimina su ficha
    }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
///aqui un par de funciones que no hacen nada
void nada1(uniendo* U){}
void nada2(uniendo* U,int x,int y){}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/// las funciones de detonacion simulada sirven para calcular los
/// danos causados por una detonacioon, y con ello las casilas que
/// cada jugador esta amenazando con sus bombas
///
/// como parte del parche, la detonacion simulada es posible para
/// todos los jugadores, pero ninguno tiene la opcion de realizarla
///
/// por el momento aun no esta optimizada: es un vil parche
void sim_detona1(uniendo* U,int y,int x,tablero_booleano* pendientes,
                bool* inmune,tablero_booleano* polvora){
int color=U->Bobjects.el_color_de(y,x);
    polvora->haz1(y,x);
    U->Bbackground.haz1(y,x);
    if(U->Bobjects.comparar(y,x,0) || color==0
       || U->Bobjects.comparar(y,x,'V')
       || U->Bobjects.comparar(y,x,'R')){
    ///para ahorrar tiempo, verifica y no hace nada si haay un 0
    /// si tiene color 0 y no es vacio, es una ficha-ceniza
    }
    else if(U->Bobjects.comparar(y,x,'F')){
        if(inmune[color-1]==false){
            if(U->player[color-1].tiene_salvavidas()){
                U->player[color-1].carga();
                inmune[color-1]=true;
            }
            else{
                U->player[color-1].muere();
                U->Bobjects.haz_ceniza(y,x);
            }
        }
    }
    else if(U->Bobjects.comparar(y,x,'B') ||
            U->Bobjects.comparar(y,x,'T')){
                pendientes->haz1(y,x);
            ///aqui hay algo que aclara y es que a este punto
            ///el color de la bomba NO es 0; de ahi tomo como un
            ///estadar que una bomba pendiente conserva su color,
            ///y al detonar su color sera 0
    }
    else{
        U->Bobjects.haz_ceniza(y,x);
    }
}
void sim_detona1_2(uniendo* U,int y,int x,tablero_booleano* pendientes,
                   bool* inmune,tablero_booleano* polvora){
    sim_detona1(U,y,x,pendientes,inmune,polvora);
    if(U->Bobjects.comparar(y,x,'R')){
        int color=U->Bobjects.el_color_de(y,x)-1;
        sim_detona1(U,-1,color,pendientes,inmune,polvora);
    }
}
void sim_detona2(uniendo* U,int y,int x,tablero_booleano* pendientes,
                 bool* inmune,tablero_booleano* polvora){
        U->Beffects.haz1(y,x);
    polvora->haz1(y,x);
    if(U->Bobjects.comparar(y,x,'F')==false){
        U->Bobjects.haz_ceniza(y,x);
    }
    coordenada col[4];
    char dir[4];///esta idea se me ocurrio al programar en python
    dir[0]='>';dir[1]='A';dir[2]='<';dir[3]='V';
    ///para las 4 direcciones de detonacion: >,A,<,V
    for(int z=0;z<4;z++){
        col[z]=U->Bobjects.colindante_a(y,x,dir[z],dir[z]);
        if(col[z].valido){
            sim_detona1_2(U,col[z].yy,col[z].xx,pendientes,inmune,polvora);
            U->Beffects.haz1(col[z].yy,col[z].xx);
        }
    }

    coordenada col2[4];
    for(int z=0;z<4;z++){
        col2[z]=U->Bobjects.colindante_a(col[z].yy,col[z].xx,dir[z],dir[z]);
        if(col2[z].valido && U->Bobjects.comparar(col[z].yy,col[z].xx,0)){
            sim_detona1_2(U,col2[z].yy,col2[z].xx,pendientes,inmune,polvora);
            U->Beffects.haz1(col2[z].yy,col2[z].xx);
        }
    }
}
void sim_detona3(uniendo* U,int y,int x,tablero_booleano* polvora){
tablero_booleano pendientes;
int respaldo_tablero[7][7];///parche
bool inmune[5];
    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++){
            respaldo_tablero[i][j]=U->Bobjects.el_color_de(i,j);
        }
    }
    for(int i=0;i<5;i++){
        inmune[i]=false;
    }
    ///inicializaciones de rutina
    U->Bbackground.haz1(y,x);
    U->Beffects.haz1(y,x);
    sim_detona2(U,y,x,&pendientes,inmune,polvora);
    if(U->Bobjects.comparar(y,x,'F')){
        U->player[U->wo_ist_dran()].usa_salvabomba();
    }
    else{
        U->Bobjects.haz_ceniza(y,x);
    }
    ///la detonacion es ANTES de borrar el color contemplando la
    ///posibilidad de que se este detonando un salvabomba
    while(pendientes.cuenta()>0){
        for(int i=0;i<7;i++){
            for(int j=0;j<7;j++){
                if(pendientes.valor(i,j)){
                    sim_detona2(U,i,j,&pendientes,inmune,polvora);
                    U->Bobjects.haz_ceniza(i,j);
                    pendientes.haz0(i,j);
                }
            }
        }
    }
    ///mientras haya pendientes: buscalos y detonalos
    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++){
            if(U->Bobjects.comparar(i,j,0)==false){
                if(U->Bobjects.el_color_de(i,j)==0){
                    if(U->Bobjects.comparar(i,j,'S')){
                        U->Bobjects.cambiar(i,j,'T',respaldo_tablero[i][j]);
                    }
                    else{
                        if(U->Bobjects.comparar(i,j,'V')==false){
                            U->Bobjects.elimina(i,j);
                        }
                    }
                }
            }
        }
    }
    U->Bbackground.reinicia();
    ///limpia las cenizas y el fondo rojo
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void sim_detona_agujero(uniendo* U,int y,int x,tablero_booleano* polvora){
        U->Bbackground.haz1(y,x);
        U->Bobjects.haz_ceniza(y,x);

        polvora->haz1(y,x);
    coordenada col[4];
    char dir[4];///esta idea se me ocurrio al programar en python
    dir[0]='>';dir[1]='A';dir[2]='<';dir[3]='V';
    ///para las 4 direcciones de detonacion: >,A,<,V
    for(int z=0;z<4;z++){
        col[z]=U->Bobjects.colindante_a(y,x,dir[z],dir[z]);
        if(col[z].valido){
            polvora->haz1(col[z].yy,col[z].xx);
            U->Bbackground.haz1(col[z].yy,col[z].xx);
            if(U->Bobjects.comparar(col[z].yy,col[z].xx,'F')){
                int color=U->Bobjects.el_color_de(col[z].yy,col[z].xx);
                if(U->player[color-1].tiene_salvavidas()){
                    U->player[color-1].usa_salvavidas();
                    ///pierde el salvavidas pero no muere
                }
                else{
                    U->player[color-1].muere();
                    U->Bobjects.haz_ceniza(col[z].yy,col[z].xx);
                }
            }
            else{
                if(U->Bobjects.comparar(col[z].yy,col[z].xx,'V')==false){
                    U->Bobjects.elimina(col[z].yy,col[z].xx);
                }
            }
        }
    }

    coordenada col2[4];
    for(int z=0;z<4;z++){
        col2[z]=U->Bobjects.colindante_a(col[z].yy,col[z].xx,dir[z],dir[z]);
        if(col2[z].valido && U->Bobjects.comparar(col[z].yy,col[z].xx,0)){
            polvora->haz1(col2[z].yy,col2[z].xx);
            U->Bbackground.haz1(col2[z].yy,col2[z].xx);
            if(U->Bobjects.comparar(col2[z].yy,col2[z].xx,'F')){
                int color=U->Bobjects.el_color_de(col2[z].yy,col2[z].xx);
                if(U->player[color-1].tiene_salvavidas()){
                    U->player[color-1].usa_salvavidas();
                    ///pierde el salvavidas pero no muere
                }
                else{
                    U->player[color-1].muere();
                    U->Bobjects.haz_ceniza(col2[z].yy,col2[z].xx);
                }
            }
            else{
                U->Bobjects.haz_ceniza(col2[z].yy,col2[z].xx);
            }
        }
    }

    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++){
            if(U->Bobjects.comparar(i,j,0)==false){
                if(U->Bobjects.el_color_de(i,j)==0){
                    U->Bobjects.elimina(i,j);
                }
            }
        }
    }
    U->Bbackground.reinicia();
    U->Bobjects.cambiar(y,x,'V',0);
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void sim_detona4(uniendo* U,int y,int x,tablero_booleano* polvora){
    if(U->Bobjects.comparar(y,x,'H')){
        sim_detona_agujero(U,y,x,polvora);
    }
    else{
        sim_detona3(U,y,x,polvora);
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void detona5_s(uniendo* U,int y,int x){
    tablero_booleano relleno;
    sim_detona4(U,y,x,&relleno);
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void uniendo::genera_polvoras(tablero_booleano* propio,tablero_booleano* ajeno){
    propio->reinicia();
    ajeno->reinicia();

    uniendo V(*regalo_final());
    /// fase 1-1 : bombas y salvavidas tuyos
    bomba_tuya_o_salvabomba(&V);
    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++){
            if(V.Bbuttons.valor(i,j)){
                sim_detona4(&V,i,j,propio);
                V.reasimila_elementos(*regalo_final());
                bomba_tuya_o_salvabomba(&V);
            }
        }
    }
    /// fase 1-2 : bombas y salvavidas ajenos
    for(int w=0;w<cuantos_jugadores();w++){
        if(w!=dran && player[w].vive()){
            V.reasimila_elementos(*regalo_final());
            V.set_dran(w);
            bomba_tuya_o_salvabomba(&V);
            for(int i=0;i<7;i++){
                for(int j=0;j<7;j++){
                    if(V.Bbuttons.valor(i,j)){

                        sim_detona4(&V,i,j,ajeno);
                        V.reasimila_elementos(*regalo_final());
                        V.set_dran(w);
                        bomba_tuya_o_salvabomba(&V);
                    }
                }
            }
        }
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
opcion::opcion(){
/// nota a principios de 2014: no he encontrado una manera mas
/// efectiva de hacer esto
    n_opciones=9;
    opciona=new jugada[n_opciones];
    n_tests=0;
    test=new paso[n_tests];
    //////////////////////////////////////////////////////////////////
    nula=(jugada){'_',&nada1,&nada2};
    opciona[0]=(jugada){'M',&casilla_colindante_movible,&mover,&mover_simula};
    opciona[1]=(jugada){'B',&casilla_colindante_vacia,&bomba,&bomba};
    opciona[2]=(jugada){'P',&casilla_colindante_vacia,&pared,&pared};
    opciona[3]=(jugada){'D',&bomba_tuya_o_salvabomba,&detona4,&detona5_s};
    opciona[4]=(jugada){'S',&casilla_colindante_vacia_salvavidas,
                        &salvavidas,&salvavidas};
    opciona[5]=(jugada){'T',&casilla_colindante_vacia_salvabombas,
                        &salvabombas,&salvabombas};
    opciona[6]=(jugada){'K',&tiene_salvavidas,&cargar,&cargar};
    opciona[7]=(jugada){'H',&casilla_colindante_vacia,&agujeronegro,&agujeronegro};
    opciona[8]=(jugada){'R',&casilla_colindante_vacia,&portal,&portal};
    //opciona[7]=(jugada){'N',&ninja_saltable,&mover,&mover};
    //opciona[8]=(jugada){'*',&jugador_cercano,&toque_critico,&toque_critico};
    //opciona[9]=(jugada){'b',&casilla_colindante_vacia,&mover_y_bomba,&mover_y_bomba};
    //////////////////////////////////////////////////////////////////
    nulb=(paso){'A',&nada1};
}
#endif
