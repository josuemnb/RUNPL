#include "Tokenizer.h"
#include "Error.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>


extern Erros ERROS;;;

    int Tokenizer::GetLengthOfTheToken() //retorna o tamanho do Token
    {
        return LenghtOfToken;
    }
    char *Tokenizer::ReturnTheToken()//retorn o valor do Token como caracteres
    {
        return (char*)Token;
    }

    int Tokenizer::GetTheNextName(){
         int i=0;
         while(FileStream[Pointer]==' '|| FileStream[Pointer]=='\t' || FileStream[Pointer]=='\r')
                Pointer++;
         TotalOfTokens++;

         //if(FileStream[Pointer]=='\0')return -1;

         if(isalpha(FileStream[Pointer])){
            while(isalnum(FileStream[Pointer]))
               Token[i++]=FileStream[Pointer++];
            Token[i]='\0';
            LenghtOfToken=i-1;
            return 1;
         }
         return 0;
    }

    int Tokenizer::GetTheNextNumber(){
         int i=0;
         while(FileStream[Pointer]==' '|| FileStream[Pointer]=='\t' || FileStream[Pointer]=='\r')
                Pointer++;
         TotalOfTokens++;

         if(isdigit(FileStream[Pointer])){
                while(isdigit(FileStream[Pointer]))
                        Token[i++]=FileStream[Pointer++];
                Token[i]='\0';
                LenghtOfToken=i-1;
                return 1;
         }
         return 0;
    }

    int Tokenizer::GetTheNextToken()//carrega o proximo Token
    {
         int i=0;
         while(FileStream[Pointer]==' '|| FileStream[Pointer]=='\t' || FileStream[Pointer]=='\r')
                Pointer++;
         TotalOfTokens++;

         if(isalpha(FileStream[Pointer])){
            while(isalnum(FileStream[Pointer]))
               Token[i++]=FileStream[Pointer++];
            Token[i++]='\0';
            LenghtOfToken=i-1;
            return NAME;
         }
         else if(isdigit(FileStream[Pointer])){
                while(isdigit(FileStream[Pointer]))
                        Token[i++]=FileStream[Pointer++];
                Token[i++]='\0';
                LenghtOfToken=i;
                return NUMBER;
         }
         else if(FileStream[Pointer]=='+'){
            Pointer++;
            if(FileStream[Pointer]=='+'){
                Pointer++;
                LenghtOfToken=2;
                Token[0]=INC;
                Token[1]='\0';
                return ARITMETIC;
            }
            LenghtOfToken=1;
            Token[0]=ADD;
            Token[1]='\0';
            return ARITMETIC;
         }
         else if(FileStream[Pointer]=='-'){
            Pointer++;
            if(FileStream[Pointer]=='-'){
                Pointer++;
                LenghtOfToken=2;
                Token[0]=DEC;
                Token[1]='\0';
                return ARITMETIC;
            }
            LenghtOfToken=1;
            Token[0]=SUB;
            Token[1]='\0';
            return ARITMETIC;
         }
         else if(FileStream[Pointer]=='<'){
            Pointer++;
            if(FileStream[Pointer]=='='){
                Pointer++;
                LenghtOfToken=2;
                Token[0]=LE;
                Token[1]='\0';
                return COMPARE;
            }
            else if(FileStream[Pointer]=='<'){
                Pointer++;
                LenghtOfToken=2;
                Token[0]=READIN;
                Token[1]='\0';
                return READIN;
            }
            LenghtOfToken=1;
            Token[0]=LT;
            Token[1]='\0';
            return COMPARE;
        }
        else if(FileStream[Pointer]=='>'){
            Pointer++;
            if(FileStream[Pointer]=='='){
                Pointer++;
                LenghtOfToken=2;
                Token[0]=GE;
                Token[1]='\0';
                return COMPARE;
            }
            LenghtOfToken=1;
            Token[0]=GT;
            Token[1]='\0';
            return COMPARE;
        }
        else if(FileStream[Pointer]=='='){
            Pointer++;
            if(FileStream[Pointer]=='='){
                Pointer++;
                LenghtOfToken=2;
                Token[0]=EQ;
                Token[1]='\0';
                return COMPARE;
            }
            LenghtOfToken=1;
            return ATRIB;
        }
        else if(FileStream[Pointer]=='!'){
            Pointer++;
            if(FileStream[Pointer]=='='){
                Pointer++;
                LenghtOfToken=2;
                Token[0]=NE;
                Token[1]='\0';
                return COMPARE;
            }
            LenghtOfToken=1;
            Token[0]=NOT;
            Token[1]='\0';
            return LOGICAL;
        }
        switch(FileStream[Pointer]){
            case '\0':return END;break;
            case '\n':Pointer++;LineCounter++;return NL;break;

            case ';':LenghtOfToken=1;Pointer++;;return COMM;break;

            case '/':
            case '*':
            case '^':
            case '%':
            case '(':
            case '+':
            case '-':
            case ')':LenghtOfToken=1;Pointer++;return ARITMETIC;break;

            case '"':
            case '\'':LenghtOfToken=1;Pointer++;return STRING;break;

            case '#':LenghtOfToken=1;Pointer++;return COMPARE;break;

            case ':':LenghtOfToken=1;Pointer++;return DEFINE;break;
            case ',':
            case '_':
            case '.':LenghtOfToken=1;Pointer++;return PONTUATOR;break;
            default:return ERROR;break;

        }
        return 0;
}

    long Tokenizer::GetSizeOfTheFile()//retorna o tamanho do ficheiro
    {
        return SizeOfFile;
    }
    int Tokenizer::GetTheTotalOfTokens()//retorna a quantidade de Token já lido
    {
        return TotalOfTokens;
    }

    int Tokenizer::GetTheTotalOfLines()
    {
        return LineCounter;
    }
    void Tokenizer::SkipSpaces()//salta espaços na linha
    {
        while(FileStream[Pointer]==' '|| FileStream[Pointer]=='\t')
            Pointer++;
//        while(isspace(FileStream[Pointer]))
//            Pointer++;
    }
    int Tokenizer::IsThisTheNextChar(char c)//confirma o proximo caracter
    {
        SkipSpaces();
        if(FileStream[Pointer]!=c){
            //Pointer--;
            return 0;
        }
        if(c=='\n')LineCounter++;
        Pointer++;
        return 1;
    }

    int Tokenizer::GetAttrib(){

        SkipSpaces();
        if(FileStream[Pointer]=='<'){
            Pointer++;
            if(FileStream[Pointer]=='<'){
                Pointer++;
                return 1;
            }
            Pointer--;
            return 0;
        }
        return 0;

    }

    void Tokenizer::GotoToPosition(long pos){
        Pointer=pos;
    }

    long Tokenizer::ReturnActualPosition(){
        return Pointer;
    }


    char Tokenizer::GetTheNextChar() //devolve o proximo caracter
    {
        while(FileStream[Pointer]==' '|| FileStream[Pointer]=='\t' || FileStream[Pointer]=='\r')
            Pointer++;
        return FileStream[Pointer++];
    }

    int Tokenizer::GetStringInTheLine()//carrega uma string entre aspas
    {
        int i=0;

        SkipSpaces();
        while((FileStream[Pointer]!='\''&& FileStream[Pointer]!='"') && i<255)
        {
            if(FileStream[Pointer]=='\0'||FileStream[Pointer]=='\n'|| FileStream[Pointer]=='\r'){
                if(FileStream[Pointer]=='\n')LineCounter++;
                return 0;
            }
            StringInTheLine[i++]=FileStream[Pointer++];
        }
        Pointer++;
        StringInTheLine[i++]='\0';
        return 1;
    }

    void Tokenizer::GotoTheNextLine(){
        SkipLineInCode();
    }

    char *Tokenizer::ReturnTheFileStream(void)
    {
        return FileStream;
    }

    char *Tokenizer::ReturnTheString()//devolve a string obtida
    {
        return StringInTheLine;
    }
    int Tokenizer::LoadThisFile(const char *str)// carrega o ficheiro para a variavel file
    {
        FILE *fp;

        if((fp=fopen(str,"rt"))==NULL)
	{
		showErr(ERROS.FILE_NOT_FOUND,0,str);
		return 0;
	}
	fseek(fp,0,2);
	SizeOfFile=ftell(fp);
	fseek(fp,0,0);
	if((FileStream=(char*)calloc(sizeof(char),SizeOfFile))==NULL)
        {
            showErr(ERROS.LACK_OF_MEMORY,0,str);
               return -1;
        }
	fread((char*)FileStream,SizeOfFile,1,fp);
	fclose(fp);
        FileLoaded=1;
        Pointer=0;
        StateOfFileStream=1;
        strcpy(FileName,str);
	//printf("Ficheiro Carregado\n\n");

	return 1;
}

