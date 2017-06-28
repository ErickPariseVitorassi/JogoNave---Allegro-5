#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <cstdlib>
#include <ctime>
#include "classes.h"

//-----Variáveis Globais-----
const int TELA_LARGURA = 1366;
const int TELA_ALTURA = 768;
const int FPS = 60;
char imagemJogador[] = {"nave_sprites/nave.bmp"};
char imagemBalas[] = {"nave_sprites/projetil1.bmp"};
char imagemMeteoro1[] = {"inimigos_sprites/meteoro1.bmp"};
char imagemMeteoro2[] = {"inimigos_sprites/meteoro2.bmp"};
char imagemMeteoro3[] = {"inimigos_sprites/meteoro1.bmp"};
char imagemMeteoro4[] = {"inimigos_sprites/meteoro1.bmp"};
char imagemMeteoro5[] = {"inimigos_sprites/meteoro1.bmp"};
char imagemInimigo[] = {"inimigos_sprites/inimigo1.bmp"};
char imagemInimigoBalas[] = {"nave_sprites/projetil2.bmp"};
char imagemInimigoLateralEsquerda[] = {"nave_sprites/naveInimigoLateral1.bmp"};
char imagemInimigoLateralDireita[] = {"nave_sprites/naveInimigoLateral2.bmp"};
char imagemLife[] = {"nave_sprites/life.bmp"};

int contador=1, k=0, l=0, chance=1450, chanceLateral=600, hpM=4, hpI=2, pontosMeteoro=8, pontosInimigo=6, nivel=1;

ALLEGRO_SAMPLE *trilhaSonora = NULL;
/*AllEGRO_SAMPLE *tiro = NULL;
ALLEGRO_SAMPLE *explosaoMeteoro = NULL;
ALLEGRO_SAMPLE *explosaoNave = NULL;
ALLEGRO_SAMPLE *gameOver = NULL;*/

ALLEGRO_SAMPLE_INSTANCE *instTrilhaSonora = NULL;
/*AllEGRO_SAMPLE_INSTANCE *instTiro = NULL;
ALLEGRO_SAMPLE_INSTANCE *instExplosaoMeteoro = NULL;
ALLEGRO_SAMPLE_INSTANCE *instExplosaoNave = NULL;
ALLEGRO_SAMPLE_INSTANCE *instGameOver = NULL;*/

enum TECLAS{CIMA, BAIXO, ESQUERDA, DIREITA, R, ENTER};

//-----Prototipos-----
void colisaoBalaMeteoro();
void colisaoNaveMeteoro();
void colisaoBalaInimigo();
void colisaoNaveInimigo();
void colisaoNaveInimigoBala();
void colisaoNaveInimigoLateralBala();
void colisaoNaveLife();

//-----Inicialização de Objetos-----  
Naves Jogador;
Naves InimigoLateral;
Naves Life;
Elementos ProjeteisInimigoLateral;
Elementos InimigoMeteoro;
Elementos ProjeteisJogador; 
Elementos Inimigo;
Elementos ProjeteisInimigo;
Particulas Plano1;
Particulas Plano2;
Particulas Plano3;

//________________________________v MAIN v______________________________________
 
