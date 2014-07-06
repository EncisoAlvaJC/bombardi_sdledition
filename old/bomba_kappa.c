#include <stdio.h>
#define CENIZA 'Y' /* valor de pared detonada; necesito que sea una letra, pero es mejor referirla comn una palabra */
#define PENDIENTE 'X' /* lo mismo que CENIZA, pero para bombas que aun no sonn detonadas */
#define VACIO ' ' /* es el espacio en ascii, pero luego se llega a olvidar su significado */

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
la funcion que imprime el titulo solo tiene texto y no sirve para nada mas
luego la hare mas estetica
*/

void titulo(void){
    printf("\t\t Bombardi. C-edition 0.3.2  por Julio Enciso, 2013\n\t\t\tVersion sin prueba completa de errores\n\n\n");
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
la funcion que dibuja el tablero se compone de:
    >t_a dibuja la parte de arriba
    >t_b dibuja las partes entre casillas
    >t_c dibuja la parte de abajo
    >t_d dibuja las casillas en si con los espacios entre estas
    >haz_tablero une las funciones anteriores y da el resultado final
*/
void t_a(void){
    printf("     1 2 3 4 5 6 7\n    %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",-38,-60,-62,-60,-62,-60,-62,-60,-62,-60,-62,-60,-62,-60,-65);
}
void t_b(void){
 printf("    %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",-61,-60,-59,-60,-59,-60,-59,-60,-59,-60,-59,-60,-59,-60,-76);
}
void t_c(void){
 printf("    %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",-64,-60,-63,-60,-63,-60,-63,-60,-63,-60,-63,-60,-63,-60,-39);
}
void t_d(unsigned char t[7][7],int k){ /* realmente solo hay un arreglo que vale la ena pedir */
int i;
printf("  %c %c",'a'+k,-77);  /* es fundamental que escriba el nombre de la coordenada */
for(i=0;i<7;i++)
    printf("%c%c",t[k][i],-61);
putchar('\n');
}
void haz_tablero(unsigned char t[7][7]){
int j;
printf("\n\n");
t_a();
for(j=0;j<6;j++){
    t_d(t,j);
    t_b();
    }
t_d(t,6);  /* no se me ha ocurrido una forma mas efectiva de ahorrar lineas de codigo */
t_c();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
la funcion que inicializa el tablero se compone de:
    >i_g (general) llena el tablero de espacios en blanco y TODOS los marcadores pertinentes con 0's
    >i_j (jugadores) pregunta sobre los jugadores que participaran
    >i_f (final) llena los marcadores pertinentes segun los jugadores activos
    >i_total (total) una simple recopilacion de lo anterior, para facilitar su escritura
*/
void i_g(unsigned char t[7][7],unsigned char j[],unsigned char nb[],unsigned char p[5][2]){
int m,n;
for(m=0;m<7;m++){
    for(n=0;n<7;n++)
        t[m][n]=VACIO;
    }
for(m=0;m<5;m++){
    j[m]=nb[m]=0;
    p[m][0]=p[m][1]=0;
    }
}
void i_j(unsigned char j[]){  /* esta funcion no me gusta porque no tiene prueba de errores, lo corregire despues */
int i;
char w;
printf("Fase de incicializacion.\n\nPara indicar que el espacio de jugador será usado, presionar 'S' o 's'.\n");
printf("En caso contrario, presionar 'N' o 'n'.\n\n");
for(i=0;i<5;i++){
    printf("Jugador %d: ",i+1);
        if((w=getchar())=='S' || w=='s')
            j[i]=1;
        getchar();
    }
}
void i_f(unsigned char t[7][7],unsigned char j[],unsigned char p[5][2]){
if(j[0]){
    t[1][1]='A';
    p[0][0]=1;
    p[0][1]=1;
    }
if(j[1]){
    t[1][5]='B';
    p[1][0]=1;
    p[1][1]=5;
    }
if(j[2]){
    t[3][3]='C';
    p[2][0]=3;
    p[2][1]=3;
    }
if(j[3]){
    t[5][1]='D';
    p[3][0]=5;
    p[3][1]=1;
    }
if(j[4]){
    t[5][5]='E';
    p[4][0]=5;
    p[4][1]=5;
    }
}
void i_total(unsigned char t[7][7],unsigned char j[],unsigned char nb[],unsigned char p[5][2]){
    i_g(t,j,nb,p);
    i_j(j);
    i_f(t,j,p);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
las funciones de leer el turno, poco desarrolladas pero funcionales en un primer momento:
    >lee_turno
    >valida_turno
    >eliminacion_por_ahogado
*/
void lee_turno(unsigned char t[]){  // esta funcion no 'piensa', solo recibe tres caracteres y el \n obligado
t[0]=getchar();
t[1]=getchar()-'a';
t[2]=getchar()-'1';
while(getchar()!='\n')
    ;
}
unsigned char absolute(char k){  // la usare para la funcion de validacion
return k>=0?k:k*(-1);
}
unsigned char valida_turno(unsigned char i,unsigned char tu[],unsigned char ta[7][7],unsigned char po[5][2],unsigned char nb[]){ // ¿lo que dijiste que vas a hacer esta permitido?
unsigned char validacion=0;
    if(tu[0]=='X'){
        validacion=1;
        goto se_rinde;
    }
    if(tu[0]!='M'&&tu[0]!='B'&&tu[0]!='P'&&tu[0]!='D') // ahorrar tiempo de evaluacion (¿?) basta que sea UNA letra valida
        goto nope;
    if(tu[1]<0||tu[1]>6) // solo invalida si no estas dentro
        goto nope;
    if(tu[2]<0||tu[2]>6) // solo invalida si no estas dentro
        goto nope;
    /* si se ha llegado a este punto, se presume que hay una jugada bien escrita */
    switch(tu[0]){
case 'M':
    if(absolute(po[i][0]-tu[1])>1||absolute(po[i][1]-tu[2])>1) // si no se selecciona una casilla colindante...
        printf("Movimiento ilegal: esa casilla no esta suficientemente cerca.\n");
    else validacion++;
    if(!(absolute(po[i][0]-tu[1])||absolute(po[i][1]-tu[2])))
        printf("Movimiento ilegal: no puede moverse hacia la casilla donde esta.\n");
    else validacion++;
    if(ta[tu[1]][tu[2]]!=VACIO)
        printf("Movimiento ilegal: la casilla no esta vacia.\n");
    else validacion++;
    validacion=((validacion==3)?1:0); // el turno es valido solo si se cumplen las tres condiciones anteriores
    break;
case 'B':
    if(absolute(po[i][0]-tu[1])>1||absolute(po[i][1]-tu[2])>1) // si no se selecciona una casilla colindante...
        printf("Movimiento ilegal: esa casilla no esta suficientemente cerca.\n");
    else validacion++;
    if(!(absolute(po[i][0]-tu[1])||absolute(po[i][1]-tu[2])))
        printf("Movimiento ilegal: no puede poner una bomba en la casilla donde esta.\n");
    else validacion++;
    if(ta[tu[1]][tu[2]]!=VACIO)
        printf("Movimiento ilegal: la casilla no esta vacia.\n");
    else validacion++;
    validacion=((validacion==3)?1:0); // el turno es valido solo si se cumplen las tres condiciones anteriores
    break;
case 'P':
    if(absolute(po[i][0]-tu[1])>1||absolute(po[i][1]-tu[2])>1) // si no se selecciona una casilla colindante...
        printf("Movimiento ilegal: esa casilla no esta suficientemente cerca.\n");
    else validacion++;
    if(!(absolute(po[i][0]-tu[1])||absolute(po[i][1]-tu[2])))
        printf("Movimiento ilegal: no puede poner una pared en la casilla donde esta.\n");
    else validacion++;
    if(ta[tu[1]][tu[2]]!=VACIO)
        printf("Movimiento ilegal: la casilla no esta vacia.\n");
    else validacion++;
    validacion=((validacion==3)?1:0); // el turno es valido solo si se cumplen las tres condiciones anteriores
    break;
case 'D':
    if(!nb[i])
        printf("Movimiento imposible: usted no tiene bombas para detonar.\n");
    else validacion++;
    if(ta[tu[1]][tu[2]]!=('a'+i))
        printf("Movimiento ilegal: lo que intenta detonar no es una bomba suya.\n");
    else validacion++;
    validacion=((validacion==2)?1:0); // el turno es valido solo si se cumplen las tres condiciones anteriores
    break;
    }

if(0){  // esta es una burbuja, solo se puede entrar mediante teletransportacion, pero cualquiera sale
nope: // la idea de usar el goto es porque hay mas formas de escribir incorrecta que correctamente
    printf("Jugada esscrita en un formato incorrecto. Por favor, intente de nuevo.");
}
se_rinde:
return validacion;
}
/* la siguiente funcion es particularmente larga y rudimentaria; posteriormente vere como reemplazarla por una mas ligera
   si se me permite un adjetivo mas para la siguiente funcion, sera altmanete impractica: ¿y si quiero cambiar el tamaño del tablero? */
unsigned char eliminacion_por_ahogado(unsigned char t[7][7],unsigned char pos_a,unsigned char pos_b,unsigned char nb,unsigned int jug){
unsigned char vive=0;
if(pos_a){
    if(6-pos_a){
        if(pos_b){
            if(6-pos_b){
                if(t[pos_a-1][pos_b-1]==VACIO)
                    vive++;
                if(t[pos_a-1][pos_b]==VACIO)
                    vive++;
                if(t[pos_a-1][pos_b+1]==VACIO)
                    vive++;
                if(t[pos_a][pos_b-1]==VACIO)
                    vive++;
                if(t[pos_a][pos_b+1]==VACIO)
                    vive++;
                if(t[pos_a+1][pos_b-1]==VACIO)
                    vive++;
                if(t[pos_a+1][pos_b]==VACIO)
                    vive++;
                if(t[pos_a+1][pos_b+1]==VACIO)
                    vive++;
            }
            else{
                if(t[pos_a-1][pos_b-1]==VACIO)
                    vive++;
                if(t[pos_a-1][pos_b]==VACIO)
                    vive++;
                //if(t[pos_a-1][pos_b+1]==VACIO)
                //    vive++;
                if(t[pos_a][pos_b-1]==VACIO)
                    vive++;
                //if(t[pos_a][pos_b+1]==VACIO)
                //    vive++;
                if(t[pos_a+1][pos_b-1]==VACIO)
                    vive++;
                if(t[pos_a+1][pos_b]==VACIO)
                    vive++;
                //if(t[pos_a+1][pos_b+1]==VACIO)
                //    vive++;
            }
        }
        else{
                //if(t[pos_a-1][pos_b-1]==VACIO)
                //    vive++;
                if(t[pos_a-1][pos_b]==VACIO)
                    vive++;
                if(t[pos_a-1][pos_b+1]==VACIO)
                    vive++;
                //if(t[pos_a][pos_b-1]==VACIO)
                //    vive++;
                if(t[pos_a][pos_b+1]==VACIO)
                    vive++;
                //if(t[pos_a+1][pos_b-1]==VACIO)
                //    vive++;
                if(t[pos_a+1][pos_b]==VACIO)
                    vive++;
                if(t[pos_a+1][pos_b+1]==VACIO)
                    vive++;
        }
    }
    else{
        if(pos_b){
            if(6-pos_b){
                if(t[pos_a-1][pos_b-1]==VACIO)
                    vive++;
                if(t[pos_a-1][pos_b]==VACIO)
                    vive++;
                if(t[pos_a-1][pos_b+1]==VACIO)
                    vive++;
                if(t[pos_a][pos_b-1]==VACIO)
                    vive++;
                if(t[pos_a][pos_b+1]==VACIO)
                    vive++;
                //if(t[pos_a+1][pos_b-1]==VACIO)
                //    vive++;
                //if(t[pos_a+1][pos_b]==VACIO)
                //    vive++;
                //if(t[pos_a+1][pos_b+1]==VACIO)
                //    vive++;
            }
            else{
                if(t[pos_a-1][pos_b-1]==VACIO)
                    vive++;
                if(t[pos_a-1][pos_b]==VACIO)
                    vive++;
                //if(t[pos_a-1][pos_b+1]==VACIO)
                //    vive++;
                if(t[pos_a][pos_b-1]==VACIO)
                    vive++;
                //if(t[pos_a][pos_b+1]==VACIO)
                //    vive++;
                //if(t[pos_a+1][pos_b-1]==VACIO)
                //    vive++;
                //if(t[pos_a+1][pos_b]==VACIO)
                //    vive++;
                //if(t[pos_a+1][pos_b+1]==VACIO)
                //    vive++;
            }
        }
        else{
                //if(t[pos_a-1][pos_b-1]==VACIO)
                //    vive++;
                if(t[pos_a-1][pos_b]==VACIO)
                    vive++;
                if(t[pos_a-1][pos_b+1]==VACIO)
                    vive++;
                //if(t[pos_a][pos_b-1]==VACIO)
                //    vive++;
                if(t[pos_a][pos_b+1]==VACIO)
                    vive++;
                //if(t[pos_a+1][pos_b-1]==VACIO)
                //    vive++;
                //if(t[pos_a+1][pos_b]==VACIO)
                //    vive++;
                //if(t[pos_a+1][pos_b+1]==VACIO)
                //    vive++;
        }
    }
}
else{
        if(pos_b){
            if(6-pos_b){
                //if(t[pos_a-1][pos_b-1]==VACIO)
                //    vive++;
                //if(t[pos_a-1][pos_b]==VACIO)
                //    vive++;
                //if(t[pos_a-1][pos_b+1]==VACIO)
                //    vive++;
                if(t[pos_a][pos_b-1]==VACIO)
                    vive++;
                if(t[pos_a][pos_b+1]==VACIO)
                    vive++;
                if(t[pos_a+1][pos_b-1]==VACIO)
                    vive++;
                if(t[pos_a+1][pos_b]==VACIO)
                    vive++;
                if(t[pos_a+1][pos_b+1]==VACIO)
                    vive++;
            }
            else{
                //if(t[pos_a-1][pos_b-1]==VACIO)
                //    vive++;
                //if(t[pos_a-1][pos_b]==VACIO)
                //    vive++;
                //if(t[pos_a-1][pos_b+1]==VACIO)
                //    vive++;
                if(t[pos_a][pos_b-1]==VACIO)
                    vive++;
                //if(t[pos_a][pos_b+1]==VACIO)
                //    vive++;
                if(t[pos_a+1][pos_b-1]==VACIO)
                    vive++;
                if(t[pos_a+1][pos_b]==VACIO)
                    vive++;
                //if(t[pos_a+1][pos_b+1]==VACIO)
                //    vive++;
            }
        }
        else{
                //if(t[pos_a-1][pos_b-1]==VACIO)
                //    vive++;
                //if(t[pos_a-1][pos_b]==VACIO)
                //    vive++;
                //if(t[pos_a-1][pos_b+1]==VACIO)
                //   vive++;
                //if(t[pos_a][pos_b-1]==VACIO)
                //    vive++;
                if(t[pos_a][pos_b+1]==VACIO)
                    vive++;
                //if(t[pos_a+1][pos_b-1]==VACIO)
                //    vive++;
                if(t[pos_a+1][pos_b]==VACIO)
                    vive++;
                if(t[pos_a+1][pos_b+1]==VACIO)
                    vive++;
        }
    }
vive+=nb;
if(!vive)
    jug=0;
return !vive;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
las funciones de turno cambian el tablero segun las acciones realizadas, son el corazon del juego
el verdadero protagonista es la funcion de detonacion, particularmente extensa
*/
void mover(unsigned char i,unsigned char ta[7][7],unsigned char p[5][2],unsigned char tu[]){
    ta[tu[1]][tu[2]]='A'+i; // pone la ficha en el lugar deseado
    ta[p[i][0]][p[i][1]]=VACIO; // limpia el lugar anterior
    p[i][0]=tu[1]; // sobreescribe la direccion de la ficha por la nueva
    p[i][1]=tu[2];
}
void bomba(unsigned char i,unsigned char ta[7][7],unsigned char nb[],unsigned char tu[]){
    ta[tu[1]][tu[2]]='a'+i; // pone una bomba en el lugar indicado
    nb[i]++; // indica que el jugador tiene una bomba mas
}
void pared(unsigned char i,unsigned char ta[7][7],unsigned char tu[]){
    ta[tu[1]][tu[2]]='P'+i; // pone una bomba en el lugar indicado
        /* aqui vale la pena destacar que en nuevo prototipo, las paredes se distinguen como las bombas
        esto es para reducir el tiempo en que una persona busca bombas de su color para colocar */
}
// la funcion detona_puntual solo detona una casilla; con ella sera mas rapida la fncion de detonacion en si
void detona_punto(unsigned char t[7][7],unsigned char j[],unsigned char nb[],unsigned char cord_a,unsigned char cord_b,unsigned int *pe){
switch(t[cord_a][cord_b]){  // depende de que haya en la casilla
case 'A':
    t[cord_a][cord_b]=CENIZA; // para fines de detonacion simultanea, debe dejar "bloques que estorben"
    j[0]=0; //elimina al jugador
    break;
case 'B':
    t[cord_a][cord_b]=CENIZA;
    j[1]=0;
    break;
case 'C':
    t[cord_a][cord_b]=CENIZA;
    j[2]=0;
    break;
case 'D':
    t[cord_a][cord_b]=CENIZA;
    j[3]=0;
    break;
case 'E':
    t[cord_a][cord_b]=CENIZA;
    j[4]=0;
    break;
case 'P':
    t[cord_a][cord_b]=CENIZA;  // sobra decir que asa a las paredes
    break;
case 'Q':
    t[cord_a][cord_b]=CENIZA;
    break;
case 'R':
    t[cord_a][cord_b]=CENIZA;
    break;
case 'S':
    t[cord_a][cord_b]=CENIZA;
    break;
case 'T':
    t[cord_a][cord_b]=CENIZA;
    break;
case 'a':
    t[cord_a][cord_b]=PENDIENTE;  // al ser alcanzada por una explosion, una bomba es activada
    nb[0]--; //disminuye la cantidad de bombas del jugador, y aumenta el numero de detonaciones pendientes
    (*pe)++;
    break;
case 'b':
    t[cord_a][cord_b]=PENDIENTE;
    nb[1]--;
    (*pe)++;
    break;
case 'c':
    t[cord_a][cord_b]=PENDIENTE;
    nb[2]--;
    (*pe)++;
    break;
case 'd':
    t[cord_a][cord_b]=PENDIENTE;
    nb[3]--;
    (*pe)++;
    break;
case 'e':
    t[cord_a][cord_b]=PENDIENTE;
    nb[4]--;
    (*pe)++;
    break;
default:  // no esta escrito, pero si en la casilla hay CENIZA o PENDIENTE, no hace nada
    break;
}
}
void detona_varios(unsigned char t[7][7],unsigned char j[],unsigned char cord_a,unsigned char cord_b,unsigned char nb[],unsigned int *pendientes){
t[cord_a][cord_b]=VACIO; // primero, quitar la bomba a detonar
if(cord_a){  //si la bomba detonada NO esta en la fila 'a'
    if((cord_a-1) && t[cord_a-1][cord_b]==VACIO)
        detona_punto(t,j,nb,cord_a-2,cord_b,pendientes);
    else
        detona_punto(t,j,nb,cord_a-1,cord_b,pendientes);
} // si la bomba detonada esta en la fila 'a', no pasa nada por arriba
if(cord_b){  //si la bomba detonada NO esta en la fila '1'
    if((cord_b-1) && t[cord_a][cord_b-1]==VACIO)
        detona_punto(t,j,nb,cord_a,cord_b-2,pendientes);
    else
        detona_punto(t,j,nb,cord_a,cord_b-1,pendientes);
}
if(6-cord_a){  //si la bomba detonada NO esta en la fila 'g'
    if((5-cord_a) && t[cord_a+1][cord_b]==VACIO)
        detona_punto(t,j,nb,cord_a+2,cord_b,pendientes);
    else
        detona_punto(t,j,nb,cord_a+1,cord_b,pendientes);
}
if(6-cord_b){  //si la bomba detonada NO esta en la fila ''7
    if((5-cord_b) && t[cord_a][cord_b+1]==VACIO)
        detona_punto(t,j,nb,cord_a,cord_b+2,pendientes);
    else
        detona_punto(t,j,nb,cord_a,cord_b+1,pendientes);
}

}
void detonar(unsigned int i,unsigned char t[7][7],unsigned char j[],unsigned char tu[],unsigned char nb[]){
unsigned int pendientes=0;
unsigned int m,n; // para recorrer el tablero
t[tu[1]][tu[2]]=PENDIENTE;  //lol, para que detonarla a previa orden si se puede mandar al ciclo de detonacion
pendientes++;
nb[i]--;
// mientras haya bombas pendientes por detonar
while(pendientes){
    for(m=0;m<7;m++){
        for(n=0;n<7;n++){
            if(t[m][n]==PENDIENTE){
                detona_varios(t,j,m,n,nb,&pendientes);
                pendientes--;
                }
        }
    }
/* de haber hecho las cosas bien a este punto, deberia darse el efecto esperado de detonar bombas en cadena */
for(m=0;m<7;m++){
    for(n=0;n<7;n++)
        if(t[m][n]==CENIZA)
            t[m][n]=VACIO; // ciclo de limpieza
    }
}
}
void rendirse(unsigned char i,unsigned char t[7][7],unsigned char j[],unsigned char p[5][2]){
    j[i]=0; // declara que el jugador deja de estar activo
    t[p[i][0]][p[i][1]]=VACIO;  //quita la ficha del jugador rendido
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
el objetivo de la siguiente funcion solo es recopilar las funciones anteriores para que el codigo sea mas entendible
*/
void burocracia(unsigned char k,unsigned char ta[7][7],unsigned char j[],unsigned char nb[],unsigned char p[5][2],unsigned char tu[]){
switch(tu[0]){
case 'M':
    mover(k,ta,p,tu);
    break;
case 'P':
    pared(k,ta,tu);
    break;
case 'B':
    bomba(k,ta,nb,tu);
    break;
case 'D':
    detonar(k,ta,j,tu,nb);
    break;
case 'X':
    rendirse(k,ta,j,p);
    break;
default:
    printf("El autor de este programa les recuerda que esta version no es a prueba de errores. Lo siento.\n");
}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
main(){
unsigned char tablero[7][7];  /* el tablero de 7x7 casillas */
unsigned char jugador[5];  /* por cada jugador, define si esta activo */
unsigned char numero_bombas[5];  /* por cada jugador, el numero de bombas que tiene; asi es facil saber si puede hacer algo */
unsigned char posicion[5][2];  /* da la posicion de la ficha de cada jugador tipo coordenada; facilita el movimiento */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
titulo();

i_total(tablero,jugador,numero_bombas,posicion);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
haz_tablero(tablero);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* ciclo principal del juego, mientra hayan dos o mas jugadores activos*/
unsigned char i=0; // numero del jugador en turno
unsigned char trofeo=VACIO; // guarda el numero del ultimo jugador en haber realizado un turno
while((jugador[0]+jugador[1]+jugador[2]+jugador[3]+jugador[3]+jugador[4])>1){
unsigned char turno[3];  /* para almacenar temporalmente el turno; se renueva a cada turno */
if(jugador[i]){  // si el jugador en turno esta jugando
    if(eliminacion_por_ahogado(tablero,posicion[i][0],posicion[i][1],numero_bombas[i],jugador[i]))
        printf("Jugador %d, ha sido eliminado por no tener movimientos posibles.\n",i+1);
if(jugador[i]){
do{  // leera el turno del jugador hasta que escriba uno legal
//    haz_tablero(tablero);  // porque se pierde visibilidad en cmd; sera eliminado posteriormente
    printf("Jugador %d introduzca su jugada: ",i+1);
    lee_turno(turno);
}
while(!valida_turno(i,turno,tablero,posicion,numero_bombas));
    burocracia(i,tablero,jugador,numero_bombas,posicion,turno);
        trofeo=i;
    haz_tablero(tablero);
}
}
    i=(i+1)%5; // avanzar al siguiente jugador
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* esta seccion se trata de declarar el ganador, de forma relativamente dimple */
switch(jugador[0]+jugador[1]+jugador[2]+jugador[3]+jugador[3]+jugador[4]){
case 1:
    printf("\n\nJugador %d, ha resultado ganador. Felicidades.\n",trofeo+1);
    break;
case 2:
    printf("\n\nEs un empate. Sin embargo, el reglamento favorece al ultimo jugador en realizar una jugada.\n\nJugador %d, la victoria es suya.",trofeo+1);
    break;
default:
    break;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
printf("\n\n\nEl autor de Bombardi, Julio Enciso, les agradece por probar este programa. Tengan un buen dia :D");
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

test_end: // fin de prueba
getchar();
return 0;
}
