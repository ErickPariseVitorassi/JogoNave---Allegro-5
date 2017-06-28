#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include "classes.h"

ALLEGRO_BITMAP *minhaNave = NULL;

void Naves::InitNave(int X, int Y, int Vidas, int Velocidade, int Borda_x, int Borda_y, int Pontos){
    x = X;
    y = Y;
    vidas = Vidas;
    velocidade = Velocidade;
    borda_x = Borda_x;
    borda_y = Borda_y;
    pontos = Pontos;     
}

void Naves::InitInimigoLateral(int Velocidade){
    ativo = false;
    velocidade = Velocidade;
    tutorial = true;     
}
void Naves::LiberaInimigoLateral(int TELA_LARGURA, int TELA_ALTURA, int chanceLateral){ 
    if(!ativo){
        if(rand() % chanceLateral == 0){
            lateralidadeInimigo = rand() % 2;      
            if(lateralidadeInimigo == 0){      
                x = -100;
                y = (TELA_ALTURA/2-150)-(rand()%200);          
                ativo = true;
            }else{      
                x = TELA_LARGURA+100;
                y = (TELA_ALTURA/2-150)-(rand()%200);          
                ativo = true;
            }        
        }             
    }          
}
void Naves::AtualizarInimigoLateral(int TELA_LARGURA, int TELA_ALTURA, int contador){
    if(ativo){
        if(lateralidadeInimigo == 0){
            if(contador%4==0)                         
                y += velocidade;    
            else
                x += velocidade;      
            if(x > TELA_LARGURA+70)
                ativo = false;
            if(x > TELA_LARGURA/2 && tutorial)    
                tutorial = false;        
        }else{      
            if(contador%4==0)                         
                y += velocidade;    
            else
                x -= velocidade;       
            if(x < -70)
                ativo = false;
            if(x < TELA_LARGURA/2 && tutorial)    
                tutorial = false;
        }             
    } 
}

void Naves::MoveNaveCima(){
    y -= velocidade;
    if(y < 40)
        y = 40;     
}
void Naves::MoveNaveBaixo(int TELA_LARGURA, int TELA_ALTURA){
    y += velocidade;
    if(y > TELA_ALTURA-40)
        y = TELA_ALTURA-40;   
}
void Naves::MoveNaveDireita(int TELA_LARGURA, int TELA_ALTURA){
    x += velocidade;
    if(x > TELA_LARGURA-30)
        x = TELA_LARGURA-30;
}
void Naves::MoveNaveEsquerda(){
    x -= velocidade;
    if(x < 30)
        x = 30;  
}        
void Naves::DesenhaNave(char imagemNave[]){
    minhaNave = al_load_bitmap(imagemNave);
    al_convert_mask_to_alpha(minhaNave, al_map_rgb(255,0,255));
    al_draw_bitmap(minhaNave, x-30, y-40, 0);
    al_destroy_bitmap(minhaNave);    
}

void Naves::DesenhaLife(char imagemNave[]){
    minhaNave = al_load_bitmap(imagemNave);
    al_convert_mask_to_alpha(minhaNave, al_map_rgb(255,0,255));
    al_draw_bitmap(minhaNave, x-25, y-25, 0);
    al_destroy_bitmap(minhaNave);    
}

int Naves::getValX(){
    return x;
}
int Naves::getValY(){
    return y;
}



           
