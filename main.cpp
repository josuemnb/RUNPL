#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <time.h>

#include "Tokenizer.h"
#include "Structs.h"
#include "Error.h"

#ifndef nil
#define nil "\0"
#endif

enum types { NONE=0, VAR, READ, CONST, IF, ELSE, ENDIF, PUTS, NEXT, BREAK, CONTINUE, SPIN, EXIT, FROM, TO, BY, AGAIN};
enum group { VOID, DECLARE, LOOPS, CONTROL, FUNCTIONS, BREAKERS };

RESERVED RWORDS[]={
    {"var",      VAR,        DECLARE},
    {"read",     READ,       FUNCTIONS},
    {"const",    CONST,      DECLARE},
    {"if",       IF,         CONTROL},
    {"else",     ELSE,       CONTROL},
    {"endif",    ENDIF,      CONTROL},
    {"''",       PUTS,       FUNCTIONS},
    {"next",     NEXT,       BREAKERS},
    {"break",    BREAK,      BREAKERS},
    {"continue", CONTINUE,   BREAKERS},
    {"spin",     SPIN,       LOOPS},
    {"exit",     EXIT,       FUNCTIONS},
    {"from",     FROM,       LOOPS},
    {"to",       TO,         LOOPS},
    {"by",       BY,         LOOPS},
    {"again",    AGAIN,      BREAKERS}
};

Tokenizer token;
extern Erros ERROS;

struct LOOP_TABLE {
    int init;
    int step;
    int final;
    int count;
    long pos;
    char *varPtr;
}LOOPt[10];

struct IF_TABLE {
    bool isUsed;
    bool hasElse;
    char varName[20];
}IFt[10];

unsigned int ind = 0;
unsigned int blk = 0;
unsigned int ifBlock = 0;

time_t start,stop;
//bool running=true;

void evalIf();      int evalBlock();
void evalElse();    void evalVar();
void evalAttrib();  void evalConst();
void evalDebug();   void evalPuts();
void evalRead();    void evalSpin();
void gotoElse();

void evalVar() {

    int j;
    char ch;
    char type=0;
    char name[20];
    if (!token.GetTheNextName())
        showErr(ERROS.EXPECTED_NAME,token.ReturnActualLine(),token.ReturnTheToken());
    if(CheckVars(token.ReturnTheToken()))
        showErr(ERROS.VARIABLE_EXISTS,token.ReturnActualLine(),token.ReturnTheToken());
    strcpy(name,token.ReturnTheToken());
    ch=token.GetTheNextChar();
    if (ch==':'){//se encontra :
        if (!token.GetTheNextName())
            showErr(ERROS.EXPECTED_NAME,token.ReturnActualLine(),token.ReturnTheToken());
        if (!strcmp(token.ReturnTheToken(), "string"))
            type = 1;
        else if (!strcmp(token.ReturnTheToken(), "real"))
            type = 2;
        else
            showErr(ERROS.UNKNOW_TYPE,token.ReturnActualLine(),token.ReturnTheToken());
        AddVar(name,type,nil);
        return;
    }
    else if (ch == '<'){//se encontra <<
        if(token.GetTheNextChar()!='<')
            showErr(ERROS.EXPECTED_ATTRIB,token.ReturnActualLine(),token.ReturnTheToken());
        if(token.IsThisTheNextChar('\'')){
        //se detecta que é uma string
            if(!token.GetStringInTheLine())
                showErr(ERROS.MISSING_QUOTE,token.ReturnActualLine(),token.ReturnTheToken());
            AddVar(name,1,token.ReturnTheString());
        }
        return;
    }
    else AddVar(name,0,nil);
    return;
}

