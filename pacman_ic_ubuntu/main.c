#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define MAXLINHAS 20
#define MAXCOLS 31
#define RANGE 10
#define MUSICA if (1)
#define FANTASMAS if (1)
#define FANT1 if (1)
#define FANT2 if (1)
#define FANT3 if (1)
#define FANT4 if (1)

char mapa[MAXLINHAS][MAXCOLS] = {
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "X...........X   X...........X",
    "X.XXX.XXXXX.X   X.XXXXX.XXX.X",
    "X.XXX.XXXXX.XXXXX.XXXXX.XXX.X",
    "X...........................X",
    "X.XXX.XX.XXXXXXXXXXX.XX.XXX.X",
    "X.....XX.....X X.....XX.....X",
    "X.XXX.XXXXXX.XXX.XXXXXX.XXX.X",
    "X.XXX.XX......F......XX.XXX.X",
    "W.....XX.XXXXXXXXXXX.XX.....W",
    "X.XXX.XX.XXXXXXXXXXX.XX.XXX.X",
    "X.X X.XX.............XX.X X.X",
    "X.XXX.XXXXXX.XXX.XXXXXX.XXX.X",
    "X.....XX.....X X.....XX.....X",
    "X.XXX.XX.XXXXXXXXXXX.XX.XXX.X",
    "X.X X.........F.........X X.X",
    "X.XXX.XXXXXXXXXXXXXXXXX.XXX.X",
    "X............. .............X",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
};

char mapa_backup[MAXLINHAS][MAXCOLS] = {
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "X...........X   X...........X",
    "X.XXX.XXXXX.X   X.XXXXX.XXX.X",
    "X.XXX.XXXXX.XXXXX.XXXXX.XXX.X",
    "X...........................X",
    "X.XXX.XX.XXXXXXXXXXX.XX.XXX.X",
    "X.....XX.....X X.....XX.....X",
    "X.XXX.XXXXXX.XXX.XXXXXX.XXX.X",
    "X.XXX.XX......F......XX.XXX.X",
    "W.....XX.XXXXXXXXXXX.XX.....W",
    "X.XXX.XX.XXXXXXXXXXX.XX.XXX.X",
    "X.X X.XX.............XX.X X.X",
    "X.XXX.XXXXXX.XXX.XXXXXX.XXX.X",
    "X.....XX.....X X.....XX.....X",
    "X.XXX.XX.XXXXXXXXXXX.XX.XXX.X",
    "X.X X.........F.........X X.X",
    "X.XXX.XXXXXXXXXXXXXXXXX.XXX.X",
    "X............. .............X",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
};


//Ponteiros para parte audio-visual
ALLEGRO_FONT *fonte = NULL;
ALLEGRO_FONT *fonte2 = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_DISPLAY *display = NULL;

ALLEGRO_BITMAP *mapa_png = NULL;
ALLEGRO_BITMAP *parede = NULL;
ALLEGRO_BITMAP *pacbmp = NULL;
ALLEGRO_BITMAP *pacgodbmp = NULL;
ALLEGRO_BITMAP *comida = NULL;
ALLEGRO_BITMAP *fruta = NULL;
ALLEGRO_BITMAP *inimigobmp = NULL;
ALLEGRO_BITMAP *inimigobmp2 = NULL;
ALLEGRO_BITMAP *inimigobmp3 = NULL;
ALLEGRO_BITMAP *inimigobmp4 = NULL;
ALLEGRO_BITMAP *mortebmp = NULL;
ALLEGRO_BITMAP *banner = NULL;
ALLEGRO_BITMAP *banner2 = NULL;
ALLEGRO_BITMAP *bannerWin = NULL;
ALLEGRO_BITMAP *icone = NULL;

ALLEGRO_SAMPLE *musica_inicio = NULL;
ALLEGRO_SAMPLE *musica_comendo = NULL;
ALLEGRO_SAMPLE *musica_morte = NULL;
FILE * aR;
int score, contaMusica, godMode, godCount;


//Variáveis de controle
int dir = 4;
int px = 30 * 14, py = 30 * 17;
int px_anterior, py_anterior;
int pontos = 0;
int livre1 = 0, livre2 = 0, livre3 = 0, livre4 = 0;
ALLEGRO_BITMAP *bitmapf1 = NULL;
ALLEGRO_BITMAP *bitmapf2 = NULL;
ALLEGRO_BITMAP *bitmapf3 = NULL;
ALLEGRO_BITMAP *bitmapf4 = NULL;

struct inimigo {
    int fdir;
    int _x;
    int _y;
};

struct inimigo inimigos[4];


//Declarações de funções
bool inicializar();
void desenhar_mapa();
void desenhar_personagem();
bool game_over();
void desenhar_fantasma();
void fantasma();
void choque_pacman();
void reset_mapa();
void inicializar_fantasmas();
void recorde();
void salvaRecorde();

