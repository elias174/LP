/* front.c - a lexical analyzer system for simple arithmetic expressions */
#include <stdio.h>
#include <ctype.h>

/* Global declarations */
/* Variables */
int charClass;
char lexeme [100];
char nextChar;
int lexLen;
int token;
int nextToken;
FILE *in_fp, *fopen();

/* Function declarations */
void addChar();
void getChar();
void getNonBlank();
int lex();

int prevToken = 0;

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token codes */
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define FOR_CODE 30
#define IF_CODE 31
#define ELSE_CODE 32
#define WHILE_CODE 33
#define DO_CODE 34
#define INT_CODE 35
#define FLOAT_CODE 36
#define SWITCH_CODE 37
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define COMMENT 66

/******************************************************/
/* main driver */
main() {
    /* Open the input data file and process its contents */
    if ((in_fp = fopen("front.in", "r")) == NULL)
        printf("ERROR - cannot open front.in \n");
    else {
        getChar();
        do {
            lex();
        } while (nextToken != EOF);
    }
}

int getComment() {

    getChar();
    if (charClass != MULT_OP)
        return DIV_OP;
    else {
        do {
            getChar();
            while (charClass != MULT_OP)
                getChar();
        } while (charClass != DIV_OP);
        return COMMENT;
    }
}

/*****************************************************/
/* lookup - a function to lookup operators and parentheses and return the token */
int lookup(char ch) {
switch (ch) {
    case '(':
        addChar();
        nextToken = LEFT_PAREN;
        break;

    case ')':
        addChar();
        nextToken = RIGHT_PAREN;
        break;

    case '+':
        addChar();
        nextToken = ADD_OP;
        break;

    case '-':
        addChar();
        nextToken = SUB_OP;
        break;

    case '*':
        prevToken = nextToken;
        addChar();
        nextToken = MULT_OP;
        if(prevToken == DIV_OP){
            printf("Begin Comment found\n");
            while (charClass == LETTER || charClass == DIGIT || charClass == UNKNOWN) {
                addChar();
                getChar();
                if(lexeme[lexLen-2] == '*' && lexeme[lexLen-1] == '/'){
                    break;
                }
                nextToken = COMMENT;
            }
            break;
        }
        break;

    case '/':
        addChar();
        nextToken = DIV_OP;
        break;

    default:
        addChar();
        nextToken = EOF;
        break;
}
return nextToken;
}

    /*****************************************************/
/* addChar - a function to add nextChar to lexeme */
void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    }
    else
        printf("Error - lexeme is too long \n");
}
/*****************************************************/
/* getChar - a function to get the next character of input and determine its character class */
void getChar() {
    if ((nextChar = getc(in_fp)) != EOF) {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else charClass = UNKNOWN;
    }
    else
        charClass = EOF;
}
/*****************************************************/
/* getNonBlank - a function to call getChar until it returns a non-whitespace character */
void getNonBlank() {
    while (isspace(nextChar))
    getChar();
}

/* lex - a simple lexical analyzer for arithmetic expressions */
int lex() {
    lexLen = 0;
    getNonBlank();
    switch (charClass) {

        /* Parse identifiers */
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }
            if( lexeme[lexLen-3] == 'f' && lexeme[lexLen-2] == 'o' && lexeme[lexLen-1] == 'r'){
                nextToken = FOR_CODE;
                break;
            }
            else if(lexeme[lexLen-2] == 'i' && lexeme[lexLen-1] == 'f'){
                nextToken = IF_CODE;
                break;
            }
            else if(lexeme[lexLen-4] == 'e' && lexeme[lexLen-3] == 'l' && lexeme[lexLen-2] == 's' && lexeme[lexLen-1] == 'e'){
                nextToken = ELSE_CODE;
                break;
            }
            else if(lexeme[lexLen-5] == 'w' && lexeme[lexLen-4] == 'h' && lexeme[lexLen-3] == 'i' && lexeme[lexLen-2] == 'l' && lexeme[lexLen-1] == 'e'){
                nextToken = WHILE_CODE;
                break;
            }
            else if(lexeme[lexLen-2] == 'd' && lexeme[lexLen-1] == 'o'){
                nextToken = DO_CODE;
                break;
            }
            else if( lexeme[lexLen-3] == 'i' && lexeme[lexLen-2] == 'n' && lexeme[lexLen-1] == 't'){
                nextToken = INT_CODE;
                break;
            }
            else if(lexeme[lexLen-5] == 'f' && lexeme[lexLen-4] == 'l' && lexeme[lexLen-3] == 'o' && lexeme[lexLen-2] == 'a' && lexeme[lexLen-1] == 't'){
                nextToken = FLOAT_CODE;
                break;
            }

            else if(lexeme[lexLen-6] == 's' && lexeme[lexLen-5] == 'w' && lexeme[lexLen-4] == 'i' && lexeme[lexLen-3] == 't' && lexeme[lexLen-2] == 'c' && lexeme[lexLen-1] == 'h'){
                nextToken = SWITCH_CODE;
                break;
            }

            nextToken = IDENT;
            break;

        /* Parse integer literals */
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;

        /* Parentheses and operators */
        case UNKNOWN:
            lookup(nextChar);
            getChar();
            break;

        /* EOF */
        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;
            break;

    } /* End of switch */

    printf("Next token is: %d, Next lexeme is %s\n",
    nextToken, lexeme);
    return nextToken;
} /* End of function lex */