void evalDebug() {

    if(token.GetTheNextName()){
        vV=CheckVars(token.ReturnTheToken());
        if(vV==NULL){
            showErr(ERROS.UNKNOW_VARIABLE,token.ReturnActualLine(),token.ReturnTheToken());
            return;
        }
        if(vV->value[0]=='\0')
            puts("DEBUG: NULL");
        else
            printf("DEBUG: %s\n", vV->value);
        return;
    }
    showErr(ERROS.EXPECTED_NAME,token.ReturnActualLine(),token.ReturnTheToken());
    return;
}

void evalRead() {

    if (!token.GetTheNextName())
        showErr(ERROS.EXPECTED_NAME,token.ReturnActualLine(),token.ReturnTheToken());
    vV=CheckVars(token.ReturnTheToken());
    if(vV==NULL)
        showErr(ERROS.UNKNOW_VARIABLE,token.ReturnActualLine(),token.ReturnTheToken());
    if(vV->type==3)
        showErr(ERROS.CONSTANT_UNMODIFIABLE,token.ReturnActualLine(),token.ReturnTheToken());
    scanf(vV->value);
    UpdateVar(vV);
    return;
}

void evalConst() {
    int j;

    if(!token.GetTheNextName())
        showErr(ERROS.EXPECTED_NAME,token.ReturnActualLine(),token.ReturnTheToken());
    if(CheckVars(token.ReturnTheToken()))
        showErr(ERROS.UNKNOW_VARIABLE,token.ReturnActualLine(),token.ReturnTheToken());
    char *temp=token.ReturnTheToken();
    if (token.GetTheNextChar()!='=')
        showErr(ERROS.EXPECTED_ASSIGN,token.ReturnActualLine(),token.ReturnTheToken());
    j=token.GetTheNextToken();
    if (j == STRING) {
        if (!token.GetStringInTheLine())
            showErr(ERROS.MISSING_QUOTE,token.ReturnActualLine(),token.ReturnTheToken());
        if (!token.IsTheEndOfTheLine())
            showErr(ERROS.EXPECTED_END_OF_LINE,token.ReturnActualLine(),token.ReturnTheToken());
        token.IncreaseLineCounter();
        AddVar(temp,'3',token.ReturnTheString());
        return;
    } else if (j == NUMBER) {
        if (!token.IsTheEndOfTheLine())
            showErr(ERROS.EXPECTED_END_OF_LINE,token.ReturnActualLine(),token.ReturnTheToken());
        token.IncreaseLineCounter();
        AddVar(temp,'3',token.ReturnTheToken());
        return;
    }
    showErr(ERROS.EXPECTED_NAME_NUMBER,token.ReturnActualLine(),token.ReturnTheToken());
    return;
}
void gotoElse() {
    int tok;

    tok=token.GetTheNextName();
    while(strcmp("endif",token.ReturnTheToken()) && strcmp("else",token.ReturnTheToken()) && tok!=-1){
        token.GotoTheNextLine();
        tok=token.GetTheNextName();
    }
    if(!strcmp("else",token.ReturnTheToken())){
        evalElse();
        return;
    }
    token.GotoTheNextLine();
    return;
}

void evalElse() {
    int tok=1;
    if(!token.IsTheEndOfTheLine()){
        token.GetTheNextName();
        if(strcmp(token.ReturnTheToken(),"if")){
            //se não existir um if á frente do else significa que é só com uma linha
            //e portanto tem que retornar atrás o codigo lido e analisar essa linha
            token.PullBackToken();
            evalBlock();
            token.IncreaseLineCounter();
            return;
        }
        else evalIf();
        return;
    }
    //como encontrou o fim da linha no código do else
    //significa que está perante um bloco para o else
    //processa tudo o que está dentro do bloco até que surja um else ou endif
    while(tok==1)
        tok=evalBlock();
    if(tok==3)showErr(ERROS.EXPECTED_ENDIF,token.ReturnActualLine(),token.ReturnTheToken());
    else if(tok==4)return;
}

