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
    char titulo[128] = "                           Instruções do Jogo:";
    char descricao[200] = "* O Void Pong é um jogo simples, com movimentos baseados apenas no teclado.";
    char objetivo[200] = "* O objetivo do jogo é conseguir defender seu lado da arena rebatendo a bola de forma que o seu oponente não consiga defendê-la.";
    char howtoplay[200] = "* O usuário deve usar as  setas para jogar. Já para se movimentar nos menus, as setas e as teclas enter e ESC.";
    char comoganhar[200] = "* Ganha um round quem conseguir fazer 3 pontos primeiro. Como uma partida é com-posta de no máximo 3 rounds, ou seja, quem ganhar 2 rounds, é o vencedor.";
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
