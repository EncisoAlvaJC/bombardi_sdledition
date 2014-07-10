#ifndef BOMBARDI_ENGINE_H
#define BOMBARDI_ENGINE_H
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#include "eventos.h"
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
    renglones=49; columnas=49;
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

    for(int x=0;x<7;x++){
        for(int y=0;y<7;y++){
            for(int i=0;i<7;i++){
                if(abs(x-i)<2){
                    for(int j=0;j<7;j++){
                        if(abs(y-j)<2){
                            if(abs(x-i)+abs(y-j)!=0){
                                if(U->Bobjects.comparar(i,j,0)){
                                    contar[x][y]+=1;
                                }
                                else{
                                    if(U->Bobjects.comparar(i,j,'F') &&
                                    U->Bobjects.el_color_de(i,j)==en_turno+1){
                                        contar[x][y]+=1;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
/// paso 3: calcular las probabilidades
    float prob;
    for(int i=0;i<49;i++){
        entrada[i][i]=0.5;
    }
    for(int x=0;x<7;x++){
        for(int y=0;y<7;y++){
            if(contar[x][y]){
                prob=0.5/contar[x][y];
                for(int i=0;i<7;i++){
                    if(abs(x-i)<2){
                        for(int j=0;j<7;j++){
                            if(abs(y-j)<2){
                                if(abs(x-i)+abs(y-j)!=0){
                                    if(U->Bobjects.comparar(i,j,0)){
                                        entrada[7*x+y][7*i+j]=prob;
                                    }
                                    else{
                                        if(U->Bobjects.comparar(i,j,'F') &&
                                        U->Bobjects.el_color_de(i,j)==en_turno+1){
                                            entrada[7*x+y][7*i+j]=prob;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else{
                /// si no puede moverse, esta encerrado
                entrada[7*x+y][7*x+y]=1;
            }
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
    /*matriz R(renglones,columnas);
    for(int i=0;i<renglones;i++){
        for(int j=0;j<columnas;j++){
            R.entrada[i][j]=entrada[i][j];
        }
    }*/
    matriz R(*this);
    for(int i=0;i<5;i++){
        R=R*R;
    }
return R;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
matriz matriz::derivado_markov(){
    matriz R(98,98);
    for(int i=0;i<49;i++){
        R.entrada[i][i]=entrada[i][i];
        R.entrada[49+i][49+i]=1;
        /// ahorrando variable ya inicializadas
        for(int j=0;j<49;j++){
            if(i!=j)
                R.entrada[i][49+j]=entrada[i][j];
        }
    }
return R;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
matriz matriz::reextraer_de(int y,int x){
    matriz R(7,7);
    int D=7*x+y;
    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++)
            R.entrada[i][j]=entrada[D][49+7*i+j];
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
class grad_markoviano{
private:
    int sho;
    int n_jugadores;
    float *p_vivir,*p_toque,*poder_contra;
    bool* vive_el;
    /// p_vivir me da la prob de no ser atacado
    /// p_toque me da la prob de estar en una casilla colindante
    char etiqueta[3];
public:
    grad_markoviano(uniendo*,int,tablero_booleano,
                    tablero_booleano,char*);
    grad_markoviano(grad_markoviano&);
    void informa();
    float aplasta(int);
    int candidato_a_odiar();
    char* parche_de_acceso(){char* R=(char*)etiqueta;return R;}
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
grad_markoviano::grad_markoviano(uniendo* U,int en_turno,
                                 tablero_booleano polvora_propia,
                                 tablero_booleano polvora_ajena,
                                 char* leer){
    for(int i=0;i<3;i++){
        etiqueta[i]=leer[i];
    }
    sho=en_turno;
    n_jugadores=U->cuantos_jugadores();
    p_vivir=new float[n_jugadores];
    p_toque=new float[n_jugadores];
    poder_contra=new float[n_jugadores];
    vive_el=new bool[n_jugadores];
    for(int i=0;i<n_jugadores;i++){
        p_vivir[i]=0;
        p_toque[i]=0;
        poder_contra[i]=0;
        vive_el[i]=U->player[i].vive();
    }

    /// lo importante es calcular las p_'s mediante cadenas de markov
    /// transicion 1 : matriz de transicion clasica
    ///  sin_hueso 1 : derivado de markov
    ///  sin_hueso 2 : matriz limite de sin_hueso 1
    /// transicion 2 : si fuera ingeniero, esta llevaria mi nombre
    /// las posiciones las conservare para calcular cercanias
    matriz *transicion,*sin_hueso;
    int *x,*y;

    x=new int[n_jugadores];
    y=new int[n_jugadores];
    transicion=new matriz[n_jugadores];
    sin_hueso=new matriz[n_jugadores];
    for(int i=0;i<n_jugadores;i++){
        if(U->player[i].vive()){
            x[i]=U->player[i].coordx();
            y[i]=U->player[i].coordy();
            transicion[i]=matriz(U,i);
            sin_hueso[i]=transicion[i].derivado_markov();
            sin_hueso[i]=sin_hueso[i].tendencia_markov();
            transicion[i]=sin_hueso[i].reextraer_de(x[i],y[i]);
        }
        else{
            transicion[i]=matriz(7,7);
        }
    }
    /// ahora biene la parte buena: donde se calcula el "estado"
    /// del tablero como un punto en [0,1]'n
    for(int i=0;i<7;i++){
        for(int j=0;j<n_jugadores;j++){
            if(polvora_ajena.valor(i,j)==false){
                p_vivir[sho]+=(transicion[sho].muestra(i,j));
            }
        }
    }
    for(int w=0;w<n_jugadores;w++){
        if(w!=sho && U->player[w].vive()){
            for(int i=0;i<7;i++){
                for(int j=0;j<7;j++){
                    if(polvora_propia.valor(i,j)==false){
                        p_vivir[w]+=(transicion[w].muestra(i,j));
                    }
                }
            }
        }
    }
    /// calculando para el jugador w
    for(int w=0;w<n_jugadores;w++){
        if(vive_el[w]){ /// no calcular nada de jugadores eliminados
            for(int z=0;z<n_jugadores;z++){
                if(w!=z && vive_el[z]){
                    for(int i=0;i<7;i++){
                        if(abs(i-x[z])<2){
                            for(int j=0;j<7;j++){
                                if(abs(j-y[z])<2){
                                    p_toque[w]+=transicion[w].muestra(i,j);
                                }
                            }
                        }
                    }
                }
            }
        }
        p_toque[w]/=(U->cuantos_jugadores()-1); /// normalizando
    }

    for(int w=0;w<n_jugadores;w++){
        if(w!=sho && vive_el[w]){
            for(int i=0;i<7;i++){
                if(abs(i-x[w])<2){
                    for(int j=0;j<7;j++){
                        if(abs(j-y[w])<2){
                            if(polvora_propia.valor(i,j)){
                                poder_contra[w]+=transicion[w].muestra(i,j);
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
grad_markoviano::grad_markoviano(grad_markoviano& G){
    sho=G.sho;
    n_jugadores=G.n_jugadores;
    p_vivir=new float[n_jugadores];
    p_toque=new float[n_jugadores];
    for(int i=0;i<n_jugadores;i++){
        p_vivir[i]=G.p_vivir[i];
        p_toque[i]=G.p_toque[i];
    }
    for(int i=0;i<3;i++){
        etiqueta[i]=G.etiqueta[i];
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void grad_markoviano::informa(){
    for(int i=0;i<n_jugadores;i++){
        cout<<endl
            <<"jugador "<<(i+1)<<endl
            <<"p_vivir : "<<p_vivir[i]<<endl
            <<"p_tocar : "<<p_toque[i]<<endl;
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/// como no tengo suficiente informacion sobre la funcion como para
/// optimizar sus resultados, voy a "aplastar" los vectores que
/// representan el tablero a un solo numero
float grad_markoviano::aplasta(int el_enemigo){
    float R=10;
    R*=(0.1+p_vivir[sho]);
    for(int i=0;i<n_jugadores;i++){
        //if(i!=sho && i!=el_enemigo && vive_el[i]){
        {
            R*=(2.5-p_vivir[i]);
            //R*=(1+p_toque[i]);
        }
    }
    R*=(2-p_vivir[el_enemigo]);

    //R*=(1+p_toque[el_enemigo]);
    R*=(1+p_toque[sho]);
return R;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
int grad_markoviano::candidato_a_odiar(){
    int R=0;
    float vida_larga=0;
    for(int i=0;i<n_jugadores;i++){
        if(i!=sho){
            if(p_vivir[i]>vida_larga){
                R=i;
                vida_larga=p_vivir[i];
            }
        }
    }
    /*
    float menos_amenazado=1;
    for(int i=0;i<n_jugadores;i++){
        if(i!=sho){
            if(poder_contra[i]<menos_amenazado){
                R=i;
                menos_amenazado=poder_contra[i];
            }
        }
    }
    */
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
                    char*,int*,int*,bool,bool*);
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
                                 bool hay_salvavidas,
                                 bool* es_humano)
:uniendo(S,ruta,nom_logo,nom_credit,nom_arrow,
         ruta,num_opciones,nom_opciones,
         ruta,num_fichas,nom_fichas,
         num_colores,nom_colores,
         ruta,nom_fondo,nom_boton,
         num_jugadores,rol_de,X0,Y0,hay_salvavidas,es_humano)
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
        else{
            /// fase 1 : calcular las polvoras
            tablero_booleano polvora_propia,polvora_ajena;
            genera_polvoras(&polvora_propia,&polvora_ajena);
            /// fase 2 : calcular el contenido del "gradiente markoviano"
            grad_markoviano original(regalo_final(),wo_ist_dran(),
                                     polvora_propia,polvora_ajena,
                                     (char*)"ini");
            /// fase 3 : calcular el "valor aplastado" del tablero actual
            int amenaza=original.candidato_a_odiar();

            float aplastado_original=original.aplasta(amenaza);
            /*
            cout<<"  amenaza actual : jugador "<<(amenaza+1)<<endl;
            cout<<" valor aplastado : "<<aplastado_original<<endl;
            */
            /// fase 4 : ejecutar todas las jugadas posibles en tableros temporales
            ///
            /// como las polvoras no se van a utilizar mas, reciclare las variables
            /// pero si que hay que generar los tableros temporles
            uniendo T1(*regalo_final()),T2(T1);
            grad_markoviano mejor_grad(original),grad_temporal(original);
            float mejor_puntuacion=-1,puntuacion_temporal=0;
            char etiquetador[3];

            int lim1; char aux; /// este evitara confusiones
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

                            polvora_propia.reinicia();
                            polvora_ajena.reinicia();

                            ejecuta_simula(aux,&T2,i,j);
                            T2.genera_polvoras(&polvora_propia,&polvora_ajena);
                            grad_temporal=grad_markoviano(&T2,dran,
                                                          polvora_propia,
                                                          polvora_ajena,
                                                          etiquetador);

                            puntuacion_temporal=grad_temporal.aplasta(amenaza);
                            /*cout<<"             "
                                <<etiquetador[0]
                                <<etiquetador[1]
                                <<etiquetador[2]
                                <<" : "<<puntuacion_temporal<<endl;*/

                            //grad_temporal.informa();
                            //polvora_propia.parche_impresion();
                            //polvora_ajena.parche_impresion();

                            if(puntuacion_temporal>=mejor_puntuacion){
                                mejor_grad=grad_markoviano(grad_temporal);
                                mejor_puntuacion=puntuacion_temporal;
                            }

                            T2=uniendo(T1);
                        }
                    }
                }
                T1.Bbuttons.reinicia();
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