void Tokenizer::PullBackToken(){
    int i;

    for(i=0;i<=LenghtOfToken;i++)
        Pointer--;
}

int Tokenizer::ReturnActualLine(){
    return LineCounter;
}

int Tokenizer::IsTheEndOfTheLine(void)//confirma o fim da linha
{
//    if(FileStream[Pointer]=='\n' || FileStream[Pointer]=='\0' )
//        return 1;
    SkipSpaces();
    //Pointer++;
    if(FileStream[Pointer]=='\n' || FileStream[Pointer]=='\0' ){
        Pointer++;
        LineCounter++;
        return 1;
    }
    return 0;
}

int  Tokenizer::ExtractLineFrom(char *src, int pos, char delim){
    int i=0;
    //printf("%d\n",pos);
    if(pos>255)return -1;

    if(StateOfFileStream==0)
    {
       FileStream=(char*)calloc(sizeof(char),255);
       StateOfFileStream=1;
    }
    Pointer=0;
    if(src[pos]=='\0')return 0;
    for(i=pos;*src!='\0' && *src!=delim;i++)
        FileStream[Pointer++]=*src++;
    FileStream[Pointer]='\0';
    //printf("Retorn da extração: %s",FileStream);
    Pointer=0;
    ++i;
    return i;
}


int Tokenizer::LoadThisLine(char *str)//define o texto a controlar
{
   if(StateOfFileStream==0)
   {
      FileStream=(char*)calloc(sizeof(char),255);
      StateOfFileStream=1;
   }

   Pointer=0;
   strcpy(FileStream,str);
   return 1;
}