int main()
{
    //-----Variáveis do Jogo-----
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_EVENT ev;
    ALLEGRO_BITMAP *fundo = NULL;
    ALLEGRO_BITMAP *logo = NULL;
    ALLEGRO_BITMAP *fundoInicio = NULL;
    ALLEGRO_FONT *font30 = NULL;
    ALLEGRO_FONT *font20 = NULL;
    ALLEGRO_FONT *font60 = NULL;
    bool desenha = true;
    bool teclas[] = {false, false, false, false, false, false}; 
    bool fim = false;
    bool game_over = false;
    bool jogoIniciado = false;
    bool tocar_game_over = false; 
    
    //-----Inicialização do Allegro-----
    if(!al_init()){
        fprintf(stderr, "Falha ao inicializar Allegro!\n");
        return -1;          
    }
    if (!al_init_primitives_addon()){
        fprintf(stderr, "Falha ao inicializar complemento Primitives!\n");
        return -1;
    }
    if (!al_install_keyboard()){
        fprintf(stderr, "Falha ao inicializar teclado!\n");
        return -1;
    }
    
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    
    al_install_audio();
    al_init_acodec_addon();
    
    al_reserve_samples(10);

    //-----Criação da Tela-----
    
    al_set_new_display_flags(ALLEGRO_FULLSCREEN);
    display = al_create_display(TELA_LARGURA, TELA_ALTURA);
    al_hide_mouse_cursor(display);
    if(!display){
        fprintf(stderr, "Falha ao criar display!\n");   
        return -1;      
    }
    
    //-----Nome do Jogo-----
    al_set_window_title(display, "JogoNave");        
    
    //-----indicação do bitmap alvo-----
    al_set_target_bitmap(al_get_backbuffer(display));
    
    //-----criação de lista de eventos-----
    fila_eventos = al_create_event_queue();
    if (!fila_eventos){
        fprintf(stderr, "Falha ao criar listaEventos!\n");
        al_destroy_display(display);
        return -1;
    }
    
    timer = al_create_timer(1.0 / FPS);
    font30 = al_load_font("fonte/gbb__.ttf", 30, NULL);
    font20 = al_load_font("fonte/gbb__.ttf", 20, NULL);
    font60 = al_load_font("fonte/gbb__.ttf", 60, NULL);
    
    al_clear_to_color(al_map_rgb(0,0,0));
    al_draw_textf(font30, al_map_rgb(255, 255, 255), TELA_LARGURA/2, TELA_ALTURA/2, ALLEGRO_ALIGN_CENTRE, "STARTING...");
    al_flip_display();
    
    trilhaSonora = al_load_sample("trilha_sonora.ogg");
    instTrilhaSonora = al_create_sample_instance(trilhaSonora);
    al_attach_sample_instance_to_mixer(instTrilhaSonora, al_get_default_mixer());
    al_set_sample_instance_playmode(instTrilhaSonora, ALLEGRO_PLAYMODE_LOOP);
    al_set_sample_instance_gain(instTrilhaSonora, 0.8);
     
    //-----Registro de Sources----- 
    al_register_event_source(fila_eventos, al_get_display_event_source(display));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    
    //-----Funções Iniciais-----
    Jogador.InitNave(TELA_LARGURA/2, TELA_ALTURA-150, 20, 10, 20, 25, 0);
    InimigoLateral.InitInimigoLateral(1);
    ProjeteisInimigoLateral.InitElementos(5, 0, 15);
    Life.InitInimigoLateral(2);
    
    ProjeteisJogador.InitElementos(0, 0, 25); 
    InimigoMeteoro.InitElementos(25, 25, 4);
    Inimigo.InitElementos(25, 35, 7);
    ProjeteisInimigo.InitElementos(5, 0, 12);
    
    Plano1.InitEstrelas(TELA_LARGURA, TELA_ALTURA, 6);
    Plano2.InitEstrelas(TELA_LARGURA, TELA_ALTURA, 2);
    Plano3.InitEstrelas(TELA_LARGURA, TELA_ALTURA, 1);
    
    logo = al_load_bitmap("fundo/logoAfterSky.bmp");
    fundo = al_load_bitmap("fundo/fundoEspacial.bmp");
    fundoInicio = al_load_bitmap("fundo/inicioFundoEspacial.bmp");

    //____________________________LAÇO PRINCIPAL________________________________
    al_start_timer(timer);
        
    while(!fim){ 
        al_wait_for_event(fila_eventos, &ev);
        
        //-----Eventos e Lógica do Jogo-----
        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            fim = true;
        }else if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
            switch(ev.keyboard.keycode){
                case ALLEGRO_KEY_ESCAPE:
                    fim = true;
                    break;
                case ALLEGRO_KEY_UP:
                    teclas[CIMA] = true;
                    break;
                case ALLEGRO_KEY_DOWN:
                    teclas[BAIXO] = true;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    teclas[DIREITA] = true;
                    break; 
                case ALLEGRO_KEY_LEFT:
                    teclas[ESQUERDA] = true;
                    break;
                case ALLEGRO_KEY_R: 
                    teclas[R] = true;
                    break;
                case ALLEGRO_KEY_ENTER:
                    teclas[ENTER] = true;
                    break;                                                     
            }  
        }else if(ev.type == ALLEGRO_EVENT_KEY_UP){
            switch(ev.keyboard.keycode){
                case ALLEGRO_KEY_UP:
                    teclas[CIMA] = false;
                    break;
                case ALLEGRO_KEY_DOWN:
                    teclas[BAIXO] = false;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    teclas[DIREITA] = false;
                    break; 
                case ALLEGRO_KEY_LEFT:
                    teclas[ESQUERDA] = false;
                    break;
                case ALLEGRO_KEY_R:
                    teclas[R] = false;
                    break;
                case ALLEGRO_KEY_ENTER:
                    teclas[ENTER] = false;
                    break;
                    
            }        
        }else if(ev.type == ALLEGRO_EVENT_TIMER){
            desenha = true;
            
            if(teclas[CIMA])
                Jogador.MoveNaveCima();
            if(teclas[BAIXO])
                Jogador.MoveNaveBaixo(TELA_LARGURA, TELA_ALTURA);
            if(teclas[DIREITA])
                Jogador.MoveNaveDireita(TELA_LARGURA, TELA_ALTURA);
            if(teclas[ESQUERDA])
                Jogador.MoveNaveEsquerda();
            if(teclas[R]){}
            if(teclas[ENTER])
                jogoIniciado = true;                  
            
            if(jogoIniciado){
                if(!game_over){
                               
                    Plano1.AtualizarEstrelas(TELA_ALTURA);
                    Plano2.AtualizarEstrelas(TELA_ALTURA);
                    Plano3.AtualizarEstrelas(TELA_ALTURA);
                  
                    if(contador % 1800 == 0 && chance > 550){
                        chance -= 100; //aumenta a dificuldade do jogo 
                        chanceLateral -= 50; //aumenta a chance do inimigo lateral aparecer
                        hpM += 1; //aumenta a vida do meteoro 
                        pontosMeteoro+=3;
                        pontosInimigo+=2;
                        nivel++;
                        if(contador % 5400 == 0)
                            hpI += 1; //aumenta a vida da nave inimiga
                        if(contador % 3600 == 0)
                            Life.LiberaInimigoLateral(TELA_LARGURA, TELA_ALTURA, 1);      
                    }
             
                    InimigoMeteoro.LiberaMeteoros(TELA_LARGURA, hpM, chance);
                    Inimigo.LiberaNaves(TELA_LARGURA, hpI, chance);
                    InimigoLateral.LiberaInimigoLateral(TELA_LARGURA, TELA_ALTURA, chanceLateral);
            
                    InimigoMeteoro.AtualizarInimigos(TELA_ALTURA, 21);
                    Inimigo.AtualizarInimigos(TELA_ALTURA, 21);
                    InimigoLateral.AtualizarInimigoLateral(TELA_LARGURA, TELA_ALTURA, contador);
                    Life.AtualizarInimigoLateral(TELA_LARGURA, TELA_ALTURA, contador);
                
                    ProjeteisInimigoLateral.AtualizarBalasInimigoLateral(10, TELA_ALTURA);
                    ProjeteisInimigoLateral.AtiraBalasInimigoLateral(contador, InimigoLateral.getValX(), InimigoLateral.getValY(), Jogador.getValY(), InimigoLateral.ativo, InimigoLateral.lateralidadeInimigo);
            
                    ProjeteisInimigo.AtualizarBalasInimigos(TELA_ALTURA, 21);
                    ProjeteisInimigo.AtiraBalasInimigos(Inimigo.x, Inimigo.y, Inimigo.ativo);
              
                    ProjeteisJogador.AtualizarBalas(10);     
                    ProjeteisJogador.AtiraBalas(contador, Jogador.getValX(), Jogador.getValY());
            
                    colisaoBalaMeteoro();
                    colisaoNaveMeteoro();
                    colisaoBalaInimigo();
                    colisaoNaveInimigo();
                    colisaoNaveInimigoBala();
                    colisaoNaveInimigoLateralBala();
                    colisaoNaveLife();
                    
                    if(Jogador.vidas<=0)
                        game_over = true;
                    if(teclas[R]){
                        jogoIniciado = false;
                        al_stop_sample_instance(instTrilhaSonora);
                        al_play_sample_instance(instTrilhaSonora);              
                                      
                        Jogador.InitNave(TELA_LARGURA/2, TELA_ALTURA-150, 20, 10, 20, 25, 0);
                        InimigoLateral.InitInimigoLateral(1);
                        ProjeteisInimigoLateral.InitElementos(5, 0, 15);
                        Life.InitInimigoLateral(2);    
                        ProjeteisJogador.InitElementos(0, 0, 25); 
                        InimigoMeteoro.InitElementos(25, 25, 4);
                        Inimigo.InitElementos(25, 35, 7);
                        ProjeteisInimigo.InitElementos(5, 0, 12);    
                        Plano1.InitEstrelas(TELA_LARGURA, TELA_ALTURA, 6);
                        Plano2.InitEstrelas(TELA_LARGURA, TELA_ALTURA, 2);
                        Plano3.InitEstrelas(TELA_LARGURA, TELA_ALTURA, 1);
                        contador = 1;
                        chance = 1450; 
                        chanceLateral = 600;
                        hpM = 4;
                        hpI = 2;
                        pontosMeteoro = 8;
                        pontosInimigo = 6;
                        nivel = 1;
                    }        
                }
                contador++;
            }  
        }  
        //-----Desenhos-----
        if(desenha && al_is_event_queue_empty(fila_eventos)){
            desenha = false;  
            
            if(jogoIniciado){
                if(!game_over){
                    Plano1.DesenhaEstrelas(); 
                    if(InimigoLateral.lateralidadeInimigo == 0 && InimigoLateral.ativo){ 
                        InimigoLateral.DesenhaNave(imagemInimigoLateralEsquerda);                                                      
                    }else if(InimigoLateral.ativo){
                        InimigoLateral.DesenhaNave(imagemInimigoLateralDireita);  
                    }
                    Plano2.DesenhaEstrelas();
                    Plano3.DesenhaEstrelas();    
             
                    ProjeteisInimigoLateral.DesenhaElementos(imagemInimigoBalas, 1, 5);
                    InimigoMeteoro.DesenhaMeteorosNaves(imagemMeteoro1, imagemMeteoro2, imagemMeteoro3, imagemMeteoro4, imagemMeteoro5, 25, 25);
                    
                    Inimigo.DesenhaElementos(imagemInimigo, 30, 40);
                    ProjeteisJogador.DesenhaElementos(imagemBalas, 1, 5);
                    Jogador.DesenhaNave(imagemJogador);
                    ProjeteisInimigo.DesenhaElementos(imagemInimigoBalas, 1, 5);
                    
                    if(Life.ativo)
                        Life.DesenhaLife(imagemLife);
                    
                    if(InimigoLateral.ativo && InimigoLateral.tutorial && InimigoLateral.lateralidadeInimigo == 0){
                        al_draw_textf(font20, al_map_rgb(130, 130, 150), InimigoLateral.x-250, InimigoLateral.y+5, NULL, "|  Enemy  in  the  background");
                        al_draw_textf(font20, al_map_rgb(130, 130, 150), InimigoLateral.x-250, InimigoLateral.y+25, NULL, "|  Your  shots  can't  hit  him");
                        al_draw_textf(font20, al_map_rgb(130, 130, 150), InimigoLateral.x-250, InimigoLateral.y+45, NULL, "|  but  his  shots  can  hit  you");
                    }else if(InimigoLateral.ativo && InimigoLateral.tutorial){
                        al_draw_textf(font20, al_map_rgb(130, 130, 150), InimigoLateral.x+30, InimigoLateral.y+5, NULL, "|  Enemy  in  the  background");
                        al_draw_textf(font20, al_map_rgb(130, 130, 150), InimigoLateral.x+30, InimigoLateral.y+25, NULL, "|  Your  shots  can't  hit  him");
                        al_draw_textf(font20, al_map_rgb(130, 130, 150), InimigoLateral.x+30, InimigoLateral.y+45, NULL, "|  but  his  shots  can  hit  you");  
                    }   
                    
                    al_draw_textf(font20, al_map_rgb(130, 130, 150), 13, 10, NULL, "R  to  start  a  new  game");
                    al_draw_textf(font20, al_map_rgb(130, 130, 150), TELA_LARGURA-205, 10, NULL, "ESC  to  close  the  game");
                    al_draw_textf(font30, al_map_rgb(255, 255, 255), 13, TELA_ALTURA-35, NULL, "Life:  %d", Jogador.vidas);
                    if(nivel < 10)
                        al_draw_textf(font30, al_map_rgb(255, 255, 255), TELA_LARGURA/2, 5, ALLEGRO_ALIGN_CENTRE, "Level  0%d", nivel);
                    else
                        al_draw_textf(font30, al_map_rgb(255, 255, 255), TELA_LARGURA/2, 5, ALLEGRO_ALIGN_CENTRE, "Level  %d", nivel);
                    
                    if(Jogador.pontos < 10)
                        al_draw_textf(font30, al_map_rgb(255, 255, 255), TELA_LARGURA-193, TELA_ALTURA-35, NULL, "Score:  0000%d", Jogador.pontos);
                    else if(Jogador.pontos < 100)
                        al_draw_textf(font30, al_map_rgb(255, 255, 255), TELA_LARGURA-193, TELA_ALTURA-35, NULL, "Score:  000%d", Jogador.pontos);
                    else if(Jogador.pontos < 1000)   
                        al_draw_textf(font30, al_map_rgb(255, 255, 255), TELA_LARGURA-193, TELA_ALTURA-35, NULL, "Score:  00%d", Jogador.pontos);
                    else if(Jogador.pontos < 10000)
                        al_draw_textf(font30, al_map_rgb(255, 255, 255), TELA_LARGURA-193, TELA_ALTURA-35, NULL, "Score:  0%d", Jogador.pontos);
                    else 
                        al_draw_textf(font30, al_map_rgb(255, 255, 255), TELA_LARGURA-193, TELA_ALTURA-35, NULL, "Score:  %d", Jogador.pontos);
                    
                    al_flip_display();
                    al_draw_bitmap(fundo, 0, 0, 0);
                }else{
                    al_stop_sample_instance(instTrilhaSonora);  
                      
                    al_draw_textf(font60, al_map_rgb(255, 255, 255), TELA_LARGURA/2, 290, ALLEGRO_ALIGN_CENTRE, "GAME    OVER");
                    al_draw_textf(font20, al_map_rgb(255, 255, 255), 13, 10, NULL, "ENTER  to  start  a  new  game");
                    al_draw_textf(font20, al_map_rgb(255, 255, 255), TELA_LARGURA-205, 10, NULL, "ESC  to  close  the  game");
                    al_draw_textf(font30, al_map_rgb(255, 255, 255), TELA_LARGURA/2, 420, ALLEGRO_ALIGN_CENTRE, "Your Score   %d", Jogador.pontos);
                    
                    
                    if(teclas[ENTER]){
                        al_play_sample_instance(instTrilhaSonora);              
                                      
                        Jogador.InitNave(TELA_LARGURA/2, TELA_ALTURA-150, 20, 10, 20, 25, 0);
                        InimigoLateral.InitInimigoLateral(1);
                        ProjeteisInimigoLateral.InitElementos(5, 0, 15);
                        Life.InitInimigoLateral(2);    
                        ProjeteisJogador.InitElementos(0, 0, 25); 
                        InimigoMeteoro.InitElementos(25, 25, 4);
                        Inimigo.InitElementos(25, 35, 7);
                        ProjeteisInimigo.InitElementos(5, 0, 12);    
                        Plano1.InitEstrelas(TELA_LARGURA, TELA_ALTURA, 6);
                        Plano2.InitEstrelas(TELA_LARGURA, TELA_ALTURA, 2);
                        Plano3.InitEstrelas(TELA_LARGURA, TELA_ALTURA, 1);
                        contador = 1;
                        chance = 1450; 
                        chanceLateral = 600;
                        hpM = 4;
                        hpI = 2;
                        pontosMeteoro = 8;
                        pontosInimigo = 6;
                        nivel = 1;              
                        game_over = false;
                    }    
                      
                      
                    al_flip_display();
                    al_clear_to_color(al_map_rgb(0,0,0));
                }
            }else{
                al_play_sample_instance(instTrilhaSonora);
                 
                 al_convert_mask_to_alpha(logo, al_map_rgb(255,0,255));
                 al_draw_bitmap(logo, TELA_LARGURA/2-150, 180, 0);
                 
                al_draw_textf(font20, al_map_rgb(255, 255, 255), TELA_LARGURA-205, 10, NULL, "ESC  to  close  the  game");  
                al_draw_textf(font30, al_map_rgb(255, 255, 255), TELA_LARGURA/2, 680, ALLEGRO_ALIGN_CENTRE, "ENTER  to  start");
                al_flip_display();
                al_draw_bitmap(fundoInicio, 0, 0, 0);      
            }
        }
        
    }//_________________________________________________________________________   
   
    //-----Liberação de Recursos Alocados
    al_destroy_display(display);
    al_destroy_event_queue(fila_eventos);
    al_destroy_timer(timer);
    al_uninstall_keyboard();
    al_destroy_font(font30);
    al_destroy_font(font20); 
    al_destroy_font(font60);
    al_destroy_sample(trilhaSonora);
    al_destroy_sample_instance(instTrilhaSonora);     
    return 0;
}
//________________________________^ MAIN ^______________________________________

