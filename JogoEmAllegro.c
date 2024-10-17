#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <stdbool.h>

#define FRAME_WIDTH 44   
#define FRAME_HEIGHT 48  
#define NUM_FRAMES 4     
#define FRAME_DURATION 0.2 
#define JUMP_HEIGHT 100  
#define GRAVITY 4        

int main() {
    al_init();
    al_install_keyboard();
    al_init_image_addon();

    ALLEGRO_DISPLAY* disp = al_create_display(600, 400);
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE* fila_eventos = al_create_event_queue();

    bool sair = false;
    bool on_homepage = true; 
    float x = 30;
    float y = 270;
    float frame = 0;

    bool pulando = false;
    float jump_speed = 0;
    float jump_start_y = y;

    bool andando_esquerda = false;
    bool andando_direita = false;
    bool parado = true;

    
    ALLEGRO_BITMAP* imagem = al_load_bitmap("F:/vscode/repos/jogo/Sprites/imagem/BG.jpg");
    ALLEGRO_BITMAP* Personagem = al_load_bitmap("F:/vscode/repos/jogo/Sprites/imagem/Walking.png");
    ALLEGRO_BITMAP* Pulando = al_load_bitmap("F:/vscode/repos/jogo/Sprites/imagem/Pulando.png");
    ALLEGRO_BITMAP* Parado = al_load_bitmap("F:/vscode/repos/jogo/Sprites/imagem/PGParado.png"); 
    ALLEGRO_BITMAP* AndandoESQ = al_load_bitmap("F:/vscode/repos/jogo/Sprites/imagem/WalkingLeft.png");
    ALLEGRO_BITMAP* Homepage = al_load_bitmap("F:/vscode/repos/jogo/Sprites/imagem/bg2.jpg");

    al_register_event_source(fila_eventos, al_get_display_event_source(disp));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));

    al_start_timer(timer);

    while (!sair) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);

        if (evento.type == ALLEGRO_EVENT_TIMER) {
            
            if (on_homepage) {
                
                al_draw_scaled_bitmap(Homepage, 0, 0, al_get_bitmap_width(Homepage), al_get_bitmap_height(Homepage), 0, 0, 600, 400, 0);
                al_flip_display();
            }
            else {
               
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

            
                al_draw_scaled_bitmap(imagem, 0, 0, al_get_bitmap_width(imagem), al_get_bitmap_height(imagem), 0, 0, 600, 400, 0);

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
        else if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (on_homepage && evento.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                on_homepage = false; 
            }
            else if (!on_homepage) {
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

   
    al_destroy_bitmap(Personagem);
    al_destroy_bitmap(imagem);
    al_destroy_bitmap(Homepage);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(fila_eventos);

    return 0;
}
