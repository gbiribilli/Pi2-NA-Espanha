#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdlib.h>
#include <time.h>

//Constantes legais
const float FPS = 60.0;
const int screen_w = 800;
const int screen_h = 600;
const int tamanho_jogador = 32;
const int tamanho_inimigo = 32;
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
    al_init_primitives_addon();

    ALLEGRO_DISPLAY* display = al_create_display(screen_w, screen_h);
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();

    //Valores de player/tiro
    Player jogador = { screen_w / 2 - tamanho_jogador / 2, screen_h - tamanho_jogador, 5.0 };
    Tiro bala = { jogador.x, jogador.y, false };


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

    al_start_timer(timer);
    bool redraw = true;
    bool Sair_jogo = false;

    while (!Sair_jogo) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

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

                        system("C:\\Users\\Leon\\source\\repos\\JogoEmAllegro\\Debug\\JogoEmAllegro.exe");
                        Sair_jogo = true;
                    }
                }
            }

            // Se algum inimigo atingiu a borda, todos mudam de direção e descem
            if (mudar_direcao) {
                direcao_inimigo *= -1;
                for (int i = 0; i < 5; i++) {
                    inimigos[i].y += tamanho_inimigo / 2;
                }
            }

            redraw = true;
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            Sair_jogo = true;
        }

        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;
            al_clear_to_color(al_map_rgb(0, 0, 0));

            //Desenha o jogador
            al_draw_filled_rectangle(jogador.x, jogador.y, jogador.x + tamanho_jogador, jogador.y + tamanho_jogador, al_map_rgb(0, 255, 0));

            //Desenha o tiro do jogador
            if (bala.active) {
                al_draw_filled_rectangle(bala.x, bala.y, bala.x + 5, bala.y + 10, al_map_rgb(255, 255, 255));
            }

            //Desenha os inimigos e tiros deles
            for (int i = 0; i < 5; i++) {
                if (inimigos[i].active) {
                    al_draw_filled_rectangle(inimigos[i].x, inimigos[i].y, inimigos[i].x + tamanho_inimigo, inimigos[i].y + tamanho_inimigo, al_map_rgb(255, 0, 0));
                }

                if (inimigos[i].tiro_inimigo.active) {
                    al_draw_filled_rectangle(inimigos[i].tiro_inimigo.x, inimigos[i].tiro_inimigo.y, inimigos[i].tiro_inimigo.x + 5, inimigos[i].tiro_inimigo.y + 10, al_map_rgb(255, 255, 0));
                }
            }

            al_flip_display();
        }
    }

    //Finalização e limpeza de memória
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);

    return 0;
}