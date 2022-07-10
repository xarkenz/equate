#include "equate.h"

#include <stdlib.h>
#include <string.h>


EqObj eqNewInt(EqInt value) {
    EqObj obj = {.type = EQ_INT, .vInt = value};
    return obj;
}

EqObj eqNewNum(EqNum value) {
    EqObj obj = {.type = EQ_NUM, .vNum = value};
    return obj;
}

EqObj eqNewBool(EqBool value) {
    EqObj obj = {.type = EQ_BOOL, .vBool = value};
    return obj;
}

EqObj eqNewChar(EqChar value) {
    EqObj obj = {.type = EQ_CHAR, .vChar = value};
    return obj;
}

EqObj eqNewStr(EqSize len, const EqChar* data) {
    EqStr value = {1, len, data};
    EqStr* str = malloc(sizeof(EqStr));
    *str = value;
    EqObj obj = {.type = EQ_STR, .vStr = str};
    return obj;
}

EqObj eqNewArray(EqSize len, EqObj* data) {
    EqArray value = {1, len, len, data};
    EqArray* array = malloc(sizeof(EqArray));
    *array = value;
    EqObj obj = {.type = EQ_ARRAY, .vArray = array};
    return obj;
}

EqObj eqNewSpace(EqStr* name) {
    EqSpace value = {1, name, 0, 0, NULL};
    EqSpace* space = malloc(sizeof(EqSpace));
    *space = value;
    EqObj obj = {.type = EQ_SPACE, .vSpace = space};
    return obj;
}

EqObj eqNewFunc(EqSize paramCt, EqStr** params, EqObj body) {
    EqFunc value = {1, paramCt, params, body};
    EqFunc* func = malloc(sizeof(EqFunc));
    *func = value;
    EqObj obj = {.type = EQ_FUNC, .vFunc = func};
    return obj;
}

EqObj eqCopy(EqObj obj) {
    switch (obj.type) {
        case EQ_NULL:
        case EQ_INT:
        case EQ_NUM:
        case EQ_BOOL:
        case EQ_CHAR:
            break;
        case EQ_STR:
        case EQ_NAME:
            ++obj.vStr->refCt;
            break;
        case EQ_ARRAY:
        default:
            ++obj.vArray->refCt;
            break;
        case EQ_SPACE:
            ++obj.vSpace->refCt;
            break;
        case EQ_FUNC:
            ++obj.vFunc->refCt;
            break;
    }
    return obj;
}

EqStr* eqCopyStr(EqStr* str) {
    ++str->refCt;
    return str;
}

void eqDelete(EqObj obj) {
    switch (obj.type) {
        case EQ_NULL:
        case EQ_INT:
        case EQ_NUM:
        case EQ_BOOL:
        case EQ_CHAR:
            break;
        case EQ_STR:
        case EQ_NAME:
            eqDeleteStr(obj.vStr);
            break;
        case EQ_ARRAY:
        default:
            if (obj.vArray->refCt <= 1) {
                for (EqSize i = 0; i < obj.vArray->len; ++i) {
                    eqDelete(obj.vArray->data[i]);
                }
                free(obj.vArray->data);
                free(obj.vArray);
                break;
            }
            --obj.vArray->refCt;
            break;
        case EQ_SPACE:
            if (obj.vSpace->refCt <= 1) {
                for (EqSize i = 0; i < obj.vSpace->len; ++i) {
                    eqDeleteStr(obj.vSpace->data[i].name);
                    eqDelete(obj.vSpace->data[i].value);
                }
                eqDeleteStr(obj.vSpace->name);
                free(obj.vSpace->data);
                free(obj.vSpace);
                break;
            }
            --obj.vSpace->refCt;
            break;
        case EQ_FUNC:
            if (obj.vFunc->refCt <= 1) {
                for (EqSize i = 0; i < obj.vFunc->paramCt; ++i) {
                    eqDeleteStr(obj.vFunc->params[i]);
                }
                free(obj.vFunc->params);
                eqDelete(obj.vFunc->body);
                free(obj.vFunc);
                break;
            }
            --obj.vFunc->refCt;
            break;
    }
}

void eqDeleteStr(EqStr* str) {
    if (str->refCt <= 1) {
        free((EqChar*) str->data);
        free(str);
        return;
    }
    --str->refCt;
}

EqObj eqConcatStr(EqStr* s1, EqStr* s2) {
    EqSize len = s1->len + s2->len;
    EqChar* data = malloc((len + 1) * sizeof(EqChar));
    memcpy(data, s1->data, s1->len * sizeof(EqChar));
    memcpy(data + s1->len, s2->data, s2->len * sizeof(EqChar));
    data[len] = 0;
    return eqNewStr(len, data);
}

EqInt eqCompareStr(EqStr* s1, EqStr* s2) {
    EqChar c1, c2;
    for (EqSize i = 0; ; ++i) {
        c1 = s1->data[i];
        c2 = s2->data[i];
        if (!c1 || !c2)
            return s1->len - s2->len;
        else if (c1 != c2)
            return c1 - c2;
    }
}