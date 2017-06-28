#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <cstdlib>
#include <ctime>
#include "classes.h"

int m;

void Particulas::InitEstrelas(int TELA_LARGURA, int TELA_ALTURA, int Velocidade){
    for(m=0;m<100;m++){
        x[m]= 5 + rand()%(TELA_LARGURA - 10);
        y[m]=rand()%TELA_ALTURA; 
        velocidade[m]=Velocidade;                 
    }
}

void Particulas::AtualizarEstrelas(int TELA_ALTURA){
    for(m=0;m<100;m++){
        y[m] += velocidade[m];
        if(y[m] > TELA_ALTURA)
            y[m] = 0;                   
    } 
}

void Particulas::DesenhaEstrelas(){
    for(m=0;m<100;m++){
        al_draw_pixel(x[m], y[m], al_map_rgb(204,204,230));                   
    } 
}