int main(void)
{
    bool sair = false;
    int opMenu = 1;
    godMode = godCount = 0;

    recorde();

    if (!inicializar())
    {
        return -1;
    }


    srand (time(NULL));

    //Carrega todos os arquivos necessários para os fantasmas

    inicializar_fantasmas();

    //Carrega arquivos que serão ultilizados na parte audio-visual

    mapa_png = al_load_bitmap("./images/mapa-pacman.png");
    parede = al_load_bitmap("./images/parede.png");
    pacbmp = al_load_bitmap("./images/pacman.png");
    pacgodbmp = al_load_bitmap("./images/pacman-god.png");
    comida = al_load_bitmap("./images/comida.png");
    fruta = al_load_bitmap("./images/fruta.png");
    mortebmp = al_load_bitmap("./images/morte.png");
    banner = al_load_bitmap("./images/menu-pacman-1.png");
    banner2 = al_load_bitmap("./images/menu-pacman-2.png");
    bannerWin = al_load_bitmap("./images/pacman-win.png");
    icone = al_load_bitmap("./images/ico.png");

    musica_comendo = al_load_sample("./sounds/comendo.wav");
    musica_morte = al_load_sample("./sounds/morte.wav");
    musica_inicio = al_load_sample("./sounds/comeco.wav");

    fprintf(stderr, "Jogo Inicializado\n");
    al_set_display_icon(display, icone);

    /* MENU PRINCIPAL */

Menu:

    al_clear_to_color(al_map_rgb(0,0,0));
    if (opMenu == 1)
        al_draw_bitmap(banner, 0, 0, 0);

    al_flip_display();

    while (!sair)
    {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);

        if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            switch(evento.keyboard.keycode)
            {
            case ALLEGRO_KEY_UP:
                opMenu--;
                if (opMenu < 1) opMenu = 2;
                break;
            case ALLEGRO_KEY_DOWN:
                opMenu++;
                if (opMenu > 2) opMenu = 1;
                break;
            case ALLEGRO_KEY_ENTER:
ALLEGRO_KEY_PAD_ENTER:
                if (opMenu == 1) sair = true;
                else if (opMenu == 2)
                {
                    al_destroy_display(display);
                    al_destroy_event_queue(fila_eventos);
                    al_destroy_font(fonte);
                    al_destroy_bitmap(parede);
                    al_destroy_bitmap(pacbmp);
                    al_destroy_bitmap(bitmapf1);
                    al_destroy_bitmap(bitmapf2);
                    al_destroy_bitmap(bitmapf3);
                    al_destroy_bitmap(bitmapf4);
                    al_destroy_bitmap(comida);
                    al_destroy_bitmap(mortebmp);
                    return 0;
                }
                break;
            case ALLEGRO_KEY_ESCAPE:
                al_destroy_display(display);
                al_destroy_event_queue(fila_eventos);
                al_destroy_font(fonte);
                al_destroy_bitmap(parede);
                al_destroy_bitmap(pacbmp);
                al_destroy_bitmap(bitmapf1);
                al_destroy_bitmap(bitmapf2);
                al_destroy_bitmap(bitmapf3);
                al_destroy_bitmap(bitmapf4);
                al_destroy_bitmap(comida);
                al_destroy_bitmap(mortebmp);
                return 0;
                break;
            }
        }
        else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            al_destroy_display(display);
            al_destroy_event_queue(fila_eventos);
            al_destroy_font(fonte);
            al_destroy_bitmap(parede);
            al_destroy_bitmap(pacbmp);
            al_destroy_bitmap(bitmapf1);
            al_destroy_bitmap(bitmapf2);
            al_destroy_bitmap(bitmapf3);
            al_destroy_bitmap(bitmapf4);
            al_destroy_bitmap(comida);
            al_destroy_bitmap(mortebmp);
            return 0;
        }

        al_clear_to_color(al_map_rgb(0,0,0));
        if (opMenu == 1)
        {
            al_draw_bitmap(banner, 0, 0, 0);
        }
        else
        {
            al_draw_bitmap(banner2, 0, 0, 0);
        }
        al_flip_display();
    }

    sair = false;

    /* LOOP PRINCIPAL DO JOGO */

    MUSICA al_play_sample(musica_inicio, 0.6, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
    contaMusica = 0;

    while (!sair && game_over())
    {
        px_anterior = px;
        py_anterior = py;

        while (!al_is_event_queue_empty(fila_eventos))
        {
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);

            if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                switch(evento.keyboard.keycode)
                {
                case ALLEGRO_KEY_RIGHT:
                    dir = 1;
                    break;
                case ALLEGRO_KEY_LEFT:
                    dir = 0;
                    break;
                case ALLEGRO_KEY_UP:
                    dir = 2;
                    break;
                case ALLEGRO_KEY_DOWN:
                    dir = 3;
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    goto Menu;
                    return 0;
                    break;
                }
            }
            else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                sair = true;
            }
        }

        if (dir == 0)
        {
            if (mapa[py / 30][(px - 30) / 30] != 'X')
                px -= 30;
            else
                dir = 4;
        }
        if (dir == 1)
        {
            if (mapa[py / 30][(px + 30) / 30] != 'X')
                px += 30;
            else
                dir = 4;
        }
        if (dir == 2)
        {
            if (mapa[(py - 30) / 30][px/30] != 'X')
                py -= 30;
            else
                dir = 4;
        }
        if (dir == 3)
        {
            if (mapa[(py + 30) / 30][px/30] != 'X')
                py += 30;
            else
                dir = 4;
        }

        //Controle para o atalho
        if (px <= -30)	px = 840;
        else if (px >= 870) px = 0;

        if (pontos > score)
            score = pontos;

        if (dir != 4 && contaMusica % 2 == 0)
            MUSICA al_play_sample(musica_comendo, 0.2, 0, 0.95, ALLEGRO_PLAYMODE_ONCE, NULL);
        al_clear_to_color(al_map_rgb(0,0,0));
        al_draw_bitmap(mapa_png, 0, 0, 0);
        al_draw_textf(fonte, al_map_rgb(255,255,255), 1050, 240, ALLEGRO_ALIGN_CENTRE, "%d", pontos);
        al_draw_textf(fonte, al_map_rgb(204,206,216), 1050, 430, ALLEGRO_ALIGN_CENTRE, "%d", score);
        desenhar_mapa();
        al_rest(0.11);
        desenhar_personagem();
        FANTASMAS fantasma();
        al_flip_display();
        al_rest(0.010);

        al_clear_to_color(al_map_rgb(0,0,0));
        al_draw_bitmap(mapa_png, 0, 0, 0);
        al_draw_textf(fonte, al_map_rgb(255,255,255), 1050, 240, ALLEGRO_ALIGN_CENTRE, "%d", pontos);
        al_draw_textf(fonte, al_map_rgb(204,206,216), 1050, 430, ALLEGRO_ALIGN_CENTRE, "%d", score);
        desenhar_mapa();
        al_rest(0.11);
        if (!godMode) al_draw_bitmap_region(pacbmp, 4 * 33, 0, 33, 33, px, py, 0);
        else al_draw_bitmap_region(pacgodbmp, 4 * 33, 0, 33, 33, px, py, 0);
        FANT1 al_draw_bitmap_region(bitmapf1, inimigos[0].fdir * 60 + 30, 0, 30, 30, inimigos[0]._x, inimigos[0]._y, 0);
        FANT2 al_draw_bitmap_region(bitmapf2, inimigos[1].fdir * 60 + 30, 0, 30, 30, inimigos[1]._x, inimigos[1]._y, 0);
        FANT3 al_draw_bitmap_region(bitmapf3, inimigos[2].fdir * 60 + 30, 0, 30, 30, inimigos[2]._x, inimigos[2]._y, 0);
        FANT4 al_draw_bitmap_region(bitmapf4, inimigos[3].fdir * 60 + 30, 0, 30, 30, inimigos[3]._x, inimigos[3]._y, 0);
        al_flip_display();
        al_rest(0.010);

        contaMusica++;
        if (godMode) godCount++;
        if (godCount > 20)
        {
            godCount = 0;
            godMode = 0;
        }
        salvaRecorde();
    }

    /* VENCEU */

    if (!game_over())
    {
        al_flip_display();
        al_draw_bitmap(bannerWin, 0, 0, 0);
        al_rest(4);
    }

    al_destroy_display(display);
    al_destroy_event_queue(fila_eventos);
    al_destroy_font(fonte);
    al_destroy_bitmap(parede);
    al_destroy_bitmap(pacbmp);
    al_destroy_bitmap(bitmapf1);
    al_destroy_bitmap(bitmapf2);
    al_destroy_bitmap(bitmapf3);
    al_destroy_bitmap(bitmapf4);
    al_destroy_bitmap(comida);
    al_destroy_bitmap(mortebmp);
}

