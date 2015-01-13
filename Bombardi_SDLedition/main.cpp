#include "menu.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
int main ( int argc, char** argv ){
    srand(time(NULL));

    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
        cout<<"Error al iniciar SDL: "<<SDL_GetError()<<endl;
        return 1;
    }
    atexit(SDL_Quit);

    SDL_WM_SetCaption("Kalium || Prototype for AI Testing || Julio Enciso (4.34)",
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

    //
    bool humanos[]={true,false,false,false,false};
    //

    menu M(pantalla,e,
           (char*)"./bild/",10,(char*)"0123456789");
    bombardi_engine UN;

    int control=0;

 while(M.dime_salida_prematura()==false &&
       UN.dime_salida_prematura()==false){
    switch(control){
    case 0:
        M=menu(pantalla,e,(char*)"./bild/",10,(char*)"0123456789ngpqrt");
        M.elegir();
    case 1:
        UN=bombardi_engine(e,&pantalla,
                (char*)"./bild/",
               (char*)"logo",(char*)"credits",(char*)"pua",
               //
               9,(char*)"MBPDSTKRH",
               //
               7,(char*)"FBPSTRH",
               M.dime_n_jugadores(),M.dime_colores_elegidos(),
               //
               (char*)"cuadro",(char*)"U",
               //
               M.dime_n_jugadores(),
               //
               (char*)"qqqqq",//M.los_roles(),
               x_vector,y_vector,
               humanos);

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
