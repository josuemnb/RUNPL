/*
 * File:   Error.h
 * Author: jony
 *
 * Created on 12 de Junho de 2013, 22:51
 */

#ifndef ERROR_H
#define	ERROR_H


 class Erros {
    public:
    static const int LACK_OF_MEMORY=1;
    static const int FILE_NOT_FOUND=2;
    static const int EXPECTED_ATTRIB=3;
    static const int MISSING_QUOTE=4;
    static const int MISSING_KEY_AS=5;
    static const int LIBRARY_EXISTS=6;
    static const int UNKNOW_TYPE=7;
    static const int TYPE_NOT_MATCH=8;
    static const int EXPECTED_NAME=9;
    static const int EXPECTED_POINT=10;
    static const int UNKNOW_NAME=11;
    static const int EXPECTED_DO=12;
    static const int BLOCK_NOT_CLOSED=13;
    static const int VARIABLE_EXISTS=14;
    static const int EXPECTED_NEXT=15;
    static const int EXPECTED_TO=16;
    static const int BAD_CONDITION=17;
    static const int UNKNOW_LIBRARY=18;
    static const int EXPECTED_END_OF_LINE=19;
    static const int EXPECTED_TWO_POINTS=20;
    static const int UNEXPECTED_END_OF_LINE=21;
    static const int UNKNOW_VARIABLE=22;
    static const int IF_BLOCK_BAD_DEFINIED=23;
    static const int EXPECTED_ASSIGN=24;
    static const int UNKNOW_KEYWORD=25;
    static const int CONSTANT_UNMODIFIABLE=26;
    static const int EXPECTED_NUMBER=27;
    static const int EXPECTED_SETENCE=28;
    static const int EXPECTED_FROM=29;
    static const int EXPECTED_COMPARASION=30;
    static const int EXPECTED_NAME_NUMBER=31;
    static const int BLOCK_NAME_USED=32;
    static const int EXPECTED_ENDIF=33;
};


//enum ERRORS{
//    MEMORY,
//    FILENOTFOUND
//};

int showErr(int msg, int line, const char token[50]);
int get_errors();



#endif	/* ERROR_H */

