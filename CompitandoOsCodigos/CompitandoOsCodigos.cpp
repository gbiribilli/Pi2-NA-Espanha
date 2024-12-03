#include <iostream>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/keyboard.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_FRAMES 4 
ALLEGRO_BITMAP* Blsolido;
ALLEGRO_BITMAP* Bldestrutivel;
ALLEGRO_BITMAP* Objetivo;
ALLEGRO_BITMAP* persona;
ALLEGRO_BITMAP* livro;
ALLEGRO_BITMAP* vazio;

const int linhas = 15;
const int colunas = 20;
int board[15][20];
bool sair = false;
bool sairBomber = false;
int inicialX = 1;
int inicialY = 1;
ALLEGRO_BITMAP* cima;
ALLEGRO_BITMAP* esquerda;
ALLEGRO_BITMAP* baixo;
ALLEGRO_BITMAP* direita;

int main()
{
    al_init();
    al_init_primitives_addon();
    al_install_keyboard();
    al_init_image_addon();
    al_init_font_addon();
    al_install_mouse();
    al_init_ttf_addon();

    ALLEGRO_DISPLAY* bomber = al_create_display(800, 600);
    ALLEGRO_EVENT_QUEUE* fila_de_eventos = al_create_event_queue();
    ALLEGRO_FONT* fonte = al_load_font("arial.ttf", 24, 0);



    al_register_event_source(fila_de_eventos, al_get_display_event_source(bomber));
    al_register_event_source(fila_de_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_de_eventos, al_get_mouse_event_source());

    srand(time(NULL));


    float frame = 0;
    srand(time(NULL));
    cima = al_load_bitmap("F:\\repos\\Imagens\\Bomberman\\BCima.png");
    esquerda = al_load_bitmap("F:\\repos\\Imagens\\Bomberman\\BEsquerda.png");
    baixo = al_load_bitmap("F:\\repos\\Imagens\\Bomberman\\BBaixo.png");
    direita = al_load_bitmap("F:\\repos\\Imagens\\Bomberman\\BDireita.png");
    persona = al_load_bitmap("F:\\repos\\Imagens\\Bomberman\\Bcima.png");

    Blsolido = al_load_bitmap("F:\\repos\\Imagens\\Bomberman\\parede.png");
    Bldestrutivel = al_load_bitmap("F:\\repos\\Imagens\\Bomberman\\paredeQuebravel.png");
    livro = al_load_bitmap("F:\\repos\\Imagens\\Bomberman\\livrop.png");
    vazio = al_load_bitmap("F:\\repos\\Imagens\\Bomberman\\new.png");



    bool historia = false;
    float mouselocal_x = 670;
    float mouselocal_y = 540;
    float mouselocal_largura = 120;
    float mouselocal_altura = 30;
    int historia_etapa =  0 ;


    // Inicialização do tabuleiro
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            int n1 = rand() % 10;
            board[i][j] = (n1 % 4 == 0) ? 1 : 0; // 1: Bloco sólido, 0: Espaço vazio
        }
    }
    for (int i = 0; i < linhas; i += 2) {
        for (int j = 0; j < colunas; j += 2) {
            if (board[i][j] == 0) {
                board[i][j] = 2;
            } // Bloco destrutível
        }
    }
    board[inicialY][inicialX] = 3;           // Posição inicial do jogador

    board[linhas - 1][colunas - 1] = 4;      // Objetivo
    //começa o game
    while (!sairBomber) {
        ALLEGRO_EVENT event;
        al_wait_for_event(fila_de_eventos, &event);
        // Desenha o tabuleiro
        for (int i = 0; i < linhas; i++) {
            for (int j = 0; j < colunas; j++) {
                ALLEGRO_BITMAP* imagens = NULL;

                

                switch (board[i][j]) {
                case 0:
                    // al_draw_filled_rectangle(j * 40, i * 40, (j + 1) * 50, (i + 1) * 50, al_map_rgb(169, 169, 169));
                    imagens = vazio;
                    break; // Espaço vazio

                case 1:
                    //al_draw_filled_rectangle(j * 40, i * 40, (j + 1) * 40, (i + 1) * 40, al_map_rgb(199, 83, 43));
                    imagens = Blsolido;
                    break;       // Bloco sólido

                case 2:
                    //al_draw_filled_rectangle(j * 40, i * 40, (j + 1) * 40, (i + 1) * 40, al_map_rgb(159, 97, 43));
                    imagens = Bldestrutivel;
                    break;     // Bloco destrutível
                case 3:
                    // al_draw_filled_rectangle(j * 40, i * 40, (j + 1) * 40, (i + 1) * 40, al_map_rgb(0, 0, 255));

                    imagens = persona;

                    break;     // Jogador
                case 4:
                    //al_draw_filled_rectangle(j * 40, i * 40, (j + 1) * 40, (i + 1) * 40, al_map_rgb(0, 255, 0));
                    imagens = livro;
                    break;   // Objetivo
                }
                if (imagens) {
                    al_draw_bitmap(imagens, j * 40, i * 40, 0);
                }
               

            }
            if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && event.mouse.button == 1) {
                    // Verifique se o clique do mouse está dentro da área do "Next"
                    if (event.mouse.x >= mouselocal_x && event.mouse.x <= mouselocal_x + mouselocal_largura &&
                        event.mouse.y >= mouselocal_y && event.mouse.y <= mouselocal_y + mouselocal_altura) {
                        printf("Mouse clicado em: (%.2f, %.2f)\n", mouselocal_x, mouselocal_y);
                        historia_etapa++;  // Avança para a próxima etapa da história
                        if (historia_etapa >= 6) {  // Se for a última etapa, encerre o jogo
                            historia = false;
                        }
                    }
                }
            }
        }
        //al_draw_textf(fonte, al_map_rgb(0, 0, 0), 10, 10, 0, "Chances: %d", chances);

        al_flip_display();
        frame += 0.3;
        if ((int)frame >= NUM_FRAMES) {
            frame = 0;
        }
        // Processamento de eventos

      


        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && event.mouse.button == 1) {
                    // Verifique se o clique do mouse está dentro da área do "Next"
                    if (event.mouse.x >= mouselocal_x && event.mouse.x <= mouselocal_x + mouselocal_largura &&
                        event.mouse.y >= mouselocal_y && event.mouse.y <= mouselocal_y + mouselocal_altura) {
                        printf("Mouse clicado em: (%.2f, %.2f)\n", mouselocal_x, mouselocal_y);
                        historia_etapa++;  // Avança para a próxima etapa da história
                        if (historia_etapa >= 6) {  // Se for a última etapa, encerre o jogo
                            historia = false;
                        }
                    }
                }
            }
            int dx = 0, dy = 0;
            switch (event.keyboard.keycode) {
            case ALLEGRO_KEY_RIGHT:
                dx = 1;
                persona = direita;
                break;
            case ALLEGRO_KEY_LEFT:
                dx = -1;
                persona = esquerda;
                break;
            case ALLEGRO_KEY_UP:
                dy = -1;
                persona = cima;
                break;
            case ALLEGRO_KEY_DOWN:
                dy = 1;
                persona = baixo;
                break;

            case ALLEGRO_KEY_R: // Reinicializa o tabuleiro

                al_destroy_display(bomber);
                main();

                return 1;

            case ALLEGRO_KEY_ESCAPE: // Encerra o jogo
                sairBomber = true;
                break;
            }

            int novoX = inicialX + dx;
            int novoY = inicialY + dy;
            if (novoX >= 0 && novoX < colunas && novoY >= 0 && novoY < linhas &&
                (board[novoY][novoX] == 0 || board[novoY][novoX] == 4)) {
                board[inicialY][inicialX] = 0;  // Atualiza a posição anterior do jogador
                inicialX = novoX;
                inicialY = novoY;
                board[inicialY][inicialX] = 3;  // Atualiza a nova posição do jogador

            }
        }
        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && event.mouse.button == 1) {
                // Verifique se o clique do mouse está dentro da área do "Next"
                if (event.mouse.x >= mouselocal_x && event.mouse.x <= mouselocal_x + mouselocal_largura &&
                    event.mouse.y >= mouselocal_y && event.mouse.y <= mouselocal_y + mouselocal_altura) {
                    printf("Mouse clicado em: (%.2f, %.2f)\n", mouselocal_x, mouselocal_y);
                    historia_etapa++;  // Avança para a próxima etapa da história
                    if (historia_etapa >= 6) {  // Se for a última etapa, encerre o jogo
                        historia = false;
                    }
                }
            }
            switch (event.keyboard.keycode) {
            case ALLEGRO_KEY_SPACE:
                printf("Apertado");
                if (board[inicialY + 1][inicialX] == 2) {
                    board[inicialY + 1][inicialX] = 0;
                }
                if (board[inicialY - 1][inicialX] == 2) {
                    board[inicialY - 1][inicialX] = 0;
                }
                if (board[inicialY][inicialX + 1] == 2) {
                    board[inicialY][inicialX + 1] = 0;
                }
                if (board[inicialY][inicialX - 1] == 2) {
                    board[inicialY][inicialX - 1] = 0;
                }
                break;
            }
        }
        if (board[inicialY][inicialX] == board[linhas - 1][colunas - 1]) {
            printf("Voce conseguiu");
            //fase_atual = casa;
            historia_etapa = 0;

            historia = true;
          
            sairBomber = true;
           
           // return 0;
        }

        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            sairBomber = true;
            printf("Encerrado");

        }
    }
    ALLEGRO_BITMAP* historia1 = al_load_bitmap("F:\\repos\\Imagens\\historia do jogo\\jp1.png");
    ALLEGRO_BITMAP* historia2 = al_load_bitmap("F:\\repos\\Imagens\\historia do jogo\\jp2.png");
    ALLEGRO_BITMAP* historia3 = al_load_bitmap("F:\\repos\\Imagens\\historia do jogo\\jp3.png");
    ALLEGRO_BITMAP* historia4 = al_load_bitmap("F:\\repos\\Imagens\\historia do jogo\\jp4.png");
    ALLEGRO_BITMAP* historia5 = al_load_bitmap("F:\\repos\\Imagens\\historia do jogo\\jp5.png");
    ALLEGRO_BITMAP* historia6 = al_load_bitmap("F:\\repos\\Imagens\\historia do jogo\\jp6.png");
    ALLEGRO_BITMAP* historia7 = al_load_bitmap("F:\\repos\\Imagens\\historia do jogo\\jp7.png");
    ALLEGRO_BITMAP* creditos = al_load_bitmap("F:\\repos\\Imagens\\telas\\NEXT.png");

    if (board[inicialY][inicialX] == board[linhas - 1][colunas - 1]) {
        printf("Voce conseguiu");
        //fase_atual = casa;
        historia_etapa = 0;
        historia = true;
        sairBomber = true;
        //return 0;
    }

    while (historia) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(fila_de_eventos, &ev);

        if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && ev.mouse.button == 1) {
            // Verifique se o clique do mouse está dentro da área do "Next"
            if (ev.mouse.x >= mouselocal_x && ev.mouse.x <= mouselocal_x + mouselocal_largura &&
                ev.mouse.y >= mouselocal_y && ev.mouse.y <= mouselocal_y + mouselocal_altura) {
                printf("Mouse clicado em: (%.2f, %.2f)\n", mouselocal_x, mouselocal_y);
                historia_etapa++;  // Avança para a próxima etapa da história
                if (historia_etapa > 6) {  // Se for a última etapa, encerre o jogo
                    historia = false;
                }
            }
        }
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
        al_draw_bitmap(creditos, mouselocal_x, mouselocal_y, 0);
       // al_draw_filled_rectangle(mouselocal_x, mouselocal_y, mouselocal_x + mouselocal_largura, mouselocal_y + mouselocal_altura, al_map_rgb(255, 0, 0));

        al_flip_display();
    }

    al_destroy_bitmap(esquerda);
    al_destroy_bitmap(direita);
    al_destroy_bitmap(cima);
    al_destroy_bitmap(baixo);
    al_destroy_bitmap(persona);
    al_destroy_bitmap(vazio);
    al_destroy_bitmap(livro);
    al_destroy_bitmap(Bldestrutivel);
    al_destroy_bitmap(Blsolido);
    al_destroy_display(bomber);
    al_destroy_event_queue(fila_de_eventos);
}

