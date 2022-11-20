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
    int tokens[2000];
    int tokensIndex;
    // tokensIndex = 0;
    char nextChar;
    int lexLen;
    int token;
    int nextToken;
    int syntaxErrorCounter;
    bool started;
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
    //function to start off and control the syntax analyzer that is nested through calls to the stmt func
    //within the while loop that will terminate at EOF
    void syntaxAnalyzer(void)
    {
        tokensIndex = 0;
        syntaxErrorCounter = 0;
        if (tokens[0] == START_CODE)
        {
            tokensIndex++;
        }
        else
        {
            syntaxErrorCounter++;
            printf("SYNTAX ERROR: missing keyword to initialize program\n");
        }

        while (tokens[tokensIndex] != END_CODE && tokens[tokensIndex] != EOF)
        {
            stmt();
        }
        if (tokens[(tokensIndex)] != END_CODE && tokens[(tokensIndex - 1)] != END_CODE)
        {
            syntaxErrorCounter++;
            printf("SYNTAX ERROR: missing keyword to end program\n");
        }

        if (syntaxErrorCounter == 0)
        {
            printf("no syntax errors found! Well coded.\n");
        }
        else
        {
            printf("Syntax errors were found. You SUCK at coding.\n");
        }
    }

    void stmt()
    {

        if (tokens[tokensIndex] == IDENT) // {
        {
            tokensIndex++;
            if (tokens[tokensIndex] == ASSIGN_OP) // x
            {
                tokensIndex++;

                if (expr()) // ex: (y + 2) + 3 // y / 2 - (5 + 3) // (((y * x + 5)))
                {
                    tokensIndex++;
                    if (tokens[tokensIndex] == END_OF_LINE)
                    {

                        tokensIndex++;
                    }
                    else
                    {
                        //nextToken = 99;
                        printf("missing end of line semicolon on token %d\n", tokens[tokensIndex]);
                        syntaxErrorCounter++;
                        tokensIndex++;
                    }
                }
                else if (nextToken == RIGHT_PAREN) // (
                {
                    tokensIndex++;
                    if (expr())
                    {
                        tokensIndex++;
                        if (nextToken == LEFT_PAREN)
                        {
                            tokensIndex++;
                            if (nextToken == END_OF_LINE)
                            {
                                tokensIndex++;
                            }
                            else
                            {
                                printf("SYNTAX ERROR: missing end of line semicolon on token %d\n", tokens[tokensIndex]);
                                syntaxErrorCounter++;
                                tokensIndex++;
                            }
                        }
                        else
                        {
                            printf("SYNTAX ERROR: missing closing parenthesis on token %d\n", tokens[tokensIndex]);
                            syntaxErrorCounter++;
                            tokensIndex++;
                        }
                    }
                    else
                    {
                        printf("syntax error in arithemetic expression\n");
                        syntaxErrorCounter++;
                        tokensIndex++;
                    }
                }

                else
                {
                    // printf("SYNTAX ERROR: invalid start to arithemetic expression on token %d\n", tokens[tokensIndex]);
                    syntaxErrorCounter++;
                    // tokensIndex++;
                    stmt();
                }
            }
            else
            {
                printf("SYNTAX ERROR: assigment operator not found after identifier on token %d\n", tokens[tokensIndex]);
                syntaxErrorCounter++;
                tokensIndex++;
            }
        }
        else if (tokens[tokensIndex] == VARIABLE) // --> V <-- of "V this_is_var;""
        {
            tokensIndex++;
            if (tokens[tokensIndex] == IDENT) //this_is_var
            {
                tokensIndex++;
                if (tokens[tokensIndex] == END_OF_LINE) // ;
                {
                    tokensIndex++;
                }
                else
                {
                    printf("SYNTAX ERROR: missing end of line semicolon on token %d\n", tokens[tokensIndex]);
                    syntaxErrorCounter++;
                    tokensIndex++;
                }
            }
            else
            {
                printf("SYNTAX ERROR: missing identifier after variable declaration on token %d\n", tokens[tokensIndex]);
                syntaxErrorCounter++;
                tokensIndex++;
            }
        }

        else if (tokens[tokensIndex] == WHILE) // W
        {
            tokensIndex++;
            if (tokens[tokensIndex] == LEFT_PAREN) // (
            {
                tokensIndex++;
                if (tokens[tokensIndex] == IDENT) // variable name
                {
                    tokensIndex++;
                    if (tokens[tokensIndex] == LESS_THAN ||
                        tokens[tokensIndex] == EQUAL_TO ||
                        tokens[tokensIndex] == NOT_EQUAL_TO ||
                        tokens[tokensIndex] == GREATER_THAN ||
                        tokens[tokensIndex] == GREATER_THAN_EQUAL ||
                        tokens[tokensIndex] == LESS_THAN_EQUAL) // any bool operator
                    {
                        tokensIndex++;
                        if (expr()) //checks to see if the right side of operator is x > 7 or x > (9+6) or x > ((y+ 2) / 3)

                        { // needs to cover ( in bool statement for example while(x > (6 + 5)){}
                            tokensIndex++;
                            if (tokens[tokensIndex] == RIGHT_PAREN)
                            {
                                tokensIndex++;
                                if (tokens[tokensIndex] == CODE_BLOCK_START)
                                {
                                    tokensIndex++;
                                    stmt();
                                    if (tokens[tokensIndex] == CODE_BLOCK_END)
                                    {
                                        tokensIndex++;
                                    }
                                    else if (tokens[tokensIndex] == IDENT ||
                                             tokens[tokensIndex] == VARIABLE ||
                                             tokens[tokensIndex] == WHILE ||
                                             tokens[tokensIndex] == IF)
                                    {
                                        stmt();
                                        while (tokens[tokensIndex] == IDENT ||
                                               tokens[tokensIndex] == VARIABLE ||
                                               tokens[tokensIndex] == WHILE ||
                                               tokens[tokensIndex] == IF)
                                        {
                                            stmt();
                                        }
                                        if (tokens[tokensIndex] == CODE_BLOCK_END)
                                        {
                                            tokensIndex++;
                                        }
                                        else
                                        {
                                            printf("SYNTAX ERROR: missing closing bracket for else statement on token %d\n", tokens[tokensIndex]);
                                            syntaxErrorCounter++;
                                            tokensIndex++;
                                        }
                                    }
                                    else
                                    {
                                        printf("SYNTAX ERROR: missing closing bracket for while statement on token %d\n", tokens[tokensIndex]);
                                        syntaxErrorCounter++;
                                        tokensIndex++;
                                    }
                                }
                                else
                                {
                                    printf("SYNTAX ERROR: missing opening bracket for while statement on token %d\n", tokens[tokensIndex]);
                                    syntaxErrorCounter++;
                                    tokensIndex++;
                                }
                            }
                            else
                            {
                                printf("SYNTAX ERROR: missing closing parenthesis for while statement bool expression on token %d\n", tokens[tokensIndex]);
                                syntaxErrorCounter++;
                                tokensIndex++;
                            }
                        }
                        else
                        {
                            printf("SYNTAX ERROR: incorrect arithmetic expression for while statement bool expression on token %d\n", tokens[tokensIndex]);
                            syntaxErrorCounter++;
                            tokensIndex++;
                        }
                    }
                    else
                    {
                        printf("SYNTAX ERROR: missing boolean operator for while statement bool expression on token %d\n", tokens[tokensIndex]);
                        syntaxErrorCounter++;
                        tokensIndex++;
                    }
                }
                else
                {
                    printf("SYNTAX ERROR: missing identifier to start while statement bool expression on token %d\n", tokens[tokensIndex]);
                    syntaxErrorCounter++;
                    tokensIndex++;
                }
            }
            else
            {
                printf("SYNTAX ERROR: missing opening parenethesis for while statement bool expression on token %d\n", tokens[tokensIndex]);
                syntaxErrorCounter++;
                tokensIndex++;
            }
        }
        else if (tokens[tokensIndex] == VARIABLE)
        {
            tokensIndex++;
            if (tokens[tokensIndex] == IDENT)
            {
                tokensIndex++;
                if (tokens[tokensIndex] == END_OF_LINE)
                {

                    tokensIndex++;
                }
                else
                {
                    printf("SYNTAX ERROR: missing end of line semicolon on token %d\n", tokens[tokensIndex]);
                    syntaxErrorCounter++;
                    tokensIndex++;
                }
            }
            else
            {
                printf("SYNTAX ERROR: missing variable identifier on token %d\n", tokens[tokensIndex]);
                syntaxErrorCounter++;
                tokensIndex++;
            }
        }
        else if (tokens[tokensIndex] == IF)
        {
            tokensIndex++;
            if (tokens[tokensIndex] == LEFT_PAREN)
            {
                tokensIndex++;
                if (tokens[tokensIndex] == IDENT)
                {
                    tokensIndex++;
                    if (tokens[tokensIndex] == LESS_THAN ||
                        tokens[tokensIndex] == EQUAL_TO ||
                        tokens[tokensIndex] == NOT_EQUAL_TO ||
                        tokens[tokensIndex] == GREATER_THAN ||
                        tokens[tokensIndex] == GREATER_THAN_EQUAL ||
                        tokens[tokensIndex] == LESS_THAN_EQUAL)
                    {
                        tokensIndex++;
                        if (expr()) //needs to check if right side of operator can be a math expression
                        {
                            tokensIndex++;
                            if (tokens[tokensIndex] == RIGHT_PAREN)
                            {
                                tokensIndex++;
                                if (tokens[tokensIndex] == CODE_BLOCK_START)
                                {
                                    tokensIndex++;
                                    stmt();
                                    if (tokens[tokensIndex] == CODE_BLOCK_END)
                                    {
                                        tokensIndex++;
                                        if (tokens[tokensIndex] == ELSE)
                                        {
                                            tokensIndex++;
                                            if (tokens[tokensIndex] == CODE_BLOCK_START)
                                            {
                                                tokensIndex++;
                                                stmt();
                                                if (tokens[tokensIndex] == CODE_BLOCK_END)
                                                {
                                                    tokensIndex++;
                                                }
                                                else if (tokens[tokensIndex] == IDENT ||
                                                         tokens[tokensIndex] == VARIABLE ||
                                                         tokens[tokensIndex] == WHILE ||
                                                         tokens[tokensIndex] == IF)
                                                {

                                                    stmt();
                                                    while (tokens[tokensIndex] == IDENT ||
                                                           tokens[tokensIndex] == VARIABLE ||
                                                           tokens[tokensIndex] == WHILE ||
                                                           tokens[tokensIndex] == IF)
                                                    {
                                                        stmt();
                                                    }
                                                    if (tokens[tokensIndex] == CODE_BLOCK_END)
                                                    {
                                                        tokensIndex++;
                                                    }
                                                    else
                                                    {
                                                        printf("SYNTAX ERROR: missing closing bracket for else statement on token %d\n", tokens[tokensIndex]);
                                                        syntaxErrorCounter++;
                                                        tokensIndex++;
                                                    }
                                                }
                                                else
                                                {
                                                    printf("SYNTAX ERROR: missing closing bracket for else statement on token %d\n", tokens[tokensIndex]);
                                                    syntaxErrorCounter++;
                                                    tokensIndex++;
                                                }
                                            }
                                            else
                                            {
                                                printf("SYNTAX ERROR: missing opening bracket for else statement on token %d\n", tokens[tokensIndex]);
                                                syntaxErrorCounter++;
                                                tokensIndex++;
                                            }
                                        }
                                    }
                                    else if (tokens[tokensIndex] == IDENT ||
                                             tokens[tokensIndex] == VARIABLE ||
                                             tokens[tokensIndex] == WHILE ||
                                             tokens[tokensIndex] == IF)
                                    {

                                        stmt();
                                        while (tokens[tokensIndex] == IDENT ||
                                               tokens[tokensIndex] == VARIABLE ||
                                               tokens[tokensIndex] == WHILE ||
                                               tokens[tokensIndex] == IF)
                                        {
                                            stmt();
                                        }
                                        if (tokens[tokensIndex] == CODE_BLOCK_END)
                                        {
                                            tokensIndex++;
                                            if (tokens[tokensIndex] == ELSE)
                                            {
                                                tokensIndex++;
                                                if (tokens[tokensIndex] == CODE_BLOCK_START)
                                                {
                                                    tokensIndex++;
                                                    stmt();
                                                    if (tokens[tokensIndex] == CODE_BLOCK_END)
                                                    {
                                                        tokensIndex++;
                                                    }
                                                    else if (tokens[tokensIndex] == IDENT ||
                                                             tokens[tokensIndex] == VARIABLE ||
                                                             tokens[tokensIndex] == WHILE ||
                                                             tokens[tokensIndex] == IF)
                                                    {
                                                        stmt();
                                                        while (tokens[tokensIndex] == IDENT ||
                                                               tokens[tokensIndex] == VARIABLE ||
                                                               tokens[tokensIndex] == WHILE ||
                                                               tokens[tokensIndex] == IF)
                                                        {
                                                            stmt();
                                                        }
                                                        if (tokens[tokensIndex] == CODE_BLOCK_END)
                                                        {
                                                            tokensIndex++;
                                                        }
                                                        else
                                                        {
                                                            printf("SYNTAX ERROR: missing closing bracket for else statement on token %d\n", tokens[tokensIndex]);
                                                            syntaxErrorCounter++;
                                                            tokensIndex++;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        printf("SYNTAX ERROR: missing closing bracket for else statement on token %d\n", tokens[tokensIndex]);
                                                        syntaxErrorCounter++;
                                                        tokensIndex++;
                                                    }
                                                }
                                                else
                                                {
                                                    printf("SYNTAX ERROR: missing opening bracket for else statement on token %d\n", tokens[tokensIndex]);
                                                    syntaxErrorCounter++;
                                                    tokensIndex++;
                                                }
                                            }
                                        }
                                        else
                                        {
                                            printf("SYNTAX ERROR: missing closing bracket for else statement on token %d\n", tokens[tokensIndex]);
                                            syntaxErrorCounter++;
                                            tokensIndex++;
                                        }
                                    }
                                    else if (tokens[tokensIndex] == ELSE)
                                    {
                                        tokensIndex++;
                                        if (tokens[tokensIndex] == CODE_BLOCK_START)
                                        {
                                            tokensIndex++;
                                            stmt();
                                            if (tokens[tokensIndex] == CODE_BLOCK_END)
                                            {
                                                tokensIndex++;
                                            }
                                            else if (tokens[tokensIndex] == IDENT ||
                                                     tokens[tokensIndex] == VARIABLE ||
                                                     tokens[tokensIndex] == WHILE ||
                                                     tokens[tokensIndex] == IF)
                                            {
                                                stmt();
                                                while (tokens[tokensIndex] == IDENT ||
                                                       tokens[tokensIndex] == VARIABLE ||
                                                       tokens[tokensIndex] == WHILE ||
                                                       tokens[tokensIndex] == IF)
                                                {
                                                    stmt();
                                                }
                                                if (tokens[tokensIndex] == CODE_BLOCK_END)
                                                {
                                                    tokensIndex++;
                                                }
                                                else
                                                {
                                                    printf("SYNTAX ERROR: missing closing bracket for else statement on token %d\n", tokens[tokensIndex]);
                                                    syntaxErrorCounter++;
                                                    tokensIndex++;
                                                }
                                            }
                                            else
                                            {
                                                printf("SYNTAX ERROR: missing closing bracket for else statement on token %d\n", tokens[tokensIndex]);
                                                syntaxErrorCounter++;
                                                tokensIndex++;
                                            }
                                        }
                                        else
                                        {
                                            printf("SYNTAX ERROR: missing opening bracket for else statement on token %d\n", tokens[tokensIndex]);
                                            syntaxErrorCounter++;
                                            tokensIndex++;
                                        }
                                    }
                                    else
                                    {
                                        printf("SYNTAX ERROR: missing closing bracket for if statement on token %d\n", tokens[tokensIndex]);
                                        syntaxErrorCounter++;
                                        tokensIndex++;
                                    }
                                }
                                else
                                {
                                    printf("SYNTAX ERROR: missing opening bracket for if statement on token %d\n", tokens[tokensIndex]);
                                    syntaxErrorCounter++;
                                    tokensIndex++;
                                }
                            }
                            else
                            {
                                printf("SYNTAX ERROR: missing closing parenthesis for if statement bool expression on token %d\n", tokens[tokensIndex]);
                                syntaxErrorCounter++;
                                tokensIndex++;
                            }
                        }
                        else
                        {
                            printf("SYNTAX ERROR: incorrect arithmetic expression for if statement bool expression on token %d\n", tokens[tokensIndex]);
                            syntaxErrorCounter++;
                            tokensIndex++;
                        }
                    }
                    else
                    {
                        printf("SYNTAX ERROR: missing boolean operator for if statement bool expression on token %d\n", tokens[tokensIndex]);
                        syntaxErrorCounter++;
                        tokensIndex++;
                    }
                }
                else
                {
                    printf("SYNTAX ERROR: missing identifier to start if statement bool expression on token %d\n", tokens[tokensIndex]);
                    syntaxErrorCounter++;
                    tokensIndex++;
                }
            }
            else
            {
                printf("SYNTAX ERROR: missing opening parenethesis for if statement bool expression on token %d\n", tokens[tokensIndex]);
                syntaxErrorCounter++;
                tokensIndex++;
            }
        }
        else
        {
            printf("SYNTAX ERROR: invalid start to statement on token %d\n", tokens[tokensIndex]);
            syntaxErrorCounter++;
            tokensIndex++;
        }
    }
    bool expr()
    {

        /*
        either next token is a var or number ex: just had =, so = 12 + (1 + 2); or = x + z + y; or = x / (y + 2);
        */

        if (tokens[tokensIndex] == IDENT)
        {
            if (tokens[(tokensIndex + 1)] == RIGHT_PAREN && tokens[tokensIndex + 2] == CODE_BLOCK_START)
            {
                return true;
            }
            else if (tokens[(tokensIndex + 1)] == END_OF_LINE)
            {
                return true;
            }
            else if (tokens[tokensIndex + 1] == RIGHT_PAREN)
            {
                return true;
            }
            else if (tokens[tokensIndex + 1] == RIGHT_PAREN && tokens[tokensIndex + 2] == END_OF_LINE)
            {
                return true;
            }
            tokensIndex++;
            if (tokens[tokensIndex] == ADD_OP ||
                tokens[tokensIndex] == SUB_OP ||
                tokens[tokensIndex] == DIV_OP ||
                tokens[tokensIndex] == MULT_OP)
            {
                tokensIndex++;

                if (expr())
                {
                    return true;
                }
                else
                {
                    printf("SYNTAX ERROR: missing identifier or number in arithmetic expression on token %d\n", tokens[tokensIndex]);
                    syntaxErrorCounter++;
                    return false;
                }
            }
            else
            {
                printf("SYNTAX ERROR: missing arithmetic operator in arithmetic expression on token %d\n", tokens[tokensIndex]);
                syntaxErrorCounter++;
                return false;
            }
        }
        else if (tokens[tokensIndex] == BYTE ||
                 tokens[tokensIndex] == SHORT ||
                 tokens[tokensIndex] == INT ||
                 tokens[tokensIndex] == LONG)
        {
            if (tokens[(tokensIndex + 1)] == RIGHT_PAREN && tokens[tokensIndex + 2] == CODE_BLOCK_START)
            {
                return true;
            }
            else if (tokens[(tokensIndex + 1)] == END_OF_LINE)
            {
                return true;
            }
            else if (tokens[tokensIndex + 1] == RIGHT_PAREN)
            {
                return true;
            }
            else if (tokens[tokensIndex + 1] == RIGHT_PAREN && tokens[tokensIndex + 2] == END_OF_LINE)
            {
                return true;
            }
            tokensIndex++;
            if (tokens[tokensIndex] == ADD_OP ||
                tokens[tokensIndex] == SUB_OP ||
                tokens[tokensIndex] == DIV_OP ||
                tokens[tokensIndex] == MULT_OP)
            {
                tokensIndex++;

                if (expr())
                {
                    return true;
                }
                else
                {
                    printf("SYNTAX ERROR: missing identifier or number in arithmetic expression on token %d\n", tokens[tokensIndex]);
                    syntaxErrorCounter++;
                    return false;
                }
            }
            else if (tokens[tokensIndex] != END_OF_LINE)
            {
                printf("SYNTAX ERROR: missing end of line symbol on token %d\n", tokens[tokensIndex]);
                syntaxErrorCounter++;
                //tokensIndex++;
                return false;
            }
            else
            {
                printf("SYNTAX ERROR: missing arithmetic operator in arithmetic expression on token %d\n", tokens[tokensIndex]);
                syntaxErrorCounter++;
                return false;
            }
        }
        else if (tokens[tokensIndex] == LEFT_PAREN)
        {
            tokensIndex++;
            if (expr())
            {
                tokensIndex++;
                if (tokens[tokensIndex] == RIGHT_PAREN && (tokens[tokensIndex + 1] == END_OF_LINE))
                {

                    return true;
                }
                else if (tokens[tokensIndex + 1] == ADD_OP ||
                         tokens[tokensIndex + 1] == SUB_OP ||
                         tokens[tokensIndex + 1] == DIV_OP ||
                         tokens[tokensIndex + 1] == MULT_OP)
                {
                    tokensIndex += 2;
                    if (expr())
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    return false;
                }
            }
            else
            {
                printf("SYNTAX ERROR: missing identifier or number in arithmetic expression on token %d\n", tokens[tokensIndex]);
                syntaxErrorCounter++;
                return false;
            }
        }
        else
        {
            // printf("SYNTAX ERROR: missing identifier or number in arithmetic expression on token %d\n", tokens[tokensIndex]);
            syntaxErrorCounter++;
            return false;
        }
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
                   charClass == UNDERSCORE)
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
            addChar();
            getChar();

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

        printf("Next token is: %d, Next lexeme is %s\n",
               nextToken, lexeme);
        return nextToken;
    }
};

/******************************************************/
/* main driver */
int main(void)
{
    Lexical myLex;
    int lexCounter = 0;

    /* Open the input data file and process its contents */
    if ((myLex.in_fp = fopen("no_errors2.in", "r")) == NULL)
        printf("ERROR - cannot open front2.in \n");
    else
    {
        myLex.getChar();
        myLex.lex();
        myLex.tokens[lexCounter] = myLex.nextToken;
        lexCounter++;
        do
        {
            myLex.lex();

            //create an int array of all tokens
            myLex.tokens[lexCounter] = myLex.nextToken;
            lexCounter++;

        } while (myLex.nextToken != EOF);
        //call syntax analyzer function to check if array of tokens contains syntax errors or not
        printf("Syntax Analysis:\n");
        myLex.syntaxAnalyzer();
    }
    return 0;
}
