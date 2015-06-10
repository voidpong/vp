// INCLUI FUNCOES DE EXIBICAO NO TERMINAL, VOIDS SEM RETORNO (menu, instruçoes, creditos, telas intermediarias, etc)
#include "imprimir.h" // contem funcoes para imprimir de maneira especial
#include "constantes.h" // contem constantes globais, que definem as regras do jogo

void instrucoes(), menu(), creditos(); //funcoes abaixo

void instrucoes(){
    char string[200];
    system ("clear"); //limpar a tela
    centralizar_vertical(16); // centralizar verticalmente para imprimir 16 linhas no meio da tela
    print_central ("|  Instruções do Jogo  |");
    puts (""); //equivalente a um /n
    puts ("");
    print_central ("* Void Pong é um jogo simples, com movimentos baseados apenas no teclado.");
    puts (""); 
    print_central ("* O objetivo do jogo é conseguir defender seu lado da arena rebatendo a bola de forma que o seu oponente não consiga defendê-la.");
    puts (""); 
    print_central ("* O usuário deve usar as setas para jogar. Já para se movimentar nos menus, as setas e as teclas ENTER e ESC.");
    puts (""); 
    strcpy (string,"* Ganha um round quem conseguir fazer D pontos primeiro. Como uma partida é composta de no máximo D rounds, ou seja, quem ganhar D"); //gambiarra pra enviar a string pra ser impressa
    string[38]=NUMERO_ROUNDS+48; //gambiarra pra enviar a string pra ser impressa
    string[100]=NUMERO_PONTOS_GANHAR+48; //gambiarra pra enviar a string pra ser impressa
    string[131]=ROUNDS_GANHAR+48; //gambiarra pra enviar a string pra ser impressa
    print_central(string);
    print_central("rounds, é o vencedor.");
    puts (""); 
    puts (""); 
    print_central ("ENJOY!");
    getch();
}
void menu(){
    system ("clear"); //limpar a tela
    centralizar_vertical(10); // centralizar verticalmente para imprimir 10 linhas no meio da tela
    print_central("|  Menu  |");
    puts (""); //equivalente a um /n
    print_central("APERTE [ESPAÇO] PARA JOGAR!");
    puts (""); 
    print_central("[I]nstruções");
    print_central("[C]réditos");
    print_central("[S]air");
}
void creditos(){
    system ("clear"); //limpar a tela
    centralizar_vertical(16); // centralizar verticalmente para imprimir 16 linhas no meio da tela
    print_central("|  Créditos  |");
    puts ("");   //equivalente a um /n
    print_central("Equipe:");
    print_central("Caio");
    print_central("Igor");
    print_central("Sergio");
    print_central("Vinícius");
    puts ("");
    print_central("Agradecimentos:");
    puts ("");
    print_central("Rodrigo Paes");
    print_central("Alfredo Lima");
    print_central("Arthur Vangasse");
    getch();
}
