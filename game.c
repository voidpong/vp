
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define UP 65
#define DOWN 66

#define LINHAS_DA_MATRIZ 23
#define COLUNAS_DA_MATRIZ 59
#define CENTRO_DA_MATRIZ_X ((COLUNAS_DA_MATRIZ / 2))
#define CENTRO_DA_MATRIZ_Y ((LINHAS_DA_MATRIZ / 2))

char matriz_campo_do_jogo[LINHAS_DA_MATRIZ][COLUNAS_DA_MATRIZ];
int posicao_bola_x, posicao_bola_y;
int COEFICIENTE_DE_X, COEFICIENTE_DE_Y;
int primeiro3esquerdo;
int primeiro3direito;
int numjogador;

int done=0; 
int sockfd;
pthread_mutex_t mutexsum = PTHREAD_MUTEX_INITIALIZER;  

void *enviarmensagem(char code[]);

void *recebermensagem();


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
void upesquerdo (int p)
{
    matriz_campo_do_jogo[p-1][0] = '3';
    matriz_campo_do_jogo[p][0] = '2';
    matriz_campo_do_jogo[p+1][0] = '1';
    matriz_campo_do_jogo[p+2][0] = '2';
    matriz_campo_do_jogo[p+3][0] = '3';
    matriz_campo_do_jogo[p+4][0] = ' ';
}

void downesquerdo (int p)
{
    matriz_campo_do_jogo[p][0] = ' ';
    matriz_campo_do_jogo[p+1][0] = '3';
    matriz_campo_do_jogo[p+2][0] = '2';
    matriz_campo_do_jogo[p+3][0] = '1';
    matriz_campo_do_jogo[p+4][0] = '2';
    matriz_campo_do_jogo[p+5][0] = '3';
}

void updireito (int p)
{
    matriz_campo_do_jogo[p-1][COLUNAS_DA_MATRIZ-1] = '3';
    matriz_campo_do_jogo[p][COLUNAS_DA_MATRIZ-1] = '2';
    matriz_campo_do_jogo[p+1][COLUNAS_DA_MATRIZ-1] = '1';
    matriz_campo_do_jogo[p+2][COLUNAS_DA_MATRIZ-1] = '2';
    matriz_campo_do_jogo[p+3][COLUNAS_DA_MATRIZ-1] = '3';
    matriz_campo_do_jogo[p+4][COLUNAS_DA_MATRIZ-1] = ' ';
}

void downdireito (int p)
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
	COEFICIENTE_DE_Y *= -1; 
	if(posicao_bola_y != 1) 
		posicao_bola_y = 1; 
} 
if (posicao_bola_x >= COLUNAS_DA_MATRIZ-2) 
{ 
	if(posicao_bola_x != COLUNAS_DA_MATRIZ-2) 
		posicao_bola_x = COLUNAS_DA_MATRIZ-2; 
	if(posicao_bola_y >= posicao3direito && posicao_bola_y <= posicao3direito+4)
		COEFICIENTE_DE_X *= -1;
	else
	{
		posicao_bola_y = CENTRO_DA_MATRIZ_Y; 
		posicao_bola_x = CENTRO_DA_MATRIZ_X;
		COEFICIENTE_DE_X = 1; 
		COEFICIENTE_DE_Y = 2;
	}
} 
if (posicao_bola_y >= LINHAS_DA_MATRIZ-2) 
{ 
	COEFICIENTE_DE_Y *= -1; 
	if(posicao_bola_y != LINHAS_DA_MATRIZ-2) 
		posicao_bola_y = LINHAS_DA_MATRIZ-2; 
} 
if (posicao_bola_x <= 1) 
{ 
	if(posicao_bola_x != 1) 
		posicao_bola_x = 1; 
	if(posicao_bola_y >= posicao3esquerdo && posicao_bola_y <= posicao3esquerdo+4)
		COEFICIENTE_DE_X *= -1;
	else
	{
		posicao_bola_y = CENTRO_DA_MATRIZ_Y; 
		posicao_bola_x = CENTRO_DA_MATRIZ_X;
		COEFICIENTE_DE_X = 1; 
		COEFICIENTE_DE_Y = 2;
	} 
}

void movercima(int jogouadv){ //jogouadv = 1 -> jogador; jogouadv = 2 -> adversario
    if(jogouadv==numjogador && primeiro3esquerdo != 1){
        upesquerdo(primeiro3esquerdo);
        primeiro3esquerdo--;
    }else if(jogouadv!=numjogador && primeiro3direito != 1){
        updireito(primeiro3direito);
        primeiro3direito--;
    }
}

void moverbaixo(int jogouadv){
    if(jogouadv==numjogador && primeiro3esquerdo != LINHAS_DA_MATRIZ-6){
        downesquerdo(primeiro3esquerdo);
        primeiro3esquerdo++;
    }else if(jogouadv!=numjogador && primeiro3direito != LINHAS_DA_MATRIZ-6){
        downdireito(primeiro3direito);
        primeiro3direito++;
    }
}

void main ()
{
    int i, j, primeiro3, ispressed, moverbola=0;
    char keypressed;
    char msgsocket[2];
    COEFICIENTE_DE_X = 1;
    COEFICIENTE_DE_Y = 2;

    configurarsocket();
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
    primeiro3esquerdo = 9;
    primeiro3direito = 9;
    while (1)
    {
        if(moverbola==0)
            movimentarbola();
        if(moverbola==5)
            moverbola=0;
        else
            moverbola++;
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
            if (keypressed == UP)
            {
                movercima(1);
                msgsocket[0]='a';
                enviarmensagem(msgsocket);
            }
            if (keypressed == DOWN)
            {
                moverbaixo(1);
                msgsocket[0]='b';
                enviarmensagem(msgsocket);
            }
        }
        usleep(40000);
    }
}

void configurarsocket(){
    int len;
    int result;
    char buf[256];
    struct sockaddr_in address;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(9035);

    len = sizeof(address);

    result = connect(sockfd, (struct sockaddr *)&address, len);

    printf("\n\nConectando\n\n");

    if(result == -1)
    {
        perror("\nConexao falhou. Tente novamente.\n");
        exit(1);
    }
    else
    {
        printf("\n\nConexao sucedida\nEsperando o outro jogador\n");
    }

    pthread_t threads[1];
    void *status;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    pthread_create(&threads[0], &attr, recebermensagem, NULL);

    while(!done);
}

void *enviarmensagem(char code[])
{

    int bufsize=100;
    char *buffer=malloc(bufsize);

    bzero(buffer,bufsize);
        
    /*if(strcmp(code,"exit")==0)
    {

        done = 1;


        pthread_mutex_destroy(&mutexsum);
        pthread_exit(NULL);
        close(sockfd);
    } */   

    write(sockfd,code,strlen(code));

    pthread_mutex_lock (&mutexsum);

    pthread_mutex_unlock (&mutexsum);

}

void *recebermensagem()
{
    char str[80];
    int bufsize=100;
    char *buffer=malloc(bufsize);

    while(1)
    {
        bzero(buffer,bufsize);
        read(sockfd,buffer,bufsize);

        if(buffer[0]=='j'){
            numjogador = buffer[1]-'0';
        }
        if(buffer[0]=='a'){
            movercima(2);
        }
        if(buffer[0]=='b'){
            moverbaixo(2);
        }
        if(buffer[0]=='c'){
            done=1;
        }
        //printf("%c\n", buffer[0]);

        pthread_mutex_lock (&mutexsum);
        pthread_mutex_unlock (&mutexsum);
    }
}