void evalIf() { //esta função vai desenvolver sobre a keyword IF, ELSE e ELSEIF

    int tok=1;
    int l=0;//retorno da comparação
    int ifT=ifBlock;
    bool result=false;

    if(!token.GetTheNextName())
        showErr(ERROS.EXPECTED_NAME,token.ReturnActualLine(),token.ReturnTheToken());
    vV=CheckVars(token.ReturnTheToken());
    if(vV==NULL)
        showErr(ERROS.UNKNOW_VARIABLE,token.ReturnActualLine(),token.ReturnTheToken());
    if(!strcmp(IFt[ifT].varName,vV->name))
        showErr(ERROS.BLOCK_NAME_USED,token.ReturnActualLine(),token.ReturnTheToken());

    int cond=token.GetComparasionSymbol();//passa o valor de comparação para o COND
    if(!cond)
        showErr(ERROS.EXPECTED_COMPARASION,token.ReturnActualLine(),token.ReturnTheToken());
    if(vV->type==1){
        if(token.GetTheNextChar()!='\'')
            showErr(ERROS.EXPECTED_SETENCE,token.ReturnActualLine(),token.ReturnTheToken());
        if(!token.GetStringInTheLine())
            showErr(ERROS.MISSING_QUOTE,token.ReturnActualLine(),token.ReturnTheToken());
        l=strcmp(vV->value,token.ReturnTheString());
    }
    else if(vV->type==2){
        if(token.GetTheNextToken()!=NUMBER)
            showErr(ERROS.EXPECTED_NUMBER,token.ReturnActualLine(),token.ReturnTheToken());
        l=strcmp(vV->value,token.ReturnTheToken());
    }
    switch(cond){//compara os retorno da strcmp com a COND
        case EQ:
            if(l==0)result=true;
            break;
        case LE:
            if(l<=0) result=true;
            break;
        case GE:
            if(l>=0)result=true;
            break;
        case LT:
            if(l<0) result=true;
            break;
        case GT:
            if(l>0) result=true;
            break;
        case NE:
            if(l!=0) result=true;
            break;
        default: showErr(ERROS.BAD_CONDITION,token.ReturnActualLine(),token.ReturnTheToken());
            break;
    }
    if(!token.GetTheNextName())
        showErr(ERROS.EXPECTED_DO,token.ReturnActualLine(),token.ReturnTheToken());
    if(strcmp(token.ReturnTheToken(),"do"))
        showErr(ERROS.EXPECTED_DO,token.ReturnActualLine(),token.ReturnTheToken());
    //bloco de uma linha
    //ou seja, sem keyword endif ou else
    if(!token.IsTheEndOfTheLine()){
    // o bloco if só tem um linha
        if(result==false){
            token.GotoTheNextLine();
            return;
        }
        evalBlock();
        return;
    }
    if(result==false){
        gotoElse();
        return;
    }
    while(tok<3)
        tok=evalBlock();
    if(tok==3){//encontrou um else
        if(!token.IsTheEndOfTheLine()){
            token.GotoTheNextLine();
            return;
        }
        tok=token.GetTheNextName();
        while(strcmp("endif",token.ReturnTheToken()) && tok!=-1){
            token.GotoTheNextLine();
            tok=token.GetTheNextName();
        }
        if(!token.IsTheEndOfTheLine())
            showErr(ERROS.EXPECTED_END_OF_LINE,token.ReturnActualLine(),token.ReturnTheToken());
        token.IncreaseLineCounter();
    }
    else if(tok==4){//encontrou um endif
        if(!token.IsTheEndOfTheLine())
            showErr(ERROS.EXPECTED_END_OF_LINE,token.ReturnActualLine(),token.ReturnTheToken());
        token.IncreaseLineCounter();
    }
    return;
}

void evalAttrib(){

}

