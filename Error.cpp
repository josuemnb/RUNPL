#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Error.h"

int erro;

Erros ERROS;

int showErr(int msg, int line, const char token[50]){
	char s[80];

	switch(msg){
		case 1:strcpy(s,"Falta memoria");break;
		case 2:strcpy(s,"Ficheiro não existe");break;
		case 3:strcpy(s,"Esperado caracter de atribuicao");break;
		case 4:strcpy(s,"Falta aspas");break;
		case 5:strcpy(s,"Falta keyword AS");break;
		case 6:strcpy(s,"Biblioteca ja existe");break;
		case 7:strcpy(s,"Tipo desconhecido");break;
		case 8:strcpy(s,"Incompatibilidade entre tipos");break;
		case 9:strcpy(s,"Esperado um Nome");break;
		case 10:strcpy(s,"Esperado ponto de definicao");break;
		case 11:strcpy(s,"Nome desconhecido");break;
		case 12:strcpy(s,"Esperado palavra DO");break;
		case 13:strcpy(s,"Blocos nao fechados");break;
		case 14:strcpy(s,"Variavel ja existe");break;
		case 15:strcpy(s,"Esperado palavra NEXT");break;
		case 16:strcpy(s,"Esperado palavra TO");break;
		case 17:strcpy(s,"Condicao mal definida");break;
		case 18:strcpy(s,"Biblioteca Desconhecida");break;
		case 19:strcpy(s,"Esperado Fim da linha");break;
		case 20:strcpy(s,"Esperado ':'");break;
		case 21:strcpy(s,"Inesperado Fim da linha");break;
		case 22:strcpy(s,"Variavel Desconhecida");break;
		case 23:strcpy(s,"Bloco IF nao iniciado");break;
		case 24:strcpy(s,"Esperado =");break;
		case 25:strcpy(s,"KeyWord inesperada");break;
		case 26:strcpy(s,"Não é possivel alterar o valor de uma constante");break;
		case 27:strcpy(s,"Esperado um numero");break;
		case 28:strcpy(s,"Esperado uma frase");break;
		case 29:strcpy(s,"Esperado FROM");break;
		case 30:strcpy(s,"Esperado uma comparação");break;
		case 31:strcpy(s,"Esperado um valor numerico ou frase");break;
		case 32:strcpy(s,"Nome da variavel ou nome de comparacao já está a ser usado");break;
		case 33:strcpy(s,"Esperado um endif");break;
		default:strcpy(s,"Erro desconhecido");break;
	}
	erro++;
	printf("ERROR: na linha %d - \"%s\" - %s\n",line,token,s);
	//printf("\n\tErro na linha %d - %s\n\t\t%s - %s\n",LineIndex,Line,s,token);
	exit(msg);
}

int get_errors(){
    return erro;
}
