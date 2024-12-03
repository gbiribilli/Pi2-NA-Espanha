#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

//Constantes legais
const float FPS = 60.0;
const int screen_w = 800;
const int screen_h = 600;
const int tamanho_jogador = 50;  // Alterado para 50x50
const int tamanho_inimigo = 50;  // Alterado para 50x50
const int velocidade_tiro_inimigo = 4;
const int velocidade_inimigo = 2;

typedef struct {
    float x, y;
    float vel_jogador;
} Player;

typedef struct {
    float x, y;
    bool active;
} Tiro;

typedef struct {
    float x, y;
    bool active;
} TiroInimigo;

typedef struct {
    double x;
    double y;
    bool active;
    bool municao;
    int tiro;
    TiroInimigo tiro_inimigo;
} Inimigo;



int main() {

    //Funções de iniciação
    srand(time(NULL));
    al_init();
    al_install_keyboard();
    al_init_image_addon();
    al_install_mouse();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    ALLEGRO_DISPLAY* display = al_create_display(screen_w, screen_h);
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();

    ALLEGRO_BITMAP* img_jogador = al_load_bitmap("F:\\repos\\Imagens\\space\\jogador.png");
    ALLEGRO_BITMAP* img_inimigo = al_load_bitmap("F:\\repos\\Imagens\\space\\inimigo.png");
    ALLEGRO_BITMAP* cenario = al_load_bitmap("F:\\repos\\Imagens\\space\\ceu.jpg");
    ALLEGRO_BITMAP* historia1 = al_load_bitmap("F:\\repos\\Imagens\\historia do jogo\\pear1.png");
    ALLEGRO_BITMAP* historia2 = al_load_bitmap("F:\\repos\\Imagens\\historia do jogo\\pear2.png");
    ALLEGRO_BITMAP* historia3 = al_load_bitmap("F:\\repos\\Imagens\\historia do jogo\\pear3.png");
    ALLEGRO_BITMAP* historia4 = al_load_bitmap("F:\\repos\\Imagens\\historia do jogo\\pear4.png");
    ALLEGRO_BITMAP* historia5 = al_load_bitmap("F:\\repos\\Imagens\\historia do jogo\\pear5.png");
    ALLEGRO_BITMAP* historia6 = al_load_bitmap("F:\\repos\\Imagens\\historia do jogo\\pear6.png");
    ALLEGRO_BITMAP* creditos = al_load_bitmap("F:\\repos\\Imagens\\telas\\NEXT.png");

    //Valores de player/tiro
    Player jogador = { screen_w / 2 - tamanho_jogador / 2, screen_h - tamanho_jogador, 5.0 };
    Tiro bala = { jogador.x, jogador.y, false };

    float mouselocal_x = 670;
    float mouselocal_y = 540;
    float mouselocal_largura = 120;
    float mouselocal_altura = 30;

    //Valores dos inimigos 
    Inimigo inimigos[5];
    bool andar_esquerda = false;
    bool andar_direita = false;
    int direcao_inimigo = 1;
    for (int i = 0; i < 5; i++) {
        inimigos[i].x = i * 100 + 50.0;
        inimigos[i].y = 50.0;
        inimigos[i].active = true;
        inimigos[i].municao = true;
        inimigos[i].tiro_inimigo.active = false;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

    al_start_timer(timer);
    bool redraw = true;
    bool Sair_jogo = false;
    bool historia = false;
    int historia_etapa = 0;
    bool mensagem_exibida = false;

    while (!Sair_jogo) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && ev.mouse.button == 1) {
            // Verifique se o clique do mouse está dentro da área do "Next"
            if (ev.mouse.x >= mouselocal_x && ev.mouse.x <= mouselocal_x + mouselocal_largura &&
                ev.mouse.y >= mouselocal_y && ev.mouse.y <= mouselocal_y + mouselocal_altura) {
                printf("Mouse clicado em: (%.2f, %.2f)\n", mouselocal_x, mouselocal_y);
                historia_etapa++;  // Avança para a próxima etapa da história
                if (historia_etapa > 5) {  // Se for a última etapa, encerre o jogo
                    Sair_jogo = true;
                }
            }
        }
        //Verificação da tecla apertada para movimento/tiro
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            
           
            switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_LEFT:
                andar_esquerda = true;
                break;
            case ALLEGRO_KEY_RIGHT:
                andar_direita = true;
                break;
            case ALLEGRO_KEY_A:
                andar_esquerda = true;
                break;
            case ALLEGRO_KEY_D:
                andar_direita = true;
                break;
            case ALLEGRO_KEY_SPACE:
                if (!bala.active) {
                    bala.x = jogador.x + tamanho_jogador / 2;
                    bala.y = jogador.y;
                    bala.active = true;
                }
                break;
            }
        }
        //Verificação da tecla solta para movimento/tiro
        else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
            switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_LEFT:
                andar_esquerda = false;
                break;
            case ALLEGRO_KEY_RIGHT:
                andar_direita = false;
                break;
            case ALLEGRO_KEY_A:
                andar_esquerda = false;
                break;
            case ALLEGRO_KEY_D:
                andar_direita = false;
                break;
            }
        }
        //Atualização da posição do jogador
        if (ev.type == ALLEGRO_EVENT_TIMER) {
            if (andar_esquerda) jogador.x -= jogador.vel_jogador;
            if (andar_direita) jogador.x += jogador.vel_jogador;

            //Verifica se a bala está na tela e "recarrega" ela caso não
            if (bala.active) {
                bala.y -= 5.0;
                if (bala.y < 0) bala.active = false;
            }

            bool mudar_direcao = false;

            //Verificação se o tiro do jogador atingiu um inimigo
            for (int i = 0; i < 5; i++) {
                if (inimigos[i].active && bala.active &&
                    bala.x < inimigos[i].x + tamanho_inimigo &&
                    bala.x + 5 > inimigos[i].x &&
                    bala.y < inimigos[i].y + tamanho_inimigo &&
                    bala.y + 10 > inimigos[i].y) {
                    inimigos[i].active = false;
                    bala.active = false;
                }

                // Movimento dos inimigos
                inimigos[i].x += direcao_inimigo * velocidade_inimigo;

                // Verificar se algum inimigo atingiu a borda da tela, para depois mudar de direção
                if (inimigos[i].x < 0 || inimigos[i].x + tamanho_inimigo > screen_w) {
                    mudar_direcao = true;
                }

                // Tiro dos inimigos
                if (inimigos[i].active && !inimigos[i].tiro_inimigo.active) {
                    if (rand() % 100 < 2) {
                        inimigos[i].tiro_inimigo.x = inimigos[i].x + tamanho_inimigo / 2;
                        inimigos[i].tiro_inimigo.y = inimigos[i].y + tamanho_inimigo;
                        inimigos[i].tiro_inimigo.active = true;
                    }
                }

                //Verificações do tiro inimigo
                if (inimigos[i].tiro_inimigo.active) {
                    inimigos[i].tiro_inimigo.y += velocidade_tiro_inimigo;
                    //Recarga do tiro inimigo caso tenha saido da tela
                    if (inimigos[i].tiro_inimigo.y > screen_h) {
                        inimigos[i].tiro_inimigo.active = false;
                    }
                    //Verifica se pegou no jogador, ou se os inimigos chegaram muito próximos dele, caso sim, fecha o jogo(mudar depois)
                    if (inimigos[i].tiro_inimigo.x > jogador.x &&
                        inimigos[i].tiro_inimigo.x < jogador.x + tamanho_jogador &&
                        inimigos[i].tiro_inimigo.y > jogador.y &&
                        inimigos[i].tiro_inimigo.y < jogador.y + tamanho_jogador || inimigos[i].y >= jogador.y - 50) {

                        //system("C:\\Users\\Leon\\source\\repos\\JogoEmAllegro\\Debug\\JogoEmAllegro.exe");
                        al_destroy_display(display);
                        main();
                    }             
                }

            }

            // Se algum inimigo atingiu a borda, todos mudam de direção e descem
            if (mudar_direcao) {
                direcao_inimigo = -direcao_inimigo;
                for (int i = 0; i < 5; i++) {
                    inimigos[i].y += 30;
                }
            }

            // Verifica se todos os inimigos foram derrotados
            bool todos_derrotados = true;
            for (int i = 0; i < 5; i++) {
                if (inimigos[i].active) {
                    todos_derrotados = false;
                    break;
                }
            }

            if (todos_derrotados && !mensagem_exibida) {
                printf("Inimigos derrotados! Você venceu!\n");
                historia = true;  // Ativa a exibição da história
                mensagem_exibida = true;  // Impede que a mensagem apareça novamente
                historia_etapa = 0; // Começa a história na primeira etapa
            
            }

           redraw = true;
        }
       
       

        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            Sair_jogo = true;
        }

        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;
            al_clear_to_color(al_map_rgb(0, 0, 0));

            if (cenario) {
                al_draw_scaled_bitmap(cenario, 0, 0, al_get_bitmap_width(cenario), al_get_bitmap_height(cenario),
                    0, 0, screen_w, screen_h, 0);
            }

            // Desenha o jogador com a imagem redimensionada
            if (img_jogador) {
                al_draw_scaled_bitmap(img_jogador, 0, 0, al_get_bitmap_width(img_jogador), al_get_bitmap_height(img_jogador), 
                                      jogador.x, jogador.y, tamanho_jogador, tamanho_jogador, 0);
            }

            // Desenha o tiro do jogador
            if (bala.active) {
                al_draw_filled_rectangle(bala.x, bala.y, bala.x + 5, bala.y + 10, al_map_rgb(255, 255, 255));
            }

            // Desenha os inimigos com a imagem redimensionada
            for (int i = 0; i < 5; i++) {
                if (inimigos[i].active && img_inimigo) {
                    al_draw_scaled_bitmap(img_inimigo, 0, 0, al_get_bitmap_width(img_inimigo), al_get_bitmap_height(img_inimigo), 
                                          inimigos[i].x, inimigos[i].y, tamanho_inimigo, tamanho_inimigo, 0);
                }

                // Desenha o tiro dos inimigos
                if (inimigos[i].tiro_inimigo.active) {
                    al_draw_filled_rectangle(inimigos[i].tiro_inimigo.x, inimigos[i].tiro_inimigo.y,
                                              inimigos[i].tiro_inimigo.x + 5, inimigos[i].tiro_inimigo.y + 10,
                                              al_map_rgb(255, 0, 0));
                }
            }
            if (historia) {
                
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
                al_draw_bitmap(creditos, mouselocal_x, mouselocal_y, 0);
              //  al_draw_filled_rectangle(mouselocal_x, mouselocal_y, mouselocal_x + mouselocal_largura, mouselocal_y + mouselocal_altura, al_map_rgb(255, 0, 0));
                
                al_flip_display();
            
            }
            
            al_flip_display();
        }
    }

    al_destroy_bitmap(historia6);
    al_destroy_bitmap(historia5);
    al_destroy_bitmap(historia4);
    al_destroy_bitmap(historia3);
    al_destroy_bitmap(historia2);
    al_destroy_bitmap(historia1);
    al_destroy_bitmap(cenario);
    al_destroy_bitmap(img_jogador);
    al_destroy_bitmap(img_inimigo);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);

    return 0;
}