void desenhar_mapa()
{
    int row, col;

    for (row = 0; row < MAXLINHAS; ++row)
    {
        for (col = 0; col < MAXCOLS; ++col)
        {
            if (mapa[row][col] == 'X')
            {
                al_draw_bitmap(parede, col * 30, row * 30, 0);
            }
            else if (mapa[row][col] == '.')
            {
                al_draw_bitmap(comida, col * 30, row * 30, 0);

                if (py/30 == row && px/30 == col)
                {
                    ++pontos;
                    mapa[row][col] = ' ';
                }
            }
            else if (mapa[row][col] == 'F')
            {
                al_draw_bitmap(fruta, col * 30, row * 30, 0);

                if (py/30 == row && px/30 == col)
                {
                    godMode = 1;
                    mapa[row][col] = ' ';
                }
            }
        }
    }
}

void desenhar_personagem()
{
    if (godMode)
        al_draw_bitmap_region(pacgodbmp, dir * 33, 0, 33, 33, px, py, 0);
    else
        al_draw_bitmap_region(pacbmp, dir * 33, 0, 33, 33, px, py, 0);
}

void desenhar_fantasma()
{
    FANT1 al_draw_bitmap_region(bitmapf1, inimigos[0].fdir * 60, 0, 30, 30, inimigos[0]._x, inimigos[0]._y, 0);
    FANT2 al_draw_bitmap_region(bitmapf2, inimigos[1].fdir * 60, 0, 30, 30, inimigos[1]._x, inimigos[1]._y, 0);
    FANT3 al_draw_bitmap_region(bitmapf3, inimigos[2].fdir * 60, 0, 30, 30, inimigos[2]._x, inimigos[2]._y, 0);
    FANT4 al_draw_bitmap_region(bitmapf4, inimigos[3].fdir * 60, 0, 30, 30, inimigos[3]._x, inimigos[3]._y, 0);
}

void reset_mapa()
{
    int i;

    for (i = 0; i < MAXLINHAS; ++i)
    {
        memcpy(mapa[i], mapa_backup[i], MAXCOLS);
    }
}

