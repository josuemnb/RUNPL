/*
 * File:   tokenizer.h
 * Author: josue
 *
 * Created on 8 de Novembro de 2011, 10:57
 */

#ifndef TOKENIZER_H
#define	TOKENIZER_H

#define END     0      //\0

#define PONTUATOR       1
#define ARITMETIC       2
#define LOGICAL         3
#define COMPARE         4
#define NUMBER          5
#define NAME            6
#define STRING          7

//bloco de pontuadores
#define PUNCT   10      //.
#define COMMA   11      //,
#define TREMA   12      //;
#define ATRIB   13      //=
#define DEFINE  14      //:
#define CHAR    15      //'
#define COMM    16      // coment ;
#define READIN  17      // <<

//bloco de aritmetica
#define ADD     20      //+
#define SUB     21      //-
#define MUL     22      //*
#define DIV     23      // /
#define POW     24      //^
#define MOD     25      //%
#define INC     26      //++
#define DEC     27      //--
#define OPENP   28      //(
#define CLOSEP  29      //)

//bloco logicos
#define OR      30      //
#define AND     31      //
#define NOT     32      //
#define XOR     33      //

//bloco de comparadores
#define EQ      40
#define NE      41
#define LT      42
#define LE      43
#define GT      44
#define GE      45

#define ERROR   50
#define NL      51


class Tokenizer
{
private:

char        Token[50];//token obtido
int         StateOfFileStream;
int         LenghtOfToken;//tamanho do token
long int    SizeOfFile;//tamanho do ficheiro
char  *     FileStream;//string com o ficheiro
int         FileLoaded;//confirma que o ficheiro foi carregado
char        FileName[100];//nome do ficheiro
int         TotalOfTokens;//total de token obtidos
long        Pointer;//posicao na linha
int         LineCounter;//contador de linhas
char        StringInTheLine[255];//string entre aspas
int         NumberOfTokensInTheLine;//quantidades
int         StartedNewLine;

public:

int     GetLengthOfTheToken();//retorna o tamanho do token
long    GetSizeOfTheFile();//retorna o tamanho do texto
int     GetTheNextToken();//carrega o proximo token
int     GetTheTotalOfTokens();//retorna a quantidade de token já lido
int     GetTheTotalOfLines();//devolve a quantidade de linhas lidas
char    GetTheNextChar();//devolve o proximo caracter
int     GetStringInTheLine();//carrega uma string entre aspas
int     GetComparasionSymbol();//retorna qual o valor de comparacao
int     GetTheNextName();//obtem o proximo nome
int     GetTheNextNumber();//obtem o proximo numero
int     LoadThisFile(const char *str);//carrega o ficheiro de leitura
int     LoadThisLine(char *str);//funcao especial para o controle do token
int     ExtractLineFrom(char *src, int pos, char delim);
void    GotoTheNextLine();
void    GotoToPosition(long pos);
long    ReturnActualPosition();
char *  ReturnTheFileName();//devolve o nome do ficheiro
char *  ReturnTheString();//devolve a string carregado
char *  ReturnTheToken();//retorn o valor do token como caracteres
int     IsTheEndOfTheLine();//verifica o fim da linha
int     IsThisTheNextChar(char c);//confirma a presença do proximo caracter
void    SkipLineInCode();//salta até a proxima linha
void    SkipSpaces();//salta espaços em branco na linha
int     ReturnTheNumber();//devolve o inteiro obtido
char *  ReturnTheFileStream(void);//devolve a stream carrega
int     ReturnActualLine();
void    IncreaseLineCounter(){LineCounter++;};
void    PullBackToken();
int     GetAttrib();
        Tokenizer();//inicialiador
        ~Tokenizer();//destruidor

};

#endif	/* TOKENIZER_H */

