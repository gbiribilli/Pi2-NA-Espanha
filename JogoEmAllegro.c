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
//C:\\Users\\Leon\\source\\repos\\JogoEmAllegro\\JogoEmAllegro\\

#define FRAME_WIDTH 44   
#define FRAME_HEIGHT 48  
#define NUM_FRAMES 4     
#define FRAME_DURATION 0.2 
#define JUMP_HEIGHT 80  
#define GRAVITY 4     

int fase_atual = 0;
bool menu_aberto = false;
char jogo_ativo = 0;
bool perdeu_fase2 = true;


void desenhar_menu(ALLEGRO_FONT* fonte) {
    al_clear_to_color(al_map_rgb(0, 0, 0));


    al_draw_text(fonte, al_map_rgb(255, 255, 255), 400, 200, ALLEGRO_ALIGN_CENTER, "1. Fase 1");
    al_draw_text(fonte, al_map_rgb(255, 255, 255), 400, 250, ALLEGRO_ALIGN_CENTER, "2. Fase 2");
    al_draw_text(fonte, al_map_rgb(255, 255, 255), 400, 300, ALLEGRO_ALIGN_CENTER, "3. Fase 3");
    al_draw_text(fonte, al_map_rgb(255, 255, 255), 400, 350, ALLEGRO_ALIGN_CENTER, "4. Voltar para casa");

    al_flip_display();
}
void jogo_dinossauro() {

const int LARGURA_TELA = 1000;
const int ALTURA_TELA = 300;
const float GRAVIDADE = 0.5;       
const float PULO_FORCA = -10.0;    
const float REDUCAO_GRAVIDADE = 0.2;  // reducao na gravidade quando segura o espaco
const float VELOCIDADE_CACTOS = -5.0;
const int LARGURA_CACTO = 40;
const int ALTURA_CACTO = 80;
const int LARGURA_DINO = 40;
const int ALTURA_DINO = 40;
const int NUM_CACTOS = 10; // numero de cactos no jogo
const int LARGURA_OBSTACULO_ESCALADO = 50;
const int ALTURA_OBSTACULO_ESCALADO = 80;

bool sair = false;
bool pulando = false;
bool no_chao = true;
bool segurando_espaco = false;  // para saber se a tecla espaco esta sendo pressionada
float dino_x = 100;
float dino_y = ALTURA_TELA - ALTURA_DINO - 10;
float velocidade_y = 0;

float cacto_x[10];
for (int i = 0; i < NUM_CACTOS; i++) {
    cacto_x[i] = LARGURA_TELA + (i * 500); // aumenta o espacamento entre cactos
}

int pontuacao = 0;
int tempo_passado = 0;
float fundo_x1 = 0, fundo_x2 = LARGURA_TELA; // posicoes dos fundos

ALLEGRO_DISPLAY* display = al_create_display(LARGURA_TELA, ALTURA_TELA);
ALLEGRO_EVENT_QUEUE* fila_eventos = al_create_event_queue();
ALLEGRO_TIMER* timer_jogo = al_create_timer(1.0 / 60.0);
ALLEGRO_TIMER* timer_tempo = al_create_timer(1.0); // temporizador de 1 segundo
ALLEGRO_FONT* fonte = al_create_builtin_font();
ALLEGRO_BITMAP* dino_sprite = al_load_bitmap("Imagens/Personagem/PGParado.png");
ALLEGRO_BITMAP* obstaculo = al_load_bitmap("Imagens/Dinossauro/Cactus.png");

al_register_event_source(fila_eventos, al_get_display_event_source(display));
al_register_event_source(fila_eventos, al_get_timer_event_source(timer_jogo));
al_register_event_source(fila_eventos, al_get_timer_event_source(timer_tempo));
al_register_event_source(fila_eventos, al_get_keyboard_event_source());

al_start_timer(timer_jogo);
al_start_timer(timer_tempo);

while (!sair) {
    ALLEGRO_EVENT evento;
    al_wait_for_event(fila_eventos, &evento);

    if (evento.type == ALLEGRO_EVENT_TIMER) {
        if (evento.timer.source == timer_jogo) {
            // atualizacao do dinossauro
            if (!no_chao) {
                // se o espaco estiver sendo segurado, a gravidade vai diminuir e para fazer o dinossauro descer mais devagar
                if (segurando_espaco) {
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

            // atualizacao dos cactos
            for (int i = 0; i < NUM_CACTOS; i++) {
                cacto_x[i] += VELOCIDADE_CACTOS;
                if (cacto_x[i] < -LARGURA_CACTO) {
                    bool posicao_valida = false;
                    while (!posicao_valida) {
                        // gere uma nova posição para o cacto
                        cacto_x[i] = LARGURA_TELA + (rand() % 600 + 200); // espaco maior entre os cactos

                        posicao_valida = true; // suponha que a posicao seja valida

                        // verifique a distancia entre o cacto atual e os outros
                        for (int j = 0; j < NUM_CACTOS; j++) {
                            if (j != i && abs(cacto_x[i] - cacto_x[j]) < LARGURA_CACTO + 50) {
                                posicao_valida = false; // posicao invalida, ta perto de outro cacto
                                break;
                            }
                        }
                    }
                    pontuacao++;
                }

                // verificacao de colisao com cada cacto
                if (dino_x + LARGURA_DINO > cacto_x[i] && dino_x < cacto_x[i] + LARGURA_CACTO &&
                    dino_y + ALTURA_DINO > ALTURA_TELA - ALTURA_CACTO) {
                    sair = true; 
                }
            }
            if (pontuacao >= 10) {
                sair = true;
            }

            // desenho na tela
            al_clear_to_color(al_map_rgb(255, 255, 255)); 
            al_draw_bitmap(dino_sprite, dino_x, dino_y, 0);
            //al_draw_filled_rectangle(dino_x, dino_y, dino_x + LARGURA_DINO, dino_y + ALTURA_DINO, al_map_rgb(0, 0, 255));
            for (int i = 0; i < NUM_CACTOS; i++) {
                al_draw_scaled_bitmap(
                    obstaculo, 0, 0,
                    al_get_bitmap_width(obstaculo),
                    al_get_bitmap_height(obstaculo),
                    cacto_x[i],
                    ALTURA_TELA - ALTURA_OBSTACULO_ESCALADO,
                    LARGURA_OBSTACULO_ESCALADO,
                    ALTURA_OBSTACULO_ESCALADO,
                    0
                );
            }
            al_draw_textf(fonte, al_map_rgb(0, 0, 0), 10, 10, 0, "Pontuacao: %d", pontuacao);
            al_draw_textf(fonte, al_map_rgb(0, 0, 0), 10, 30, 0, "Tempo: %d s", tempo_passado);
            al_flip_display();
        }
        else if (evento.timer.source == timer_tempo) {
            tempo_passado++;
            if (tempo_passado >= 10) {
                sair = true; 
            }
        }
    }

    // Controle do teclado
    if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
        if (evento.keyboard.keycode == ALLEGRO_KEY_SPACE && no_chao) {
            pulando = true;
            no_chao = false;
            velocidade_y = PULO_FORCA; // aplica a forca do pulo quando o espaço e pressionado
        }
        // quando a tecla espaço e mantida, a descida vai ser mais devagar
        if (evento.keyboard.keycode == ALLEGRO_KEY_SPACE && !no_chao) {
            segurando_espaco = true; // comeca a reduzir a gravidade
        }
    }

    // quando aperta espaço e solta a tecla de volta, volta ao normal a gravidade
    if (evento.type == ALLEGRO_EVENT_KEY_UP) {
        if (evento.keyboard.keycode == ALLEGRO_KEY_SPACE) {
            segurando_espaco = false; // para de reduzir a gravidade
        }
    }

    // aperta ESC pra sair do jogo (preciso fazer pra voltar pro menu ao inves de fechar a janela)
    if (evento.type == ALLEGRO_EVENT_KEY_DOWN && evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
        sair = true;
    }
}

al_clear_to_color(al_map_rgb(255, 255, 255));
al_draw_textf(fonte, al_map_rgb(0, 0, 0), 500, 150, ALLEGRO_ALIGN_CENTER, "Fim de jogo! Pontuacao final: %d", pontuacao);
al_flip_display();
al_rest(3.0); // espera 3 segundos pra fechar o jogo e mostra a pontuacao da pessoa

while (true) {
    ALLEGRO_EVENT evento;
    al_wait_for_event(fila_eventos, &evento);
    if (evento.type == ALLEGRO_EVENT_KEY_DOWN && evento.keyboard.keycode == ALLEGRO_KEY_ENTER) {
        break; // sai do loop e reinicia o jogo de novo
    }
    //if (evento.type == ALLEGRO_EVENT_KEY_DOWN && evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
        //menu_aberto = true;
        //break;
    //}
}

jogo_dinossauro();

al_destroy_display(display);
al_destroy_event_queue(fila_eventos);
al_destroy_timer(timer_jogo);
al_destroy_timer(timer_tempo);
al_destroy_font(fonte);

}

ALLEGRO_BITMAP* Blsolido;
ALLEGRO_BITMAP* Bldestrutivel;
ALLEGRO_BITMAP* Objetivo;
ALLEGRO_BITMAP* persona;
ALLEGRO_BITMAP* livro;
const int linhas = 15;
const int colunas = 20;
int board[15][20];
bool sair = false;
bool sairBomber = true;
int inicialX = 1;
int inicialY = 1;

int Bomberman(int a) {
    float frame = 0;
    srand(time(NULL));
    persona = al_load_bitmap("Imagens/Personagem/PGParado.png");
    Blsolido = al_load_bitmap("Imagens/Bomberman/parede.png");
    Bldestrutivel = al_load_bitmap("Imagens/Bomberman/paredeQuebravel.png");
    livro = al_load_bitmap("Imagens/Cenas/livrop.png");

    ALLEGRO_DISPLAY* bomber = al_create_display(800, 600);
    ALLEGRO_EVENT_QUEUE* fila_de_eventos = al_create_event_queue();
    al_register_event_source(fila_de_eventos, al_get_display_event_source(bomber));
    al_register_event_source(fila_de_eventos, al_get_keyboard_event_source());

    // Inicialização do tabuleiro
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            int n1 = rand() % 10;
            board[i][j] = (n1 % 4 == 0) ? 1 : 0; // 1: Bloco sólido, 0: Espaço vazio
        }
    }
    for (int i = 0; i < linhas; i += 2) {
        for (int j = 0; j < colunas; j += 2) {
            if (board[i][j] == 0) board[i][j] = 2; // Bloco destrutível
        }
    }

    board[inicialY][inicialX] = 3;           // Posição inicial do jogador
    board[linhas - 1][colunas - 1] = 4;      // Objetivo

    while (!sairBomber) {
        // Desenha o tabuleiro
        for (int i = 0; i < linhas; i++) {
            for (int j = 0; j < colunas; j++) {
                ALLEGRO_BITMAP* imagens = NULL;

                switch (board[i][j]) {
                case 0:
                    al_draw_filled_rectangle(j * 40, i * 40, (j + 1) * 40, (i + 1) * 40, al_map_rgb(0, 0, 0));
                    break; // Espaço vazio

                case 1:
                    imagens = Blsolido;
                    //al_draw_filled_rectangle(j * 40, i * 40, (j + 1) * 40, (i + 1) * 40, al_map_rgb(255, 255, 255));

                    break;       // Bloco sólido

                case 2:
                    imagens = Bldestrutivel;
                    //al_draw_filled_rectangle(j * 40, i * 40, (j + 1) * 40, (i + 1) * 40, al_map_rgb(200,30,22));
                    break;     // Bloco destrutível
                case 3:
                    //al_draw_filled_rectangle(j * 40, i * 40, (j + 1) * 40, (i + 1) * 40, al_map_rgb(0, 0, 255));

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
        }
        al_flip_display();
        frame += 0.3;
        if ((int)frame >= NUM_FRAMES) {
            frame = 0;
        }
        // Processamento de eventos
        ALLEGRO_EVENT event;
        al_wait_for_event(fila_de_eventos, &event);

        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            int dx = 0, dy = 0;
            switch (event.keyboard.keycode) {
            case ALLEGRO_KEY_RIGHT: dx = 1; break;
            case ALLEGRO_KEY_LEFT:  dx = -1; break;
            case ALLEGRO_KEY_UP:    dy = -1; break;
            case ALLEGRO_KEY_DOWN:  dy = 1; break;
            case ALLEGRO_KEY_R: // Reinicializa o tabuleiro
                al_destroy_display(bomber);
                Bomberman(a);
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
                    board[inicialY][inicialX - 1] == 0;
                }
                break;
            }


        }
        if (board[inicialY][inicialX] == board[linhas - 1][colunas - 1]) {
            printf("Voce conseguiu");
            //fase_atual = casa;
            sairBomber = true;
            return 0;
        }

        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            sair = true;
            return 0;
        }
    }
    al_destroy_bitmap(livro);
    al_destroy_bitmap(Bldestrutivel);
    al_destroy_bitmap(Blsolido);
    
    al_destroy_display(bomber);
    al_destroy_event_queue(fila_de_eventos);
}