void choque_pacman()
{
    if (!godMode)
    {
        if (py == inimigos[0]._y && px == inimigos[0]._x || inimigos[0]._y == py_anterior && inimigos[0]._x == px_anterior  ||
                py == inimigos[1]._y && px == inimigos[1]._x || inimigos[1]._y == py_anterior && inimigos[1]._x == px_anterior  ||
                py == inimigos[2]._y && px == inimigos[2]._x || inimigos[2]._y == py_anterior && inimigos[2]._x == px_anterior  ||
                py == inimigos[3]._y && px == inimigos[3]._x || inimigos[3]._y == py_anterior && inimigos[3]._x == px_anterior)
        {
            MUSICA al_play_sample(musica_morte, 0.5, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            int i;
            for (i = 0; i <= 5; ++i)
            {
                al_clear_to_color(al_map_rgb(0,0,0));
                al_draw_bitmap(mapa_png, 0, 0, 0);
                desenhar_mapa();

                al_draw_bitmap_region(mortebmp, i * 33, 0, 33, 33, px, py, 0);
                al_flip_display();
                al_rest(0.08);
            }

            // Reseta variáveis do PacMan
            px = 30 * 14;
            py = 30 * 17;
            dir = 4;
            godMode = 0;
            // Reseta variaveis dos Fantasmas
            inimigos[0].fdir = 3;
            inimigos[1].fdir = 1;
            inimigos[2].fdir = 0;
            inimigos[3].fdir = 1;
            inimigos[0]._x = 30 * 22;
            inimigos[0]._y = 30 * 15;
            inimigos[1]._x = 30 * 4;
            inimigos[1]._y = 30 * 4;
            inimigos[2]._x = 30 * 14;
            inimigos[2]._y = 30 * 8;
            inimigos[3]._x = 30 * 5;
            inimigos[3]._y = 30 * 11;

            reset_mapa();
            contaMusica = 0;
            pontos = 0;
        }
    }
}

//Função de calculo de distancia entre pontos, para ser ultilizada no movimento dos fantasmas
int calcula_dist(int X, int Y, int x, int y)
{
    return sqrt(pow(X-x,2) + pow(Y-y,2));
}

void fantasma()
{
    desenhar_fantasma();
    choque_pacman();

    //Primeiro Fantasma

    int dist_menor1;

    dist_menor1 = calcula_dist(inimigos[0]._x/30, inimigos[0]._y/30, px/30, py/30);

    /* Segue para a esquerda */
    if      (calcula_dist((inimigos[0]._x/30) - 1, inimigos[0]._y/30, px/30, py/30) < dist_menor1)
    {
        /* Se não for parede, vai para esquerda */
        if (!godMode)
        {
            if      (mapa[inimigos[0]._y/30][(inimigos[0]._x/30) - 1] != 'X') {
                inimigos[0].fdir = 0;
                inimigos[0]._x -= 30;
            }
            else    inimigos[0].fdir = rand() % 3 + 1;
        }
        else
        {
            if      (mapa[inimigos[0]._y/30][(inimigos[0]._x/30) + 1] != 'X') {
                inimigos[0].fdir = 1;
                inimigos[0]._x += 30;
            }
            else    inimigos[0].fdir = rand() % 3 + 1;
        }
        if (inimigos[0].fdir == 1)
        {
            if    (mapa[inimigos[0]._y/30][(inimigos[0]._x/30) + 1] != 'X') inimigos[0]._x += 30;
            else  inimigos[0].fdir = rand() % 2 + 2;
        }
        if (inimigos[0].fdir == 2)
        {
            if    (mapa[(inimigos[0]._y/30) - 1][inimigos[0]._x/30] != 'X') inimigos[0]._y -= 30;
            else  inimigos[0].fdir = 3;
        }
        if (inimigos[0].fdir == 3)
        {
            if    (mapa[(inimigos[0]._y/30) + 1][inimigos[0]._x/30] != 'X') inimigos[0]._y += 30;
            else  inimigos[0].fdir = rand() % 2 + 1;
        }
    }
    /* Segue para a direita */
    else if (calcula_dist((inimigos[0]._x/30) + 1, inimigos[0]._y/30, px/30, py/30) < dist_menor1)
    {
        if (!godMode)
        {
            if      ((mapa[inimigos[0]._y/30][(inimigos[0]._x/30) + 1] != 'X')) {
                inimigos[0].fdir = 1;
                inimigos[0]._x += 30;
            }
            else    inimigos[0].fdir = rand() % 2 + 2;
        }
        else
        {
            if      ((mapa[inimigos[0]._y/30][(inimigos[0]._x/30) - 1] != 'X')) {
                inimigos[0].fdir = 0;
                inimigos[0]._x -= 30;
            }
            else    inimigos[0].fdir = rand() % 2 + 2;
        }
        if (inimigos[0].fdir == 2)
        {
            if    (mapa[(inimigos[0]._y/30) - 1][inimigos[0]._x/30] != 'X') inimigos[0]._y -= 30;
            else  inimigos[0].fdir = 3;
        }
        if (inimigos[0].fdir == 3)
        {
            if    (mapa[(inimigos[0]._y/30) + 1][inimigos[0]._x/30] != 'X') inimigos[0]._y += 30;
            else  inimigos[0].fdir = 0;
        }
    }
    /* Segue para cima */
    else if (calcula_dist(inimigos[0]._x/30, (inimigos[0]._y/30) - 1, px/30, py/30) < dist_menor1)
    {
        if (!godMode)
        {
            if      (mapa[(inimigos[0]._y/30) - 1][inimigos[0]._x/30] != 'X') {
                inimigos[0].fdir = 2;
                inimigos[0]._y -= 30;
            }
            else    inimigos[0].fdir = rand() % 2;
        }
        else
        {
            if      (mapa[(inimigos[0]._y/30) + 1][inimigos[0]._x/30] != 'X') {
                inimigos[0].fdir = 3;
                inimigos[0]._y += 30;
            }
            else    inimigos[0].fdir = rand() % 2;
        }
        if (inimigos[0].fdir == 0)
        {
            if    (mapa[inimigos[0]._y/30][(inimigos[0]._x/30) - 1] != 'X') inimigos[0]._x -= 30;
            else  inimigos[0].fdir = 1;
        }
        if (inimigos[0].fdir == 1)
        {
            if    (mapa[inimigos[0]._y/30][(inimigos[0]._x/30) + 1] != 'X') inimigos[0]._x += 30;
            else  inimigos[0].fdir = 3;
        }
    }
    /* Segue para baixo */
    else if (calcula_dist(inimigos[0]._x/30, (inimigos[0]._y/30) + 1, px/30, py/30) < dist_menor1)
    {
        if (!godMode)
        {
            if      (mapa[(inimigos[0]._y/30) + 1][inimigos[0]._x/30] != 'X') {
                inimigos[0].fdir = 3;
                inimigos[0]._y += 30;
            }
            else    inimigos[0].fdir = rand() % 3;
        }
        else
        {
            if      (mapa[(inimigos[0]._y/30) - 1][inimigos[0]._x/30] != 'X') {
                inimigos[0].fdir = 2;
                inimigos[0]._y -= 30;
            }
            else    inimigos[0].fdir = rand() % 3;
        }
        if (inimigos[0].fdir == 0)
        {
            if    (mapa[inimigos[0]._y/30][(inimigos[0]._x/30) - 1] != 'X') inimigos[0]._x -= 30;
            else  inimigos[0].fdir = 1;
        }
        if (inimigos[0].fdir == 1)
        {
            if    (mapa[inimigos[0]._y/30][(inimigos[0]._x/30) + 1] != 'X') inimigos[0]._x += 30;
            else  inimigos[0].fdir = 2;
        }
        if (inimigos[0].fdir == 2)
        {
            if    (mapa[(inimigos[0]._y/30) - 1][inimigos[0]._x/30] != 'X') inimigos[0]._y -= 30;
            else  inimigos[0].fdir = rand() % 2;
        }
    }

    //Atalhos do primeiro fantasma
    if       (inimigos[0]._x <= -30) inimigos[0]._x = 840;
    else if  (inimigos[0]._x >= 870) inimigos[0]._x = 0;

    //---------------------------------------------------------------------------------------
    //Segundo Fantasma

    int dist_menor2;

    dist_menor2 = calcula_dist(inimigos[1]._x/30, inimigos[1]._y/30, px/30, py/30);

    /* Segue para a esquerda */
    if      (calcula_dist((inimigos[1]._x/30) - 1, inimigos[1]._y/30, px/30, py/30) < dist_menor2)
    {
        if (!godMode)
        {
            /* Se não for parede, vai para esquerda */
            if      (mapa[inimigos[1]._y/30][(inimigos[1]._x/30) - 1] != 'X') {
                inimigos[1].fdir = 0;
                inimigos[1]._x -= 30;
            }
            else    inimigos[1].fdir = rand() % 3 + 1;
        }
        else
        {
            if      (mapa[inimigos[1]._y/30][(inimigos[1]._x/30) + 1] != 'X') {
                inimigos[1].fdir = 1;
                inimigos[1]._x += 30;
            }
            else    inimigos[1].fdir = rand() % 3 + 1;
        }
        if (inimigos[1].fdir == 1)
        {
            if    (mapa[inimigos[1]._y/30][(inimigos[1]._x/30) + 1] != 'X') inimigos[1]._x += 30;
            else  inimigos[1].fdir = rand() % 2 + 2;
        }
        if (inimigos[1].fdir == 2)
        {
            if    (mapa[(inimigos[1]._y/30) - 1][inimigos[1]._x/30] != 'X') inimigos[1]._y -= 30;
            else  inimigos[1].fdir = 3;
        }
        if (inimigos[1].fdir == 3)
        {
            if    (mapa[(inimigos[1]._y/30) + 1][inimigos[1]._x/30] != 'X') inimigos[1]._y += 30;
            else  inimigos[1].fdir = rand() % 2 + 1;
        }
    }
    /* Segue para a direita */
    else if (calcula_dist((inimigos[1]._x/30) + 1, inimigos[1]._y/30, px/30, py/30) < dist_menor2)
    {
        if (!godMode)
        {
            if      ((mapa[inimigos[1]._y/30][(inimigos[1]._x/30) + 1] != 'X')) {
                inimigos[1].fdir = 1;
                inimigos[1]._x += 30;
            }
            else    inimigos[1].fdir = rand() % 2 + 2;
        }
        else
        {
            if      ((mapa[inimigos[1]._y/30][(inimigos[1]._x/30) - 1] != 'X')) {
                inimigos[1].fdir = 0;
                inimigos[1]._x -= 30;
            }
            else    inimigos[1].fdir = rand() % 2 + 2;
        }
        if (inimigos[1].fdir == 2)
        {
            if    (mapa[(inimigos[1]._y/30) - 1][inimigos[1]._x/30] != 'X') inimigos[1]._y -= 30;
            else  inimigos[1].fdir = 3;
        }
        if (inimigos[1].fdir == 3)
        {
            if    (mapa[(inimigos[1]._y/30) + 1][inimigos[1]._x/30] != 'X') inimigos[1]._y += 30;
            else  inimigos[1].fdir = 0;
        }
    }
    /* Segue para cima */
    else if (calcula_dist(inimigos[1]._x/30, (inimigos[1]._y/30) - 1, px/30, py/30) < dist_menor2)
    {
        if (!godMode)
        {
            if      (mapa[(inimigos[1]._y/30) - 1][inimigos[1]._x/30] != 'X') {
                inimigos[1].fdir = 2;
                inimigos[1]._y -= 30;
            }
            else    inimigos[1].fdir = rand() % 2;
        }
        else
        {
            if      (mapa[(inimigos[1]._y/30) + 1][inimigos[1]._x/30] != 'X') {
                inimigos[1].fdir = 3;
                inimigos[1]._y += 30;
            }
            else    inimigos[1].fdir = rand() % 2;
        }
        if (inimigos[1].fdir == 0)
        {
            if    (mapa[inimigos[1]._y/30][(inimigos[1]._x/30) - 1] != 'X') inimigos[1]._x -= 30;
            else  inimigos[1].fdir = 1;
        }
        if (inimigos[1].fdir == 1)
        {
            if    (mapa[inimigos[1]._y/30][(inimigos[1]._x/30) + 1] != 'X') inimigos[1]._x += 30;
            else  inimigos[1].fdir = 3;
        }
    }
    /* Segue para baixo */
    else if (calcula_dist(inimigos[1]._x/30, (inimigos[1]._y/30) + 1, px/30, py/30) < dist_menor2)
    {
        if (!godMode)
        {
            if      (mapa[(inimigos[1]._y/30) + 1][inimigos[1]._x/30] != 'X') {
                inimigos[1].fdir = 3;
                inimigos[1]._y += 30;
            }
            else    inimigos[1].fdir = rand() % 3;
        }
        else
        {
            if      (mapa[(inimigos[1]._y/30) - 1][inimigos[1]._x/30] != 'X') {
                inimigos[1].fdir = 2;
                inimigos[1]._y -= 30;
            }
            else    inimigos[1].fdir = rand() % 3;
        }
        if (inimigos[1].fdir == 0)
        {
            if    (mapa[inimigos[1]._y/30][(inimigos[1]._x/30) - 1] != 'X') inimigos[1]._x -= 30;
            else  inimigos[1].fdir = 1;
        }
        if (inimigos[1].fdir == 1)
        {
            if    (mapa[inimigos[1]._y/30][(inimigos[1]._x/30) + 1] != 'X') inimigos[1]._x += 30;
            else  inimigos[1].fdir = 2;
        }
        if (inimigos[1].fdir == 2)
        {
            if    (mapa[(inimigos[1]._y/30) - 1][inimigos[1]._x/30] != 'X') inimigos[1]._y -= 30;
            else  inimigos[1].fdir = rand() % 2;
        }
    }

    //Atalhos do segundo fantasma
    if       (inimigos[1]._x <= -30) inimigos[1]._x = 840;
    else if  (inimigos[1]._x >= 870) inimigos[1]._x = 0;

    //---------------------------------------------------------------------------------------
    //Terceiro Fantasma

    int dist_menor3;

    dist_menor3 = calcula_dist(inimigos[2]._x/30, inimigos[2]._y/30, px/30, py/30);

    /* Segue para a esquerda */
    if      (calcula_dist((inimigos[2]._x/30) - 1, inimigos[2]._y/30, px/30, py/30) < dist_menor3)
    {
        if (!godMode)
        {
            /* Se não for parede, vai para esquerda */
            if      (mapa[inimigos[2]._y/30][(inimigos[2]._x/30) - 1] != 'X') {
                inimigos[2].fdir = 0;
                inimigos[2]._x -= 30;
            }
            else    inimigos[2].fdir = rand() % 3 + 1;
        }
        else
        {
            if      (mapa[inimigos[2]._y/30][(inimigos[2]._x/30) + 1] != 'X') {
                inimigos[2].fdir = 1;
                inimigos[2]._x += 30;
            }
            else    inimigos[2].fdir = rand() % 3 + 1;
        }
        if (inimigos[2].fdir == 1)
        {
            if    (mapa[inimigos[2]._y/30][(inimigos[2]._x/30) + 1] != 'X') inimigos[2]._x += 30;
            else  inimigos[2].fdir = rand() % 2 + 2;
        }
        if (inimigos[2].fdir == 2)
        {
            if    (mapa[(inimigos[2]._y/30) - 1][inimigos[2]._x/30] != 'X') inimigos[2]._y -= 30;
            else  inimigos[2].fdir = 3;
        }
        if (inimigos[2].fdir == 3)
        {
            if    (mapa[(inimigos[2]._y/30) + 1][inimigos[2]._x/30] != 'X') inimigos[2]._y += 30;
            else  inimigos[2].fdir = rand() % 2 + 1;
        }
    }
    /* Segue para a direita */
    else if (calcula_dist((inimigos[2]._x/30) + 1, inimigos[2]._y/30, px/30, py/30) < dist_menor3)
    {
        if (!godMode)
        {
            if      ((mapa[inimigos[2]._y/30][(inimigos[2]._x/30) + 1] != 'X')) {
                inimigos[2].fdir = 1;
                inimigos[2]._x += 30;
            }
            else    inimigos[2].fdir = rand() % 2 + 2;
        }
        else
        {
            if      ((mapa[inimigos[2]._y/30][(inimigos[2]._x/30) - 1] != 'X')) {
                inimigos[2].fdir = 0;
                inimigos[2]._x -= 30;
            }
            else    inimigos[2].fdir = rand() % 2 + 2;
        }
        if (inimigos[2].fdir == 2)
        {
            if    (mapa[(inimigos[2]._y/30) - 1][inimigos[2]._x/30] != 'X') inimigos[2]._y -= 30;
            else  inimigos[2].fdir = 3;
        }
        if (inimigos[2].fdir == 3)
        {
            if    (mapa[(inimigos[2]._y/30) + 1][inimigos[2]._x/30] != 'X') inimigos[2]._y += 30;
            else  inimigos[2].fdir = 0;
        }
    }
    /* Segue para cima */
    else if (calcula_dist(inimigos[2]._x/30, (inimigos[2]._y/30) - 1, px/30, py/30) < dist_menor3)
    {
        if (!godMode)
        {
            if      (mapa[(inimigos[2]._y/30) - 1][inimigos[2]._x/30] != 'X') {
                inimigos[2].fdir = 2;
                inimigos[2]._y -= 30;
            }
            else    inimigos[2].fdir = rand() % 2;
        }
        else
        {
            if      (mapa[(inimigos[2]._y/30) + 1][inimigos[2]._x/30] != 'X') {
                inimigos[2].fdir = 3;
                inimigos[2]._y += 30;
            }
            else    inimigos[2].fdir = rand() % 2;
        }
        if (inimigos[2].fdir == 0)
        {
            if    (mapa[inimigos[2]._y/30][(inimigos[2]._x/30) - 1] != 'X') inimigos[2]._x -= 30;
            else  inimigos[2].fdir = 1;
        }
        if (inimigos[2].fdir == 1)
        {
            if    (mapa[inimigos[2]._y/30][(inimigos[2]._x/30) + 1] != 'X') inimigos[2]._x += 30;
            else  inimigos[2].fdir = 3;
        }
    }
    /* Segue para baixo */
    else if (calcula_dist(inimigos[2]._x/30, (inimigos[2]._y/30) + 1, px/30, py/30) < dist_menor3)
    {
        if (!godMode)
        {
            if      (mapa[(inimigos[2]._y/30) + 1][inimigos[2]._x/30] != 'X') {
                inimigos[2].fdir = 3;
                inimigos[2]._y += 30;
            }
            else    inimigos[2].fdir = rand() % 3;
        }
        else
        {
            if      (mapa[(inimigos[2]._y/30) - 1][inimigos[2]._x/30] != 'X') {
                inimigos[2].fdir = 2;
                inimigos[2]._y -= 30;
            }
            else    inimigos[2].fdir = rand() % 3;
        }
        if (inimigos[2].fdir == 0)
        {
            if    (mapa[inimigos[2]._y/30][(inimigos[2]._x/30) - 1] != 'X') inimigos[2]._x -= 30;
            else  inimigos[2].fdir = 1;
        }
        if (inimigos[2].fdir == 1)
        {
            if    (mapa[inimigos[2]._y/30][(inimigos[2]._x/30) + 1] != 'X') inimigos[2]._x += 30;
            else  inimigos[2].fdir = 2;
        }
        if (inimigos[2].fdir == 2)
        {
            if    (mapa[(inimigos[2]._y/30) - 1][inimigos[2]._x/30] != 'X') inimigos[2]._y -= 30;
            else  inimigos[2].fdir = rand() % 2;
        }
    }

    //Atalhos do terceiro fantasma
    if       (inimigos[2]._x <= -30) inimigos[2]._x = 840;
    else if  (inimigos[2]._x >= 870) inimigos[2]._x = 0;

    //---------------------------------------------------------------------------------------
    //Quarto Fantasma

    int dist_menor4;

    dist_menor4 = calcula_dist(inimigos[3]._x/30, inimigos[3]._y/30, px/30, py/30);

    /* Segue para a esquerda */
    if      (calcula_dist((inimigos[3]._x/30) - 1, inimigos[3]._y/30, px/30, py/30) < dist_menor4)
    {
        if (!godMode)
        {
            if      (mapa[inimigos[3]._y/30][(inimigos[3]._x/30) - 1] != 'X') {
                inimigos[3].fdir = 0;
                inimigos[3]._x -= 30;
            }
            else    inimigos[3].fdir = rand() % 3 + 1;
        }
        else
        {
            if      (mapa[inimigos[3]._y/30][(inimigos[3]._x/30) + 1] != 'X') {
                inimigos[3].fdir = 1;
                inimigos[3]._x += 30;
            }
            else    inimigos[3].fdir = rand() % 3 + 1;
        }
        if (inimigos[3].fdir == 1)
        {
            if    (mapa[inimigos[3]._y/30][(inimigos[3]._x/30) + 1] != 'X') inimigos[3]._x += 30;
            else  inimigos[3].fdir = rand() % 2 + 2;
        }
        if (inimigos[3].fdir == 2)
        {
            if    (mapa[(inimigos[3]._y/30) - 1][inimigos[3]._x/30] != 'X') inimigos[3]._y -= 30;
            else  inimigos[3].fdir = 3;
        }
        if (inimigos[3].fdir == 3)
        {
            if    (mapa[(inimigos[3]._y/30) + 1][inimigos[3]._x/30] != 'X') inimigos[3]._y += 30;
            else  inimigos[3].fdir = rand() % 2 + 1;
        }
    }
    /* Segue para a direita */
    else if (calcula_dist((inimigos[3]._x/30) + 1, inimigos[3]._y/30, px/30, py/30) < dist_menor4)
    {
        if (!godMode)
        {
            if      ((mapa[inimigos[3]._y/30][(inimigos[3]._x/30) + 1] != 'X')) {
                inimigos[3].fdir = 1;
                inimigos[3]._x += 30;
            }
            else    inimigos[3].fdir = rand() % 2 + 2;
        }
        else
        {
            if      ((mapa[inimigos[3]._y/30][(inimigos[3]._x/30) - 1] != 'X')) {
                inimigos[3].fdir = 0;
                inimigos[3]._x -= 30;
            }
            else    inimigos[3].fdir = rand() % 2 + 2;
        }
        if (inimigos[3].fdir == 2)
        {
            if    (mapa[(inimigos[3]._y/30) - 1][inimigos[3]._x/30] != 'X') inimigos[3]._y -= 30;
            else  inimigos[3].fdir = 3;
        }
        if (inimigos[3].fdir == 3)
        {
            if    (mapa[(inimigos[3]._y/30) + 1][inimigos[3]._x/30] != 'X') inimigos[3]._y += 30;
            else  inimigos[3].fdir = 0;
        }
    }
    /* Segue para cima */
    else if (calcula_dist(inimigos[3]._x/30, (inimigos[3]._y/30) - 1, px/30, py/30) < dist_menor4)
    {
        if (!godMode)
        {
            if      (mapa[(inimigos[3]._y/30) - 1][inimigos[3]._x/30] != 'X') {
                inimigos[3].fdir = 2;
                inimigos[3]._y -= 30;
            }
            else    inimigos[3].fdir = rand() % 2;
        }
        else
        {
            if      (mapa[(inimigos[3]._y/30) + 1][inimigos[3]._x/30] != 'X') {
                inimigos[3].fdir = 3;
                inimigos[3]._y += 30;
            }
            else    inimigos[3].fdir = rand() % 2;
        }
        if (inimigos[3].fdir == 0)
        {
            if    (mapa[inimigos[3]._y/30][(inimigos[3]._x/30) - 1] != 'X') inimigos[3]._x -= 30;
            else  inimigos[3].fdir = 1;
        }
        if (inimigos[3].fdir == 1)
        {
            if    (mapa[inimigos[3]._y/30][(inimigos[3]._x/30) + 1] != 'X') inimigos[3]._x += 30;
            else  inimigos[3].fdir = 3;
        }
    }
    /* Segue para baixo */
    else if (calcula_dist(inimigos[3]._x/30, (inimigos[3]._y/30) + 1, px/30, py/30) < dist_menor4)
    {
        if (!godMode)
        {
            if      (mapa[(inimigos[3]._y/30) + 1][inimigos[3]._x/30] != 'X') {
                inimigos[3].fdir = 3;
                inimigos[3]._y += 30;
            }
            else    inimigos[3].fdir = rand() % 3;
        }
        else
        {
            if      (mapa[(inimigos[3]._y/30) - 1][inimigos[3]._x/30] != 'X') {
                inimigos[3].fdir = 2;
                inimigos[3]._y += 30;
            }
            else    inimigos[3].fdir = rand() % 3;
        }
        if (inimigos[3].fdir == 0)
        {
            if    (mapa[inimigos[3]._y/30][(inimigos[3]._x/30) - 1] != 'X') inimigos[3]._x -= 30;
            else  inimigos[3].fdir = 1;
        }
        if (inimigos[3].fdir == 1)
        {
            if    (mapa[inimigos[3]._y/30][(inimigos[3]._x/30) + 1] != 'X') inimigos[3]._x += 30;
            else  inimigos[3].fdir = 2;
        }
        if (inimigos[3].fdir == 2)
        {
            if    (mapa[(inimigos[3]._y/30) - 1][inimigos[3]._x/30] != 'X') inimigos[3]._y -= 30;
            else  inimigos[3].fdir = rand() % 2;
        }
    }

    //Atalhos do quarto fantasma
    if       (inimigos[3]._x <= -30) inimigos[3]._x = 840;
    else if  (inimigos[3]._x >= 870) inimigos[3]._x = 0;
}

bool game_over()
{
    int row, col;

    for (row = 0; row < MAXLINHAS; ++row)
        for (col = 0; col < MAXCOLS; ++col)
            if (mapa[row][col] == '.')
                return true;

    return false;
}

bool inicializar()
{
    if (!al_init())
    {
        fprintf(stderr, "Falha ao inicializar a Allegro.\n");
        return false;
    }

    al_init_font_addon();

    if (!al_init_ttf_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on allegro_ttf.\n");
        return false;
    }

    if (!al_init_image_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on allegro_image.\n");
        return false;
    }

    if (!al_install_keyboard())
    {
        fprintf(stderr, "Falha ao inicializar o teclado.\n");
        return false;
    }

    display = al_create_display(1200, 570);
    if (!display)
    {
        fprintf(stderr, "Falha ao criar janela.\n");
        return false;
    }

    al_set_window_title(display, "PacMan - IC Party");

    fonte = al_load_font("./fonts/arcade.ttf", 36, 0);
    if (!fonte)
    {
        fprintf(stderr, "Falha ao carregar \"fonte emulogic.ttf\".\n");
        al_destroy_display(display);
        return false;
    }

    fonte2 = al_load_font("./fonts/arcade.ttf", 42, 0);
    if (!fonte2)
    {
        fprintf(stderr, "Falha ao carregar \"fonte emulogic.ttf\".\n");
        al_destroy_display(display);
        return false;
    }

    if (!al_install_audio())
    {
        fprintf(stderr, "Falha ao inicializar áudio.\n");
        return false;
    }

    if (!al_init_acodec_addon())
    {
        fprintf(stderr, "Falha ao inicializar codecs de áudio.\n");
        return false;
    }

    if (!al_reserve_samples(3))
    {
        fprintf(stderr, "Falha ao alocar canais de áudio.\n");
        return false;
    }

    fila_eventos = al_create_event_queue();
    if (!fila_eventos)
    {
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        al_destroy_display(display);
        return false;
    }

    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(display));

    return true;
}

