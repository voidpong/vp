
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

#define UP 65
#define DOWN 66

#define LINHAS_DA_MATRIZ 23
#define COLUNAS_DA_MATRIZ 59
#define CENTRO_DA_MATRIZ_X ((COLUNAS_DA_MATRIZ / 2))
#define CENTRO_DA_MATRIZ_Y ((LINHAS_DA_MATRIZ / 2))

char matriz_campo_do_jogo[LINHAS_DA_MATRIZ][COLUNAS_DA_MATRIZ];
int posicao_bola_x, posicao_bola_y;
int COEFICIENTE_DE_X, COEFICIENTE_DE_Y;

static struct termios g_old_kbd_mode;
/*****************************************************************************
*****************************************************************************/
static void cooked(void)
{
    tcsetattr(0, TCSANOW, &g_old_kbd_mode);
}
/*****************************************************************************
*****************************************************************************/
static void raw(void)
{
    static char init;
/**/
    struct termios new_kbd_mode;

    if(init)
        return;
/* put keyboard (stdin, actually) in raw, unbuffered mode */
    tcgetattr(0, &g_old_kbd_mode);
    memcpy(&new_kbd_mode, &g_old_kbd_mode, sizeof(struct termios));
    new_kbd_mode.c_lflag &= ~(ICANON | ECHO);
    new_kbd_mode.c_cc[VTIME] = 0;
    new_kbd_mode.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &new_kbd_mode);
/* when we exit, go back to normal, "cooked" mode */
    atexit(cooked);

    init = 1;
}
/*****************************************************************************
*****************************************************************************/
static int kbhit(void)
{
    struct timeval timeout;
    fd_set read_handles;
    int status;

    raw();
/* check stdin (fd 0) for activity */
    FD_ZERO(&read_handles);
    FD_SET(0, &read_handles);
    timeout.tv_sec = timeout.tv_usec = 0;
    status = select(0 + 1, &read_handles, NULL, NULL, &timeout);
    if(status < 0)
    {
        printf("select() failed in kbhit()\n");
        exit(1);
    }
    return status;
}
/*****************************************************************************
*****************************************************************************/
static int getch(void)
{
    unsigned char temp;

    raw();
/* stdin = fd 0 */
    if(read(0, &temp, 1) != 1)
        return 0;
    return temp;
}
void up (int p)
{
    matriz_campo_do_jogo[p-1][0] = '3';
    matriz_campo_do_jogo[p][0] = '2';
    matriz_campo_do_jogo[p+1][0] = '1';
    matriz_campo_do_jogo[p+2][0] = '2';
    matriz_campo_do_jogo[p+3][0] = '3';
    matriz_campo_do_jogo[p+4][0] = ' ';
}

void down (int p)
{
    matriz_campo_do_jogo[p][0] = ' ';
    matriz_campo_do_jogo[p+1][0] = '3';
    matriz_campo_do_jogo[p+2][0] = '2';
    matriz_campo_do_jogo[p+3][0] = '1';
    matriz_campo_do_jogo[p+4][0] = '2';
    matriz_campo_do_jogo[p+5][0] = '3';
}

void upadversario (int p)
{
    matriz_campo_do_jogo[p-1][COLUNAS_DA_MATRIZ-1] = '3';
    matriz_campo_do_jogo[p][COLUNAS_DA_MATRIZ-1] = '2';
    matriz_campo_do_jogo[p+1][COLUNAS_DA_MATRIZ-1] = '1';
    matriz_campo_do_jogo[p+2][COLUNAS_DA_MATRIZ-1] = '2';
    matriz_campo_do_jogo[p+3][COLUNAS_DA_MATRIZ-1] = '3';
    matriz_campo_do_jogo[p+4][COLUNAS_DA_MATRIZ-1] = ' ';
}

