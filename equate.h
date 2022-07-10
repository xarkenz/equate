#ifndef EQ_EQUATE_H
#define EQ_EQUATE_H


#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>


#define EQ_ANSI_RED     "\x1b[31m"
#define EQ_ANSI_GREEN   "\x1b[32m"
#define EQ_ANSI_YELLOW  "\x1b[33m"
#define EQ_ANSI_BLUE    "\x1b[34m"
#define EQ_ANSI_MAGENTA "\x1b[35m"
#define EQ_ANSI_CYAN    "\x1b[36m"
#define EQ_ANSI_RESET   "\x1b[0m"


typedef size_t EqSize;
typedef int64_t EqInt;
typedef double EqNum;
typedef bool EqBool;
typedef wchar_t EqChar;

typedef enum EqType {
    EQ_NULL = 0,
    EQ_INT,
    EQ_NUM,
    EQ_BOOL,
    EQ_CHAR,
    EQ_STR,
    EQ_ARRAY,
    EQ_SPACE,
    EQ_FUNC,
    EQ_NAME,
    EQ_OPER,
    EQ_PROC,
} EqType;

typedef enum EqOper {
    EQO_INVALID,
    EQO_ARRAY, EQO_PROC, EQO_SPACE,                            // Container
    EQO_ACCESS,                                                // Access
    EQO_CALL, EQO_INDEX,                                       // Postfix
    EQO_POS, EQO_NEG, EQO_BNOT, EQO_NOT, EQO_EVAL, EQO_FINAL,  // Prefix
    EQO_EXP,                                                   // Exponent
    EQO_MUL, EQO_DIV, EQO_MOD,                                 // Multiplicative
    EQO_ADD, EQO_SUB,                                          // Additive
    EQO_LSHIFT, EQO_RSHIFT,                                    // Shift
    EQO_LT, EQO_GT, EQO_LTEQ, EQO_GTEQ,                        // Relational
    EQO_EQ, EQO_NEQ,                                           // Equality
    EQO_BAND, EQO_BXOR, EQO_BOR,                               // Bitwise
    EQO_AND, EQO_XOR, EQO_OR,                                  // Logical
    EQO_RANGE,                                                 // Array definition
    EQO_COND,                                                  // Conditional
    EQO_SET, EQO_SETEVAL,                                      // Assignment
} EqOper;

typedef enum EqPrecedence {
    EQP_INVALID,
    EQP_CONTAINER,
    EQP_ACCESS,
    EQP_POSTFIX,
    EQP_PREFIX,
    EQP_EXPONENT,
    EQP_MULTIPLICATIVE,
    EQP_ADDITIVE,
    EQP_SHIFT,
    EQP_RELATIONAL,
    EQP_EQUALITY,
    EQP_BITWISE_AND, EQP_BITWISE_XOR, EQP_BITWISE_OR,
    EQP_LOGICAL_AND, EQP_LOGICAL_XOR, EQP_LOGICAL_OR,
    EQP_ARRAY_DEF,
    EQP_CONDITIONAL,
    EQP_ASSIGNMENT,
} EqPrecedence;

typedef enum EqStmtKind {
    EQS_EMPTY,
    EQS_EXPR,
    EQS_IF,
    EQS_WHILE,
    EQS_EACH,
    EQS_BREAK,
    EQS_CONTINUE,
    EQS_YIELD,
    EQS_DELETE,
    EQS_PRINT,
    EQS_PRINTLN,
    EQS_LOAD,
} EqStmtKind;

typedef struct EqStr {
    EqSize refCt;
    EqSize len;
    const EqChar* data;
} EqStr;

struct EqArray;
struct EqSpace;
struct EqFunc;
struct EqProc;

typedef struct EqObj {
    EqType type;
    union {
        EqInt vInt;
        EqNum vNum;
        EqBool vBool;
        EqChar vChar;
        EqStr* vStr;
        struct EqArray* vArray;
        struct EqSpace* vSpace;
        struct EqFunc* vFunc;
        struct EqProc* vProc;
    };
} EqObj;

typedef struct EqArray {
    EqSize refCt;
    EqSize len;
    EqSize cap;
    EqObj* data;
} EqArray;

typedef struct EqVar {
    EqStr* name;
    EqObj value;
    EqBool isFinal;
} EqVar;

typedef struct EqSpace {
    EqSize refCt;
    EqStr* name;
    EqSize len;
    EqSize cap;
    EqVar* data;
} EqSpace;

typedef struct EqFunc {
    EqSize refCt;
    EqSize paramCt;
    EqStr** params;
    EqObj body;
} EqFunc;

struct EqStmt;

typedef struct EqProc {
    EqSize refCt;
    EqSize len;
    struct EqStmt* data;
} EqProc;

typedef struct EqStmt {
    EqStmtKind kind;
    union {
        EqObj expr;
    };
} EqStmt;


EqObj eqNewInt(EqInt value);
EqObj eqNewNum(EqNum value);
EqObj eqNewBool(EqBool value);
EqObj eqNewChar(EqChar value);
EqObj eqNewStr(EqSize len, const EqChar* data);
EqObj eqNewArray(EqSize len, EqObj* data);
EqObj eqNewSpace(EqStr* type);
EqObj eqNewFunc(EqSize paramCt, EqStr** params, EqObj body);

EqObj eqCopy(EqObj obj);
EqStr* eqCopyStr(EqStr* str);

void eqDelete(EqObj obj);
void eqDeleteStr(EqStr* str);

EqObj eqConcatStr(EqStr* s1, EqStr* s2);
EqInt eqCompareStr(EqStr* s1, EqStr* s2);


#endif /* EQ_EQUATE_H */