void inicializar_fantasmas()
{
    inimigos[0].fdir = 3;
    inimigos[0]._x = 30 * 22;
    inimigos[0]._y = 30 * 15;
    inimigos[1].fdir = 1;
    inimigos[1]._x = 30 * 4;
    inimigos[1]._y = 30 *  4;
    inimigos[2].fdir = 0;
    inimigos[2]._x = 30 * 14;
    inimigos[2]._y = 30 *  8;
    inimigos[3].fdir = 1;
    inimigos[3]._x = 30 * 5;
    inimigos[3]._y = 30 * 11;
    bitmapf1 = al_load_bitmap("./images/fantasma.png");
    bitmapf2 = al_load_bitmap("./images/fantasma2.png");
    bitmapf3 = al_load_bitmap("./images/fantasma3.png");
    bitmapf4 = al_load_bitmap("./images/fantasma4.png");
}

void recorde()
{
    aR = fopen("recorde.txt", "r");

    if (!aR)
    {
        printf("Erro ao abrir o arquivo recorde.txt\n");
        exit(0);
    }

    fscanf(aR, "%d", &score);

    fprintf(stderr, "Highscore - %d\n", score);

    fclose(aR);
}

void salvaRecorde()
{
    aR = fopen("recorde.txt", "w");

    if (aR)
    {
        fprintf(aR, "%d\n", score);
    }

    fclose(aR);
}
