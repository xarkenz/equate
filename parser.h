#ifndef EQ_PARSER_H
#define EQ_PARSER_H


#include "equate.h"


typedef enum EqTokenKind {
    EQT_EOF,
    // Literals
    EQT_NAME, EQT_INT, EQT_NUM, EQT_BOOL, EQT_CHAR, EQT_STR,
    // Keywords
    EQT_IF, EQT_ELIF, EQT_ELSE,
    EQT_WHILE, EQT_EACH, EQT_IN,
    EQT_BREAK, EQT_CONTINUE, EQT_YIELD,
    EQT_PRINT, EQT_PRINTLN,
    EQT_LOAD,
    // Symbols
    EQT_ADD, EQT_SUB, EQT_MUL, EQT_DIV, EQT_MOD, EQT_EXP,
    EQT_BNOT, EQT_BAND, EQT_BXOR, EQT_BOR, EQT_LSHIFT, EQT_RSHIFT,
    EQT_NOT, EQT_AND, EQT_XOR, EQT_OR,
    EQT_EQ, EQT_NEQ, EQT_LT, EQT_GT, EQT_LTEQ, EQT_GTEQ,
    EQT_SET, EQT_SETEVAL,
    EQT_COND,
    EQT_DOTS,
    EQT_DOT,
    EQT_ATSYM,
    EQT_COMMA, EQT_COLON, EQT_SEMICOLON,
    EQT_LPAREN, EQT_RPAREN,
    EQT_LSQUARE, EQT_RSQUARE,
    EQT_LCURLY, EQT_RCURLY,
} EqTokenKind;

typedef enum EqParseStatus {
    EQPS_OK,
    EQPS_NO_MEM,
    EQPS_UNEXPECTED,
    EQPS_EXPECTED,
    EQPS_UNCLOSED,
    EQPS_NOT_ENOUGH_OPERANDS,
    EQPS_EMPTY_PARENS,
} EqParseStatus;

typedef struct EqToken {
    EqTokenKind kind;
    EqSize pos;
    union {
        EqInt vInt;
        EqNum vNum;
        EqBool vBool;
        EqChar vChar;
        EqStr* vStr;
    };
} EqToken;

typedef struct EqParser {
    FILE* stream;
    EqSize pos;
    EqToken token;
    EqOper* opers;
    EqSize operCt;
    EqObj* objs;
    EqSize objCt;
    EqParseStatus status;
    EqStr* what;
} EqParser;


void eqPrintToken(EqToken token);  // Debug

void eqGrabWord(EqParser* parser);
void eqGrabNumber(EqParser* parser);
void eqGrabString(EqParser* parser);
void eqGrabSymbol(EqParser* parser);

void eqSkipLineComment(EqParser* parser);
void eqSkipBlockComment(EqParser* parser);

void eqGrabToken(EqParser* parser);
bool eqNextToken(EqParser* parser);


#endif /* EQ_PARSER_H */