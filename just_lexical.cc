#include <stdio.h>
#include <ctype.h>
#include <string.h>

class Lexical
{
public:
/* Character classes */
#define LETTER 0
#define UNDERSCORE 16
#define DIGIT 1
#define UNKNOWN 99

/* Token codes */
#define INT_LIT 10
#define IDENT 11
#define WHILE 12
#define IF 13
#define ELSE 14
#define VARIABLE 15
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define GREATER_THAN 27
#define LESS_THAN 28
#define GREATER_THAN_EQUAL 29
#define LESS_THAN_EQUAL 30
#define NOT_EQUAL_TO 31
#define EQUAL_TO 32
#define END_OF_LINE 33
#define CODE_BLOCK_START 34
#define CODE_BLOCK_END 35
#define MODULUS 36
#define SPACE 37
#define START_CODE 17 //start
#define END_CODE 18   //end

#define BYTE 38  //_X
#define SHORT 39 //_S
#define INT 40   //_M
#define LONG 41  // _L
    /* Variables */
    int charClass;
    char lexeme[100];
    char nextChar;
    int lexLen;
    int token;
    int nextToken;
    FILE *in_fp;
    /* lookup - a function to look up operators and
    parentheses and return the token */
    int lookup(char ch)
    {
        if (isupper(ch))
        {
            nextToken = UNKNOWN;
        }
        switch (ch)
        {
        case '=':
            addChar();
            nextToken = ASSIGN_OP; // =
            break;
        case '(':
            addChar();
            nextToken = LEFT_PAREN; // (
            break;
        case ')':
            addChar();
            nextToken = RIGHT_PAREN; // )
            break;
        case '+':
            addChar();
            nextToken = ADD_OP; // +
            break;
        case '-':
            addChar();
            nextToken = SUB_OP; // -
            break;
        case '*':
            addChar();
            nextToken = MULT_OP; // *
            break;
        case '/':
            addChar();
            nextToken = DIV_OP; // /
            break;
        case '<':
            addChar();
            nextToken = LESS_THAN; // <
            break;
        case '>':
            addChar();
            nextToken = GREATER_THAN; // >
            break;
        case '!':
            addChar();
            nextToken = NOT_EQUAL_TO; // !
            break;
        case '@': //less than equal to
            addChar();
            nextToken = LESS_THAN_EQUAL; // @
            break;
        case '#': //greater than equal to
            addChar();
            nextToken = GREATER_THAN_EQUAL; // #
            break;
        case '$':
            addChar();
            nextToken = EQUAL_TO; // $
            break;
        case ';':
            addChar();
            nextToken = END_OF_LINE; // ;
            break;
        case '{':
            addChar();
            nextToken = CODE_BLOCK_START; // {
            break;
        case '}':
            addChar();
            nextToken = CODE_BLOCK_END; // }
            break;
        case '%':
            addChar();
            nextToken = MODULUS; // %
            break;
        case 'W':
            addChar();
            nextToken = WHILE; // W
            break;
        case 'I':
            addChar();
            nextToken = IF; // I
            break;
        case 'E':
            addChar();
            nextToken = ELSE; // E
            break;
        case ' ':
            addChar();
            nextToken = SPACE; // ' '
            break;
        case 'V':
            addChar();
            nextToken = VARIABLE; // V
            break;
        case 'X':
            //BYTE INT
            addChar();
            nextToken = BYTE; // X
            break;
        case 'S':
            addChar();
            nextToken = SHORT; // S
            break;
        case 'M':
            addChar();
            nextToken = INT; // M
            break;
        case 'L':
            addChar();
            nextToken = LONG; // L
            break;
        default:
            addChar();
            nextToken = EOF;
            break;
        }
        return nextToken;
    }
    /* addChar - a function to add nextChar to lexeme */
    void addChar(void)
    {
        if (lexLen <= 98)
        {
            lexeme[lexLen++] = nextChar;
            lexeme[lexLen] = '\0';
        }
        else
            printf("Error - lexeme is too long \n");
    }
    /* getChar - a function to get the next character of
    input and determine its character class */
    void getChar(void)
    {
        if ((nextChar = fgetc(in_fp)) != EOF)
        {
            if (islower(nextChar) &&
                isalpha(nextChar) &&
                nextChar != 'W' &&
                nextChar != 'I' &&
                nextChar != 'E' &&
                nextChar != 'V' &&
                nextChar != 'X' &&
                nextChar != 'S' &&
                nextChar != 'M' &&
                nextChar != 'L')
                charClass = LETTER;
            else if (nextChar == '_')
                charClass = UNDERSCORE;
            else if (isdigit(nextChar))
                charClass = DIGIT;
            else if (nextChar == 'W')
            {
                charClass = WHILE;
            }
            else if (nextChar == 'I')
            {
                charClass = IF;
            }
            else if (nextChar == 'E')
            {
                charClass = ELSE;
            }
            else if (nextChar == 'V')
            {
                charClass = VARIABLE;
            }
            else if (nextChar == 'X')
            {
                charClass = BYTE;
            }
            else if (nextChar == 'S')
            {
                charClass = SHORT;
            }
            else if (nextChar == 'M')
            {
                charClass = INT;
            }
            else if (nextChar == 'L')
            {
                charClass = LONG;
            }
            else if (isupper(nextChar))
            {
                charClass = UNKNOWN;
            }
            else
                charClass = UNKNOWN;
        }
        else
            charClass = EOF;
    }
    /* getNonBlank - a function to call getChar until it
    returns a non-whitespace character */
    void getNonBlank(void)
    {
        while (isspace(nextChar))
            getChar();
    }
    /* lex - a simple lexical analyzer for arithmetic
    expressions */
    int lex(void)
    {
        lexLen = 0;
        getNonBlank();
        switch (charClass)
        {
            /* Identifiers */
        case UNDERSCORE:
            addChar();
            getChar();
            break;

        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER ||
                   charClass == UNDERSCORE && (lexLen >= 6 || lexLen <= 8))
            {
                addChar();
                getChar();
            }
            if (!strcmp(lexeme, "start"))
            {
                nextToken = START_CODE;
            }
            else if (!strcmp(lexeme, "end"))
            {
                nextToken = END_CODE;
            }
            else if (lexLen >= 6 && lexLen <= 8)
                nextToken = IDENT;
            else
            {
                while (charClass == UNKNOWN)
                {
                    addChar();

                    getChar();
                }
                nextToken = UNKNOWN;
            }
            break;
            /* Integer literals */
        case DIGIT: //1_S
            addChar();
            getChar();
            while (charClass == DIGIT ||
                   charClass == UNDERSCORE ||
                   charClass == BYTE ||
                   charClass == SHORT ||
                   charClass == INT ||
                   charClass == LONG)
            {
                addChar();
                getChar();
            }
            if (charClass == BYTE)
            {
                nextToken = BYTE;
            }
            else if (charClass == SHORT)
            {
                nextToken = SHORT;
            }
            else if (charClass == INT)
            {
                nextToken = INT;
            }
            else if (charClass == LONG)
            {
                nextToken = LONG;
            }
            else if (charClass == UNKNOWN)
            {
                nextToken = UNKNOWN;
                charClass = UNKNOWN;
            }

            break;
            /* Parentheses and operators */
        case UNKNOWN:
            lookup(nextChar);
            getChar();
            break;
        case WHILE:
            nextToken = WHILE;
            addChar();
            getChar();
            break;
        case IF:
            addChar();
            getChar();
            nextToken = IF;
            break;
        case ELSE:
            addChar();
            getChar();
            nextToken = ELSE;
            break;
        case VARIABLE:
            addChar();
            getChar();
            nextToken = VARIABLE;
            break;
            /* EOF */
        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = '\0';
            break;
        } /* End of switch */
        if (nextToken == 99 || (nextToken == -1 && charClass != EOF))
        {
            printf("lexeme is an error and not in the language. Token is UNKNOWN: 99. Unknown lexeme is: %s\n", lexeme);
        }
        else
        {
            printf("Next token is: %d, Next lexeme is %s\n",
                   nextToken, lexeme);
        }
        return nextToken;
    }
};

/******************************************************/
/* main driver */
int main(void)
{
    Lexical myLex;
    /* Open the input data file and process its contents */
    if ((myLex.in_fp = fopen("lexical_error2.in", "r")) == NULL)
        printf("ERROR - cannot open front4.in \n");
    else
    {
        myLex.getChar();
        do
        {
            myLex.lex();
        } while (myLex.nextToken != EOF && myLex.nextToken != UNKNOWN);
    }
    return 0;
}
