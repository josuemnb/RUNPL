#include "Structs.h"
#include "Error.h"

#include <string.h>
#include <malloc.h>
#include <stdio.h>


VARt *V_TABLE;// listas principais
FUNCTIONt *F_TABLE;
LIBt *L_TABLE;
FORt *O_TABLE;

VARt* vV;// listas auxiliares
LIBt* vL;
FUNCTIONt* vF;
FORt *vO;

extern Erros ERROS;

int maxSize=0;
int totalVars=0;
//obrigatorio executar esta função no inicio do main
void InitTables(void)
{
	V_TABLE=NULL;
	//L_TABLE=NULL;
	//F_TABLE=NULL;
	//O_TABLE=NULL;
	vV=NULL;
	//vF=NULL;
	//vL=NULL;
	//vO=NULL;

}

void CloseTables()
{
    VARt *start, *list_ptr;
    start=vV;
    while (start) {
        list_ptr = start;
        start = list_ptr->next;
        free(list_ptr);
    }
}

void PrintVars(void)
{
	sVAR *V;

	if(V_TABLE==NULL)
		return;

	V=V_TABLE;
	while(V)
	{
		puts(V->name);
		printf("%d\n",V->index);
		puts(V->value);
		V=V->next;
	}
	return;
}

FUNCTIONt* CheckLibFunction(LIBt L,char *name)// percorre todo o vector de variaveis e verifica se existe
{
	FUNCTIONt *V;

	if(L.n_functions ==0)
		return 0;
	V=L.functions;
	while(V!='\0')
	{
		if(!strcmp(V->name,name))
			return V;
		V=V->next;
	}
	return '\0';
}

int AddLibFunction(LIBt* L,char name[100])
{
	FUNCTIONt *V;

		//é necessário abrir o ficheiro e pedir as definicoes com respeito á funcao

		V=(FUNCTIONt*)malloc(sizeof(FUNCTIONt));
		if(V=='\0')
		{
			showErr(ERROS.LACK_OF_MEMORY,0,name);
			return 0;
		}
		strcpy(V->name,name);
		V->status=1;
		V->next='\0';
		if(L_TABLE->functions =='\0')
			L_TABLE->functions =V;
		else
		{
			V->next =L_TABLE->functions;
			L_TABLE->functions =V;
		}
		return 1;
}

FUNCTIONt* CheckFunction(char *name)// percorre todo o vector de variaveis e verifica se existe
{
	FUNCTIONt *V;

	if(F_TABLE=='\0')
		return 0;
	V=F_TABLE;
	while(V!='\0')
	{
		if(!strcmp(V->name,name))
			return V;
		V=V->next;
	}
	return '\0';
}

int AddFunction(char name[100],char ret, long size)
{
	FUNCTIONt *V;

		V=(FUNCTIONt*)malloc(sizeof(FUNCTIONt));
		if(V=='\0')
		{
			showErr(ERROS.LACK_OF_MEMORY,0,name);
			return 0;
		}
		strcpy(V->name,name);
		V->ret=ret;
		V->status=1;
		V->next='\0';
      if((V->code=(char*)malloc(size))==NULL)
      {
          showErr(ERROS.LACK_OF_MEMORY,0,name);
          return -1;
      }

		if(F_TABLE=='\0')
			F_TABLE=V;
		else
		{
			V->next =F_TABLE;
			F_TABLE=V;
		}
		return 1;
}

VARt* CheckVars(char *name)// percorre todo o vector de variaveis e verifica se existe
{
	VARt *V=V_TABLE;

	if(V=='\0')
		return NULL;
	while(V!='\0')
	{
		if(!strcmp(V->name,name))
			return V;
		V=V->next;
	}
	return NULL;
}

void UpdateVar(VARt * V ){

    int l=strlen(V->value);

    if(V->maxSize<l)
        V->maxSize=l;
    V->hasChanged=true;
    V->nChanged++;
    return;
}

int AddVar(char name[20],char type, char value[100])
{
    VARt *V;
    V=(VARt*)malloc(sizeof(VARt));
    if(V=='\0'){
        showErr(ERROS.LACK_OF_MEMORY,0,name);
	return 0;
    }
    V->type=type;
    strcpy(V->name,name);
    strcpy(V->value,value);
    V->next='\0';
    if(V_TABLE=='\0')
        V_TABLE=V;
    else {
        V->next =V_TABLE;
        V_TABLE=V;
    }
    int size=strlen(value);
    if(size>maxSize)
        maxSize=size;
    totalVars++;
    return 1;
}

int ReturnTotalVars(){ return totalVars; }
int ReturnMaxSize(){ return maxSize; }

LIBt* CheckLibrary(char *name)// percorre todo o vector de bibiotecas e verifica se existe
{
	LIBt *V;

	if(L_TABLE=='\0')
		return 0;
	V=L_TABLE;
	while(V!='\0')
	{
		if(!strcmp(V->name,name))
			return V;
		else V=V->next;
	}
	return '\0';
}

int AddLib(char name[100], char address[1000])
{
	LIBt *V;

		V=(LIBt*)malloc(sizeof(LIBt));
		if(V=='\0'){
		    showErr(ERROS.LACK_OF_MEMORY,0,name);
			return 0;
		}
		strcpy(V->name,name);
		strcpy(V->address,address);
		V->n_functions=0;
		V->status=1;
		V->next='\0';
		if(L_TABLE=='\0')
			L_TABLE=V;
		else
		{
			V->next =L_TABLE;
			L_TABLE=V;
		}
		return 1;
}

int AddFor(int init, int final, long pos, sVAR v){
    FORt *O;

    O=(FORt*)malloc(sizeof(FORt));
    if(O=='\0'){
        showErr(ERROS.LACK_OF_MEMORY,0,"");
        return 0;
    }
    O->init=init;
    O->final=final;
    O->posCode=pos;
    O->var=v;
    O->next='\0';
    if(O_TABLE=='\0')
        O_TABLE=O;
    else {
        O->next=O_TABLE;
        O_TABLE=O;
    }
    return 1;
}