void evalPuts() {
    char ch;
    int j;

    if(!token.GetStringInTheLine())
        showErr(ERROS.MISSING_QUOTE,token.ReturnActualLine(),token.ReturnTheToken());
    printf("%s",token.ReturnTheString());
    if(token.IsTheEndOfTheLine())
        return;
    ch=token.GetTheNextChar();
    while(ch!='\n'){
        if(ch=='+'){
            j = token.GetTheNextToken();
            if (j == NAME) {
                vV=CheckVars(token.ReturnTheToken());
                if(vV==NULL)
                    showErr(ERROS.UNKNOW_VARIABLE,token.ReturnActualLine(),token.ReturnTheToken());
                printf("%s", vV->value);
            }
            else if(j == STRING){
                if(!token.GetStringInTheLine())
                    showErr(ERROS.MISSING_QUOTE,token.ReturnActualLine(),token.ReturnTheToken());
                printf("%s",token.ReturnTheString());
            }
        }
        else if(ch=='\''){
            if(!token.GetStringInTheLine())
                showErr(ERROS.MISSING_QUOTE,token.ReturnActualLine(),token.ReturnTheToken());
            printf("%s",token.ReturnTheString());
        }
        else if(ch==';'){
            if(!token.IsTheEndOfTheLine())
                showErr(ERROS.EXPECTED_END_OF_LINE,token.ReturnActualLine(),token.ReturnTheToken());
            token.IncreaseLineCounter();
            printf("\n");
            return;
        }
        else if(ch=='>'){
            if(token.GetTheNextChar()!='>')
                showErr(ERROS.EXPECTED_ATTRIB,token.ReturnActualLine(),token.ReturnTheToken());
            //espera-se que seja uma variavel para receber o valor tipo read
            if(!token.GetTheNextName())
                showErr(ERROS.EXPECTED_NAME,token.ReturnActualLine(),token.ReturnTheToken());
            vV=CheckVars(token.ReturnTheToken());
            if(vV==NULL)
                showErr(ERROS.UNKNOW_VARIABLE,token.ReturnActualLine(),token.ReturnTheToken());
            gets(vV->value);
            UpdateVar(vV);
            return;
        }
        else showErr(ERROS.UNKNOW_TYPE,token.ReturnActualLine(),token.ReturnTheToken());
        ch=token.GetTheNextChar();
    }
    token.IncreaseLineCounter();
    return;
}

void evalSpin() {

    int tok=1;

    blk=ind;
    ind++;
    if(token.GetTheNextNumber()){
        LOOPt[blk].init=0;
        LOOPt[blk].final=atoi(token.ReturnTheToken());
        for(LOOPt[blk].count=LOOPt[blk].init;LOOPt[blk].count<=LOOPt[blk].final;LOOPt[blk].count++){
                tok=1;
        while(tok==1)
            tok=evalBlock();
        if(tok==2)
            break;
        }
        if(ind>0)ind--;
        return;
    }
    if(!token.GetTheNextName())
        showErr(ERROS.EXPECTED_NAME,token.ReturnActualLine(),token.ReturnTheToken());
    vV=CheckVars(token.ReturnTheToken());
    if(vV==NULL)
        showErr(ERROS.UNKNOW_VARIABLE,token.ReturnActualLine(),token.ReturnTheToken());
    if(!token.GetTheNextName())
        showErr(ERROS.EXPECTED_FROM,token.ReturnActualLine(),token.ReturnTheToken());
    if(strcmp(token.ReturnTheToken(),"from"))
        showErr(ERROS.EXPECTED_FROM,token.ReturnActualLine(),token.ReturnTheToken());
    if(!token.GetTheNextNumber())
        showErr(ERROS.EXPECTED_NUMBER,token.ReturnActualLine(),token.ReturnTheToken());
    if(vV->type==3)
        showErr(ERROS.CONSTANT_UNMODIFIABLE,token.ReturnActualLine(),token.ReturnTheToken());
    vV->type=2;
    LOOPt[blk].varPtr=vV->value;
    LOOPt[blk].init=atoi(token.ReturnTheToken());
    if(!token.GetTheNextName())
        showErr(ERROS.EXPECTED_TO,token.ReturnActualLine(),token.ReturnTheToken());
    if(strcmp(token.ReturnTheToken(),"to"))
        showErr(ERROS.EXPECTED_TO,token.ReturnActualLine(),token.ReturnTheToken());
    if(!token.GetTheNextNumber())
        showErr(ERROS.EXPECTED_NUMBER,token.ReturnActualLine(),token.ReturnTheToken());
    LOOPt[blk].final=atoi(token.ReturnTheToken());
    if(!token.IsTheEndOfTheLine())
        showErr(ERROS.EXPECTED_END_OF_LINE,token.ReturnActualLine(),token.ReturnTheToken());
    token.IncreaseLineCounter();
    LOOPt[blk].pos=token.ReturnActualPosition();
    for(LOOPt[blk].count=LOOPt[blk].init;LOOPt[blk].count<=LOOPt[blk].final;LOOPt[blk].count++) {
        sprintf(LOOPt[blk].varPtr,"%d",LOOPt[blk].count);
        UpdateVar(vV);
        tok=1;
        while(tok==1)
            tok=evalBlock();
        if(tok==2)
            break;
    }
    if(ind>0)ind--;
}

