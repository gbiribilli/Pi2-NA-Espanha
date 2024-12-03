#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define LARGURA_TELA 800
#define ALTURA_TELA 600
#define GRAVIDADE 0.5
#define PULO_FORCA -10.0
#define VELOCIDADE_CACTOS -5.0
#define NUM_CACTOS 50
#define PONTUACAO_VITORIA 10
#define DISTANCIA_MINIMA_CACTOS 300  // Distância mínima entre os cactos
#define ANIMACAO_PULO_FPS 8
#define FRAME_WIDTH 45
#define FRAME_HEIGHT 60

int main() {
    // Inicialização
    al_init();
    al_init_image_addon();
    al_init_primitives_addon();
    al_install_keyboard();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_mouse();

    ALLEGRO_DISPLAY* display = al_create_display(LARGURA_TELA, ALTURA_TELA);
    ALLEGRO_EVENT_QUEUE* fila_eventos = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_FONT* fonte = al_create_builtin_font();

    al_register_event_source(fila_eventos, al_get_display_event_source(display));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_mouse_event_source());

    // Carregar imagens
    ALLEGRO_BITMAP* dino_pulo = al_load_bitmap("F:\\repos\\Imagens\\Personagem\\PGParado.png");
    ALLEGRO_BITMAP* obstaculo_original = al_load_bitmap("F:\\repos\\Imagens\\Dino\\predio.png");
    ALLEGRO_BITMAP* fundo = al_load_bitmap("F:\\repos\\Imagens\\Dino\\fundo.png");

    ALLEGRO_BITMAP* historia1 = al_load_bitmap("F:\\repos\\Imagens\\historia do jogo\\PO1.png");
    ALLEGRO_BITMAP* historia2 = al_load_bitmap("F:\\repos\\Imagens\\historia do jogo\\PO2.png");
    ALLEGRO_BITMAP* historia3 = al_load_bitmap("F:\\repos\\Imagens\\historia do jogo\\PO3.png");
    ALLEGRO_BITMAP* historia4 = al_load_bitmap("F:\\repos\\Imagens\\historia do jogo\\PO4.png");
    ALLEGRO_BITMAP* historia5 = al_load_bitmap("F:\\repos\\Imagens\\historia do jogo\\PO5.png");
    ALLEGRO_BITMAP* creditos = al_load_bitmap("F:\\repos\\Imagens\\telas\\NEXT.png");


    // Ajustar tamanho dos cactos
    int largura_cacto = 50;
    int altura_cacto = 100;
    ALLEGRO_BITMAP* obstaculo = al_create_bitmap(largura_cacto, altura_cacto);

    al_set_target_bitmap(obstaculo);
    al_draw_scaled_bitmap(obstaculo_original,
        0, 0, al_get_bitmap_width(obstaculo_original), al_get_bitmap_height(obstaculo_original),
        0, 0, largura_cacto, altura_cacto, 0);

    al_set_target_bitmap(al_get_backbuffer(display));

    // Variáveis do jogo
    const int ALTURA_DINO = 40;
    const float REDUCAO_GRAVIDADE = 0.2;
    bool sair = false;
    bool no_chao = true;
    bool espaco_pressionado = false;
    float dino_x = 100;
    float dino_y = ALTURA_TELA - ALTURA_DINO - 10;
    float velocidade_y = 0;
    float cacto_x[NUM_CACTOS];
    int pontuacao = 0;
    int frame = 0;
    float tempo_animacao = 0;
    bool historia = false;
    int historia_etapa = 0;
    bool perdeu = true;
    bool movimentacao = true;
    bool pulando = false;

    float mouselocal_x = 670;
    float mouselocal_y = 540;
    float mouselocal_largura = 120;
    float mouselocal_altura = 30;

    // Variável para controlar a posição dos cactos
    float ultimo_cacto_x = LARGURA_TELA;

    for (int i = 0; i < NUM_CACTOS; i++) {
        // Gerar a posição dos cactos com distância mínima de 250 pixels
        cacto_x[i] = ultimo_cacto_x + rand() % (LARGURA_TELA / 2);  // Gera uma posição aleatória mas dentro da largura da tela
        if (cacto_x[i] - ultimo_cacto_x < DISTANCIA_MINIMA_CACTOS) {
            cacto_x[i] = ultimo_cacto_x + DISTANCIA_MINIMA_CACTOS;  // Garante a distância mínima
        }
        ultimo_cacto_x = cacto_x[i];  // Atualiza a posição do último cacto
    }

    al_start_timer(timer);

    // Loop principal do jogo
    while (!sair) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);

        if (evento.type == ALLEGRO_EVENT_TIMER) {

            // Atualizar posição do dinossauro
            if (!no_chao) {
                // se o espaco estiver sendo segurado, a gravidade vai diminuir e para fazer o dinossauro descer mais devagar
                if (espaco_pressionado) {
                    velocidade_y += GRAVIDADE - REDUCAO_GRAVIDADE;
                }
                else {
                    velocidade_y += GRAVIDADE * 1.5;  // gravidade aumentando ao soltar o espaco
                }

                dino_y += velocidade_y;

                if (dino_y >= ALTURA_TELA - ALTURA_DINO - 10) {
                    dino_y = ALTURA_TELA - ALTURA_DINO - 10;
                    no_chao = true;
                    velocidade_y = 0;
                }
            }


            // Atualizar posição dos cactos
            for (int i = 0; i < NUM_CACTOS; i++) {
                cacto_x[i] += VELOCIDADE_CACTOS;

                if (cacto_x[i] < -largura_cacto) {
                    // Reposiciona os cactos quando saem da tela
                    cacto_x[i] = ultimo_cacto_x + rand() % (LARGURA_TELA / 2);  // Nova posição aleatória
                    if (cacto_x[i] - ultimo_cacto_x < DISTANCIA_MINIMA_CACTOS) {
                        cacto_x[i] = ultimo_cacto_x + DISTANCIA_MINIMA_CACTOS;  // Garante a distância mínima
                    }
                    ultimo_cacto_x = cacto_x[i];  // Atualiza a posição do último cacto
                    pontuacao++;
                }

                if (pontuacao == 8) {
                    historia_etapa = 0;
                    historia = true;
                    sair = true;
                    
                }


                if (perdeu) {
                    if (dino_x + al_get_bitmap_width(dino_pulo) > cacto_x[i] &&
                        dino_x < cacto_x[i] + largura_cacto &&
                        dino_y + al_get_bitmap_height(dino_pulo) > ALTURA_TELA - altura_cacto) {

                        al_destroy_display(display);
                        main();
                    }
                }
            }

            if (!no_chao) {
                tempo_animacao += 1.0 / 60.0;  // Incrementa o tempo de animação
                if (tempo_animacao >= 1.0 / ANIMACAO_PULO_FPS) {
                    frame++;
                    if (frame >= 4) {
                        frame = 0;  // Reseta a animação de pulo
                    }
                    tempo_animacao = 0;  // Reseta o contador de tempo de animação
                }
            }

            if (!no_chao) {
                // Desenhar o dinossauro pulando com a animação
                al_draw_scaled_bitmap(dino_pulo, frame * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT, dino_x, dino_y, 100, 100, 0);
            }
            // Desenhar elementos
            //al_clear_to_color(al_map_rgb(135, 206, 235)); // Fundo azul-claro

            al_draw_scaled_bitmap(fundo, 0, 0, al_get_bitmap_width(fundo), al_get_bitmap_height(fundo), 0, 0, 800, 600, 0);
            al_flip_display();

            al_draw_bitmap(dino_pulo, dino_x, dino_y, 0);

            for (int i = 0; i < NUM_CACTOS; i++) {
                al_draw_bitmap(obstaculo, cacto_x[i], ALTURA_TELA - altura_cacto, 0);
            }

            al_draw_textf(fonte, al_map_rgb(0, 0, 0), 10, 10, 0, "Pontuacao: %d", pontuacao);
            al_flip_display();
        }
        
        if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (evento.keyboard.keycode == ALLEGRO_KEY_SPACE && no_chao) {
                pulando = true;
                no_chao = false;
                velocidade_y = PULO_FORCA; // aplica a forca do pulo quando o espaço e pressionado
            }
            // quando a tecla espaço e mantida, a descida vai ser mais devagar
            if (evento.keyboard.keycode == ALLEGRO_KEY_SPACE && !no_chao) {
                espaco_pressionado = true; // comeca a reduzir a gravidade
            }
        }

        // quando aperta espaço e solta a tecla de volta, volta ao normal a gravidade
        if (evento.type == ALLEGRO_EVENT_KEY_UP) {
            if (evento.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                espaco_pressionado = false; // para de reduzir a gravidade
            }
        }
        
            if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                sair = true; // Sair do jogo
            }


            if (evento.type == ALLEGRO_EVENT_KEY_UP) {
                if (evento.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                    espaco_pressionado = false; // Marca que a tecla foi solta
                }
            }
        
        
    }
    while (historia) {
       // al_create_display(800, 600);
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);
        if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && evento.mouse.button == 1) {
            // Verifique se o clique do mouse está dentro da área do "Next"
            if (evento.mouse.x >= mouselocal_x && evento.mouse.x <= mouselocal_x + mouselocal_largura &&
                evento.mouse.y >= mouselocal_y && evento.mouse.y <= mouselocal_y + mouselocal_altura) {
                printf("Mouse clicado em: (%.2f, %.2f)\n", mouselocal_x, mouselocal_y);
                historia_etapa++;  // Avança para a próxima etapa da história
                if (historia_etapa >= 4) {  // Se for a última etapa, encerre o jogo
                    historia = false;
                }
            }
        }
        if (historia) {
            perdeu = false;


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
            al_draw_bitmap(creditos, mouselocal_x, mouselocal_y, 0);

            // al_draw_filled_rectangle(mouselocal_x, mouselocal_y, mouselocal_x + mouselocal_largura, mouselocal_y + mouselocal_altura, al_map_rgb(255, 0, 0));
            al_flip_display();


        }
        al_flip_display();
    }

    // Limpeza
    al_destroy_bitmap(historia5);
    al_destroy_bitmap(historia4);
    al_destroy_bitmap(historia3);
    al_destroy_bitmap(historia2);
    al_destroy_bitmap(historia1);
    al_destroy_bitmap(dino_pulo);
    al_destroy_bitmap(obstaculo);
    al_destroy_bitmap(obstaculo_original);
    al_destroy_display(display);
    al_destroy_event_queue(fila_eventos);
    al_destroy_timer(timer);
    al_destroy_font(fonte);

    return 0;
}