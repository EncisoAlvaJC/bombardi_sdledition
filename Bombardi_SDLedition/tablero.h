#ifndef TABLERO_H
#define TABLERO_H
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#include "coleccion.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/// parche: topologia alterada del tablero, por vacio
/// para trabajar con ello debo tener coordenadas
typedef struct{
    int yy;
    int xx;
} coordenada;
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class tablero_objetos{
private:
/// el tablero objetos unicamente tiene un arreglo de caracteres con la
/// letra que identifica a cada objeto, y el color del objeto
    char casilla[7][7];
    char color[7][7];
/// Raum significa "espacio" en aleman; son las casillas en el espacio
/// inter-portal
/// para referirse al raum #n es (-1,n)
    char raum[5][2];
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

    void parche_impresion();

    /// dada la topologia alteada del tablero, se vuelve complicado
    coordenada colindante_a(int,int,char,char);
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
    for(int i=0;i<5;i++){
        raum[i][0]=0;
        raum[i][1]=0;
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
    for(int i=0;i<5;i++){
        raum[i][0]=T.raum[i][0];
        raum[i][1]=T.raum[i][1];
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void tablero_objetos::cambiar(int y,int x,char k,int n){
    if(y>-1){
        casilla[y][x]=k; color[y][x]=n;
    }
    else{
        raum[x][0]=k; raum[x][1]=n;
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void tablero_objetos::elimina(int y,int x){
    if(y>-1){
        casilla[y][x]=0; color[y][x]=0;
    }
    else{
        raum[x][0]=0; raum[x][1]=0;
    }
}
void tablero_objetos::haz_ceniza(int y,int x){
    if(y>-1){
        color[y][x]=0;
    }
    else{
        raum[x][1]=0;
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
bool tablero_objetos::comparar(int y,int x,char k){
    if(y>-1){
        return (casilla[y][x]==k);
    }
    else{
        return (raum[x][0]==k);
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
int tablero_objetos::el_color_de(int y,int x){
    if(y>-1){
        int ans=color[y][x]; return ans;
    }
    else{
        int ans=raum[x][1]; return ans;
    }
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
    auxiliar.x=(32+64);lugar.y=(32+7*64);
    for(int j=0;j<5;j++,auxiliar.x+=64){
        C.imprimir_ficha(raum[j][0],raum[j][1],auxiliar,S);
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void tablero_objetos::parche_impresion(){
    cout<<endl<<endl;
    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++){
            cout<<" "<<casilla[i][j];
        }
        cout<<"    ";
        for(int j=0;j<7;j++){
            cout<<" "<<color[i][j];
        }
        cout<<endl;
    }
    for(int i=0;i<5;i++){
        cout<<raum[i][0];
    }
    cout<<"    ";
    for(int i=0;i<5;i++){
        cout<<raum[i][1];
    }
    cout<<endl;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
coordenada tablero_objetos::colindante_a(int y, int x,char d1,char d2){
/// el protocolo es "desde x,y en direccion...", donde las
/// direcciones posibles son:
/// AA : arriba
/// << : izquierda
/// VV : abajo
/// >> : derecha
/// >A , A> : arriba-derecha
/// <A , A< : arriba-izquierda
/// <V , V< : abajo-izquierda
/// >V , V> : abajo-derecha
coordenada ans; ans.xx=-1; ans.yy=-1;
int X=x, Y=y;
    ///si es coordenada "invalida", optimizacion
    if(X==-1 || Y==-1){return ans;}
    ///en direccion norsur
    if(d1=='A' && d2=='V'){return ans;}
    if(d1=='V' && d2=='A'){return ans;}
    if(d1=='<' && d2=='>'){return ans;}
    if(d1=='>' && d2=='<'){return ans;}

    if(d1=='A' || d2=='A'){
    /// AA , A> , >A , <A , A<
        if(d1=='<' || d2=='<'){
        /// A< , <A
            X--; Y--;
            while(Y>-1 && X>-1){
                ans.yy=Y; ans.xx=X;
                if(casilla[Y][X]!='V'){///si no es vacio, ya se acabo
                    return ans;
                }
                X--; Y--; ///si es vacio, sera la siguiente
            }
            return ans;
        }
        if(d1=='>' || d2=='>'){
        /// >A , A>
            X++; Y--;
            while(Y>-1 && X<7){
                ans.yy=Y; ans.xx=X;
                if(casilla[Y][X]!='V'){///si no es vacio, ya se acabo
                    return ans;
                }
                X++; Y--; ///si es vacio, sera la siguiente
            }
            return ans;
        }
        /// AA, el resto
        Y--;
        while(Y>-1){
            ans.yy=Y;
            if(casilla[Y][X]!='V'){///si la casilla no es vacio, ya se acabo
                return ans;
            }
            Y--;
        }
        return ans;
    }
    if(d1=='V' || d2=='V'){
    /// VV , V> , >V , <V , V<
        if(d1=='<' || d2=='<'){
        /// V< , <V
            X--; Y++;
            while(Y<7 && X>-1){
                ans.yy=Y; ans.xx=X;
                if(casilla[Y][X]!='V'){///si no es vacio, ya se acabo
                    return ans;
                }
                X--; Y++; ///si es vacio, sera la siguiente
            }
            return ans;
        }
        if(d1=='>' || d2=='>'){
        /// >V , V>
            X++; Y++;
            while(Y<7 && X<7){
                ans.yy=Y; ans.xx=X;
                if(casilla[Y][X]!='V'){///si no es vacio, ya se acabo
                    return ans;
                }
                X++; Y++; ///si es vacio, sera la siguiente
            }
            return ans;
        }
        /// VV, el resto
        Y++;
        while(Y<7){
            ans.yy=Y;
            if(casilla[Y][X]!='V'){///si la casilla no es vacio, ya se acabo
                return ans;
            }
            Y++;
        }
        return ans;
    }
    if(d1=='<' && d2=='<'){
        /// <<
        X--;
        while(X>-1){
            ans.xx=X;
            if(casilla[Y][X]!='V'){///si la casilla no es vacio, ya se acabo
                return ans;
            }
            X--;
        }
        return ans;
    }
    if(d1=='>' && d2=='>'){
        /// >
        X++;
        while(X<7){
            ans.xx=X;
            if(casilla[Y][X]!='V'){///si la casilla no es vacio, ya se acabo
                return ans;
            }
            X++;
        }
        return ans;
    }
return ans;///ERROR
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class tablero_booleano{
/// para acelerar las cosas, los tableros con el fondo y los botones
/// manejan bool en vez de char
/// notese que el raum se maneja como un segundo tablero
private:
    bool casilla[7][7];
    bool raum[5];
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
    for(int i=0;i<5;i++){
        cout<<" ";
        if(raum[i]){
            cout<<"1";
        }
        else{
            cout<<"_";
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
    for(int i=0;i<5;i++){
        raum[i]=false;
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
    for(int i=0;i<5;i++){
        raum[i]=T.raum[i];
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
    for(int i=0;i<5;i++){
        raum[i]=false;
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void tablero_booleano::haz0(int y,int x){
    if(y>-1){
        casilla[y][x]=false;
    }
    else{
        raum[x]=false;
    }
}
void tablero_booleano::haz1(int y,int x){
    if(y>-1){
        casilla[y][x]=true;
    }
    else{
        raum[x]=true;
    }
}
bool tablero_booleano::valor(int y,int x){
    if(y>-1){
        return (casilla[y][x]==true);
    }
    else{
        return (raum[x]==true);
    }
}
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
    auxiliar.x=(32+64);auxiliar.y=(32+7*64);
    for(int j=0;j<5;j++,auxiliar.x+=64){
        B.imprimir_boton(raum[j],auxiliar,S);
    }
}
void tablero_booleano::imprimir_rojos(fondo_boton R,
                                      SDL_Surface* S){
SDL_Rect lugar,auxiliar; lugar.x=32;lugar.y=32;
    for(int i=0;i<7;i++,lugar.y+=64){
        auxiliar=lugar;
        for(int j=0;j<7;j++,auxiliar.x+=64){
            R.imprimir_rojos(casilla[i][j],auxiliar,S);
        }
    }
    auxiliar.x=(32+64);auxiliar.y=(32+7*64);
    for(int j=0;j<5;j++,auxiliar.x+=64){
        R.imprimir_rojos(raum[j],auxiliar,S);
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
    auxiliar.x=(32+64);auxiliar.y=(32+7*64);
    for(int j=0;j<5;j++,auxiliar.x+=64){
        F.imprimir_raum(raum[j],auxiliar,S);
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
    for(int i=0;i<5;i++){
        if(raum[i]==true){
            ans+=1;
        }
    }
return ans;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#endif
