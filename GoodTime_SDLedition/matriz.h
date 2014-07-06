#ifndef MATRIZ_H
#define MATRIZ_H
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#include "eventos.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/// A partir de este punto empieza el algorimto de jugador automatico
/// que he llamado "Metodo de Gradiente Markoviano"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class matriz{
private:
    float** entrada;
    int renglones,columnas;
public:
    matriz(int,int);
    matriz(uniendo*); /// Matriz de transicion habitual

    void imprimir();
    void sustituir(int,int,float);
    float valor(int,int);

    matriz operator +(matriz);
    matriz operator *(matriz);

    matriz tendencia_de_markov();
    matriz derivado_markoviano();
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/// renglones:
///   -------
///   -------
///   -------
/// columnas:
///   |||
///   |||
///   |||
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
matriz::matriz(int ren,int col){
    renglones=ren; columnas=col;
    entrada=new float*[renglones];
    for(int i=0;i<renglones;i++){
        entrada[i]=new float[columnas];
        for(int j=0;j<columnas;j++)
            entrada[i][j]=0;
    }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
matriz::matriz(uniendo* U){
/// Este constructor toma el estado del tablero y forma la matriz de
/// transicion correspondiente al jugador en turno
/// Empieza inicializandola con 49x49 en ceros
    renglones=columnas=49;
    entrada=new float*[49];
    for(int i=0;i<49;i++){
        entrada[i]=new float[49];
        for(int j=0;j<49;j++)
            entrada[i][j]=0;
    }
/// Posteriormente hare un par de "constantes simbolicas" para
/// facilitar la referencia
    int en_turno=U->wo_ist_dran();
    //int X=U->player[en_turno].coordx();
    //int Y=U->player[en_turno].coordy();
/// Ahora, se cuentan las casillas a las cuales se puede mover uno
/// /// Inicializar
    int movimientos[7][7];
    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++)
            movimientos[i][j]=0;
    }
/// /// Contar
    for(int x=0;x<7;x++){
        for(int y=0;y<7;y++){
            for(int i=0;i<7;i++){
                if(abs(i-y)<2){
                    for(int j=0;j<7;j++){
                        if(abs(j-x)<2){
                            if(U->Bobjects.comparar(i,j,0)){
                                movimientos[x][y]+=1;
                            }
                            else if(U->Bobjects.comparar(i,j,'F') &&
                                    U->Bobjects.el_color_de(i,j)==en_turno &&
                                    y!=i && x!=j){
                                movimientos[x][y]+=1;
                            }
                        }
                    }
                }
            }
        }
    }
/// /// Calcular y escribir probabilidades
    for(int i=0;i<49;i++)
        entrada[i][i]=0.5;
    for(int x=0;x<7;x++){
        for(int y=0;y<7;y++){
            if(movimientos[x][y]){
                float prob=0.5/movimientos[x][y];
                for(int i=0;i<7;i++){
                    if(abs(x-i)<2){
                        for(int j=0;j<7;j++){
                            if(abs(y-j)<2){
                                if(U->Bobjects.comparar(i,j,0)){
                                    entrada[7*x+y][7*i+j]=prob;
                                }
                                else if(U->Bobjects.comparar(i,j,'F') &&
                                        U->Bobjects.el_color_de(i,j)==en_turno &&
                                        y!=i && x!=j){
                                    entrada[7*x+y][7*i+j]=prob;
                                }
                            }
                        }
                    }
                }
            }
            else{
                entrada[7*x+y][7*x+y]=1;
            }
        }
    }
/// Queda terminada la matriz de transicion de la ficha del jugador,
/// modelada como un Proceso de Markov
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void matriz::imprimir(){
    for(int i=0;i<renglones;i++){
        cout<<" (";
        for(int j=0;j<columnas;j++){
            cout<<"  "<<entrada[i][j];
        }
        cout<<"  )"<<endl;
    }
    cout<<endl;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void matriz::sustituir(int x,int y,float nuevo){
/// Los indices son modificados para ser mas intuitiva la notacion
    entrada[x-1][y-1]=nuevo;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
float matriz::valor(int x,int y){
    float R=entrada[x][y];
return R;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
matriz matriz::operator+(matriz M){
    matriz R(1,1);
    if(M.renglones==renglones && M.columnas==columnas){
        R=matriz(renglones,columnas);
        for(int i=0;i<renglones;i++){
            for(int j=0;j<columnas;j++)
                R.sustituir(i,j,entrada[i][j]+M.entrada[i][j]);
        }
    }
return R;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
matriz matriz::operator*(matriz M){
    matriz R(1,1);
    if(M.renglones==columnas){
        R=matriz(renglones,M.columnas);
        for(int i=0;i<renglones;i++){
            for(int j=0;j<M.columnas;j++){
                for(int m=0;m<columnas;m++){
                    R.entrada[i][j]+=entrada[i][m]*M.entrada[m][j];
                }
            }
        }
    }
return R;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
matriz matriz::tendencia_de_markov(){
    matriz R(renglones,columnas);
    for(int i=0;i<renglones;i++){
        for(int j=0;j<columnas;j++)
            R.entrada[i][j]=entrada[i][j];
    }
    for(int i=0;i<10;i++)
        R=R*R;
return R;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
matriz matriz::derivado_markoviano(){
    matriz R(2*renglones,2*columnas);
    int M=min(renglones,columnas);
    for(int i=0;i<M;i++){
        R.entrada[i][i]=entrada[i][i];
        R.entrada[M+i][M+i]=1;
    }
    for(int i=0;i<renglones;i++){
        for(int j=0;j<columnas;j++){
            if(abs(i-j)!=0){
                R.entrada[i][columnas+j]=entrada[i][j];
            }
        }
    }
return R;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#endif
