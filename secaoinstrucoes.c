/* #include <stdio.h>
#include <string.h>
#include <locale.h>
*/

void Instrucoes()
{
    /*
    LEMBRAR DAS CONSTANTES          const int NUMERO_ROUNDS = 3;
                                    const int NUMERO_PONTOS_GANHAR = 3;
                                    const int ROUNDS_GANHAR = 2;
    */
    setlocale(LC_ALL, "Portuguese");
    int i;
    long int j;
    char titulo[128] = "                           Instru��es do Jogo:";
    char descricao[200] = "* O Void Pong � um jogo simples, com movimentos baseados apenas no teclado.";
    char objetivo[200] = "* O objetivo do jogo � conseguir defender seu lado da arena rebatendo a bola de forma que o seu oponente n�o consiga defend�-la.";
    char howtoplay[200] = "* O usu�rio deve usar as  setas para jogar. J� para se movimentar nos menus, as setas e as teclas enter e ESC.";
    char comoganhar[200] = "* Ganha um round quem conseguir fazer 3 pontos primeiro. Como uma partida � com-posta de no m�ximo 3 rounds, ou seja, quem ganhar 2 rounds, � o vencedor.";
    char motivacao[200] = "*                              ENJOY!";
    for (i = 0; i < 128; i++)
    {
        printf("%c", titulo[i]);
        for (j = 0; j < 10000000; j++);
    }
    printf("\n\n\n\n");
    for (i = 0; i < 200; i++)
    {
        printf("%c", descricao[i]);
        for (j = 0; j < 10000000; j++);
    }
    printf("\n\n");
    for (i = 0; i < 200; i++)
    {
        printf("%c", objetivo[i]);
        for (j = 0; j < 10000000; j++);
    }
    printf("\n\n");
    for (i = 0; i < 200; i++)
    {
        printf("%c", howtoplay[i]);
        for (j = 0; j < 10000000; j++);
    }
    printf("\n\n");
    for (i = 0; i < 200; i++)
    {
        printf("%c", comoganhar[i]);
        for (j = 0; j < 10000000; j++);
    }
    printf("\n\n");
    for (i = 0; i < 200; i++)
    {
        printf("%c", motivacao[i]);
        for (j = 0; j < 10000000; j++);
    }
    printf("\n\n\n\n");
}
