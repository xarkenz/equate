#include "parser.h"

#include <ctype.h>


#define EQM_NEXT getc(parser->stream); ++parser->pos;
#define EQM_PREV(c) { ungetc(c, parser->stream); --parser->pos; }
#define EQM_EOF feof(parser->stream)


void eqGrabToken(EqParser* parser) {
    EqChar c = EQM_NEXT;
    while (true) {
        if (EQM_EOF) {
            EqToken token = {.kind = EQT_EOF, .pos = parser->pos};
            parser->token = token;
            return;
        } else if (isspace(c)) {
            c = EQM_NEXT;
            continue;
        } else if (isalpha(c) || c == '_') {
            EQM_PREV(c);
            eqGrabWord(parser);
            return;
        } else if (isdigit(c)) {
            EQM_PREV(c);
            eqGrabNumber(parser);
            return;
        } else if (ispunct(c)) {
            if (c == '/') {
                int c1 = EQM_NEXT;
                if (c1 == '/') {
                    eqSkipLineComment(parser);
                    c = EQM_NEXT;
                    continue;
                } else if (c1 == '*') {
                    eqSkipBlockComment(parser);
                    c = EQM_NEXT;
                    continue;
                }
                EQM_PREV(c1);
            } else if (c == '.') {
                int c1 = EQM_NEXT;
                EQM_PREV(c1);
                if (isdigit(c1)) {
                    EQM_PREV(c);
                    eqGrabNumber(parser);
                    return;
                }
            } else if (c == '"') {
                eqGrabString(parser);
                return;
            }
            EQM_PREV(c);
            eqGrabSymbol(parser);
            return;
        }

        // If c isn't accounted for, skip it
        c = EQM_NEXT;
    }
}

bool eqNextToken(EqParser* parser) {
    eqGrabToken(parser);

    switch (parser->status) {
        case EQPS_OK:
            return true;
        case EQPS_NO_MEM:
            printf(EQ_ANSI_RED "Error: Ran out of available memory." EQ_ANSI_RESET "\n");
            return false;
        case EQPS_UNEXPECTED:
            printf(EQ_ANSI_RED "Error: Encountered unexpected '%ls'." EQ_ANSI_RESET "\n", parser->what->data);
            return false;
        case EQPS_EXPECTED:
            printf(EQ_ANSI_RED "Error: Expected '%ls'." EQ_ANSI_RESET "\n", parser->what->data);
            return false;
        case EQPS_UNCLOSED:
            printf(EQ_ANSI_RED "Error: Unable to find a matching '%ls'." EQ_ANSI_RESET "\n", parser->what->data);
            return false;
        case EQPS_NOT_ENOUGH_OPERANDS:
            printf(EQ_ANSI_RED "Error: Not enough operands." EQ_ANSI_RESET "\n");
            return false;
        case EQPS_EMPTY_PARENS:
            printf(EQ_ANSI_RED "Error: Empty parentheses." EQ_ANSI_RESET "\n");
            return false;
        default:
            return false;
    }
}