//---------------Colisões----------------
void colisaoBalaMeteoro(){
    for(k=0;k<5;k++){
        if(ProjeteisJogador.ativo[k]){
            for(l=0;l<20;l++){
                if(InimigoMeteoro.ativo[l]){
                    if(ProjeteisJogador.x[k] > (InimigoMeteoro.x[l] - InimigoMeteoro.borda_x[l]) &&
                       ProjeteisJogador.x[k] < (InimigoMeteoro.x[l] + InimigoMeteoro.borda_x[l]) &&
                       ProjeteisJogador.y[k] > (InimigoMeteoro.y[l] - InimigoMeteoro.borda_y[l]) &&
                       ProjeteisJogador.y[k] < (InimigoMeteoro.y[l] + InimigoMeteoro.borda_y[l])){
                        ProjeteisJogador.AtualizarBalas(k);
                        InimigoMeteoro.hp[l]=InimigoMeteoro.hp[l]-1;
                        if(InimigoMeteoro.hp[l]<=0){
                            InimigoMeteoro.AtualizarInimigos(TELA_ALTURA, l);
                            Jogador.pontos += pontosMeteoro;
                        }    
                    }     
                }             
            }                              
        }                 
    } 
}
void colisaoNaveMeteoro(){
    for(l=0;l<20;l++){
        if(InimigoMeteoro.ativo[l]){
            if((Jogador.x + Jogador.borda_x) > (InimigoMeteoro.x[l] - InimigoMeteoro.borda_x[l]) &&
               (Jogador.x - Jogador.borda_x) < (InimigoMeteoro.x[l] + InimigoMeteoro.borda_x[l]) &&
               (Jogador.y + Jogador.borda_y) > (InimigoMeteoro.y[l] - InimigoMeteoro.borda_y[l]) &&
               (Jogador.y - Jogador.borda_y) < (InimigoMeteoro.y[l] + InimigoMeteoro.borda_y[l])){
                InimigoMeteoro.AtualizarInimigos(TELA_ALTURA, l);
                Jogador.vidas-=2;
            }     
        }             
    }                    
}

