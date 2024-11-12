#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <stdbool.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <allegro5/keyboard.h>
#include <time.h>


#define FRAME_WIDTH 44   
#define FRAME_HEIGHT 48  
#define NUM_FRAMES 4     
#define FRAME_DURATION 0.2 
#define JUMP_HEIGHT 80  
#define GRAVITY 4 


//varaiveis globais
ALLEGRO_DISPLAY* disp;
ALLEGRO_BITMAP* Blvazio;
ALLEGRO_BITMAP* Blsolido;
ALLEGRO_BITMAP* Bldestrutivel;
ALLEGRO_BITMAP* Objetivo;
ALLEGRO_BITMAP* persona;

int livros = 0;
const float FPS = 60.0;
const int screen_w = 800;
const int screen_h = 600;
const int tamanho_jogador = 32;
const int tamanho_inimigo = 32;

const int linhas = 15;
const int colunas = 20;

int board[linhas][colunas];
bool sair = false;
bool sairBomber = true;
int inicialX = 1;
int inicialY = 1;

typedef struct {
    float x, y;
    float vel_jogador;
} Player;

typedef struct {
    float x, y;
    bool active;
} Tiro;

typedef struct {
    double x;
    double y;
    bool active;
} Inimigo;

int fase_atual = 0;
bool menu_aberto = false;

int Bomberman(int a) {
    float frame = 0;
    srand(time(NULL));
    Blvazio = al_load_bitmap("F:/teste giti/Marco-1/Marco-1/imagem/vazio.png");
    persona = al_load_bitmap("F:/teste giti/Marco-1/Marco-1/imagem/PGParado.png");
    Blsolido = al_load_bitmap("F:/teste giti/Marco-1/Marco-1/imagem/parede.png");
    Bldestrutivel = al_load_bitmap("F:/teste giti/Marco-1/Marco-1/imagem/arbusto.png");


    ALLEGRO_DISPLAY* bomber = al_create_display(screen_w, screen_h);
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
            if (board[i][j] == 0) {
                board[i][j] = 2; // Bloco destrutível
            }
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
                    al_draw_filled_rectangle(j * 40, i * 40, (j + 1) * 40, (i + 1) * 40, al_map_rgb(255, 255, 255));
                    break; // Espaço vazio

                case 1:
                    imagens = Blsolido;
                    break;       // Bloco sólido

                case 2:
                    imagens = Bldestrutivel;
                    break;     // Bloco destrutível
                     case 3: 
                         imagens = persona; 
                         break;     // Jogador
                    // case 4: 
                       //  color = al_map_rgb(0, 255, 255); 
                      //   break;   // Objetivo
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
            case ALLEGRO_KEY_RIGHT: 
                dx = 1; 
                break;

            case ALLEGRO_KEY_LEFT:  
                dx = -1; 
                break;
            case ALLEGRO_KEY_UP:    
                dy = -1; 
                break;
            case ALLEGRO_KEY_DOWN:  
                dy = 1; 
                break;

            //case ALLEGRO_KEY_SPACE: // Reinicializa o tabuleiro
               // al_destroy_display(bomber);
               // Bomberman(a);
               // return 1;
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

        if (board[inicialY][inicialX] == board[linhas - 14][colunas - 18]) {
            printf("Voce conseguiu");
            sairBomber = true;
            return 0;
        }
    }
    al_destroy_bitmap(Bldestrutivel);
    al_destroy_bitmap(Blsolido);
    al_destroy_bitmap(Blvazio);
    al_destroy_display(bomber);
    al_destroy_event_queue(fila_de_eventos);
}

void desenhar_menu(ALLEGRO_FONT* fonte) {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_text(fonte, al_map_rgb(255, 255, 255), 400, 200, ALLEGRO_ALIGN_CENTER, "1. Fase 1");
    al_draw_text(fonte, al_map_rgb(255, 255, 255), 400, 250, ALLEGRO_ALIGN_CENTER, "2. Fase 2");
    al_draw_text(fonte, al_map_rgb(255, 255, 255), 400, 300, ALLEGRO_ALIGN_CENTER, "3. Fase 3");
    al_draw_text(fonte, al_map_rgb(255, 255, 255), 400, 350, ALLEGRO_ALIGN_CENTER, "4. Voltar para casa");
    al_draw_textf(fonte, al_map_rgb(255, 255, 255), 400, 400, ALLEGRO_ALIGN_CENTER, "Livros:%d", livros);
    al_flip_display();
}

