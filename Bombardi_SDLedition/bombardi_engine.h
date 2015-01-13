#ifndef BOMBARDI_ENGINE_H
#define BOMBARDI_ENGINE_H
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#include "eventos.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#define PROB_ESTANCIA 0.7
#define PROB_TOQUE 1.1 ///esto es irreal
#define UMBRAL 0.0001
#define UMBRAL_SUICIDA 0.3
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/// por alguna razon que no comprendo, no puedo incluir mas archivos
/// por simplicidad, escribire todo dentro de los archivos que ya tenia
class matriz{
/// necesitare una clase matriz para el metodo de gradiente markoviano
private:
    int renglones, columnas;
    float** entrada;
public:
    matriz(int,int);
    matriz(){} /// para arreglos
    /// la idea de inicializar con un uniendo es "medir" el estado
    /// del tablero como una matriz estocastica
    matriz(uniendo*,int);

    float muestra(int,int);
    void cambia(int,int,float);

    matriz operator+(matriz);
    matriz operator*(matriz);
    void operator=(matriz);

    /// el objetivo de estos dos es multilicar una matriz estocastica
    /// por si misma muchas veces y construir un tipo especial de
    /// matriz estocastica; temporalmente la llamo "matriz sin hueso"
    /// o "matriz derivada de un proceso de markov cronometrado"
    matriz tendencia_markov();
    matriz derivado_markov();
    matriz reextraer_de(int,int);
    matriz reextraer_raum_de(int,int);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void matriz::operator=(matriz M){
    renglones=M.renglones;
    columnas=M.columnas;
    entrada =new float*[renglones];
    for(int i=0;i<renglones;i++){
        entrada[i]=new float[columnas];
        for(int j=0;j<columnas;j++){
            entrada[i][j]=M.entrada[i][j];
        }
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
matriz::matriz(int ren,int col){
    renglones=ren; columnas=col;
    entrada =new float*[renglones];
    for(int i=0;i<renglones;i++){
        entrada[i]=new float[columnas];
        for(int j=0;j<columnas;j++){
            entrada[i][j]=0;
        }
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
matriz::matriz(uniendo* U,int en_turno){
    renglones=54; columnas=54;
    entrada =new float*[renglones];
    for(int i=0;i<renglones;i++){
        entrada[i]=new float[columnas];
        for(int j=0;j<columnas;j++){
            entrada[i][j]=0;
        }
    }
/// parte 1: contar las casillas a las cuales uno se puede mover
    int contar[7][7];
    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++){
            contar[i][j]=0;
        }
    }
    int contar_r[5];
    for(int i=0;i<5;i++){
        contar_r[i]=0;
    }

    char dir[4];
    dir[0]='>'; dir[1]='A'; dir[2]='<'; dir[3]='V';
    coordenada C;
    int aux=0;

    for(int x=0;x<7;x++){
        for(int y=0;y<7;y++){
            for(int a=0;a<4;a++){
                for(int b=0;b<4;b++){
                    C=U->Bobjects.colindante_a(y,x,dir[a],dir[b]);
                    if(C.valido){
                        if(U->Bobjects.comparar(C.yy,C.xx,0)){
                            contar[y][x]+=1;
                        }
                        else{
                            if(U->Bobjects.comparar(C.yy,C.xx,'F') &&
                               U->Bobjects.el_color_de(C.yy,C.xx)==en_turno+1){
                                contar[y][x]+=1;
                            }
                            if(U->Bobjects.comparar(C.yy,C.xx,'R')){
                                aux=U->Bobjects.el_color_de(C.yy,C.xx)-1;
                                if((U->Bobjects.comparar(-1,aux,0)) ||
                                   (U->Bobjects.comparar(-1,aux,'F') &&
                                    U->Bobjects.el_color_de(-1,aux)==en_turno+1)){
                                        contar[y][x]+=1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    for(int color=0;color<5;color++){
        for(int z=0;z<U->cuantos_jugadores();z++){
            if(z!=color){
                if((U->Bobjects.comparar(-1,aux,0)) ||
                   (U->Bobjects.comparar(-1,aux,'F') &&
                    U->Bobjects.el_color_de(-1,aux)==en_turno+1)){
                        contar_r[color]+=1;
                }
            }
        }
        for(int i=0;i<7;i++){
            for(int j=0;j<7;j++){
                if(U->Bobjects.comparar(i,j,'R')
                   && U->Bobjects.el_color_de(i,j)==color+1){
                    for(int a=0;a<4;a++){
                        for(int b=0;b<4;b++){
                            C=U->Bobjects.colindante_a(i,j,dir[a],dir[b]);
                            if(C.valido){
                                if((U->Bobjects.comparar(C.yy,C.xx,0)) ||
                                   (U->Bobjects.comparar(C.yy,C.xx,'F') &&
                                    U->Bobjects.el_color_de(C.yy,C.xx)==en_turno+1)){
                                        U->Bbuttons.haz1(C.yy,C.xx);
                                }
        ///no se puede salir de un portal y justo entrar en otro
                            }
                        }
                    }
                }
            }
        }
        contar_r[color]+=U->Bbuttons.cuenta();
        U->Bbuttons.reinicia();
    }
/// paso 3: calcular las probabilidades
    float prob;
    for(int i=0;i<54;i++){
        entrada[i][i]=PROB_ESTANCIA;
    }
    for(int x=0;x<7;x++){
        for(int y=0;y<7;y++){
            if(contar[y][x]>0){
                prob=(1-PROB_ESTANCIA)/contar[y][x];
                for(int a=0;a<4;a++){
                    for(int b=0;b<4;b++){
                        C=U->Bobjects.colindante_a(y,x,dir[a],dir[b]);
                        if(C.valido){
                            if(U->Bobjects.comparar(C.yy,C.xx,0)){
                                entrada[7*y+x][7*C.yy+C.xx]=prob;
                            }
                            else{
                                if(U->Bobjects.comparar(C.yy,C.xx,'F') &&
                                   U->Bobjects.el_color_de(C.yy,C.xx)==en_turno+1){
                                        entrada[7*y+x][7*C.yy+C.xx]=prob;
                                }
                                if(U->Bobjects.comparar(C.yy,C.xx,'R')){
                                    aux=U->Bobjects.el_color_de(C.yy,C.xx)-1;
                                    if((U->Bobjects.comparar(-1,aux,0)) ||
                                       (U->Bobjects.comparar(-1,aux,'F') &&
                                            U->Bobjects.el_color_de(-1,aux)==en_turno+1)){
                                            entrada[7*y+x][49+aux]=prob;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else{
                /// si no puede moverse, esta encerrado
                entrada[7*y+x][7*y+x]=1;
            }
        }
    }
    for(int color=0;color<5;color++){
        if(contar_r[color]>0){
            prob=(1-PROB_ESTANCIA)/contar_r[color];
            for(int z=0;z<U->cuantos_jugadores();z++){
                if(z!=color){
                    if((U->Bobjects.comparar(-1,aux,0)) ||
                       (U->Bobjects.comparar(-1,aux,'F') &&
                        U->Bobjects.el_color_de(-1,aux)==en_turno+1)){
                                entrada[49+color][49+z]=prob;
                    }
                }
            }
            for(int i=0;i<7;i++){
                for(int j=0;j<7;j++){
                    if(U->Bobjects.comparar(i,j,'R')
                       && U->Bobjects.el_color_de(i,j)==color+1){
                        for(int a=0;a<4;a++){
                            for(int b=0;b<4;b++){
                                C=U->Bobjects.colindante_a(i,j,dir[a],dir[b]);
                                if(C.valido){
                                    if((U->Bobjects.comparar(C.yy,C.xx,0)) ||
                                       (U->Bobjects.comparar(C.yy,C.xx,'F') &&
                                        U->Bobjects.el_color_de(C.yy,C.xx)==en_turno+1)){
                                            entrada[49+color][7*C.yy+C.xx]=prob;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else{
            entrada[49+color][49+color]=1;
        }
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
float matriz::muestra(int m,int n){
float R=0;
    if(m<=renglones && n<=columnas){
        R=entrada[m][n];
    }
return R;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void matriz::cambia(int m,int n,float F){
    if(m<=renglones && n<=columnas){
        entrada[m][n]=F;
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
matriz matriz::operator+(matriz M){
    matriz R(1,1);
    if(renglones==M.renglones && columnas==M.columnas){
        R=matriz(renglones,columnas);
        for(int i=0;i<renglones;i++){
            for(int j=0;j<columnas;j++){
                R.entrada[i][j]+=(entrada[i][j]+M.entrada[i][j]);
            }
        }
    }
return R;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
matriz matriz::operator*(matriz M){
    matriz R(1,1);
    if(M.columnas==renglones){
        R=matriz(renglones,M.columnas);
        for(int i=0;i<R.renglones;i++){
            for(int j=0;j<R.columnas;j++){
                for(int k=0;k<columnas;k++){
                    R.entrada[i][j]+=(entrada[i][k]*M.entrada[k][j]);
                }
            }
        }
    }
return R;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
matriz matriz::tendencia_markov(){
    matriz R(*this);
    for(int i=0;i<5;i++){
        R=R*R;
    }
return R;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
matriz matriz::derivado_markov(){
    matriz R(108,108);
    for(int i=0;i<54;i++){
        R.entrada[i][54+i]=entrada[i][i];
        R.entrada[54+i][54+i]=1;
        /// ahorrando variable ya inicializadas
        for(int j=0;j<54;j++){
            if(i!=j)
                R.entrada[i][j]=entrada[i][j];
        }
    }
return R;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
matriz matriz::reextraer_de(int y,int x){
    matriz R(7,7);
    int D=0;
    if(y>-1){
        D=7*y+x;
    }
    else{
        D=49+x;
    }
    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++)
            R.entrada[i][j]=entrada[D][54+7*i+j];
    }
return R;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
matriz matriz::reextraer_raum_de(int y,int x){
    matriz R(1,5);
    int D=0;
    if(y>-1){
        D=7*y+x;
    }
    else{
        D=49+x;
    }
    for(int i=0;i<5;i++){
        R.entrada[0][i]=entrada[D][54+49+i];
    }
return R;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class ordinal{
/// La idea es tratar con reales positivos no-estandar infinitos
/// con distintos "ordenes"
/// Asi, es posible decir que sacar de juego a una persona es
/// "infinitamente mejor" que amenazar con sacarlo, mientras que aun
/// asi es mejor si se sacaran a dos personas del juego
private:
    int orden; /// numero variable de ordenes
    float* valor;
public:
    /// inicializadores
    ordinal(); //vacio
    ordinal(float); //estandar
    ordinal(const ordinal&); //copia
    ordinal(int,float*); //extendido

    float cantidad_no(int n){return (n<=orden)?valor[n]:0;}

    /// lo realmente jugoso es que los ordinales estan bien ordenados
    /// sin embargo, lo hice un poco difuso por diversion
    bool operator==(ordinal);
    bool operator<(ordinal);
    bool operator>(ordinal);

    void imprime();
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
ordinal::ordinal(){
    orden=-1; valor=0;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
ordinal::ordinal(float V){
    orden=0;
    valor= new float[1];
    valor[0]=V;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
ordinal::ordinal(int O, float* val){
    orden=O;
    valor=new float[O+1];
    for(int i=0;i<=O;i++){
        valor[i]=val[i];
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
ordinal::ordinal(const ordinal& U){
    orden=U.orden;
    valor=new float[orden+1];
    for(int i=0;i<=orden;i++){
        valor[i]=U.valor[i];
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
bool ordinal::operator==(ordinal T){
    if(orden!=T.orden){
        return false;
    }
    else{
        for(int i=orden;i>=0;i--){
            if((valor[i]-T.valor[i])*(valor[i]-T.valor[i])<UMBRAL*UMBRAL){
                /// la diferencia es "insignificante"
            }
            else{ ///la diferencia es significativa
                return false;
            }
        }
    }
    return true;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
bool ordinal::operator<(ordinal T){
    if(orden>T.orden){
        return false;
    }
    else{
        for(int i=orden;i>=0;i--){
            if((valor[i]-T.valor[i])*(valor[i]-T.valor[i])<UMBRAL*UMBRAL){
                /// la diferencia es "insignificante"
            }
            else{ ///la diferencia es significativa
                if(valor[i]>T.valor[i]){
                    return false;
                }
                if(valor[i]<T.valor[i]){
                    return true;
                }
            }
        }
    }
    return false;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
bool ordinal::operator>(ordinal T){
    if(orden<T.orden){
        return false;
    }
    else{
        for(int i=orden;i>=0;i--){
            if((valor[i]-T.valor[i])*(valor[i]-T.valor[i])<UMBRAL*UMBRAL){
                /// la diferencia es "insignificante"
            }
            else{ ///la diferencia es significativa
                if(valor[i]<T.valor[i]){
                    return false;
                }
                if(valor[i]>T.valor[i]){
                    return true;
                }
            }
        }
    }
    return true;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void ordinal::imprime(){
    for(int i=0;i<=orden;i++){
        cout<<"("<<valor[i]<<")";
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
class grad_markoviano{
private:
    int sho;
    int n_jugadores;
    float p_vivir,*p_morir,p_toque;
    bool* vive_el;
    int* cuantos_salvavid;
    int* cuantos_salvabom;
    /// p_vivir : prob de no ser atacado por sho
    /// p_toque : prob de estar estar cerca de todos
    char etiqueta[3];

    float norma;
public:
    grad_markoviano(uniendo*,int,tablero_booleano,
                    tablero_booleano,char*,opcion);
    grad_markoviano(grad_markoviano&);

    grad_markoviano(uniendo*,int); /// con todo al maximo

    void informa();
    float aplasta(int);
    int candidato_a_odiar();
    char* parche_de_acceso(){char* R=(char*)etiqueta;return R;}

    void operator -(grad_markoviano);
    float coseno_del_angulo_con(grad_markoviano);

    float distancia_a(grad_markoviano);

    ordinal evalua();
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
grad_markoviano::grad_markoviano(uniendo* U,int en_turno,
                                 tablero_booleano polvora_propia,
                                 tablero_booleano polvora_ajena,
                                 char* leer,
                                 opcion O){ ///parche
    for(int i=0;i<3;i++){
        etiqueta[i]=leer[i];
    }
    sho=en_turno;
    n_jugadores=U->cuantos_jugadores();
    p_morir=new float[n_jugadores];
    cuantos_salvavid=new int[n_jugadores];
    cuantos_salvabom=new int[n_jugadores];
    vive_el=new bool[n_jugadores];
    p_toque=0; p_vivir=0;
    for(int i=0;i<n_jugadores;i++){
        p_morir[i]=0;
        vive_el[i]=U->player[i].vive();
        cuantos_salvavid[i]=U->player[i].cuantos_salvavidas();
        cuantos_salvabom[i]=U->player[i].cuantos_salvabombas();
    }

    /// lo importante es calcular las p_'s mediante cadenas de markov
    /// transicion 1 : matriz de transicion clasica
    ///  sin_hueso 1 : derivado de markov
    ///  sin_hueso 2 : matriz limite de sin_hueso 1
    /// transicion 2 : si fuera ingeniero, esta llevaria mi nombre
    /// las posiciones las conservare para calcular cercanias
    matriz *transicion,*sin_hueso;
    int *x,*y;

    matriz *fantasma,*fantasma_raum;

    x=new int[n_jugadores];
    y=new int[n_jugadores];
    transicion=new matriz[n_jugadores];
    sin_hueso=new matriz[n_jugadores];

    fantasma=new matriz[n_jugadores];
    fantasma_raum=new matriz[n_jugadores];
    for(int i=0;i<n_jugadores;i++){
        if(vive_el[i]){
            x[i]=U->player[i].coordx();
            y[i]=U->player[i].coordy();
            transicion[i]=matriz(U,i);
            sin_hueso[i]=transicion[i].derivado_markov();
            sin_hueso[i]=sin_hueso[i].tendencia_markov();

            fantasma[i]=sin_hueso[i].reextraer_de(y[i],x[i]);
            fantasma_raum[i]=sin_hueso[i].reextraer_raum_de(y[i],x[i]);
        }
        else{
            //transicion[i]=matriz(7,7);
            fantasma[i]=matriz(7,7);
            fantasma_raum[i]=matriz(1,5);
        }
    }
    /// ahora biene la parte buena: donde se calcula el "estado"
    /// del tablero como un punto en [0,1]'n
    if(vive_el[sho]){
        for(int i=0;i<7;i++){
            for(int j=0;j<7;j++){
                if(polvora_ajena.valor(i,j)==false){
                    p_vivir+=(fantasma[sho].muestra(i,j));
                }
            }
        }
        for(int i=0;i<5;i++){
            if(polvora_ajena.valor(-1,i)==false){
                p_vivir+=(fantasma_raum[sho].muestra(0,i));
            }
        }
        if(polvora_ajena.valor(y[sho],x[sho]==true)){
            p_vivir*=(1-PROB_ESTANCIA);
        }
    }
    else{
        p_vivir=0;
    }
    //p_vivir*=2;
    /// p_morir significa "probabilidad de ser atacado por una bomba de sho"
    for(int w=0;w<n_jugadores;w++){
        if(w!=sho){
            if(vive_el[w]){
                for(int i=0;i<7;i++){
                    for(int j=0;j<7;j++){
                        if(polvora_propia.valor(i,j)==true){
                            p_morir[w]+=(transicion[w].muestra(i,j));
                        }
                    }
                }
                for(int i=0;i<5;i++){
                    if(polvora_propia.valor(-1,i)==true){
                        p_morir[w]+=(fantasma_raum[w].muestra(0,i));
                    }
                }
                if(polvora_propia.valor(y[w],x[w]==true)){
                    p_morir[w]=(1-(1-p_morir[w])*PROB_ESTANCIA);
                }
            }
            else{
                p_morir[w]=1;
            }
        }
    }
    /// calculando para el jugador w
    uniendo V;
    for(int z=0;z<n_jugadores;z++){
        if(z!=sho && vive_el[z]){ /// no calcular nada de jugadores eliminados
            V=uniendo(*U);
            V.set_dran(z);
            O.ejecuta_botones('M',&V);
            for(int i=0;i<7;i++){
                for(int j=0;j<7;j++){
                    if(V.Bbuttons.valor(i,j)){
                        p_toque+=fantasma[sho].muestra(i,j);
                    }
                }
            }
            for(int i=0;i<5;i++){
                if(V.Bbuttons.valor(-1,i)){
                    p_toque+=fantasma_raum[sho].muestra(0,i);
                }
            }
        }
        if(z!=sho && !vive_el[z]){
            p_toque+=PROB_TOQUE;
        }
    }
    int j=0;
    for(int i=0;i<n_jugadores;i++){
        //if(vive_el[i]){
            if(i!=sho){
                j++;
            }
        //}
    }
    if(j){
        p_toque/=j; /// normalizando
    }
    else{
        p_toque=PROB_TOQUE;
    }

    norma=0;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
grad_markoviano::grad_markoviano(grad_markoviano& G){
    sho=G.sho;
    n_jugadores=G.n_jugadores;
    p_morir=new float[n_jugadores];
    vive_el=new bool[n_jugadores];
    cuantos_salvavid=new int[n_jugadores];
    cuantos_salvabom=new int[n_jugadores];
    p_toque=G.p_toque;
    p_vivir=G.p_vivir;
    for(int i=0;i<n_jugadores;i++){
        p_morir[i]=G.p_morir[i];
        vive_el[i]=G.vive_el[i];
        cuantos_salvavid[i]=G.cuantos_salvavid[i];
        cuantos_salvabom[i]=G.cuantos_salvabom[i];
    }
    for(int i=0;i<3;i++){
        etiqueta[i]=G.etiqueta[i];
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
grad_markoviano::grad_markoviano(uniendo* U,int en_turno){
    sho=en_turno;
    n_jugadores=U->cuantos_jugadores();
    p_morir=new float[n_jugadores];
    vive_el=new bool[n_jugadores];
    cuantos_salvavid=new int[n_jugadores];
    cuantos_salvabom=new int[n_jugadores];
    p_toque=PROB_TOQUE;
    p_vivir=1;
    for(int i=0;i<n_jugadores;i++){
        p_morir[i]=1;
        cuantos_salvavid[i]=0;
        cuantos_salvabom[i]=0;
        //vive_el[i]=U->player[i].vive();
    }
    cuantos_salvavid[en_turno]=3;
    cuantos_salvabom[en_turno]=3;
    //p_morir[sho]=0;
    norma=0;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void grad_markoviano::informa(){
    for(int i=0;i<n_jugadores;i++){
        cout<<endl
            <<"jugador "<<(i+1)<<endl
            <<"p_vivir : "<<(1-p_morir[i])<<endl;
    }
    cout<<"p_tocar : "<<p_toque<<endl;
    cout<<"p_vivir : "<<p_vivir<<endl;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
float grad_markoviano::coseno_del_angulo_con(grad_markoviano G){
    float R=0;
    for(int i=0;i<n_jugadores;i++){
        if(i!=sho){
            R+=(p_morir[i]*G.p_morir[i]);
        }
    }
    R+=(p_vivir*G.p_vivir);
    //R+=p_toque*G.p_toque;
    if(norma!=0 && G.norma!=0){
        R=(R/(norma*G.norma));
        //R=(R/(G.norma));
    }
    else{
        R=0;
    }
return R;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
float grad_markoviano::aplasta(int el_enemigo){
    float R=10;
    /*
    R*=(0.1+p_vivir[sho]);
    for(int i=0;i<n_jugadores;i++){
        {
            R*=(2.5-p_vivir[i]);
        }
    }
    R*=(2-p_vivir[el_enemigo]);

    R*=(1+p_toque);
    */
return R;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
int grad_markoviano::candidato_a_odiar(){
    int R=0;
    float vida_larga=0;
    for(int i=0;i<n_jugadores;i++){
        if(i!=sho){
            if((1-p_morir[i])>vida_larga){
                R=i;
                vida_larga=(1-p_morir[i]);
            }
        }
    }
return R;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void grad_markoviano::operator-(grad_markoviano G){
    p_toque-=G.p_toque;
    p_vivir-=G.p_vivir;
    for(int i=0;i<n_jugadores;i++){
        p_morir[i]-=G.p_morir[i];
    }
    /// la norma solo se calcula aqui porqe solo se ocupa despues de esto
    norma=0;
    for(int i=0;i<n_jugadores;i++){
        if(i!=sho){
            norma+=(p_morir[i]*p_morir[i]);
        }
    }
    norma+=(p_vivir*p_vivir);
    norma+=(p_toque*p_toque);
    norma=sqrtf(norma);
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
float grad_markoviano::distancia_a(grad_markoviano G){
    float R=0;
    for(int i=0;i<n_jugadores;i++){
        if(i!=sho){
            R+=(p_morir[i]-G.p_morir[i])*(p_morir[i]-G.p_morir[i]);
        }
    }
    R+=(p_vivir-G.p_vivir)*(p_vivir-G.p_vivir);
    R+=(p_toque-G.p_toque)*(p_toque-G.p_toque);
return sqrt(R);
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
ordinal grad_markoviano::evalua(){
    float R=0;
    for(int i=0;i<n_jugadores;i++){
        if(i!=sho){
            R+=p_morir[i];
        }
    }
    //R+=p_vivir;
    //R+=p_toque;
    R = sqrt(R);

    float S=0;
    for(int i=0;i<n_jugadores;i++){
        if(i!=sho){
            if(p_morir[i]>(1-UMBRAL_SUICIDA)){
                S+=1;
            }
        }
    }
    if(p_vivir==0){
        S=0;
    }

    int SALV=3*(n_jugadores-1)+cuantos_salvavid[sho];
    for(int i=0;i<n_jugadores;i++){
        if(i!=sho){
            SALV-=cuantos_salvavid[i];
        }
    }

    int DIN=cuantos_salvabom[sho];

    float* conjunto=new float[6];
    conjunto[5]=S;
        /// # de no-supervivientes
    conjunto[4]=SALV*UMBRAL;
        /// # de salvavidas ajenos
    conjunto[2]=((p_vivir>UMBRAL_SUICIDA && cuantos_salvabom[sho])?p_vivir:0);
        /// alarma de que VA a morir sho
    conjunto[3]=R;
        /// amenazas a otros jugadores
    conjunto[1]=p_toque;
        /// cercania con el baricentro de los demas jugadores
    conjunto[0]=DIN*UMBRAL;
    ordinal ord(5,conjunto);
return ord;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
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
                    char*,int*,int*,bool*);
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
                                 bool* es_humano)
:uniendo(S,ruta,nom_logo,nom_credit,nom_arrow,
         ruta,num_opciones,nom_opciones,
         ruta,num_fichas,nom_fichas,
         num_colores,nom_colores,
         ruta,nom_fondo,nom_boton,
         num_jugadores,rol_de,X0,Y0,es_humano)
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
        if(player[dran].es_humano()){
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
                                    if(CAS==-1){
                                        CAS=-2;
                                    }
                                    else{
                                        if(CAS<50){
                                            if(Bbuttons.valor(CAS/7,CAS%7)==false){
                                                CAS=-2;
                                            }
                                        }
                                        else{
                                            if(Bbuttons.valor(-1,CAS-50)==false){
                                                CAS=-2;
                                            }
                                        }
                                        if(CAS==-2){
                                            ///patch
                                        }
                                        else{
                                            if(CAS<50){
                                                XX=CAS%7; YY=CAS/7;
                                            }
                                            else{
                                                XX=CAS-50; YY=-1;
                                            }
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
        else{
            /// fase 1 : calcular las polvoras
            tablero_booleano polvora_propia,polvora_ajena;
            genera_polvoras(&polvora_propia,&polvora_ajena);
            /// fase 2 : calcular el contenido del "gradiente markoviano"
            grad_markoviano ideal(regalo_final(),wo_ist_dran());
            grad_markoviano original(regalo_final(),wo_ist_dran(),
                                     polvora_propia,polvora_ajena,
                                     (char*)"G_0",*this);

            /// fase 4 : ejecutar todas las jugadas posibles en tableros temporales
            ///
            /// como las polvoras no se van a utilizar mas, reciclare las variables
            /// pero si que hay que generar los tableros temporles
            uniendo T1(*regalo_final()),T2(T1);
            grad_markoviano mejor_grad(regalo_final(),wo_ist_dran()),
                            grad_temporal(regalo_final(),wo_ist_dran());

            ordinal mejor_ordinal(-100);
            ordinal ordinal_temporal(-100);
            char etiquetador[3];

            int lim1; char aux;
            lim1=player[dran].cuantas_jugadas();
            for(int n=0;n<lim1;n++){
                aux=player[dran].da_la_opcion(n);
                ejecuta_botones(aux,&T1);
                T2=uniendo(T1);
                etiquetador[0]=aux;
                for(int i=0;i<7;i++){
                    for(int j=0;j<7;j++){
                        if(T2.Bbuttons.valor(i,j)){
                            etiquetador[1]='a'+i;
                            etiquetador[2]='1'+j;

                            ejecuta_simula(aux,&T2,i,j);
                            T2.genera_polvoras(&polvora_propia,&polvora_ajena);

                            //polvora_propia.parche_impresion();
                            //polvora_ajena.parche_impresion();

                            grad_temporal=grad_markoviano(&T2,dran,
                                                          polvora_propia,
                                                          polvora_ajena,
                                                          etiquetador,*this);
                            //grad_temporal-original;
                            //distancia_temporal=grad_temporal.distancia_a(ideal);
                            ordinal_temporal=grad_temporal.evalua();

                            /*
                            cout//<<"             "
                                <<etiquetador[0]
                                <<etiquetador[1]
                                <<etiquetador[2]
                                <<" : ";
                                //<<distancia_temporal
                                //<<endl;
                            ordinal_temporal.imprime();
                            cout<<endl;
                            */


                            //polvora_propia.parche_impresion();
                            //polvora_ajena.parche_impresion();

                            if(mejor_ordinal<ordinal_temporal){
                                mejor_grad=grad_markoviano(grad_temporal);
                                mejor_ordinal=ordinal_temporal;
                            }
                            T2=uniendo(T1);
                        }
                    }
                }
                T1.Bbuttons.reinicia();
            }
            //SDL_Delay(50);

                while(SDL_PollEvent(e)){
                    switch(e->type){
                        case SDL_QUIT:
                            salida_prematura=true;
                            break;
                        default:
                            break;
                    }
                }

            ejecuta_opciona(mejor_grad.parche_de_acceso()[0],this,
                            mejor_grad.parche_de_acceso()[1]-'a',
                            mejor_grad.parche_de_acceso()[2]-'1');
            turnos_trascurridos++;
            trofeo=dran;
            cout<<"["<<(dran+1)<<"]"
                <<mejor_grad.parche_de_acceso()[0]
                <<"("<<mejor_grad.parche_de_acceso()[1]<<","
                <<mejor_grad.parche_de_acceso()[2]<<")"<<endl;
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