int main() {
    al_init();
    al_init_primitives_addon();
    al_install_keyboard();
    al_init_image_addon();
    al_init_font_addon();
    al_install_mouse();
   
    ALLEGRO_FONT* fonte = al_create_builtin_font();

    ALLEGRO_DISPLAY* disp = al_create_display(800, 600);
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE* fila_eventos = al_create_event_queue();
    al_set_window_title(disp, "Tale of Second War");

    bool sair = false;
    bool on_homepage = true;
    float x = 30;
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

    ALLEGRO_BITMAP* casa = al_load_bitmap("Imagens\\Cenas\\casa.png");
    ALLEGRO_BITMAP* Personagem = al_load_bitmap("Imagens\\Personagem\\Walking.png");
    ALLEGRO_BITMAP* Pulando = al_load_bitmap("Imagens\\Personagem\\Pulando.png");
    ALLEGRO_BITMAP* Parado = al_load_bitmap("Imagens\\Personagem\\PGParado.png");
    ALLEGRO_BITMAP* AndandoESQ = al_load_bitmap("Imagens\\Personagem\\WalkingLeft.png");
    ALLEGRO_BITMAP* Homepage = al_load_bitmap("Imagens\\cenas\\bg2.jpg");
    ALLEGRO_BITMAP* cenario1 = al_load_bitmap("Imagens\\cenario1.png");
    ALLEGRO_BITMAP* cenario2 = al_load_bitmap("Imagens\\cenario2.png");
    ALLEGRO_BITMAP* cenario3 = al_load_bitmap("Imagens\\cenario3.png");
    ALLEGRO_BITMAP* mapa = al_load_bitmap("Imagens\\Cenas\\mapa.png");
    ALLEGRO_BITMAP* introducao1= al_load_bitmap("Imagens\\introducao\\introducao.png");
    ALLEGRO_BITMAP* introducao2 = al_load_bitmap("Imagens\\introducao\\introducao2.png");
    ALLEGRO_BITMAP* introducao3 = al_load_bitmap("Imagens\\introducao\\introducao3.png");
    ALLEGRO_BITMAP* introducao4 = al_load_bitmap("Imagens\\introducao\\introducao4.png");
    ALLEGRO_BITMAP* introducao5 = al_load_bitmap("Imagens\\introducao\\introducao5.png");
    ALLEGRO_BITMAP* introducao6 = al_load_bitmap("Imagens\\introducao\\introducao6.png");

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
                al_draw_scaled_bitmap(introducao1, 0, 0, al_get_bitmap_width(introducao1), al_get_bitmap_height(introducao1), 0, 0, 800, 600, 0);
                al_flip_display();

                
               if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && evento.mouse.button == 1) { 

                   al_draw_scaled_bitmap(introducao2, 0, 0, al_get_bitmap_width(introducao2), al_get_bitmap_height(introducao2), 0, 0, 800, 600, 0);
                  
                }
            }



            else if (menu_aberto) {
                desenhar_menu(fonte);
            }

            else {

                ALLEGRO_BITMAP* cenario_atual;
                if (fase_atual == 1) {
                    cenario_atual = cenario1;
                }
                else if (fase_atual == 2) {
                    cenario_atual = casa;

                    if (perdeu_fase2) {
                        fase_atual = 0;
                        perdeu_fase2 = false;
                    }
                }
                else if (fase_atual == 3) {
                    cenario_atual = cenario3;
                }
                else {
                    cenario_atual = casa;
                }


                al_draw_scaled_bitmap(cenario_atual, 0, 0, al_get_bitmap_width(cenario_atual), al_get_bitmap_height(cenario_atual), 0, 0, 800, 600, 0);
                al_draw_filled_circle(maquina_x, maquina_y, maquina_raio, al_map_rgba(0, 0, 0, 0));




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


                al_flip_display();
            }
        }

        if (introducao) {
         //   al_draw_scaled_bitmap(introducao1, 0, 0, al_get_bitmap_width(introducao1), al_get_bitmap_height(introducao1), 0, 0, 800, 600, 0);
         //   al_flip_display();

            if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && evento.mouse.button == 1) {
                if (evento.mouse.x >= fase10_x && evento.mouse.x <= fase10_x + fase10_largura &&
                    evento.mouse.y >= fase10_y && evento.mouse.y <= fase10_y + fase10_altura) {
                    if (introducao = true) {

                        al_draw_scaled_bitmap(introducao2, 0, 0, al_get_bitmap_width(introducao2), al_get_bitmap_height(introducao2), 0, 0, 800, 600, 0);
                    }
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

                    fase_atual = 1;
                    mapa_visivel = false;
                    jogo_dinossauro();
                }

                else if (mouse_x >= fase2_x && mouse_x <= fase2_x + fase2_largura && mouse_y >= fase2_y && mouse_y <= fase2_y + fase2_altura) {
                    printf("Fase 2 clicada!\n");

                    fase_atual = 2;
                    mapa_visivel = false;
                    jogo_ativo = true;
                    system("C:\\Users\\Leon\\source\\repos\\spaceinvaders\\spaceinvaders\\x64\\Debug\\spaceinvaders.exe");
                    jogo_ativo = 1;
                }

                else if (mouse_x >= fase3_x && mouse_x <= fase3_x + fase3_largura && mouse_y >= fase3_y && mouse_y <= fase3_y + fase3_altura) {
                    printf("Fase 3 clicada!\n");

                    fase_atual = 3;
                    sairBomber = false;
                    mapa_visivel = false;
                    int n = Bomberman(1);
                    if (n == 0) {
                        menu_aberto = false;
                        printf("\nMenuFechado");
                    }

                    fase_atual = casa;
                }
            }
        }

        if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && on_homepage && evento.mouse.button == 1) {
            if (evento.mouse.x >= botaox && evento.mouse.x <= botaox + largura &&
                evento.mouse.y >= botaoy && evento.mouse.y <= botaoy + altura) {
                on_homepage = false;  // Fecha a homepage
                introducao = true;    // Ativa a introdução
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