void spaceInvaders() {
    ALLEGRO_DISPLAY* display = al_create_display(screen_w, screen_h);
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();

    // Objetos
    Player jogador = { screen_w / 2 - tamanho_jogador / 2, screen_h - tamanho_jogador, 5.0 };
    Tiro bala = { jogador.x, jogador.y, false };
    Inimigo inimigos[5];
    bool andar_esquerda = false;
    bool andar_direita = false;

    // Inicializando os inimigos
    for (int i = 0; i < 5; i++) {
        inimigos[i].x = i * 100 + 50.0;
        inimigos[i].y = 50.0;
        inimigos[i].active = true;
    }

    // Registrar eventos
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_start_timer(timer);
    bool redraw = true;
    bool Sair_jogo = false;

    // Laço principal
    while (!Sair_jogo) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        //Condição para checar a tecla apertada
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
        //Checa se a tecla foi solta
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

        if (ev.type == ALLEGRO_EVENT_TIMER) {

            // Atualizar posição do jogador com base em teclas pressionadas
            if (andar_esquerda == true) {
                jogador.x -= jogador.vel_jogador;
            }
            if (andar_direita == true) {
                jogador.x += jogador.vel_jogador;
            }

            // Atualizar posição do tiro
            if (bala.active) {
                bala.y -= 5.0;
                if (bala.y < 0) {
                    bala.active = false;
                }
            }
            
            // Verificar se o tiro pegou em algum inimigo
            for (int i = 0; i < 5; i++) {
                if (inimigos[i].active && bala.active &&
                    bala.x < inimigos[i].x + tamanho_inimigo &&
                    bala.x + 5 > inimigos[i].x &&
                    bala.y < inimigos[i].y + tamanho_inimigo &&
                    bala.y + 10 > inimigos[i].y) {
                    inimigos[i].active = false;
                    bala.active = false;

                }
                
            }

            redraw = true;
            
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            Sair_jogo = true;
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            // Atirando
            if (ev.keyboard.keycode == ALLEGRO_KEY_SPACE && !bala.active) {
                bala.x = jogador.x + tamanho_jogador / 2;
                bala.y = jogador.y;
                bala.active = true;
            }
        }

        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;

            // Desenhar tudo
            al_clear_to_color(al_map_rgb(0, 0, 0));

            // Desenhar player
            al_draw_filled_rectangle(jogador.x, jogador.y, jogador.x + tamanho_jogador, jogador.y + tamanho_jogador, al_map_rgb(0, 255, 0));

            // Desenhar tiro
            if (bala.active) {
                al_draw_filled_rectangle(bala.x, bala.y, bala.x + 5, bala.y + 10, al_map_rgb(255, 255, 255));
            }

            // Desenhar inimigos
            for (int i = 0; i < 5; i++) {
                if (inimigos[i].active) {
                    al_draw_filled_rectangle(inimigos[i].x, inimigos[i].y, inimigos[i].x + tamanho_inimigo, inimigos[i].y + tamanho_inimigo, al_map_rgb(255, 0, 0));
                }
            }

            al_flip_display();
        }

    }

    // Finalizando o Allegro
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
}

