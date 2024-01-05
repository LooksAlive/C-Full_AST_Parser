#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>



#include "lexer.h"


// Function to read the content of a file
char* readFile(const char* filepath) {
    // Open the file
    FILE* file = fopen(filepath, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Determine the size of the file
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    // Allocate memory for the content
    char* content = (char*)malloc(file_size + 1);
    if (content == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    // Read the content of the file into the buffer
    size_t bytesRead = fread(content, 1, file_size, file);
    if (bytesRead != (size_t)file_size) {
        perror("Error reading file");
        exit(EXIT_FAILURE);
    }

    // Null-terminate the content
    content[file_size] = '\0';

    // Close the file
    fclose(file);

    return content;
}

/*
int compare_ongoing_stream(const char* content, const int* pos, const char* word) {
    int8_t eq = 1;
    int p = 1;
    for(int i=*pos; content[i] != '\0'; i++) {
        if(content[i] == word[p] && word[p] != '\0') {
            eq = 1;
        }
        else {
            eq = 0;
        }
    }
    return eq;
}
*/
// Dummy compare_ongoing_stream function (replace with actual implementation)
int compare_ongoing_stream(const char* content, int* pos, const char* word) {
    // Compare logic goes here
    if(strncmp(content + *pos, word, strlen(word)) == 0) {
        *pos += strlen(word);
        return 1;
    }
    else {
        return 0;
    }
}

// Function to check if a string is a keyword
void isKeyword(const char* content, int* pos, struct Token** tok) {
    // Dummy implementation, replace with actual keyword checking
    if (compare_ongoing_stream(content, pos, KW_IF)) {
        /*
        if(content[(*pos)] != '(' && content[(*pos)] != ' ' && content[(*pos)] != '\t' && content[(*pos)] != '{') {
            (*pos) -= strlen(KW_IF);
            return;
        }
        */
        if(content[(*pos)] == '(' || content[(*pos)] == ' ' || content[(*pos)] == '\t' || content[(*pos)] == '{') {
            (*pos) -= 1;
        }
        (*tok)->type = TOK_KW_IF;
        (*tok)->lexeme = malloc(sizeof(KW_IF));
        strcpy((*tok)->lexeme, KW_IF);
    } else if (compare_ongoing_stream(content, pos, KW_ELSE)) {
        if(content[(*pos)] == '(' || content[(*pos)] == ' ' || content[(*pos)] == '\t' || content[(*pos)] == '{') {
            (*pos) -= 1;
        }
        (*tok)->type = TOK_KW_ELSE;
        (*tok)->lexeme = malloc(sizeof(KW_ELSE));
        strcpy((*tok)->lexeme, KW_ELSE);
    }
    else if (compare_ongoing_stream(content, pos, KW_SWITCH)) {
        if(content[(*pos)] == '(' || content[(*pos)] == ' ' || content[(*pos)] == '\t' || content[(*pos)] == '{') {
            (*pos) -= 1;
        }
        (*tok)->type = TOK_KW_SWITCH;
        (*tok)->lexeme = malloc(sizeof(KW_SWITCH));
        strcpy((*tok)->lexeme, KW_SWITCH);
    }
    else if (compare_ongoing_stream(content, pos, KW_CASE)) {
        if(content[(*pos)] == '(' || content[(*pos)] == ' ' || content[(*pos)] == '\t' || content[(*pos)] == '{') {
            (*pos) -= 1;
        }
        (*tok)->type = TOK_KW_CASE;
        (*tok)->lexeme = malloc(sizeof(KW_CASE));
        strcpy((*tok)->lexeme, KW_CASE);
    }
    else if (compare_ongoing_stream(content, pos, KW_DEFAULT)) {
        if(content[(*pos)] == '(' || content[(*pos)] == ' ' || content[(*pos)] == '\t' || content[(*pos)] == '{' || content[(*pos)] == ':') {
            (*pos) -= 1;
        }
        (*tok)->type = TOK_KW_DEFAULT;
        (*tok)->lexeme = malloc(sizeof(KW_DEFAULT));
        strcpy((*tok)->lexeme, KW_DEFAULT);
    }
    else if (compare_ongoing_stream(content, pos, KW_WHILE)) {
        if(content[(*pos)] == '(' || content[(*pos)] == ' ' || content[(*pos)] == '\t' || content[(*pos)] == '{') {
            (*pos) -= 1;
        }
        (*tok)->type = TOK_KW_WHILE;
        (*tok)->lexeme = malloc(sizeof(KW_WHILE));
        strcpy((*tok)->lexeme, KW_WHILE);
    }
    else if (compare_ongoing_stream(content, pos, KW_BREAK)) {
        if(content[(*pos)] == '(' || content[(*pos)] == ' ' || content[(*pos)] == '\t' || content[(*pos)] == '{') {
            (*pos) -= 1;
        }
        (*tok)->type = TOK_KW_BREAK;
        (*tok)->lexeme = malloc(sizeof(KW_BREAK));
        strcpy((*tok)->lexeme, KW_BREAK);
    }
    else if (compare_ongoing_stream(content, pos, KW_CONTINUE)) {
        if(content[(*pos)] == '(' || content[(*pos)] == ' ' || content[(*pos)] == '\t' || content[(*pos)] == '{') {
            (*pos) -= 1;
        }
        (*tok)->type = TOK_KW_CONTINUE;
        (*tok)->lexeme = malloc(sizeof(KW_CONTINUE));
        strcpy((*tok)->lexeme, KW_CONTINUE);
    }
    else if (compare_ongoing_stream(content, pos, KW_FOR)) {
        if(content[(*pos)] == '(' || content[(*pos)] == ' ' || content[(*pos)] == '\t' || content[(*pos)] == '{') {
            (*pos) -= 1;
        }
        char c = content[(*pos)];
        (*tok)->type = TOK_KW_FOR;
        (*tok)->lexeme = malloc(sizeof(KW_FOR));
        strcpy((*tok)->lexeme, KW_FOR);
    } else if (compare_ongoing_stream(content, pos, KW_RETURN)) {
        if(content[(*pos)] == '(' || content[(*pos)] == ' ' || content[(*pos)] == '\t' || content[(*pos)] == '{') {
            (*pos) -= 1;
        }
        (*tok)->type = TOK_KW_RETURN;
        (*tok)->lexeme = malloc(sizeof(KW_RETURN));
        strcpy((*tok)->lexeme, KW_RETURN);
    } else if (compare_ongoing_stream(content, pos, KW_INCLUDE)) {
        if(content[(*pos)] == '(' || content[(*pos)] == ' ' || content[(*pos)] == '\t' || content[(*pos)] == '{') {
            (*pos) -= 1;
        }
        (*tok)->type = TOK_KW_INCLUDE;
        (*tok)->lexeme = malloc(sizeof(KW_INCLUDE));
        strcpy((*tok)->lexeme, KW_INCLUDE);
    } else if (compare_ongoing_stream(content, pos, KW_REGISTER)) {
        if(content[(*pos)] == '(' || content[(*pos)] == ' ' || content[(*pos)] == '\t' || content[(*pos)] == '{') {
            (*pos) -= 1;
        }
        (*tok)->type = TOK_KW_REGISTER;
        (*tok)->lexeme = malloc(sizeof(KW_REGISTER));
        strcpy((*tok)->lexeme, KW_REGISTER);
    } else if (compare_ongoing_stream(content, pos, KW_DEFINE)) {
        if(content[(*pos)] == '(' || content[(*pos)] == ' ' || content[(*pos)] == '\t' || content[(*pos)] == '{') {
            (*pos) -= 1;
        }
        (*tok)->type = TOK_KW_DEFINE;
        (*tok)->lexeme = malloc(sizeof(KW_DEFINE));
        strcpy((*tok)->lexeme, KW_DEFINE);
    } else if (compare_ongoing_stream(content, pos, KW_PRAGMA)) {
        if(content[(*pos)] == '(' || content[(*pos)] == ' ' || content[(*pos)] == '\t' || content[(*pos)] == '{') {
            (*pos) -= 1;
        }
        (*tok)->type = TOK_KW_PRAGMA;
        (*tok)->lexeme = malloc(sizeof(KW_PRAGMA));
        strcpy((*tok)->lexeme, KW_PRAGMA);
    } else if (compare_ongoing_stream(content, pos, KW_UNDEF)) {
        if(content[(*pos)] == '(' || content[(*pos)] == ' ' || content[(*pos)] == '\t' || content[(*pos)] == '{') {
            (*pos) -= 1;
        }
        (*tok)->type = TOK_KW_UNDEF;
        (*tok)->lexeme = malloc(sizeof(KW_UNDEF));
        strcpy((*tok)->lexeme, KW_UNDEF);
    } else if (compare_ongoing_stream(content, pos, KW_ENDIF)) {
        if(content[(*pos)] == '(' || content[(*pos)] == ' ' || content[(*pos)] == '\t' || content[(*pos)] == '{') {
            (*pos) -= 1;
        }
        (*tok)->type = TOK_KW_ENDIF;
        (*tok)->lexeme = malloc(sizeof(KW_ENDIF));
        strcpy((*tok)->lexeme, KW_ENDIF);
    } else if (compare_ongoing_stream(content, pos, KW_IFDEF)) {
        if(content[(*pos)] == '(' || content[(*pos)] == ' ' || content[(*pos)] == '\t' || content[(*pos)] == '{') {
            (*pos) -= 1;
        }
        (*tok)->type = TOK_KW_IFDEF;
        (*tok)->lexeme = malloc(sizeof(KW_IFDEF));
        strcpy((*tok)->lexeme, KW_IFDEF);
    }
    else if (compare_ongoing_stream(content, pos, KW_IFNDEF)) {
        if(content[(*pos)] == '(' || content[(*pos)] == ' ' || content[(*pos)] == '\t' || content[(*pos)] == '{') {
            (*pos) -= 1;
        }
        (*tok)->type = TOK_KW_IFNDEF;
        (*tok)->lexeme = malloc(sizeof(KW_IFNDEF));
        strcpy((*tok)->lexeme, KW_IFNDEF);
    }
    else if (compare_ongoing_stream(content, pos, KW_TYPEDEF)) {
        if(content[(*pos)] == '(' || content[(*pos)] == ' ' || content[(*pos)] == '\t' || content[(*pos)] == '{') {
            (*pos) -= 1;
        }
        (*tok)->type = TOK_KW_TYPEDEF;
        (*tok)->lexeme = malloc(sizeof(KW_TYPEDEF));
        strcpy((*tok)->lexeme, KW_TYPEDEF);
    }
    else if (compare_ongoing_stream(content, pos, KW_STRUCT)) {
        if(content[(*pos)] == '(' || content[(*pos)] == ' ' || content[(*pos)] == '\t' || content[(*pos)] == '{') {
            (*pos) -= 1;
        }
        (*tok)->type = TOK_KW_STRUCT;
        (*tok)->lexeme = malloc(sizeof(KW_STRUCT));
        strcpy((*tok)->lexeme, KW_STRUCT);
    }
    else if (compare_ongoing_stream(content, pos, KW_UNION)) {
        if(content[(*pos)] == '(' || content[(*pos)] == ' ' || content[(*pos)] == '\t' || content[(*pos)] == '{') {
            (*pos) -= 1;
        }
        (*tok)->type = TOK_KW_UNION;
        (*tok)->lexeme = malloc(sizeof(KW_UNION));
        strcpy((*tok)->lexeme, KW_UNION);
    }
    else if (compare_ongoing_stream(content, pos, KW_SIZEOF)) {
        if(content[(*pos)] == '(' || content[(*pos)] == ' ' || content[(*pos)] == '\t' || content[(*pos)] == '{') {
            (*pos) -= 1;
        }
        (*tok)->type = TOK_KW_SIZEOF;
        (*tok)->lexeme = malloc(sizeof(KW_SIZEOF));
        strcpy((*tok)->lexeme, KW_SIZEOF);
    }
    else if (compare_ongoing_stream(content, pos, KW_ENUM)) {
        if(content[(*pos)] == '(' || content[(*pos)] == ' ' || content[(*pos)] == '\t' || content[(*pos)] == '{') {
            (*pos) -= 1;
        }
        (*tok)->type = TOK_KW_ENUM;
        (*tok)->lexeme = malloc(sizeof(KW_ENUM));
        strcpy((*tok)->lexeme, KW_ENUM);
    }
    else if (compare_ongoing_stream(content, pos, KW_EXTERN)) {
        if(content[(*pos)] == '(' || content[(*pos)] == ' ' || content[(*pos)] == '\t' || content[(*pos)] == '{') {
            (*pos) -= 1;
        }
        (*tok)->type = TOK_KW_EXTERN;
        (*tok)->lexeme = malloc(sizeof(KW_EXTERN));
        strcpy((*tok)->lexeme, KW_EXTERN);
    }
    else if (compare_ongoing_stream(content, pos, KW_STATIC)) {
        if(content[(*pos)] == '(' || content[(*pos)] == ' ' || content[(*pos)] == '\t' || content[(*pos)] == '{') {
            (*pos) -= 1;
        }
        (*tok)->type = TOK_KW_STATIC;
        (*tok)->lexeme = malloc(sizeof(KW_STATIC));
        strcpy((*tok)->lexeme, KW_STATIC);
    }
    else if (compare_ongoing_stream(content, pos, KW_CONST)) {
        if(content[(*pos)] == '(' || content[(*pos)] == ' ' || content[(*pos)] == '\t' || content[(*pos)] == '{') {
            (*pos) -= 1;
        }
        (*tok)->type = TOK_KW_CONST;
        (*tok)->lexeme = malloc(sizeof(KW_CONST));
        strcpy((*tok)->lexeme, KW_CONST);
    }


    // typespec
    else if (compare_ongoing_stream(content, pos, TYPESPEC_Void)) {
            (*pos)--;
            (*tok)->type = TOK_TYPESPEC_Void;
            (*tok)->lexeme = malloc(sizeof(TYPESPEC_Void));
            strcpy((*tok)->lexeme, TYPESPEC_Void);
        }
    else if (compare_ongoing_stream(content, pos, TYPESPEC_Char)) {
            (*pos)--;
            (*tok)->type = TOK_TYPESPEC_Char;
            (*tok)->lexeme = malloc(sizeof(TYPESPEC_Char));
            strcpy((*tok)->lexeme, TYPESPEC_Char);
        }
    else if (compare_ongoing_stream(content, pos, TYPESPEC_Short)) {
            (*pos)--;
            (*tok)->type = TOK_TYPESPEC_Short;
            (*tok)->lexeme = malloc(sizeof(TYPESPEC_Short));
            strcpy((*tok)->lexeme, TYPESPEC_Short);
        }
    else if (compare_ongoing_stream(content, pos, TYPESPEC_Int)) {
            (*pos)--;
            (*tok)->type = TOK_TYPESPEC_Int;
            (*tok)->lexeme = malloc(sizeof(TYPESPEC_Int));
            strcpy((*tok)->lexeme, TYPESPEC_Int);
        }
    else if (compare_ongoing_stream(content, pos, TYPESPEC_Long)) {
            (*pos)--;
            (*tok)->type = TOK_TYPESPEC_Long;
            (*tok)->lexeme = malloc(sizeof(TYPESPEC_Long));
            strcpy((*tok)->lexeme, TYPESPEC_Long);
        }
    else if (compare_ongoing_stream(content, pos, TYPESPEC_Float)) {
            (*pos)--;
            (*tok)->type = TOK_TYPESPEC_Float;
            (*tok)->lexeme = malloc(sizeof(TYPESPEC_Float));
            strcpy((*tok)->lexeme, TYPESPEC_Float);
        }
    else if (compare_ongoing_stream(content, pos, TYPESPEC_Double)) {
            (*pos)--;
            (*tok)->type = TOK_TYPESPEC_Double;
            (*tok)->lexeme = malloc(sizeof(TYPESPEC_Double));
            strcpy((*tok)->lexeme, TYPESPEC_Double);
        }
    else if (compare_ongoing_stream(content, pos, TYPESPEC_Signed)) {
            (*pos)--;
            (*tok)->type = TOK_TYPESPEC_Signed;
            (*tok)->lexeme = malloc(sizeof(TYPESPEC_Signed));
            strcpy((*tok)->lexeme, TYPESPEC_Signed);
        }
    else if (compare_ongoing_stream(content, pos, TYPESPEC_Unsigned)) {
            (*pos)--;
            (*tok)->type = TOK_TYPESPEC_Unsigned;
            (*tok)->lexeme = malloc(sizeof(TYPESPEC_Unsigned));
            strcpy((*tok)->lexeme, TYPESPEC_Unsigned);
        }
    else if (compare_ongoing_stream(content, pos, TYPESPEC_Bool)) {
            (*pos)--;
            (*tok)->type = TOK_TYPESPEC_Bool;
            (*tok)->lexeme = malloc(sizeof(TYPESPEC_Bool));
            strcpy((*tok)->lexeme, TYPESPEC_Bool);
        }
    else if (compare_ongoing_stream(content, pos, TYPESPEC_Complex)) {
            (*pos)--;
            (*tok)->type = TOK_TYPESPEC_Complex;
            (*tok)->lexeme = malloc(sizeof(TYPESPEC_Complex));
            strcpy((*tok)->lexeme, TYPESPEC_Complex);
        }
    else if (compare_ongoing_stream(content, pos, TYPESPEC_Atomic)) {
            (*pos)--;
            (*tok)->type = TOK_TYPESPEC_Atomic;
            (*tok)->lexeme = malloc(sizeof(TYPESPEC_Atomic));
            strcpy((*tok)->lexeme, TYPESPEC_Atomic);
        }
    else if (compare_ongoing_stream(content, pos, KW_INLINE)) {
            (*pos)--;
            (*tok)->type = TOK_KW_INLINE;
            (*tok)->lexeme = malloc(sizeof(KW_INLINE));
            strcpy((*tok)->lexeme, KW_INLINE);
        }

    else {
        //(*tok)->type = 0;
        (*tok)->lexeme = NULL;
    }
}

// Function to check if a character is an operator
void isOperator(const char* content, char c, int* pos, struct Token** tok) {
    (*tok)->lexeme = malloc(2);
    switch (c) {
        case '+':
            if(content[(*pos)+1] == '=') {
                (*pos)++;
                (*tok)->type = TOK_OP_PLUSEQUALS;
                strcpy((*tok)->lexeme, OP_PLUSEQUALS);
            }
            else {
                (*tok)->type = TOK_OP_PLUS;
                strcpy((*tok)->lexeme, OP_PLUS);
            }

            break;
        case '-':
            if(content[(*pos)+1] == '>') {
                (*pos)++;
                (*tok)->type = TOK_OP_ARROW;
                strcpy((*tok)->lexeme, OP_ARROW);
            }
            else if(content[(*pos)+1] == '=') {
                (*pos)++;
                (*tok)->type = TOK_OP_MINUSEQUALS;
                strcpy((*tok)->lexeme, OP_MINUSEQUALS);
            }
            else {
                (*tok)->type = TOK_OP_MINUS;
                strcpy((*tok)->lexeme, OP_MINUS);
            }
            break;
        case '*':
            if(content[(*pos)+1] == '=') {
                (*pos)++;
                (*tok)->type = TOK_OP_MULEQUALS;
                strcpy((*tok)->lexeme, OP_MULEQUALS);
            }
            else {
                (*tok)->type = TOK_OP_MULTIPLY;
                strcpy((*tok)->lexeme, OP_MULTIPLY);
            }
            break;
        case '/':
            if(content[(*pos)+1] == '=') {
                (*pos)++;
                (*tok)->type = TOK_OP_DIVEQUALS;
                strcpy((*tok)->lexeme, OP_DIVEQUALS);
            }
            (*tok)->type = TOK_OP_DIVIDE;
            strcpy((*tok)->lexeme, OP_DIVIDE);
            break;
        case '=':
            if(content[(*pos)+1] == '=') {
                (*pos)++;
                (*tok)->type = TOK_OP_EQUALSQUALS;
                strcpy((*tok)->lexeme, OP_EQUALSEQUALS);
            }
            (*tok)->type = TOK_OP_EQUALS;
            strcpy((*tok)->lexeme, OP_EQUALS);
            break;
        case '&':
            if(content[(*pos)+1] == '&') {
                (*pos)++;
                (*tok)->type = TOK_OP_ANDAND;
                strcpy((*tok)->lexeme, OP_ANDAND);
            }
            else {
                (*tok)->type = TOK_OP_AND;
                strcpy((*tok)->lexeme, OP_AND);
            }
            break;
        case '|':
            if(content[(*pos)+1] == '=') {
                (*pos)++;
                (*tok)->type = TOK_OP_OROR;
                strcpy((*tok)->lexeme, OP_OROR);
            }
            else {
                (*tok)->type = TOK_OP_OR;
                strcpy((*tok)->lexeme, OP_OR);
            }
            break;
        case '!':
            if(content[(*pos)+1] == '=') {
                (*pos)++;
                (*tok)->type = TOK_OP_NOTEQUALS;
                strcpy((*tok)->lexeme, OP_NOTEQUALS);
            }
            (*tok)->type = TOK_OP_NOT;
            strcpy((*tok)->lexeme, OP_NOT);
            break;
        case '^':
            (*tok)->type = TOK_OP_XOR;
            strcpy((*tok)->lexeme, OP_XOR);
            break;
        case '<':
            if(content[(*pos)+1] == '<') {
                (*pos)++;
                (*tok)->type = TOK_OP_SHIFT_LEFT;
                strcpy((*tok)->lexeme, OP_SHIFT_LEFT);
            }
            else if(content[(*pos)+1] == '=') {
                (*pos)++;
                (*tok)->type = TOK_OP_LOWER_EQUALS;
                strcpy((*tok)->lexeme, OP_LOWER_EQUALS);
            }
            else {
                (*tok)->type = TOK_OP_LOWER;
                strcpy((*tok)->lexeme, OP_LOWER);
            }
            break;
        case '>':
            if(content[(*pos)+1] == '>') {
                (*pos)++;
                (*tok)->type = TOK_OP_SHIFT_RIGHT;
                strcpy((*tok)->lexeme, OP_SHIFT_RIGHT);
            }
            else if(content[(*pos)+1] == '=') {
                (*pos)++;
                (*tok)->type = TOK_OP_BIGGER_EQUALS;
                strcpy((*tok)->lexeme, OP_BIGGER_EQUALS);
            }
            else {
                (*tok)->type = TOK_OP_BIGGER;
                strcpy((*tok)->lexeme, OP_BIGGER);
            }
            break;
        case '%':
            if(content[(*pos)+1] == '=') {
                (*pos)++;
                (*tok)->type = TOK_OP_MODULOEQUALS;
                strcpy((*tok)->lexeme, OP_MODULOEQUALS);
            }
            else {
                (*tok)->type = TOK_OP_MODULO;
                strcpy((*tok)->lexeme, OP_MODULO);
            }
            break;
        default:
            free((*tok)->lexeme);
            (*tok)->lexeme = NULL; // Empty string if not an operator
            break;
    }


}

// Function to check if a character is a punctuation mark
void isPunctuation(const char c, struct Token** tok) {
    (*tok)->lexeme = malloc(2);
    switch (c) {
        case '(':
            (*tok)->type = TOK_PUNCT_OPEN_PAREN;
            strcpy((*tok)->lexeme, PUNCT_OPEN_PAREN);
            break;
        case ')':
            (*tok)->type = TOK_PUNCT_CLOSE_PAREN;
            strcpy((*tok)->lexeme, PUNCT_CLOSE_PAREN);
            break;
        case ';':
            (*tok)->type = TOK_PUNCT_SEMICOLON;
            strcpy((*tok)->lexeme, PUNCT_SEMICOLON);
            break;
        case '.':
            (*tok)->type = TOK_PUNCT_DOT;
            strcpy((*tok)->lexeme, PUNCT_DOT);
            break;
        case '{':
            (*tok)->type = TOK_PUNCT_OPEN_BRACE;
            strcpy((*tok)->lexeme, PUNCT_OPEN_BRACE);
            break;
        case '}':
            (*tok)->type = TOK_PUNCT_CLOSE_BRACE;
            strcpy((*tok)->lexeme, PUNCT_CLOSE_BRACE);
            break;
        case '[':
            (*tok)->type = TOK_PUNCT_OPEN_BRACKET;
            strcpy((*tok)->lexeme, PUNCT_OPEN_BRACKET);
            break;
        case ']':
            (*tok)->type = TOK_PUNCT_CLOSE_BRACKET;
            strcpy((*tok)->lexeme, PUNCT_CLOSE_BRACKET);
            break;
        case ',':
            (*tok)->type = TOK_PUNCT_COMMA;
            strcpy((*tok)->lexeme, PUNCT_COMMA);
            break;
        case '#':
            (*tok)->type = TOK_PUNCT_HASH;
            strcpy((*tok)->lexeme, PUNCT_HASH);
            break;
        case '\"':
            (*tok)->type = TOK_PUNCT_DOUBLEQUOTE;
            strcpy((*tok)->lexeme, PUNCT_DOUBLEQUOTE);
            break;
        case '\'':
            (*tok)->type = TOK_PUNCT_SINGLEQUOTE;
            strcpy((*tok)->lexeme, PUNCT_SINGLEQUOTE);
            break;
        case '\\':
            (*tok)->type = TOK_PUNCT_BACKSLASH;
            strcpy((*tok)->lexeme, PUNCT_BACKSLASH);
            break;
        case ':':
            (*tok)->type = TOK_PUNCT_COLON;
            strcpy((*tok)->lexeme, PUNCT_COLON);
            break;
        case '?':
            (*tok)->type = TOK_PUNCT_QUESTIONMARK;
            strcpy((*tok)->lexeme, PUNCT_QUESTIONMARK);
            break;
            /*
        case '\n':
            (*tok)->type = TOK_PUNCT_NEWLINE;
            strcpy((*tok)->lexeme, PUNCT_NEWLINE);
            break;
            */
        default:
            free((*tok)->lexeme);
            (*tok)->lexeme = NULL; // Empty string if not a punctuation mark
            break;
    }


}

void addToken(struct Token*** tokens, int* num_tokens, int* tokenIndex, struct Token** tok) {
    if (*tokenIndex < *num_tokens) {
        // Allocate memory for the lexeme and copy it
        (*tokens)[*tokenIndex] = *tok;
        (*tokenIndex)++;
    } else {
        const int new_size = (*num_tokens) * 2;
        *tokens = realloc(*tokens, sizeof(struct Token) * new_size);

        if (*tokens != NULL) {
            *num_tokens = new_size;
            // Add the token to the resized array
            (*tokens)[*tokenIndex] = *tok;
            (*tokenIndex)++;
        } else {
            fprintf(stderr, "Error: Unable to resize token array.\n");
            // Consider aborting or returning an error code here to avoid using the potentially corrupted memory.
        }
    }
}

void take_until_punct_or_else(const char* content, int* pos, struct Token** tok) {
    int start_pos = *pos;

    // Continue until a punctuation or whitespace character is encountered
    while (content[*pos] != '\0') {
        const char currentChar = content[*pos];

        if (currentChar == PUNCT_SPACE[0] ||
            currentChar == PUNCT_TAB[0] ||
            currentChar == PUNCT_OPEN_PAREN[0] ||
            currentChar == PUNCT_CLOSE_PAREN[0] ||
            currentChar == PUNCT_SEMICOLON[0] ||
            currentChar == PUNCT_OPEN_BRACE[0] ||
            currentChar == PUNCT_CLOSE_BRACE[0] ||
            currentChar == PUNCT_OPEN_BRACKET[0] ||
            currentChar == PUNCT_CLOSE_BRACKET[0] ||
            currentChar == PUNCT_COMMA[0] ||
            currentChar == PUNCT_HASH[0] ||
            currentChar == PUNCT_DOUBLEQUOTE[0] ||
            currentChar == PUNCT_SINGLEQUOTE[0] ||
            currentChar == PUNCT_DOT[0] ||
            currentChar == PUNCT_QUESTIONMARK[0] ||
            currentChar == PUNCT_NEWLINE[0] ||
            currentChar == PUNCT_BACKSLASH[0] ||
            currentChar == PUNCT_COLON[0] ||

            currentChar == OP_MINUS[0] ||
            currentChar == OP_PLUS[0] ||
            currentChar == OP_BIGGER[0] ||
            currentChar == OP_LOWER[0] ||
            currentChar == OP_OR[0] ||
            currentChar == OP_AND[0] ||
            currentChar == OP_EQUALS[0] ||
            currentChar == OP_NOT[0] ||
            currentChar == OP_DIVIDE[0] ||
            currentChar == OP_MULTIPLY[0] ||
            currentChar == OP_XOR[0] ||
            currentChar == OP_MODULO[0]) {
            // Break the loop when a punctuation or whitespace character is encountered
            break;
        }

        (*pos)++;
    }

    // Allocate memory for the lexeme and copy the substring
    int length = *pos - start_pos;
    (*tok)->lexeme = malloc(length + 1);
    strncpy((*tok)->lexeme, &content[start_pos], length);
    (*tok)->lexeme[length] = '\0'; // Null-terminate the string

    // Set the token type (you may need to determine the type based on the lexeme)
    (*tok)->type = TOK_STRING;
}



// Function to lexically analyze the input
void lex(const char* input, int* num_tokens, int* tokenIndex, struct Token*** tokens) {
    // Iterate through the input
    for (int i = 0; input[i] != '\0'; ++i) {
        char token = input[i];
        // Check if the character is part of an identifier or keyword
        if(input[i] == '\0' || input[i] == ' ' || input[i] == '\t' || input[i] == '\n') {
            continue;
        }
        struct Token *tok = malloc(sizeof(struct Token));
        tok->lexeme = NULL;
        if (isalpha(input[i]) || input[i] == '_') {
            isKeyword(input, &i, &tok);
            token = input[i];
            if(tok->lexeme) {
                addToken(tokens, num_tokens, tokenIndex, &tok);
                continue;
            }
            token = input[i];
            take_until_punct_or_else(input, &i, &tok);
            i--;

            if(tok->lexeme) {
                addToken(tokens, num_tokens, tokenIndex,& tok);
                continue;
            }
            // free(tok);
        }
        else if(tok->lexeme) {
            addToken(tokens, num_tokens, tokenIndex, &tok);
            continue;
        }
        else {
            isOperator(input, input[i], &i, &tok);
            if(tok->lexeme) {
                addToken(tokens, num_tokens, tokenIndex, &tok);
                continue;
            }
            isPunctuation(input[i], &tok);
            if(tok->lexeme) {
                addToken(tokens, num_tokens, tokenIndex, &tok);
                continue;
            }
            if(isalnum(input[i])) {
                char num[30];
                int p = 0;
                while(isalnum(input[i]) && input[i] != '\0') {
                    num[p] = input[i];
                    i++;
                    p++;
                }
                i--;
                num[p] = '\0';
                char* number = malloc(strlen(num));
                strcpy(number, num);
                tok->lexeme = number;
                tok->type = TOK_INTEGER;
                addToken(tokens, num_tokens, tokenIndex, &tok);
                continue;
            }
        }
        //if(input[i] == '\n') { continue; }
        fprintf(stderr, "unknown token! %d", input[i]);
        //free(tok->lexeme); // Free memory allocated for lexeme
        //free(tok);        // Free memory allocated for Token
        // Add more cases for literals, comments, etc. as needed
    }
}
