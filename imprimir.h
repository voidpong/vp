// INCLUI FUNÇOES PARA IMPRIMIR NO CENTRO DA JANELA E A DIREITA DA JANELA (exemplos no final do codigo)
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void print_central(char s[]), print_direita(char s[]), centralizar_vertical(int n); // funcoes abaixo

// FUNÇAO PARA RECEBER UMA STRING E IMPRIMI-LA NO MEIO DA JANELA
void print_central (char s[]){ //recebe string a ser impressa
    struct winsize w; // criar variavel do tipo winsize
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); // pegar do sistema o tamanho da janela e salvar na variavel w
    int i, t=(unsigned)strlen(s); // salvar o tamanho da string recebida em uma variavel inteira
    for (i=0;i<w.ws_col/2-t/2;i++) // imprime espacos em branco antes da string
    	printf (" ");
    printf("%s\n", s); // imprime a string com uma quebra de linha ao final
}

// FUNÇAO PARA RECEBER UMA STRING E IMPRIMI-LA A DIREITA DA JANELA
void print_direita (char s[]){ //recebe string a ser impressa
	struct winsize w; // criar variavel do tipo winsize
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); // pegar do sistema o tamanho da janela e salvar na variavel w
    int i, t=(unsigned)strlen(s); // salvar o tamanho da string recebida em uma variavel inteira
    for (i=0;i<w.ws_col-t;i++) // imprime espacos em branco antes da string
    	printf (" ");
    printf("%s\n", s); // imprime a string com uma quebra de linha ao final
}

// FUNCAO PARA CENTRALIZAR VERTICALMENTE NA JANELA
void centralizar_vertical(int n){ // recebe o numero de linhas que serao impressas no meio da janela
	int i;
	struct winsize w; // criar variavel do tipo winsize
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); // pegar do sistema o tamanho da janela e salvar na variavel w
	for (i=0;i<w.ws_row/2-n/2;i++)
		printf("\n");
}
//FUNÇÃO PARA MUDAR O CURSOR PARA UMA DETERMINADA POSIÇÃO NO TERMINAL
void gotoxy(int x, int y) {
        char essq[100];     /* String variable to hold the escape sequence */
        char xstr[100];     /* Strings to hold the x and y coordinates */
        char ystr[100];     /* Escape sequences must be built with characters */
 
        /*
        ** Convert the screen coordinates to strings
        */
        sprintf(xstr, "%d", x);
        sprintf(ystr, "%d", y);
 
        /*
        ** Build the escape sequence (vertical move)
        */
        essq[0] = '\0';
        strcat(essq, "\033[");
        strcat(essq, ystr);
 
        /*
        ** Described in man terminfo as vpa=\E[%p1%dd
        ** Vertical position absolute
        */
        strcat(essq, "d");
 
        /*
        ** Horizontal move
        ** Horizontal position absolute
        */
        strcat(essq, "\033[");
        strcat(essq, xstr);
        /* Described in man terminfo as hpa=\E[%p1%dG */
        strcat(essq, "G");
 
        /*
        ** Execute the escape sequence
        ** This will move the cursor to x, y
        */
        printf("%s", essq);
}
/*
void main (){
	puts ("String à esquerda!");
	print_central("String central!"); // recebe a string que sera impressa no centro
	print_direita("String à direita!"); // recebe a string que sera impressa a direita
	centralizar_vertical(9); //recebe o numeros de linhas a serem impressas no meio da janela e prepara a janela para tal
}
*/
