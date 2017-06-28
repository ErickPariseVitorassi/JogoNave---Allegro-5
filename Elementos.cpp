#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include "classes.h"

ALLEGRO_BITMAP *projetilNave = NULL;
ALLEGRO_TIMER *timer = NULL;
int a, b, i, j, cordenadaRand;

void Elementos::InitElementos(int Borda_x, int Borda_y, int Velocidade){
    for(i=0; i<20; i++){
        velocidade[i] = Velocidade;
        borda_x[i] = Borda_x;
        borda_y[i] = Borda_y;
        ativo[i] = false;             
    }
}
void Elementos::AtualizarBalas(int desativar){
    for(b=0; b<5; b++){
        if(ativo[b]){
            y[b] -= velocidade[b];
            if(desativar>-1 && desativar<6)
                ativo[desativar] = false; 
            if(y[b] < 0)
                ativo[b] = false;    
        }        
    } 
}
void Elementos::AtiraBalas(int contador, int jogadorX, int jogadorY){
    for(b=0; b<5; b++){
        if(contador % 6 == 0){ 
            if(!ativo[b]){
            y[b] = jogadorY - 40;
            x[b] = jogadorX;
            ativo[b] = true; 
            break;             
            } 
        }        
    } 
}

void Elementos::AtualizarBalasInimigoLateral(int desativar, int TELA_ALTURA){
    for(b=0; b<5; b++){
        if(ativo[b]){
            if(lado[b])         
                y[b] -= velocidade[b];
            else
                y[b] += velocidade[b];    
            if(desativar>-1 && desativar<6)
                ativo[desativar] = false; 
            if(y[b] < 0 || y[b] > TELA_ALTURA)
                ativo[b] = false;    
        }        
    } 
}
void Elementos::AtiraBalasInimigoLateral(int contador, int inimigoX, int inimigoY, int jogadorY, bool inimigoAtivo, int lateralidadeInimigo){
    for(b=0; b<5; b++){
        if(contador % 20 == 0){         
            if(!ativo[b] && inimigoAtivo){ 
                if(inimigoY > jogadorY)
                    lado[b] = true;
                else
                    lado[b] = false;
                if(lateralidadeInimigo == 0){                             
                    y[b] = inimigoY-13;
                    x[b] = inimigoX+12;
                }else{
                    y[b] = inimigoY-13;
                    x[b] = inimigoX-13;
                }
                ativo[b] = true;
                break;             
            }
        }           
    } 
}

void Elementos::AtualizarBalasInimigos(int TELA_ALTURA, int desativar){
    for(i=0; i<20; i++){
        if(ativo[i]){
            y[i] += velocidade[i];
            if(desativar>-1 && desativar<20)
                ativo[desativar] = false; 
            if(y[i] > TELA_ALTURA)
                ativo[i] = false;    
        }        
    } 
}

void Elementos::AtiraBalasInimigos(int inimigoX[], int inimigoY[], bool inimigoAtivo[]){
    for(i=0; i<20; i++){
        if(inimigoAtivo[i]){
        for(j=0;j<20;j++){     
            if(rand() % 3000 == 0){ 
                if(!ativo[j]){
                y[j] = inimigoY[i] + 40;
                x[j] = inimigoX[i];
                ativo[j] = true; 
                break;             
                } 
            }
        }
        }            
    } 
}

void Elementos::AtualizarInimigos(int TELA_ALTURA, int desativar){
    for(i=0; i<20; i++){
        if(ativo[i]){
            y[i] += velocidade[i];
            if(desativar>-1 && desativar<20)
                ativo[desativar] = false;  
            if(y[i] > TELA_ALTURA+30)
                ativo[i] = false;    
        }        
    } 
}
void Elementos::LiberaMeteoros(int TELA_LARGURA, int HP, int chance){
    for(i=0; i<20; i++){ 
        if(!ativo[i]){
            if(rand() % chance == 0){
                x[i] = 35 + rand() %(TELA_LARGURA - 70);
                y[i] = -30;          
                ativo[i] = true;
                hp[i]=HP;
                escolhaElemento[i] = rand()%2;
                for(j=0;j<20;j++){
                    if(ativo[j]){
                        if(y[j] < (y[i]+100) && (x[j] < (x[i]+100) && x[j] > (x[i]-100)))
                            y[i]-=200;
                    }                 
                }
                break;
            }             
        }          
    } 
}
void Elementos::LiberaNaves(int TELA_LARGURA, int HP, int chance){
    for(i=0; i<20; i++){ 
        if(!ativo[i]){
            if(rand() % chance == 0){
                cordenadaRand = rand()%19;
                switch(cordenadaRand){
                    case 0:
                        x[i] = 50;
                        break;
                    case 1:
                        x[i] = 120;
                        break;
                    case 2:
                        x[i] = 190;
                        break;
                    case 3:
                        x[i] = 260;
                        break;
                    case 4:
                        x[i] = 330;
                        break;
                    case 5:
                        x[i] = 400;
                        break;
                    case 6:
                        x[i] = 470;
                        break;
                    case 7:
                        x[i] = 540;
                        break;
                    case 8:
                        x[i] = 610;
                        break;
                    case 9:
                        x[i] = 680;
                        break;
                    case 10:
                        x[i] = 750;
                        break;
                    case 11:
                        x[i] = 820;
                        break;
                    case 12:
                        x[i] = 890;
                        break;
                    case 13:
                        x[i] = 960;
                        break;
                    case 14:
                        x[i] = 1030;
                        break;
                    case 15:
                        x[i] = 1100;
                        break;
                    case 16:
                        x[i] = 1170;
                        break;
                    case 17:
                        x[i] = 1240;
                        break;
                    case 18:
                        x[i] = 1310;
                        break;                                          
                }      
                y[i] = -40;          
                ativo[i] = true;
                hp[i]=HP;
                escolhaElemento[i] = rand()%2;
                for(j=0;j<20;j++){
                    if(ativo[j]){
                        if(y[j] < (y[i]+200 )&& x[j] == x[i])
                            y[i]-=200;
                    }                 
                }
                break;
            }             
        }          
    } 
}

void Elementos::DesenhaElementos(char imagemNave[], int bordaX, int bordaY){
    for(i=0; i<20; i++){
        if(ativo[i]){
            projetilNave = al_load_bitmap(imagemNave);
            al_convert_mask_to_alpha(projetilNave, al_map_rgb(255,0,255));
            al_draw_bitmap(projetilNave, x[i]-bordaX, y[i]-bordaY, 0);
            al_destroy_bitmap(projetilNave);
        }       
    } 
}

void Elementos::DesenhaMeteorosNaves(char img1[], char img2[], char img3[], char img4[], char img5[], int bordaX, int bordaY){
    for(i=0; i<20; i++){
        if(ativo[i]){
            if(escolhaElemento[i]==0){         
                projetilNave = al_load_bitmap(img1);
                al_convert_mask_to_alpha(projetilNave, al_map_rgb(255,0,255));
                al_draw_bitmap(projetilNave, x[i]-bordaX, y[i]-bordaY, 0);
                al_destroy_bitmap(projetilNave);
            }else{
                projetilNave = al_load_bitmap(img2);
                al_convert_mask_to_alpha(projetilNave, al_map_rgb(255,0,255));
                al_draw_bitmap(projetilNave, x[i]-bordaX, y[i]-bordaY, 0);
                al_destroy_bitmap(projetilNave);  
            }    
        }       
    } 
}



