#ifndef OPCION_H
#define OPCION_H
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#include "opcion.h"
/// basicamenta, aqui estan definidas TODAS las jugadas permitidas
/// en Bombardi, al menos hasta el 31/diciembre/2013
/// planeo que, en el futuro, este sea el unico archivo que tenga
/// que actualizarse publicamente
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
int X,Y,dir_x,dir_y,en_turno;
SDL_Rect temp;
    en_turno=U->wo_ist_dran();
    X=U->player[en_turno].coordx();
    Y=U->player[en_turno].coordy();
    dir_x=x-X; dir_y=y-Y;
    temp.x=32+64*X; temp.y=32+64*Y;
    U->Bobjects.elimina(Y,X);
    for(int i=0;i<64;i++){
        U->imprimir();
        U->incluir_ficha_extra('F',en_turno+1,temp);
        U->voltear_pantalla();
        temp.x+=dir_x; temp.y+=dir_y;
    }
    U->player[en_turno].mover_a(y,x);
    U->Bobjects.cambiar(y,x,'F',en_turno+1);
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void mover_s(uniendo* U,int y,int x){
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
/// -una detona una bomba en particular, invocando la funcion
///  anterior para danar segun el camino predeterminado. funciona
///  "ruta de detonacion". no dira nada respecto a la bomba en si
/// -la definitiva declara un tablero_booleano donde anota las
///  bombas que se activaron durante la detonacion, y luego lo
///  recorre para detonarlas
void detona1(uniendo* U,int y,int x,tablero_booleano* pendientes,
             int excepcion,bool* inmunes){
///la excepcion es para cuando se detona una salvabomba, el vector
///de booleanos es para los que hayan usado salvavidas
///hay que notar que color esta entre 1 y 5, de forma que por
///estandar inmunes[0] se desperdicia: yo lo hago true para
///ahorrar algo de tiempo
int color=U->Bobjects.el_color_de(y,x);
    U->Bbackground.haz1(y,x);
    if(U->Bobjects.comparar(y,x,0) || color==0){
    ///para ahorrar tiempo, verifica y no hace nada si haay un 0
    /// si tiene color 0 y no es vacio, es una ficha-ceniza
    }
    else if(U->Bobjects.comparar(y,x,'F')){
        if(color!=(excepcion+1) && inmunes[color]==false){
            if(U->player[color-1].tiene_salvavidas()){
                U->player[color-1].carga();
                inmunes[color-1]=true;
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
void detona2(uniendo* U,int y,int x,tablero_booleano* pendientes,
             bool* inmunes){
int excepto=-1,tempx=x,tempy=y,wich_option;
    wich_option=U->indice_de_la_opcion('D');
        U->Beffects.haz1(tempy,tempx);
        U->imprimir();
        U->poner_opciones(true,wich_option);
        U->voltear_pantalla();
        SDL_Delay(400);
    if(U->Bobjects.comparar(y,x,'T')){
        excepto=U->Bobjects.el_color_de(y,x)-1;
    }
    if(U->Bobjects.comparar(y,x,'F')==false){
        U->Bobjects.haz_ceniza(y,x);
    }
    ///es util "capturar" el color antes de borrarlo
    {
        if(x>0){
            tempx-=1;
            detona1(U,tempy,tempx,pendientes,excepto,inmunes);
            U->Beffects.haz1(tempy,tempx);
            tempx=x;
        }
        if(x<6){
            tempx+=1;
            detona1(U,tempy,tempx,pendientes,excepto,inmunes);
            U->Beffects.haz1(tempy,tempx);
            tempx=x;
        }
        if(y>0){
            tempy-=1;
            detona1(U,tempy,tempx,pendientes,excepto,inmunes);
            U->Beffects.haz1(tempy,tempx);
            tempy=y;
        }
        if(y<6){
            tempy+=1;
            detona1(U,tempy,tempx,pendientes,excepto,inmunes);
            U->Beffects.haz1(tempy,tempx);
            tempy=y;
        }
    }
    U->imprimir();
    U->poner_opciones(true,wich_option);
    U->voltear_pantalla();
    SDL_Delay(400);
    {
        if(x>1 && U->Bobjects.comparar(tempy,tempx-1,0)){
            tempx-=2;
            detona1(U,tempy,tempx,pendientes,excepto,inmunes);
            U->Beffects.haz1(tempy,tempx);
            tempx=x;
        }
        if(x<5 && U->Bobjects.comparar(tempy,tempx+1,0)){
            tempx+=2;
            detona1(U,tempy,tempx,pendientes,excepto,inmunes);
            U->Beffects.haz1(tempy,tempx);
            tempx=x;
        }
        if(y>1 && U->Bobjects.comparar(tempy-1,tempx,0)){
            tempy-=2;
            detona1(U,tempy,tempx,pendientes,excepto,inmunes);
            U->Beffects.haz1(tempy,tempx);
            tempy=y;
        }
        if(y<5 && U->Bobjects.comparar(tempy+1,tempx,0)){
            tempy+=2;
            detona1(U,tempy,tempx,pendientes,excepto,inmunes);
            U->Beffects.haz1(tempy,tempx);
            tempy=y;
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
bool* inmunidad;
int respaldo[7][7];///parche
    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++){
            respaldo[i][j]=U->Bobjects.el_color_de(i,j);
        }
    }
    inmunidad=new bool[U->cuantos_jugadores()+1];
    for(int i=1;i<=U->cuantos_jugadores();i++){
        inmunidad[i]=false;
    } inmunidad[0]=true;
    ///inicializaciones de rutina
    U->Bbackground.haz1(y,x);
    U->Beffects.haz1(y,x);
    detona2(U,y,x,&pendientes,inmunidad);
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
                    detona2(U,i,j,&pendientes,inmunidad);
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
                        U->Bobjects.cambiar(i,j,'T',respaldo[i][j]);
                    }
                    else{
                        U->Bobjects.elimina(i,j);
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
void toque_critico(uniendo* U,int y,int x){
int victima=U->Bobjects.el_color_de(y,x);
    if(U->Bobjects.comparar(y,x,'F')){
        U->player[victima].muere();
        U->Bobjects.elimina(y,x);
    }
}
void mover_y_bomba(uniendo* U,int y,int x){
///variacio: mover y poner una bomba en el lugar previo
int X,Y,dir_x,dir_y,en_turno;
SDL_Rect temp;
    en_turno=U->wo_ist_dran();
    X=U->player[en_turno].coordx();
    Y=U->player[en_turno].coordy();
    dir_x=x-X; dir_y=y-Y;
    temp.x=32+64*X; temp.y=32+64*Y;
    U->Bobjects.cambiar(Y,X,'B',en_turno+1);
    for(int i=0;i<64;i++){
        U->imprimir();
        U->incluir_ficha_extra('F',en_turno+1,temp);
        U->voltear_pantalla();
        temp.x+=dir_x; temp.y+=dir_y;
    }
    U->player[en_turno].mover_a(y,x);
    U->Bobjects.cambiar(y,x,'F',en_turno+1);
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void casilla_colindante_vacia(uniendo* U){
/// condicion: una casilla que sea colindante y este vacia
int X,Y;
    X=U->player[U->wo_ist_dran()].coordx();
    Y=U->player[U->wo_ist_dran()].coordy();
    for(int i=0;i<7;i++){
        if(abs(i-Y)<2){
            for(int j=0;j<7;j++){
                if(abs(j-X)<2){
                    if(U->Bobjects.comparar(i,j,0)){
                        U->Bbuttons.haz1(i,j);
                    }
                }
            }
        }
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void casilla_colindante_vacia_salvavidas(uniendo* U){
/// variacio: && tiene salvavidas
int X,Y;
    if(U->player[U->wo_ist_dran()].tiene_salvavidas()==true){
        X=U->player[U->wo_ist_dran()].coordx();
        Y=U->player[U->wo_ist_dran()].coordy();
        for(int i=0;i<7;i++){
            if(abs(i-Y)<2){
                for(int j=0;j<7;j++){
                    if(abs(j-X)<2){
                        if(U->Bobjects.comparar(i,j,0)){
                            U->Bbuttons.haz1(i,j);
                        }
                    }
                }
            }
        }
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void casilla_colindante_vacia_salvabombas(uniendo* U){
/// variacio: && tiene salvabombas
int X,Y;
    if(U->player[U->wo_ist_dran()].tiene_salvabombas()==true){
        X=U->player[U->wo_ist_dran()].coordx();
        Y=U->player[U->wo_ist_dran()].coordy();
        for(int i=0;i<7;i++){
            if(abs(i-Y)<2){
                for(int j=0;j<7;j++){
                    if(abs(j-X)<2){
                        if(U->Bobjects.comparar(i,j,0)){
                            U->Bbuttons.haz1(i,j);
                        }
                    }
                }
            }
        }
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
        U->Bbuttons.haz1(Y,X);
    }
    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++){
            if(U->Bobjects.comparar(i,j,'B')
               ||U->Bobjects.comparar(i,j,'T')){
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
void ninja_saltable(uniendo* U){
/// el ninja puede moverse ahi?
int X,Y,d,dir_x,dir_y;
    d=U->wo_ist_dran();
    X=U->player[d].coordx();
    Y=U->player[d].coordy();
    for(int i=0;i<7;i++){
        if(abs(i-Y)==2 || Y==i){
            for(int j=0;j<7;j++){
                if(abs(j-X)==2 || X==j){
                    if(U->Bobjects.comparar(i,j,0)){
                        dir_x=(X-j)/2;
                        dir_y=(Y-i)/2;
                        if(!U->Bobjects.comparar(i+dir_y,
                                                 j+dir_x,0)){
                            U->Bbuttons.haz1(i,j);
                        }
                    }
                }
            }
        }
    }
    casilla_colindante_vacia(U);
/// esta comprobacion es tremendamente nada obvia, pero con palabras
/// es algo mas simple: el ninja puede mooverse normal y, ademas,
/// saltar un objeto, uno a lo mas: bombas, paredes, jugadores...
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void jugador_cercano(uniendo* U){
int X,Y,d;
    d=U->wo_ist_dran();
    X=U->player[d].coordx();
    Y=U->player[d].coordy();
    for(int i=0;i<7;i++){
        if(abs(i-Y)<2){
            for(int j=0;j<7;j++){
                if(abs(j-X)<2){
                    if(X!=j && Y!=i){
                    ///quiza luego permita el suicidio...pero hoy no
                        if(U->Bobjects.comparar(i,j,'F')){
                            U->Bbuttons.haz1(i,j);
                        }
                    }
                }
            }
        }
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
void detona1_s(uniendo* U,int y,int x,
               tablero_booleano* pendientes,
               int excepcion,bool* inmunes){
int color=U->Bobjects.el_color_de(y,x);
    U->Bbackground.haz1(y,x);
    if(U->Bobjects.comparar(y,x,0) || color==0){
    }
    else if(U->Bobjects.comparar(y,x,'F')){
        if(color!=(excepcion+1) && inmunes[color]==false){
            if(U->player[color-1].tiene_salvavidas()){
                U->player[color-1].carga();
                inmunes[color-1]=true;
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
    }
    else{
        U->Bobjects.haz_ceniza(y,x);
    }
}
void detona2_s(uniendo* U,int y,int x,tablero_booleano* pendientes,
                    bool* inmunes){
int excepto=-1,tempx=x,tempy=y;//,wich_option;
    //wich_option=U->indice_de_la_opcion('D');
        U->Beffects.haz1(tempy,tempx);
        //U->imprimir();
        //U->poner_opciones(true,wich_option);
        //U->voltear_pantalla();
        //SDL_Delay(400);
    if(U->Bobjects.comparar(y,x,'T')){
        excepto=U->Bobjects.el_color_de(y,x)-1;
    }
    if(U->Bobjects.comparar(y,x,'F')==false){
        U->Bobjects.haz_ceniza(y,x);
    }
    {
        if(x>0){
            tempx-=1;
            detona1_s(U,tempy,tempx,pendientes,excepto,inmunes);
            U->Beffects.haz1(tempy,tempx);
            tempx=x;
        }
        if(x<6){
            tempx+=1;
            detona1_s(U,tempy,tempx,pendientes,excepto,inmunes);
            U->Beffects.haz1(tempy,tempx);
            tempx=x;
        }
        if(y>0){
            tempy-=1;
            detona1_s(U,tempy,tempx,pendientes,excepto,inmunes);
            U->Beffects.haz1(tempy,tempx);
            tempy=y;
        }
        if(y<6){
            tempy+=1;
            detona1_s(U,tempy,tempx,pendientes,excepto,inmunes);
            U->Beffects.haz1(tempy,tempx);
            tempy=y;
        }
    }
    //U->imprimir();
    //U->poner_opciones(true,wich_option);
    //U->voltear_pantalla();
    //SDL_Delay(400);
    {
        if(x>1 && U->Bobjects.comparar(tempy,tempx-1,0)){
            tempx-=2;
            detona1_s(U,tempy,tempx,pendientes,excepto,inmunes);
            U->Beffects.haz1(tempy,tempx);
            tempx=x;
        }
        if(x<5 && U->Bobjects.comparar(tempy,tempx+1,0)){
            tempx+=2;
            detona1_s(U,tempy,tempx,pendientes,excepto,inmunes);
            U->Beffects.haz1(tempy,tempx);
            tempx=x;
        }
        if(y>1 && U->Bobjects.comparar(tempy-1,tempx,0)){
            tempy-=2;
            detona1_s(U,tempy,tempx,pendientes,excepto,inmunes);
            U->Beffects.haz1(tempy,tempx);
            tempy=y;
        }
        if(y<5 && U->Bobjects.comparar(tempy+1,tempx,0)){
            tempy+=2;
            detona1_s(U,tempy,tempx,pendientes,excepto,inmunes);
            U->Beffects.haz1(tempy,tempx);
            tempy=y;
        }
    }
    //U->imprimir();
    //U->poner_opciones(true,wich_option);
    //U->voltear_pantalla();
    //SDL_Delay(400);
    //U->Beffects.reinicia();
}
void detona3_s(uniendo* U,int y,int x){
tablero_booleano pendientes;
bool* inmunidad;
int respaldo[7][7];///parche
    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++){
            respaldo[i][j]=U->Bobjects.el_color_de(i,j);
        }
    }
    inmunidad=new bool[U->cuantos_jugadores()+1];
    for(int i=1;i<=U->cuantos_jugadores();i++){
        inmunidad[i]=false;
    } inmunidad[0]=true;
    ///inicializaciones de rutina
    U->Bbackground.haz1(y,x);
    U->Beffects.haz1(y,x);
    detona2_s(U,y,x,&pendientes,inmunidad);
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
                    detona2_s(U,i,j,&pendientes,inmunidad);
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
                        U->Bobjects.cambiar(i,j,'T',respaldo[i][j]);
                    }
                    else{
                        U->Bobjects.elimina(i,j);
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
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
opcion::opcion(){
/// nota a principios de 2014: no he encontrado una manera mas
/// efectiva de hacer esto
    n_opciones=10;
    opciona=new jugada[n_opciones];
    n_tests=0;
    test=new paso[n_tests];
    //////////////////////////////////////////////////////////////////
    nula=(jugada){'_',&nada1,&nada2};
    opciona[0]=(jugada){'M',&casilla_colindante_vacia,&mover,&mover_s};
    opciona[1]=(jugada){'B',&casilla_colindante_vacia,&bomba,&bomba};
    opciona[2]=(jugada){'P',&casilla_colindante_vacia,&pared,&pared};
    opciona[3]=(jugada){'D',&bomba_tuya_o_salvabomba,&detona3,&detona3_s};
    opciona[4]=(jugada){'S',&casilla_colindante_vacia_salvavidas,
                        &salvavidas,&salvavidas};
    opciona[5]=(jugada){'T',&casilla_colindante_vacia_salvabombas,
                        &salvabombas,&salvabombas};
    opciona[6]=(jugada){'K',&tiene_salvavidas,&cargar,&cargar};
    opciona[7]=(jugada){'N',&ninja_saltable,&mover,&mover};
    opciona[8]=(jugada){'*',&jugador_cercano,&toque_critico,&toque_critico};
    opciona[9]=(jugada){'b',&casilla_colindante_vacia,&mover_y_bomba,&mover_y_bomba};
    //////////////////////////////////////////////////////////////////
    nulb=(paso){'A',&nada1};
}
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
void simula_detona1(uniendo* U,int y,int x,
                    tablero_booleano* pendientes,
                    int excepcion,bool* inmunes,
                    tablero_booleano* polvora){
int color=U->Bobjects.el_color_de(y,x);
    U->Bbackground.haz1(y,x);
    polvora->haz1(y,x);
    if(U->Bobjects.comparar(y,x,0) || color==0){
    }
    else if(U->Bobjects.comparar(y,x,'F')){
        if(color!=(excepcion+1) && inmunes[color]==false){
            if(U->player[color-1].tiene_salvavidas()){
                U->player[color-1].carga();
                inmunes[color-1]=true;
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
    }
    else{
        U->Bobjects.haz_ceniza(y,x);
    }
}
void simula_detona2(uniendo* U,int y,int x,tablero_booleano* pendientes,
                    bool* inmunes,
                    tablero_booleano* polvora){
int excepto=-1,tempx=x,tempy=y;//,wich_option;
    //wich_option=U->indice_de_la_opcion('D');
        U->Beffects.haz1(tempy,tempx);
        polvora->haz1(tempy,tempx);
        //U->imprimir();
        //U->poner_opciones(true,wich_option);
        //U->voltear_pantalla();
        //SDL_Delay(400);
    if(U->Bobjects.comparar(y,x,'T')){
        excepto=U->Bobjects.el_color_de(y,x)-1;
    }
    if(U->Bobjects.comparar(y,x,'F')==false){
        U->Bobjects.haz_ceniza(y,x);
    }
    {
        if(x>0){
            tempx-=1;
            simula_detona1(U,tempy,tempx,pendientes,excepto,inmunes,polvora);
            U->Beffects.haz1(tempy,tempx);
            tempx=x;
        }
        if(x<6){
            tempx+=1;
            simula_detona1(U,tempy,tempx,pendientes,excepto,inmunes,polvora);
            U->Beffects.haz1(tempy,tempx);
            tempx=x;
        }
        if(y>0){
            tempy-=1;
            simula_detona1(U,tempy,tempx,pendientes,excepto,inmunes,polvora);
            U->Beffects.haz1(tempy,tempx);
            tempy=y;
        }
        if(y<6){
            tempy+=1;
            simula_detona1(U,tempy,tempx,pendientes,excepto,inmunes,polvora);
            U->Beffects.haz1(tempy,tempx);
            tempy=y;
        }
    }
    //U->imprimir();
    //U->poner_opciones(true,wich_option);
    //U->voltear_pantalla();
    //SDL_Delay(400);
    {
        if(x>1 && U->Bobjects.comparar(tempy,tempx-1,0)){
            tempx-=2;
            simula_detona1(U,tempy,tempx,pendientes,excepto,inmunes,polvora);
            U->Beffects.haz1(tempy,tempx);
            tempx=x;
        }
        if(x<5 && U->Bobjects.comparar(tempy,tempx+1,0)){
            tempx+=2;
            simula_detona1(U,tempy,tempx,pendientes,excepto,inmunes,polvora);
            U->Beffects.haz1(tempy,tempx);
            tempx=x;
        }
        if(y>1 && U->Bobjects.comparar(tempy-1,tempx,0)){
            tempy-=2;
            simula_detona1(U,tempy,tempx,pendientes,excepto,inmunes,polvora);
            U->Beffects.haz1(tempy,tempx);
            tempy=y;
        }
        if(y<5 && U->Bobjects.comparar(tempy+1,tempx,0)){
            tempy+=2;
            simula_detona1(U,tempy,tempx,pendientes,excepto,inmunes,polvora);
            U->Beffects.haz1(tempy,tempx);
            tempy=y;
        }
    }
    //U->imprimir();
    //U->poner_opciones(true,wich_option);
    //U->voltear_pantalla();
    //SDL_Delay(400);
    //U->Beffects.reinicia();
}
void simula_detona3(uniendo* U,int y,int x,
                    tablero_booleano* polvora){
tablero_booleano pendientes;
bool* inmunidad;
int respaldo[7][7];///parche
    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++){
            respaldo[i][j]=U->Bobjects.el_color_de(i,j);
        }
    }
    inmunidad=new bool[U->cuantos_jugadores()+1];
    for(int i=1;i<=U->cuantos_jugadores();i++){
        inmunidad[i]=false;
    } inmunidad[0]=true;
    ///inicializaciones de rutina
    U->Bbackground.haz1(y,x);
    U->Beffects.haz1(y,x);
    polvora->haz1(y,x);
    simula_detona2(U,y,x,&pendientes,inmunidad,polvora);
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
                    simula_detona2(U,i,j,&pendientes,inmunidad,polvora);
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
                        U->Bobjects.cambiar(i,j,'T',respaldo[i][j]);
                    }
                    else{
                        U->Bobjects.elimina(i,j);
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
void uniendo::genera_polvoras(tablero_booleano* propio,tablero_booleano* ajeno){
    propio->reinicia();
    ajeno->reinicia();

    uniendo V(*regalo_final());
    /// fase 1-1 : bombas y salvavidas tuyos
    //bomba_tuya_o_salvabomba(&V);
    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++){
            if((V.Bobjects.comparar(i,j,'B')
               && V.Bobjects.el_color_de(i,j)==dran+1) ||
               (V.player[dran].tiene_salvabombas()
                && V.Bobjects.comparar(i,j,'F')
                && V.Bobjects.el_color_de(i,j)==dran+1)){

                V.reasimila_elementos(*regalo_final());
                bomba_tuya_o_salvabomba(&V);
                simula_detona3(&V,i,j,propio);
            }
        }
    }
    /// fase 1-2 : bombas y salvavidas ajenos
    for(int w=0;w<cuantos_jugadores();w++){
        if(w!=dran && player[w].vive()){
            V.reasimila_elementos(*regalo_final());
            V.set_dran(w);
            //bomba_tuya_o_salvabomba(&V);
            for(int i=0;i<7;i++){
                for(int j=0;j<7;j++){
                    if((V.Bobjects.comparar(i,j,'B')
                        && V.Bobjects.el_color_de(i,j)==w+1
                        && V.player[w].vive()) ||
                        (V.player[w].tiene_salvabombas()
                        && V.Bobjects.comparar(i,j,'F')
                        && V.Bobjects.el_color_de(i,j)==w+1)){

                        V.reasimila_elementos(*regalo_final());
                        V.set_dran(w);
                        simula_detona3(&V,i,j,ajeno);
                        //bomba_tuya_o_salvabomba(&V);
                    }
                }
            }
        }
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#endif