void colisaoBalaInimigo(){
    for(k=0;k<5;k++){
        if(ProjeteisJogador.ativo[k]){
            for(l=0;l<20;l++){
                if(Inimigo.ativo[l]){
                    if(ProjeteisJogador.x[k] > (Inimigo.x[l] - Inimigo.borda_x[l]) &&
                       ProjeteisJogador.x[k] < (Inimigo.x[l] + Inimigo.borda_x[l]) &&
                       ProjeteisJogador.y[k] > (Inimigo.y[l] - Inimigo.borda_y[l]) &&
                       ProjeteisJogador.y[k] < (Inimigo.y[l] + Inimigo.borda_y[l])){
                        ProjeteisJogador.AtualizarBalas(k);
                        Inimigo.hp[l]=Inimigo.hp[l]-1;
                        if(Inimigo.hp[l]==0){
                            Inimigo.AtualizarInimigos(TELA_ALTURA, l);
                            Jogador.pontos += pontosInimigo;
                        }    
                    }     
                }             
            }                              
        }                 
    } 
}
void colisaoNaveInimigo(){
    for(l=0;l<20;l++){
        if(Inimigo.ativo[l]){
            if((Jogador.x + Jogador.borda_x) > (Inimigo.x[l] - Inimigo.borda_x[l]) &&
               (Jogador.x - Jogador.borda_x) < (Inimigo.x[l] + Inimigo.borda_x[l]) &&
               (Jogador.y + Jogador.borda_y) > (Inimigo.y[l] - Inimigo.borda_y[l]) &&
               (Jogador.y - Jogador.borda_y) < (Inimigo.y[l] + Inimigo.borda_y[l])){
                Inimigo.AtualizarInimigos(TELA_ALTURA, l);
                Jogador.vidas-=2;
            }     
        }             
    }                    
}
void colisaoNaveInimigoBala(){
    for(l=0;l<20;l++){
        if(ProjeteisInimigo.ativo[l]){
            if((Jogador.x + Jogador.borda_x) > (ProjeteisInimigo.x[l] - ProjeteisInimigo.borda_x[l]) &&
               (Jogador.x - Jogador.borda_x) < (ProjeteisInimigo.x[l] + ProjeteisInimigo.borda_x[l]) &&
               (Jogador.y + Jogador.borda_y) > (ProjeteisInimigo.y[l] - ProjeteisInimigo.borda_y[l]) &&
               (Jogador.y - Jogador.borda_y) < (ProjeteisInimigo.y[l] + ProjeteisInimigo.borda_y[l])){
                ProjeteisInimigo.AtualizarBalasInimigos(TELA_ALTURA, l);
                Jogador.vidas--;
            }     
        }             
    }                    
}
void colisaoNaveInimigoLateralBala(){
    for(l=0;l<5;l++){
        if(ProjeteisInimigoLateral.ativo[l]){
            if((Jogador.x + Jogador.borda_x) > (ProjeteisInimigoLateral.x[l] - ProjeteisInimigoLateral.borda_x[l]) &&
               (Jogador.x - Jogador.borda_x) < (ProjeteisInimigoLateral.x[l] + ProjeteisInimigoLateral.borda_x[l]) &&
               (Jogador.y + Jogador.borda_y) > (ProjeteisInimigoLateral.y[l] - ProjeteisInimigoLateral.borda_y[l]) &&
               (Jogador.y - Jogador.borda_y) < (ProjeteisInimigoLateral.y[l] + ProjeteisInimigoLateral.borda_y[l])){
                ProjeteisInimigoLateral.AtualizarBalasInimigoLateral(l, TELA_ALTURA);
                Jogador.vidas--;
            }     
        }             
    }                    
}
void colisaoNaveLife(){
    if(Life.ativo){
        if((Jogador.x + Jogador.borda_x) > (Life.x - 25) &&
           (Jogador.x - Jogador.borda_x) < (Life.x + 25) &&
           (Jogador.y + Jogador.borda_y) > (Life.y - 25) &&
           (Jogador.y - Jogador.borda_y) < (Life.y + 25)){
            Life.ativo = false;
            Jogador.vidas=20;
        }              
    }                    
}