void downadversario (int p)
{
    matriz_campo_do_jogo[p][COLUNAS_DA_MATRIZ-1] = ' ';
    matriz_campo_do_jogo[p+1][COLUNAS_DA_MATRIZ-1] = '3';
    matriz_campo_do_jogo[p+2][COLUNAS_DA_MATRIZ-1] = '2';
    matriz_campo_do_jogo[p+3][COLUNAS_DA_MATRIZ-1] = '1';
    matriz_campo_do_jogo[p+4][COLUNAS_DA_MATRIZ-1] = '2';
    matriz_campo_do_jogo[p+5][COLUNAS_DA_MATRIZ-1] = '3';
}

void movimentarbola()
{
    matriz_campo_do_jogo[posicao_bola_y][posicao_bola_x] = ' ';
    posicao_bola_y += COEFICIENTE_DE_Y;
    posicao_bola_x += COEFICIENTE_DE_X;
    matriz_campo_do_jogo[posicao_bola_y][posicao_bola_x] = '0';
    if (posicao_bola_y <= 1)
    {
        COEFICIENTE_DE_Y = +2;
    }
    if (posicao_bola_x >= COLUNAS_DA_MATRIZ-2)
    {
        COEFICIENTE_DE_X = -1;
    }
    if (posicao_bola_y >= LINHAS_DA_MATRIZ-2)
    {
        COEFICIENTE_DE_Y = -2;
    }
    if (posicao_bola_x <= 1)
    {
        COEFICIENTE_DE_X = +1;
    }
}

void main ()
{
    int i, j, primeiro3, ispressed;
    char keypressed;
    COEFICIENTE_DE_X = 1;
    COEFICIENTE_DE_Y = 2;
    //for para desenhar a matriz que representa o campo
    for (i = 0; i < LINHAS_DA_MATRIZ; i++)
    {
        for (j = 0; j < COLUNAS_DA_MATRIZ; j++)
        {
            if (i == 0 || i == LINHAS_DA_MATRIZ-1)
            {
                matriz_campo_do_jogo[i][j] = '#';
            }
            else
            {
                matriz_campo_do_jogo[i][j] = ' ';
            }
            if ((j == 0 || j == COLUNAS_DA_MATRIZ-1) && (i == 9 || i == 13))
            {
                matriz_campo_do_jogo[i][j] = '3';
            }
            if ((j == 0 || j == COLUNAS_DA_MATRIZ-1) && (i == 10 || i == 12))
            {
                matriz_campo_do_jogo[i][j] = '2';
            }
            if ((j == 0 || j == COLUNAS_DA_MATRIZ-1) && i == 11)
            {
                matriz_campo_do_jogo[i][j] = '1';
            }
            if (i == CENTRO_DA_MATRIZ_Y && j == CENTRO_DA_MATRIZ_X)
            {
                matriz_campo_do_jogo[i][j] = '0';
            }
        }
    }
    for (i = 0; i < LINHAS_DA_MATRIZ; i++)
    {
        for (j = 0; j < COLUNAS_DA_MATRIZ; j++)
        {
            printf("%c", matriz_campo_do_jogo[i][j]);
        }
        printf("\n");
    }
    posicao_bola_y = CENTRO_DA_MATRIZ_Y;
    posicao_bola_x = CENTRO_DA_MATRIZ_X;
    primeiro3 = 9;
    while (1)
    {
        movimentarbola();
        system("clear");
        for (i = 0; i < LINHAS_DA_MATRIZ; i++)
        {
            for (j = 0; j < COLUNAS_DA_MATRIZ; j++)
            {
                printf("%c", matriz_campo_do_jogo[i][j]);
            }
            printf("\n");
        }
        if(kbhit()){
            keypressed = getch();
            keypressed = getch();
            keypressed = getch();
            if (keypressed == UP && primeiro3 != 1)
            {
                up(primeiro3);
                primeiro3 -= 1;
            }
            if (keypressed == DOWN && primeiro3 != LINHAS_DA_MATRIZ-6)
            {
                down(primeiro3);
                primeiro3 += 1;
            }
        }
        usleep(80000);
    }
}
