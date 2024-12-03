#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_ttf.h>

#define FRAME_WIDTH 44   
#define FRAME_HEIGHT 48  
#define NUM_FRAMES 4     
#define FRAME_DURATION 0.2 
#define JUMP_HEIGHT 80  
#define GRAVITY 4     

#define JOGO_FASE 1
#define JOGO_HISTORIA 2
#define CENARIO_PRINCIPAL 3

int fase_atual = 0;
bool menu_aberto = false;
bool jogo_ativo = true;
bool perdeu_fase2 = true;

bool sair = false;




int main() {
    al_init();
    al_init_primitives_addon();
    al_install_keyboard();
    al_init_image_addon();
    al_init_font_addon();
    al_install_mouse();


    al_init_ttf_addon();


    ALLEGRO_FONT* fonte = al_create_builtin_font();

    ALLEGRO_DISPLAY* disp = al_create_display(800, 600);
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE* fila_eventos = al_create_event_queue();
    al_set_window_title(disp, "Tale of Second War");

    bool sair = false;
    bool on_homepage = true;
    float x = 300;
    float y = 435;
    float frame = 0;

    bool pulando = false;
    float jump_speed = 0;
    float jump_start_y = y;

    bool andando_esquerda = false;
    bool andando_direita = false;
    bool parado = true;

    int botaox = 280;
    int largura = 225;
    int botaoy = 360;
    int altura = 115;

    ALLEGRO_BITMAP* casa = al_load_bitmap("F:\\repos\\Imagens\\telas\\casa.png");
    ALLEGRO_BITMAP* Personagem = al_load_bitmap("F:\\repos\\Imagens\\Personagem\\Walking.png");
    ALLEGRO_BITMAP* Pulando = al_load_bitmap("F:\\repos\\Imagens\\Personagem\\Pulando.png");
    ALLEGRO_BITMAP* Parado = al_load_bitmap("F:\\repos\\Imagens\\Personagem\\PGParado.png");
    ALLEGRO_BITMAP* AndandoESQ = al_load_bitmap("F:\\repos\\Imagens\\Personagem\\WalkingLeft.png");
    ALLEGRO_BITMAP* Homepage = al_load_bitmap("F:\\repos\\Imagens\\telas\\bg2.jpg");
    ALLEGRO_BITMAP* cenario1 = al_load_bitmap("F:\\repos\\Imagens\\telas\\cenario1.png");
    ALLEGRO_BITMAP* cenario2 = al_load_bitmap("F:\\repos\\Imagens\\telas\\cenario2.png");
    ALLEGRO_BITMAP* cenario3 = al_load_bitmap("F:\\repos\\Imagens\\telas\\cenario3.png");
    ALLEGRO_BITMAP* mapa = al_load_bitmap("F:\\repos\\Imagens\\telas\\mapa.png");

    ALLEGRO_BITMAP* introducao1 = al_load_bitmap("F:\\repos\\Imagens\\introducao\\introducao.png");
    ALLEGRO_BITMAP* introducao2 = al_load_bitmap("F:\\repos\\Imagens\\introducao\\introducao2.png");
    ALLEGRO_BITMAP* introducao3 = al_load_bitmap("F:\\repos\\Imagens\\introducao\\introducao3.png");
    ALLEGRO_BITMAP* introducao4 = al_load_bitmap("F:\\repos\\Imagens\\introducao\\introducao4.png");
    ALLEGRO_BITMAP* introducao5 = al_load_bitmap("F:\\repos\\Imagens\\introducao\\introducao5.png");
    ALLEGRO_BITMAP* introducao6 = al_load_bitmap("F:\\repos\\Imagens\\introducao\\introducao6.png");

    ALLEGRO_BITMAP* historia1 = al_load_bitmap("F:\\repos\\Imagens\\historia do jogo\\jp1.png");
    ALLEGRO_BITMAP* historia2 = al_load_bitmap("F:\\repos\\Imagens\\historia do jogo\\jp2.png");
    ALLEGRO_BITMAP* historia3 = al_load_bitmap("F:\\repos\\Imagens\\historia do jogo\\jp3.png");
    ALLEGRO_BITMAP* historia4 = al_load_bitmap("F:\\repos\\Imagens\\historia do jogo\\jp4.png");
    ALLEGRO_BITMAP* historia5 = al_load_bitmap("F:\\repos\\Imagens\\historia do jogo\\jp5.png");
    ALLEGRO_BITMAP* historia6 = al_load_bitmap("F:\\repos\\Imagens\\historia do jogo\\jp6.png");
    ALLEGRO_BITMAP* historia7 = al_load_bitmap("F:\\repos\\Imagens\\historia do jogo\\jp7.png");

    ALLEGRO_BITMAP* next = al_load_bitmap("F:\\repos\\Imagens\\telas\\NEXT.png");
    ALLEGRO_BITMAP* gameover = al_load_bitmap("F:\\repos\\Imagens\\telas\\creditosFINAIS.png");


    ALLEGRO_BITMAP* transicao1 = al_load_bitmap("F:\\repos\\Imagens\\telas\\introFASE1.png");
    ALLEGRO_BITMAP* transicao2 = al_load_bitmap("F:\\repos\\Imagens\\telas\\introFASE2.png");
    ALLEGRO_BITMAP* transicao3 = al_load_bitmap("F:\\repos\\Imagens\\telas\\introFASE3.png");
    ALLEGRO_FONT* font = al_create_builtin_font();
    

    al_register_event_source(fila_eventos, al_get_display_event_source(disp));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_mouse_event_source());


    float maquina_x = 345;
    float maquina_y = 475;
    float maquina_raio = 50;
    bool mapa_visivel = false;

    float fase1_x = 255; // Posição x da fase 1
    float fase1_y = 200; // Posição y da fase 1
    float fase1_largura = 120; // Largura da fase 1
    float fase1_altura = 30; // Altura da fase 1

    float fase10_x = 600;
    float fase10_y = 510;
    float fase10_largura = 50;
    float fase10_altura = 30;

    float fase2_x = 397; // Posição x da fase 2
    float fase2_y = 253; // Posição y da fase 2
    float fase2_largura = 120; // Largura da fase 2
    float fase2_altura = 30; // Altura da fase 2

    float fase3_x = 295; // Posição x da fase 3
    float fase3_y = 300; // Posição y da fase 3
    float fase3_largura = 120; // Largura da fase 3
    float fase3_altura = 30; // Altura da fase 3
    bool perto_da_maquina = false;
    bool introducao = false;
    int introducao_etapa = 0;
    bool historia = false;
    int historia_etapa = 0;
    bool movimentacao = true;
    float mouselocal_x = 670;
    float mouselocal_y = 540;

    float mouselocal_largura = 120;
    float mouselocal_altura = 30;
    float resultado_fase = 0;

    bool transicao = false;
    int transicao_fases = 0;
    bool terminou = false;


    bool livro1 = false;
    bool livro2 = false;
    bool livro3 = false;
    double start_time = al_get_time();
    int creditos = 0;

    int livros = 0;
    char texto[50];
    al_start_timer(timer);

   

    while (!sair) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);
       

        if (evento.type == ALLEGRO_EVENT_TIMER) {

            if (on_homepage) {

                al_draw_scaled_bitmap(Homepage, 0, 0, al_get_bitmap_width(Homepage), al_get_bitmap_height(Homepage), 0, 0, 800, 600, 0);
                al_flip_display();
            }


            else if (introducao) {

                if (introducao_etapa == 0) {
                    al_draw_scaled_bitmap(introducao1, 0, 0, al_get_bitmap_width(introducao1), al_get_bitmap_height(introducao1), 0, 0, 800, 600, 0);
                }
                else if (introducao_etapa == 1) {
                    al_draw_scaled_bitmap(introducao2, 0, 0, al_get_bitmap_width(introducao2), al_get_bitmap_height(introducao2), 0, 0, 800, 600, 0);
                }
                else if (introducao_etapa == 2) {
                    al_draw_scaled_bitmap(introducao3, 0, 0, al_get_bitmap_width(introducao3), al_get_bitmap_height(introducao3), 0, 0, 800, 600, 0);
                }
                else if (introducao_etapa == 3) {
                    al_draw_scaled_bitmap(introducao4, 0, 0, al_get_bitmap_width(introducao4), al_get_bitmap_height(introducao4), 0, 0, 800, 600, 0);
                }
                else if (introducao_etapa == 4) {
                    al_draw_scaled_bitmap(introducao5, 0, 0, al_get_bitmap_width(introducao5), al_get_bitmap_height(introducao5), 0, 0, 800, 600, 0);
                }
                else if (introducao_etapa == 5) {
                    al_draw_scaled_bitmap(introducao6, 0, 0, al_get_bitmap_width(introducao6), al_get_bitmap_height(introducao6), 0, 0, 800, 600, 0);
                }

                al_flip_display();
            }


           


            else {

                ALLEGRO_BITMAP* cenario_atual;
                if (fase_atual == 1) {
                    cenario_atual = casa;

                  
                }
                else if (fase_atual == 2) {
                    cenario_atual = casa;

                    if (perdeu_fase2) {
                        fase_atual = 0;
                        perdeu_fase2 = false;
                    }
                }
                else if (fase_atual == 3) {
                    cenario_atual = casa;
                    
                   // terminou = true;

                    movimentacao = true;
                }
                else {
                    cenario_atual = casa;
                }


                al_draw_scaled_bitmap(cenario_atual, 0, 0, al_get_bitmap_width(cenario_atual), al_get_bitmap_height(cenario_atual), 0, 0, 800, 600, 0);
                al_draw_filled_circle(maquina_x, maquina_y, maquina_raio, al_map_rgba(0, 0, 0, 0));


                
                al_draw_textf(font, al_map_rgb(255, 255, 255), 100, 20, ALLEGRO_ALIGN_CENTER, "Livros pegos: %d !",livros);
                

                if (transicao) {
                    movimentacao = false;
                    if (transicao_fases == 1) {
                        al_draw_scaled_bitmap(transicao1, 0, 0, al_get_bitmap_width(transicao1), al_get_bitmap_height(transicao1), 0, 0, 800, 600, 0);
                        al_flip_display();
                        al_draw_bitmap(next, mouselocal_x, mouselocal_y, 0);
                      
                        //al_draw_filled_rectangle(mouselocal_x, mouselocal_y, mouselocal_x + mouselocal_largura, mouselocal_y + mouselocal_altura, al_map_rgb(255, 0, 0));

                    }
                    if (transicao_fases == 2) {
                        al_draw_scaled_bitmap(transicao2, 0, 0, al_get_bitmap_width(transicao2), al_get_bitmap_height(transicao2), 0, 0, 800, 600, 0);
                        al_flip_display();
                        al_draw_bitmap(next, mouselocal_x, mouselocal_y, 0);
                      
                      //  al_draw_filled_rectangle(mouselocal_x, mouselocal_y, mouselocal_x + mouselocal_largura, mouselocal_y + mouselocal_altura, al_map_rgb(255, 0, 0));
                    }
                    if (transicao_fases == 3) {
                        al_draw_scaled_bitmap(transicao3, 0, 0, al_get_bitmap_width(transicao3), al_get_bitmap_height(transicao3), 0, 0, 800, 600, 0);
                        al_flip_display();   
                        al_draw_bitmap(next, mouselocal_x, mouselocal_y, 0);
                       
                       // al_draw_filled_rectangle(mouselocal_x, mouselocal_y, mouselocal_x + mouselocal_largura, mouselocal_y + mouselocal_altura, al_map_rgb(255, 0, 0));
                     
                    }

                }
               
         
                   
                
                if (historia) {
                    movimentacao = false;
                    if (historia_etapa == 0) {
                        al_draw_scaled_bitmap(historia1, 0, 0, al_get_bitmap_width(historia1), al_get_bitmap_height(historia1), 0, 0, 800, 600, 0);
                    }
                    else if (historia_etapa == 1) {
                        al_draw_scaled_bitmap(historia2, 0, 0, al_get_bitmap_width(historia2), al_get_bitmap_height(historia2), 0, 0, 800, 600, 0);
                    }
                    else if (historia_etapa == 2) {
                        al_draw_scaled_bitmap(historia3, 0, 0, al_get_bitmap_width(historia3), al_get_bitmap_height(historia3), 0, 0, 800, 600, 0);
                    }
                    else if (historia_etapa == 3) {
                        al_draw_scaled_bitmap(historia4, 0, 0, al_get_bitmap_width(historia4), al_get_bitmap_height(historia4), 0, 0, 800, 600, 0);
                    }
                    else if (historia_etapa == 4) {
                        al_draw_scaled_bitmap(historia5, 0, 0, al_get_bitmap_width(historia5), al_get_bitmap_height(historia5), 0, 0, 800, 600, 0);
                    }
                    else if (historia_etapa == 5) {
                        al_draw_scaled_bitmap(historia6, 0, 0, al_get_bitmap_width(historia6), al_get_bitmap_height(historia6), 0, 0, 800, 600, 0);

                    }
                    else if (historia_etapa == 6) {
                        al_draw_scaled_bitmap(historia7, 0, 0, al_get_bitmap_width(historia7), al_get_bitmap_height(historia7), 0, 0, 800, 600, 0);
                    }
                    
                    al_draw_filled_rectangle(mouselocal_x, mouselocal_y, mouselocal_x + mouselocal_largura, mouselocal_y + mouselocal_altura, al_map_rgb(255, 0, 0));
                    al_flip_display();

                
                }

                if (livro1 && livro2 && livro3) {
                    movimentacao = false;
                    al_flip_display();
                    al_draw_scaled_bitmap(gameover, 0, 0, al_get_bitmap_width(gameover), al_get_bitmap_height(gameover), 0, 0, 800, 600, 0);
                    al_flip_display();
                    al_rest(8.0);
                    sair = true;
                   
                  
                }

                if (andando_esquerda) {
                    x -= 3;
                    parado = false;
                }
                if (andando_direita) {
                    x += 3;
                    parado = false;
                }

                if (!andando_esquerda && !andando_direita && !pulando) {
                    parado = true;
                }

                if (pulando) {
                    y -= jump_speed;
                    jump_speed -= GRAVITY * 0.1;

                    if (y >= jump_start_y) {
                        y = jump_start_y;
                        pulando = false;
                        jump_speed = 0;
                    }
                }

                if (mapa_visivel) {
                    al_draw_scaled_bitmap(mapa, 0, 0, al_get_bitmap_width(mapa), al_get_bitmap_height(mapa),
                        210, 135, 370, 250, 0);

                }

                frame += 0.3;
                if ((int)frame >= NUM_FRAMES) {
                    frame = 0;
                }

                float distancia_x = x - maquina_x;
                float distancia_y = y - maquina_y;
                float distancia = sqrt(distancia_x * distancia_x + distancia_y * distancia_y);
                perto_da_maquina = (distancia <= maquina_raio);
                if (movimentacao) {
                    if (pulando) {
                        al_draw_scaled_bitmap(Pulando, ((int)frame) * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT, x, y, 100, 100, 0);
                    }
                    else if (parado) {
                        al_draw_scaled_bitmap(Parado, 0, 0, FRAME_WIDTH, FRAME_HEIGHT, x, y, 100, 100, 0);
                    }
                    else if (andando_esquerda) {
                        al_draw_scaled_bitmap(AndandoESQ, ((int)frame) * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT, x, y, 100, 100, 0);
                    }
                    else if (andando_direita) {
                        al_draw_scaled_bitmap(Personagem, ((int)frame) * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT, x, y, 100, 100, 0);
                    }
                }

                al_flip_display();
            }
        }
        if(transicao){
        if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && evento.mouse.button == 1) {
            // Verifique se o clique do mouse está dentro da área do "Next"
            if (evento.mouse.x >= mouselocal_x && evento.mouse.x <= mouselocal_x + mouselocal_largura &&
                evento.mouse.y >= mouselocal_y && evento.mouse.y <= mouselocal_y + mouselocal_altura) {
                printf("Mouse clicado em: (%.2f, %.2f)\n", mouselocal_x, mouselocal_y);
                if (transicao_fases == 1) {
                    fase_atual = 1;
                    jogo_ativo = true;
                    creditos++;
                    system("F:\\repos\\dinossauro\\x64\\Debug\\dinossauro.exe");
                    jogo_ativo = false;
                    //  fase_atual = casa;
                    //  jogo_ativo = 1;
                    transicao = false;
                    movimentacao = true;
                    livro1 = true;
                    livros++;
                   

                }
                if (transicao_fases == 2) {
                    fase_atual = 2;
                    //jogo_ativo = true;,
                    creditos++;
                    system("F:\\repos\\spaceinvaders\\x64Space\\x64\\Debug\\spaceinvaders.exe");
                    jogo_ativo = 1;
                    transicao = false;
                    movimentacao = true;
                    livro2 = true;
                    livros++;

                   
                }
                if (transicao_fases == 3) {
                    
                    fase_atual = 3;
                    creditos++;
                   // jogo_ativo = true;
                    system("F:\\repos\\CompitandoOsCodigos\\x64Bomber\\x64\\Debug\\CompitandoOsCodigos.exe");
                   // jogo_ativo = false;
                    transicao = false;
                    //historia = true;
                //   fase_atual = casa;
                    livro3 = true;
                    livros++;


                }

            }
        }
        }

        if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && evento.mouse.button == 1) {
            // Verifique se o clique do mouse está dentro da área do "Next"
            if (evento.mouse.x >= mouselocal_x && evento.mouse.x <= mouselocal_x + mouselocal_largura &&
                evento.mouse.y >= mouselocal_y && evento.mouse.y <= mouselocal_y + mouselocal_altura) {
                printf("Mouse clicado em: (%.2f, %.2f)\n", mouselocal_x, mouselocal_y);
                historia_etapa++;  // Avança para a próxima etapa da história
                if (historia_etapa >= 6) {  // Se for a última etapa, encerre o jogo
                    historia = false;
                    movimentacao = true;
                   
                   // fase_atual = casa;
                  
                    
                }
            }
        }
        if (introducao) {
            if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && evento.mouse.button == 1) {
                if (evento.mouse.x >= fase10_x && evento.mouse.x <= fase10_x + fase10_largura &&
                    evento.mouse.y >= fase10_y && evento.mouse.y <= fase10_y + fase10_altura) {
                }
                introducao_etapa++;

                if (introducao_etapa >= 6) {
                    introducao = false;
                }

            }
        }

        if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && evento.mouse.button == 1) {
            float mouse_x = evento.mouse.x;
            float mouse_y = evento.mouse.y;
            printf("Mouse clicado em: (%.2f, %.2f)\n", mouse_x, mouse_y);
            float distancia_mouse = sqrt((mouse_x - maquina_x) * (mouse_x - maquina_x) + (mouse_y - maquina_y) * (mouse_y - maquina_y));

            if (distancia_mouse <= maquina_raio) {
                mapa_visivel = !mapa_visivel;
            }
        }

        if (mapa_visivel) {
            // Pega a posição do clique do mouse
            if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && evento.mouse.button == 1) {
                float mouse_x = evento.mouse.x;
                float mouse_y = evento.mouse.y;
                printf("Mouse clicado em: (%.2f, %.2f)\n", mouse_x, mouse_y);

                // Verifica se o clique foi dentro da área da fase 1
                if (mouse_x >= fase1_x && mouse_x <= fase1_x + fase1_largura && mouse_y >= fase1_y && mouse_y <= fase1_y + fase1_altura) {
                    printf("Fase 1 clicada!\n");
                    transicao = true;
                    transicao_fases = 1;

                    mapa_visivel = false;
         
                    //jogo_ativo = true;
                   // system("C:/Users/Leon/source/repos/dinossauro/x64/Debug/dinossauro.exe");
                   // jogo_ativo = 1;

                }

                else if (mouse_x >= fase2_x && mouse_x <= fase2_x + fase2_largura && mouse_y >= fase2_y && mouse_y <= fase2_y + fase2_altura) {
                    printf("Fase 2 clicada!\n");
                    transicao = true;
                    transicao_fases = 2;
                   // fase_atual = 2;
                    mapa_visivel = false;
                 
                }

                else if (mouse_x >= fase3_x && mouse_x <= fase3_x + fase3_largura && mouse_y >= fase3_y && mouse_y <= fase3_y + fase3_altura) {
                    printf("Fase 3 clicada!\n");
                    transicao = true;
                    transicao_fases = 3;
                    
                    mapa_visivel = false;
                   // fase_atual = 3;

                 
                    
                   
                
                }
            }
        }

        if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && on_homepage && evento.mouse.button == 1) {
            if (evento.mouse.x >= botaox && evento.mouse.x <= botaox + largura &&
                evento.mouse.y >= botaoy && evento.mouse.y <= botaoy + altura) {
                on_homepage = false;  // Fecha a homepage
                introducao = true;    // Ativa a introdução
                introducao_etapa = 0;
            }
        }


        else if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (!on_homepage) {
                switch (evento.keyboard.keycode) {
                case ALLEGRO_KEY_LEFT:
                    andando_esquerda = true;
                    parado = false;
                    break;

                case ALLEGRO_KEY_RIGHT:
                    andando_direita = true;
                    parado = false;
                    break;

                case ALLEGRO_KEY_UP:
                    if (!pulando) {
                        pulando = true;
                        jump_speed = 10;
                        jump_start_y = y;
                    }
                    break;

                }
            }
        }

        else if (evento.type == ALLEGRO_EVENT_KEY_UP) {
            switch (evento.keyboard.keycode) {
            case ALLEGRO_KEY_LEFT:
                andando_esquerda = false;
                break;

            case ALLEGRO_KEY_RIGHT:
                andando_direita = false;
                break;
            }
        }
        else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            sair = true;
        }
    }




    //al_destroy_sample(musica);
   // al_uninstall_audio();
    al_destroy_font(font);
    al_destroy_bitmap(historia7);
    al_destroy_bitmap(historia6);
    al_destroy_bitmap(historia5);
    al_destroy_bitmap(historia4);
    al_destroy_bitmap(historia3);
    al_destroy_bitmap(historia2);
    al_destroy_bitmap(historia1);
    al_destroy_bitmap(introducao6);
    al_destroy_bitmap(introducao5);
    al_destroy_bitmap(introducao4);
    al_destroy_bitmap(introducao3);
    al_destroy_bitmap(introducao2);
    al_destroy_bitmap(introducao1);
    al_destroy_bitmap(Personagem);
    al_destroy_bitmap(casa);
    al_destroy_bitmap(Homepage);
    al_destroy_bitmap(cenario1);
    al_destroy_bitmap(cenario2);
    al_destroy_bitmap(cenario3);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(fila_eventos);
    al_destroy_font(fonte);

    return 0;
}