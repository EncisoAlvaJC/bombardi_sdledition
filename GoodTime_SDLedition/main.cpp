#include "menu.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
int main ( int argc, char** argv ){

matriz A(2,2);
A.sustituir(1,1,4);
A.sustituir(1,2,2);
A.sustituir(2,1,1);
A.sustituir(1,3,0);
A.imprimir();
(A*A).imprimir();

    srand(time(NULL));

    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
        cout<<"Error al iniciar SDL: "<<SDL_GetError()<<endl;
        return 1;
    }
    atexit(SDL_Quit);

    SDL_WM_SetCaption("Bombardi: SDL edition || 14.01.07 || Julio Enciso",
                      NULL);
    SDL_Surface* pantalla = SDL_SetVideoMode(800,512,16,
                                             SDL_HWSURFACE|SDL_DOUBLEBUF);
    if(!pantalla){
        cout<<"Imposible inicializar video 800x512: "
            <<SDL_GetError()<<endl;
    }

    int x_vector[]={1,5,5,1,3};
    int y_vector[]={1,5,1,5,3};
    SDL_Event e;

    menu M(pantalla,e,
           (char*)"./bild/",8,(char*)"012345678");
    bombardi_engine UN;

    int control=0;

 while(M.dime_salida_prematura()==false &&
       UN.dime_salida_prematura()==false){
    switch(control){
    case 0:
        M=menu(pantalla,e,(char*)"./bild/",9,(char*)"012345678ngpqrt");
        M.elegir();
    case 1:
        UN=bombardi_engine(e,&pantalla,
                (char*)"./bild/",
               (char*)"logo",(char*)"credits",(char*)"pua",
               //
               7,(char*)"MBPDSTKN",
               //
               5,(char*)"FBPST",
               M.dime_n_jugadores(),M.dime_colores_elegidos(),
               //
               (char*)"cuadro",(char*)"U",
               //
               M.dime_n_jugadores(),
               //
               (char*)"_____",
               x_vector,y_vector,
               M.dime_hay_salvavidas());

        if(M.dime_salida_prematura()==false){
            control=UN.per_se();
        }
        break;
    default:
        break;
    }
}
    return 0;
}