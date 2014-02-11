/*
 * File:   structs.h
 * Author: jony
 *
 * Created on 12 de Junho de 2013, 22:46
 */

#ifndef STRUCTS_H
#define	STRUCTS_H

//estruturas para tabelas de variaveis, bibliotecas e funcoes

struct RESERVED {
    char name[10];
    char type;
    char group;
};

typedef struct sVAR{// não precisa de valores pois não é executado, só conferido

	char name[20];
	char type;
	sVAR *next;
	char value[100];
    int index;
    bool hasChanged;
    int nChanged;
    int maxSize;
}VARt;

typedef struct sFOR {
    int init;
    int final;
    int count;
    long posCode;
    sVAR var;
    sFOR *next;
    sFOR *previous;
}FORt;

typedef struct sFUNCTION {
	char name[100];
	char ret;//tipo de retorno
	char param[10];// definição dos tipos de parametros
	short n_param;//numero de parametros que pede
	sFUNCTION *next;
	char status;
    int index;
    char *code;
   	int lenName;
	sVAR* var;
}FUNCTIONt;

typedef struct sLIB{
	char name[100];
	char address[1000];
	int n_functions;
	sFUNCTION *functions;
	sLIB *next;
	char status;
	int lenName;
	int lenAddress;
    int index;
}LIBt;

int AddLib(char name[100], char address[1000]);
LIBt* CheckLibrary(char *name);
int AddVar(char name[20],char type, char value[100]);
VARt* CheckVars(char *name);
int AddFunction(char name[100],char ret, long size);
FUNCTIONt* CheckFunction(char *name);
int AddLibFunction(LIBt* L,char name[100]);
void InitTables();
void CloseTables();
void PrintVars();
int AddFor(int init, int final, long pos, sVAR v);
int ReturnTotalVars();
int ReturnMaxSize();
void UpdateVar(VARt *V);//estruturas para o tokenizer, parser, scanner

extern VARt *V_TABLE;// listas principais
extern FUNCTIONt *F_TABLE;
extern LIBt *L_TABLE;
extern FORt *O_TABLE;

extern VARt* vV;// listas auxiliares
extern LIBt* vL;
extern FUNCTIONt* vF;
extern FORt* vO;

#endif	/* STRUCTS_H */