int evalBlock() {
    int tok;

    tok=token.GetTheNextToken();
    if (tok== NAME) {
        if (!strcmp(token.ReturnTheToken(), "debug")){
            evalDebug();
            return 1;
        }
        else if (!strcmp(token.ReturnTheToken(), "var")){
            evalVar();
            return 1;
        }
        else if (!strcmp(token.ReturnTheToken(),"const")){
            evalConst();
            return 1;
        }
        else if (!strcmp(token.ReturnTheToken(),"spin")){
            evalSpin();
            return 1;
        }
        else if (!strcmp(token.ReturnTheToken(),"if")){
            evalIf();
            return 1;
        }
        else if (!strcmp(token.ReturnTheToken(),"read")){
            evalRead();
            return 1;
        }
        else if(!strcmp(token.ReturnTheToken(),"else"))
            return 3;
        else if(!strcmp(token.ReturnTheToken(),"endif"))
            return 4;
        else if(!strcmp(token.ReturnTheToken(),"exit")){
            exit(1);
        }
        else if(!strcmp(token.ReturnTheToken(),"break")){
            //puts("INFO: break");
            blk--;
            return 2;
        }
        else if(!strcmp(token.ReturnTheToken(),"continue")){
            return 6;
        }
        else if(!strcmp(token.ReturnTheToken(),"next")){
            if(LOOPt[blk].count==LOOPt[blk].final){
                blk--;
                return 2;
            }
            token.GotoToPosition(LOOPt[blk].pos);
            return 0;
        }
        else {
            vV=CheckVars(token.ReturnTheToken());
            if(vV==NULL)
                showErr(ERROS.UNKNOW_NAME,token.ReturnActualLine(),token.ReturnTheToken());
            if(vV->type==3)
                showErr(ERROS.CONSTANT_UNMODIFIABLE,token.ReturnActualLine(),token.ReturnTheToken());
            evalAttrib();
        }
    }
    else if(tok==STRING){
        evalPuts();
        return 1;
    }
    else if(tok==END)
        return -1;
    return 1;
}

void printAll(){
    printf("\n\nFim do Programa - %s\n",token.ReturnTheFileName());
    printf("Total de erros no programa - %d\n",get_errors());
    time(&stop);
    printf("Tempo decorrido: %0.2f\n", difftime(stop,start));
    CloseTables();
    printf("Tamanho maximo de valor para uma variavel é de %d\n",ReturnMaxSize());
    printf("Total of vars: %d\n",ReturnTotalVars());
}

int main() {

    int tok=0;//variavel de controlo de retorno do tokenizer
    InitTables();
    atexit(printAll);
    token.LoadThisFile("example.run");
    printf("RUN v0.2\n\n");
    time(&start);
    while(tok!=-1)
        tok=evalBlock();
    return 0;
}