int Tokenizer::GetComparasionSymbol(){

    while(FileStream[Pointer]==' '|| FileStream[Pointer]=='\t' || FileStream[Pointer]=='\r')
        Pointer++;
    TotalOfTokens++;

    if(FileStream[Pointer]=='<'){
        Pointer++;
        if(FileStream[Pointer]=='='){
            Pointer++;
            return LE;
        }
        return LT;
    }
    else if(FileStream[Pointer]=='>'){
        Pointer++;
        if(FileStream[Pointer]=='='){
            Pointer++;
            return GE;
        }
        return GT;
    }
    else if(FileStream[Pointer]=='='){
        Pointer++;
        if(FileStream[Pointer]=='='){
            Pointer++;
            return EQ;
        }
        else return 0;
    }
    else if(FileStream[Pointer]=='!'){
        Pointer++;
        if(FileStream[Pointer]=='='){
            Pointer++;
            return NE;
        }
        return 0;
    }
    else return 0;
}

void Tokenizer::SkipLineInCode()
{

    while(FileStream[Pointer]!='\n' && FileStream[Pointer]!='\0')
        Pointer++;
    Pointer++;
    LineCounter++;
    return;
}

char *Tokenizer::ReturnTheFileName()
{
    return FileName;
}
Tokenizer::Tokenizer()
 {
StateOfFileStream=0;
LenghtOfToken=0;//tamanho do token
SizeOfFile=0;//tamanho do ficheiro
FileLoaded=0;//confirma que o ficheiro foi carregado
TotalOfTokens=0;//total de token obtidos
Pointer=0;//posicao na linha
LineCounter=0;//contador de linhas
NumberOfTokensInTheLine=0;//quantidades
StartedNewLine=0;
}

Tokenizer::~Tokenizer()
{
   if(StateOfFileStream==1)
       free(FileStream);
}