int main() {
    al_init();
    al_install_mouse();
    al_install_keyboard();
    al_init_image_addon();
    al_init_font_addon();
    al_init_primitives_addon();
    al_init_ttf_addon();

    ALLEGRO_FONT* fonte = al_create_builtin_font();
    int cot = 0;
     disp = al_create_display(800, 600);
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE* fila_eventos = al_create_event_queue();

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
    //F:/vscode/repos/jogo/Sprites/projeto/Nova pasta/Sprites/
    //C:/Users/guilherme.basilva1/source/repos/Sprites/
     
    ALLEGRO_FONT* Bfont = NULL;

    ALLEGRO_BITMAP* casa = al_load_bitmap("F:/vscode/repos/jogo/Sprites/projeto/Nova pasta/Sprites/casa.png");
    ALLEGRO_BITMAP* Personagem = al_load_bitmap("F:/vscode/repos/jogo/Sprites/projeto/Nova pasta/Sprites/Walking.png");
    ALLEGRO_BITMAP* Pulando = al_load_bitmap("F:/vscode/repos/jogo/Sprites/projeto/Nova pasta/Sprites/Pulando.png");
    ALLEGRO_BITMAP* Parado = al_load_bitmap("F:/vscode/repos/jogo/Sprites/projeto/Nova pasta/Sprites/PGParado.png");
    ALLEGRO_BITMAP* AndandoESQ = al_load_bitmap("F:/vscode/repos/jogo/Sprites/projeto/Nova pasta/Sprites/WalkingLeft.png");
    ALLEGRO_BITMAP* Homepage = al_load_bitmap("F:/vscode/repos/jogo/Sprites/projeto/Nova pasta/Sprites/bg2.jpg");
    ALLEGRO_BITMAP* cenario1 = al_load_bitmap("F:/vscode/repos/jogo/Sprites/projeto/Nova pasta/Sprites/cenario1.png");
    ALLEGRO_BITMAP* cenario2 = al_load_bitmap("F:/vscode/repos/jogo/Sprites/projeto/Nova pasta/Sprites/cenario2.png");
    ALLEGRO_BITMAP* cenario3 = al_load_bitmap("F:/vscode/repos/jogo/Sprites/projeto/Nova pasta/Sprites/cenario3.png");

    al_register_event_source(fila_eventos, al_get_display_event_source(disp));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_mouse_event_source());
    //al_set_system_mouse_cursor(disp, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);

    al_start_timer(timer);

    while (!sair) {

        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);

        if (evento.type == ALLEGRO_EVENT_TIMER) {
            
            if (on_homepage) {

                al_draw_scaled_bitmap(Homepage, 0, 0, al_get_bitmap_width(Homepage), al_get_bitmap_height(Homepage), 0, 0, 800, 600, 0);
                al_flip_display();
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
                    cenario_atual = cenario2;
                }
                else if (fase_atual == 3) {
                    cenario_atual = cenario3;
                }
                else if (fase_atual == -1) {
                    on_homepage = true;
                    fase_atual = 0;
                    continue;
                }

                else {
                    cenario_atual = casa;
                }

                al_draw_scaled_bitmap(cenario_atual, 0, 0, al_get_bitmap_width(cenario_atual), al_get_bitmap_height(cenario_atual), 0, 0, 800, 600, 0);
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
                frame += 0.3;
                if ((int)frame >= NUM_FRAMES) {
                    frame = 0;
                }
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
        
        if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            if (on_homepage && evento.mouse.button == 1) {
                if (evento.mouse.x >= botaox && evento.mouse.x <= botaox + largura &&
                    evento.mouse.y >= botaoy && evento.mouse.y <= botaoy + altura) {
                    on_homepage = false;
                }
            }
        }


        if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            if (!on_homepage) {
                if(
                   evento.mouse.x >= 309 && evento.mouse.x <= 383  &&
                   evento.mouse.y >= 440 && evento.mouse.y <= 523
                    ){
                    menu_aberto = !menu_aberto;
                }
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

                case ALLEGRO_KEY_ESCAPE:

                    menu_aberto = !menu_aberto;
                    break;

                case ALLEGRO_KEY_1:
                    if (menu_aberto) {
                        fase_atual = 1;
                        menu_aberto = false;
                    }
                    break;

                case ALLEGRO_KEY_2:
                    if (menu_aberto) {
                        spaceInvaders();
                    }
                    break;

                case ALLEGRO_KEY_3:
                    if (menu_aberto) {
                        sairBomber = false;
                        int n = Bomberman(1);
                        if (n == 0) {
                            fase_atual =0;
                            printf("\nMenuFechado");
                            livros++;
                        }
                       
                    }
                    break;

                case ALLEGRO_KEY_4:
                    if (menu_aberto) {
                        fase_atual = 0;
                    }
                    break;

                case ALLEGRO_KEY_5:
                    if (menu_aberto) {
                        fase_atual = -1;
                        menu_aberto = false;
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
    al_destroy_bitmap(Bldestrutivel);
    al_destroy_bitmap(Blsolido);
    al_destroy_bitmap(Blvazio);

    return 0;
}
