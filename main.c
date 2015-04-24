#include "getch.h" // funcao para receber a tecla sem o usuario digitar enter
#include "funcoes_void.h" //funcoes de exibicao, como o menu
#include <stdio.h>
int main (){
	while (1){ //executa o menu a nao ser que o usuario digite S ou s (linha 17), e retorna 0. 
		menu(); 
		switch (getch()){
			case 32: printf("CHAMAR JOGO\n"); //32=espaço
					getch();
				break;
			case 73:
			case 105: instrucoes(); //73='I' e 105='i'
				break;
			case 67:
			case 99: creditos(); //67='C' e 99='c'
				break;
			case 83: 
			case 115: return 0;// 83='S' e 115='s' SAIR
		}
	}
}
