
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>



#include "main.h"

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
struct Token* isKeyword(const char* content, int* pos, struct Token** tok) {
    // Dummy implementation, replace with actual keyword checking
    if (compare_ongoing_stream(content, pos, KW_IF)) {
        if(content[(*pos)] != '(' && content[(*pos)] != ' ' && content[(*pos)] != '\t' && content[(*pos)] != '{') {
            (*pos) -= strlen(KW_IF);
            return *tok;
        }
        (*tok)->type = TOK_KW_IF;
        (*tok)->lexeme = malloc(sizeof(KW_IF));
        strcpy((*tok)->lexeme, KW_IF);
    } else if (compare_ongoing_stream(content, pos, KW_ELSE)) {
        if(content[(*pos)] != '(' && content[(*pos)] != ' ' && content[(*pos)] != '\t' && content[(*pos)] != '{') {
            (*pos) -= strlen(KW_ELSE);
            return (*tok);
        }
        (*tok)->type = TOK_KW_ELSE;
        (*tok)->lexeme = malloc(sizeof(KW_ELSE));
        strcpy((*tok)->lexeme, KW_ELSE);
    } else if (compare_ongoing_stream(content, pos, KW_WHILE)) {
        if(content[(*pos)] != '(' && content[(*pos)] != ' ' && content[(*pos)] != '\t' && content[(*pos)] != '{') {
            (*pos) -= strlen(KW_WHILE);
            return (*tok);
        }
        (*tok)->type = TOK_KW_WHILE;
        (*tok)->lexeme = malloc(sizeof(KW_WHILE));
        strcpy((*tok)->lexeme, KW_WHILE);
    } else if (compare_ongoing_stream(content, pos, KW_FOR)) {
        if(content[(*pos)] != '(' && content[(*pos)] != ' ' && content[(*pos)] != '\t' && content[(*pos)] != '{') {
            (*pos) -= strlen(KW_FOR);
            return (*tok);
        }
        (*tok)->type = TOK_KW_FOR;
        (*tok)->lexeme = malloc(sizeof(KW_FOR));
        strcpy((*tok)->lexeme, KW_FOR);
    } else if (compare_ongoing_stream(content, pos, KW_RETURN)) {
        if(content[(*pos)] != '(' && content[(*pos)] != ' ' && content[(*pos)] != '\t' && content[(*pos)] != '{') {
            (*pos) -= strlen(KW_RETURN);
            return (*tok);
        }
        (*tok)->type = TOK_KW_RETURN;
        (*tok)->lexeme = malloc(sizeof(KW_RETURN));
        strcpy((*tok)->lexeme, KW_RETURN);
    } else if (compare_ongoing_stream(content, pos, KW_INCLUDE)) {
        if(content[(*pos)] != '(' && content[(*pos)] != ' ' && content[(*pos)] != '\t' && content[(*pos)] != '{') {
            (*pos) -= strlen(KW_INCLUDE);
            return (*tok);
        }
        (*tok)->type = TOK_KW_INCLUDE;
        (*tok)->lexeme = malloc(sizeof(KW_INCLUDE));
        strcpy((*tok)->lexeme, KW_INCLUDE);
    } else if (compare_ongoing_stream(content, pos, KW_INCLUDE)) {
        if(content[(*pos)] != '(' && content[(*pos)] != ' ' && content[(*pos)] != '\t' && content[(*pos)] != '{') {
            (*pos) -= strlen(KW_INCLUDE);
            return (*tok);
        }
        (*tok)->type = TOK_KW_INCLUDE;
        (*tok)->lexeme = malloc(sizeof(KW_INCLUDE));
        strcpy((*tok)->lexeme, KW_INCLUDE);
    } else if (compare_ongoing_stream(content, pos, KW_REGISTER)) {
        if(content[(*pos)] != '(' && content[(*pos)] != ' ' && content[(*pos)] != '\t' && content[(*pos)] != '{') {
            (*pos) -= strlen(KW_REGISTER);
            return (*tok);
        }
        (*tok)->type = TOK_KW_REGISTER;
        (*tok)->lexeme = malloc(sizeof(KW_REGISTER));
        strcpy((*tok)->lexeme, KW_REGISTER);
    } else if (compare_ongoing_stream(content, pos, KW_DEFINE)) {
        if(content[(*pos)] != '(' && content[(*pos)] != ' ' && content[(*pos)] != '\t' && content[(*pos)] != '{') {
            (*pos) -= strlen(KW_DEFINE);
            return (*tok);
        }
        (*tok)->type = TOK_KW_DEFINE;
        (*tok)->lexeme = malloc(sizeof(KW_DEFINE));
        strcpy((*tok)->lexeme, KW_DEFINE);
    } else if (compare_ongoing_stream(content, pos, KW_PRAGMA)) {
        if(content[(*pos)] != '(' && content[(*pos)] != ' ' && content[(*pos)] != '\t' && content[(*pos)] != '{') {
            (*pos) -= strlen(KW_PRAGMA);
            return (*tok);
        }
        (*tok)->type = TOK_KW_PRAGMA;
        (*tok)->lexeme = malloc(sizeof(KW_PRAGMA));
        strcpy((*tok)->lexeme, KW_PRAGMA);
    } else if (compare_ongoing_stream(content, pos, KW_UNDEF)) {
        if(content[(*pos)] != '(' && content[(*pos)] != ' ' && content[(*pos)] != '\t' && content[(*pos)] != '{') {
            (*pos) -= strlen(KW_UNDEF);
            return (*tok);
        }
        (*tok)->type = TOK_KW_UNDEF;
        (*tok)->lexeme = malloc(sizeof(KW_UNDEF));
        strcpy((*tok)->lexeme, KW_UNDEF);
    } else if (compare_ongoing_stream(content, pos, KW_ENDIF)) {
        if(content[(*pos)] != '(' && content[(*pos)] != ' ' && content[(*pos)] != '\t' && content[(*pos)] != '{') {
            (*pos) -= strlen(KW_ENDIF);
            return (*tok);
        }
        (*tok)->type = TOK_KW_ENDIF;
        (*tok)->lexeme = malloc(sizeof(KW_ENDIF));
        strcpy((*tok)->lexeme, KW_ENDIF);
    } else if (compare_ongoing_stream(content, pos, KW_IFDEF)) {
        if(content[(*pos)] != '(' && content[(*pos)] != ' ' && content[(*pos)] != '\t' && content[(*pos)] != '{') {
            (*pos) -= strlen(KW_IFDEF);
            return (*tok);
        }
        (*tok)->type = TOK_KW_IFDEF;
        (*tok)->lexeme = malloc(sizeof(KW_IFDEF));
        strcpy((*tok)->lexeme, KW_IFDEF);
    }
    else if (compare_ongoing_stream(content, pos, KW_TYPEDEF)) {
            if(content[(*pos)] != '(' && content[(*pos)] != ' ' && content[(*pos)] != '\t' && content[(*pos)] != '{') {
                (*pos) -= strlen(KW_TYPEDEF);
                return (*tok);
            }
            (*tok)->type = TOK_KW_TYPEDEF;
            (*tok)->lexeme = malloc(sizeof(KW_TYPEDEF));
            strcpy((*tok)->lexeme, KW_TYPEDEF);
        }
    else if (compare_ongoing_stream(content, pos, KW_STRUCT)) {
            if(content[(*pos)] != '(' && content[(*pos)] != ' ' && content[(*pos)] != '\t' && content[(*pos)] != '{') {
                (*pos) -= strlen(KW_STRUCT);
                return (*tok);
            }
            (*tok)->type = TOK_KW_STRUCT;
            (*tok)->lexeme = malloc(sizeof(KW_STRUCT));
            strcpy((*tok)->lexeme, KW_STRUCT);
        }
    else if (compare_ongoing_stream(content, pos, KW_ENUM)) {
            if(content[(*pos)] != '(' && content[(*pos)] != ' ' && content[(*pos)] != '\t' && content[(*pos)] != '{') {
                (*pos) -= strlen(KW_ENUM);
                return (*tok);
            }
            (*tok)->type = TOK_KW_ENUM;
            (*tok)->lexeme = malloc(sizeof(KW_ENUM));
            strcpy((*tok)->lexeme, KW_ENUM);
        }
    else if (compare_ongoing_stream(content, pos, KW_EXTERN)) {
            if(content[(*pos)] != '(' && content[(*pos)] != ' ' && content[(*pos)] != '\t' && content[(*pos)] != '{') {
                (*pos) -= strlen(KW_EXTERN);
                return (*tok);
            }
            (*tok)->type = TOK_KW_EXTERN;
            (*tok)->lexeme = malloc(sizeof(KW_EXTERN));
            strcpy((*tok)->lexeme, KW_EXTERN);
        }
    else if (compare_ongoing_stream(content, pos, KW_STATIC)) {
            if(content[(*pos)] != '(' && content[(*pos)] != ' ' && content[(*pos)] != '\t' && content[(*pos)] != '{') {
                (*pos) -= strlen(KW_STATIC);
                return (*tok);
            }
            (*tok)->type = TOK_KW_STATIC;
            (*tok)->lexeme = malloc(sizeof(KW_STATIC));
            strcpy((*tok)->lexeme, KW_STATIC);
        }
    else if (compare_ongoing_stream(content, pos, KW_CONST)) {
            if(content[(*pos)] != '(' && content[(*pos)] != ' ' && content[(*pos)] != '\t' && content[(*pos)] != '{') {
                (*pos) -= strlen(KW_CONST);
                return (*tok);
            }
            (*tok)->type = TOK_KW_CONST;
            (*tok)->lexeme = malloc(sizeof(KW_CONST));
            strcpy((*tok)->lexeme, KW_CONST);
        }
    else {
        //(*tok)->type = 0;
        (*tok)->lexeme = NULL;
    }

    return (*tok);
}

// Function to check if a character is an operator
struct Token* isOperator(const char* content, char c, int* pos, struct Token** tok) {
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

    return (*tok);
}

// Function to check if a character is a punctuation mark
struct Token* isPunctuation(char c, struct Token** tok) {
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
        case '?':
            (*tok)->type = TOK_PUNCT_QUESTIONMARK;
            strcpy((*tok)->lexeme, PUNCT_QUESTIONMARK);
        case '\n':
            (*tok)->type = TOK_PUNCT_NEWLINE;
            strcpy((*tok)->lexeme, PUNCT_NEWLINE);
    break;
        default:
            free((*tok)->lexeme);
            (*tok)->lexeme = NULL; // Empty string if not a punctuation mark
            break;
    }

    return (*tok);
}

void addToken(struct Token*** tokens, int* num_tokens, int* tokenIndex, struct Token** tok) {
    if (*tokenIndex < *num_tokens) {
        // Allocate memory for the lexeme and copy it
        (*tokens)[*tokenIndex] = *tok;
        (*tokenIndex)++;
    } else {
        const int new_size = (*num_tokens) * 2;
        struct Token** temp = realloc(*tokens, sizeof(struct Token*) * new_size);
        *num_tokens = new_size;
        (*tokens) = temp;

        if (temp != NULL) {
            *tokens = temp;
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



struct Token* take_until_punct_or_else(const char* content, int* pos, struct Token** tok) {
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

    return (*tok);
}



// Function to lexically analyze the input
void lex(const char* input, int* num_tokens, int* tokenIndex, struct Token*** tokens) {
    // Iterate through the input
    for (int i = 0; input[i] != '\0'; ++i) {
        const char token = input[i];
        // Check if the character is part of an identifier or keyword
        if(input[i] == '\0' || input[i] == ' ' || input[i] == '\t') {
            continue;
        }
        struct Token *tok = malloc(sizeof(struct Token));
        tok->lexeme = NULL;
        if (isalpha(input[i]) || input[i] == '_') {
            tok = isKeyword(input, &i, &tok);
            if(tok->lexeme) {
                addToken(tokens, num_tokens, tokenIndex, &tok);
                continue;
            }
            tok = take_until_punct_or_else(input, &i, &tok);
            i--;
            if(tok->lexeme) {
                addToken(tokens, num_tokens, tokenIndex, &tok);
                continue;
            }
            // free(tok);
        }
        else {
            tok = isOperator(input, input[i], &i, &tok);
            if(tok->lexeme) {
                addToken(tokens, num_tokens, tokenIndex, &tok);
                continue;
            }
            tok = isPunctuation(input[i], &tok);
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
        fprintf(stderr, "unknown token! %d", input[i]);
        free(tok);
        // Add more cases for literals, comments, etc. as needed
    }
}



struct Include* parseInclude(struct Token*** tokens, int* num_tokens, int* tokenIndex) {
    struct Include* include = malloc(sizeof(struct Include));

    char* relative_included_path = malloc(100);
    struct Token* tok = (*tokens)[(*tokenIndex)];
    if((*tokens)[(*tokenIndex)]->type == TOK_OP_LOWER) {
        include->include_type = STANDARD;
    }
    else if((*tokens)[(*tokenIndex)]->type == TOK_PUNCT_DOUBLEQUOTE) {
        include->include_type = NONSTANDARD; // ""
    }
    else {
        fprintf(stderr, "error in parsing include statement!");
    }
    (*tokenIndex)++;
    while((*num_tokens) > (*tokenIndex) && (*tokens)[(*tokenIndex)]->type != TOK_PUNCT_NEWLINE) {
        tok = (*tokens)[(*tokenIndex)];
        if((*tokens)[(*tokenIndex)]->type != TOK_OP_BIGGER && (*tokens)[(*tokenIndex)]->type != TOK_PUNCT_DOUBLEQUOTE) {
            strcat(relative_included_path, (*tokens)[(*tokenIndex)]->lexeme);
            (*tokenIndex)++;
        }

        else {
            (*tokenIndex)++; // only \n follows or should at least.
        }
    }

    include->relative_included_path = relative_included_path;

    printf("path: %s ", include->relative_included_path);

    return include;
}

void parseCall(struct Token*** tokens, int* num_tokens, int* tokenIndex, struct Call** call) {
    (*call)->arguments = NULL;
    (*call)->acount = 0;
    // open brace is jummped over, - only arguments parsing.
    while(1) {
        if((*tokens)[(*tokenIndex)]->type == TOK_PUNCT_CLOSE_PAREN) {
            break;
        }
    }
}
/*
struct Expression* parseExpression(struct Token*** tokens, int* num_tokens, int* tokenIndex) {
    struct Expression* expr = malloc(sizeof(struct Expression));
    expr->expr = NULL;
    struct Token* tok = (*tokens)[(*tokenIndex)];
    // Compare
    if((*tokens)[(*tokenIndex)]->type != TOK_PUNCT_OPEN_PAREN) {
        (*tokenIndex)++;
    }
    int num_opened_paren = 0;
    enum MemoryOperator_ expr_op = NONE;
    while(1) {
        if((*tokens)[(*tokenIndex)]->type == TOK_PUNCT_OPEN_PAREN) {
            if(expr->expr != NULL) {
                //parseExpression();
            }
            else {
                // something already parsed and set.
                if(expr->type == Expr_Constant) {
                    if(((Constant*)(expr->expr))->btype == INT || ((Constant*)(expr->expr))->btype == CHAR_PTR) {
                        return expr;
                    }
                    else if(((Constant*)(expr->expr))->btype == STRING) {
                        // variable or function or type name
                        if((*tokens)[(*tokenIndex)]->type != TOK_PUNCT_OPEN_PAREN) {
                            // call
                            struct Call* call = malloc(sizeof(struct Call));
                            call->function_name = expr->expr;
                            expr->expr = call;
                            //expr->op = expr_op;
                            expr->type = Expr_Call;
                            (*tokenIndex)++;
                            parseCall(tokens, num_tokens, tokenIndex, &call);
                            return expr;
                        }
                        else if((*tokens)[(*tokenIndex)]->type != TOK_STRING) {
                            // type

                        }
                    }
                }
            }
            num_opened_paren++;
            (*tokenIndex)++;
        }
        else if((*tokens)[(*tokenIndex)]->type == TOK_PUNCT_CLOSE_PAREN) {
            num_opened_paren--;
            (*tokenIndex)++;
        }
        else if((*tokens)[(*tokenIndex)]->type == TOK_OP_MULTIPLY) {
            expr_op = POINTER;
            (*tokenIndex)++;
        }
        else if((*tokens)[(*tokenIndex)]->type == TOK_OP_AND) {
            expr_op = REFERENCE;
            (*tokenIndex)++;
        }
        else if((*tokens)[(*tokenIndex)]->type == TOK_PUNCT_OPEN_BRACE) {
            (*tokenIndex)++;
            //parseExpression();
        }
        else if((*tokens)[(*tokenIndex)]->type == TOK_PUNCT_CLOSE_BRACE) {
            (*tokenIndex)++;
            return expr;
        }

        else if((*tokens)[(*tokenIndex)]->type == TOK_STRING) {
            struct Constant* c = malloc(sizeof(struct Constant*));
            char* s = malloc(strlen((*tokens)[(*tokenIndex)]->lexeme));
            c->builtin_type = s;
            c->btype = STRING;
            expr->expr = c;
            expr->type = Expr_Constant;
            //expr->op = expr_op;
            (*tokenIndex)++;
        }
        else if((*tokens)[(*tokenIndex)]->type == TOK_INTEGER) {
            struct Constant* c = malloc(sizeof(struct Constant*));
            c->builtin_type = (*tokens)[(*tokenIndex)]->lexeme; // TODO: google conversion
            c->btype = INT;
            expr->expr = c;
            expr->type = Expr_Constant;
            //expr->op = expr_op;
            (*tokenIndex)++;
        }
    }
    fprintf(stderr, "error in parsing expression!");
    return expr;
}
*/

struct Compare* parseCompare(struct Token*** tokens, int* num_tokens, int* tokenIndex) {
    struct Compare* cmp = malloc(sizeof(struct Compare));
    struct Token* tok = (*tokens)[(*tokenIndex)];
    // Compare
    if((*tokens)[(*tokenIndex)]->type != TOK_PUNCT_OPEN_PAREN) {
        (*tokenIndex)++;
    }
    while(1) {
        tok = (*tokens)[(*tokenIndex)];
        //struct Expression* expr = parseExpression(tokens, num_tokens, tokenIndex);

    }
    return cmp;
}

struct If* parseIf(struct Token*** tokens, int* num_tokens, int* tokenIndex) {
    struct If* if_stmt = malloc(sizeof(struct If));
    if_stmt->bcount = 0;
    struct Token* tok = (*tokens)[(*tokenIndex)];
    // Compare
    if_stmt->compare = parseCompare(tokens, num_tokens, tokenIndex);
    if((*tokens)[(*tokenIndex)]->type != TOK_PUNCT_OPEN_BRACKET) {
        (*tokenIndex)++;
    }
    while(1) {
        struct Statement* s = parseStatement(tokens, num_tokens, tokenIndex);
        if((*tokens)[(*tokenIndex)]->type != TOK_PUNCT_CLOSE_BRACKET || s == NULL) {
            (*tokenIndex)++;
            break;
        }
        else {
            if_stmt->body = realloc(if_stmt->body, sizeof(Statement*) * (if_stmt->bcount + 1));
            if_stmt->body[if_stmt->bcount] = s;
            if_stmt->bcount++;
        }
    }
    return if_stmt;
}

struct For* parseFor(struct Token*** tokens, int* num_tokens, int* tokenIndex) {

}

struct IfDef* parseIfDef(struct Token*** tokens, int* num_tokens, int* tokenIndex) {

}

struct Define* parseDefine(struct Token*** tokens, int* num_tokens, int* tokenIndex) {
    struct Define* define = malloc(sizeof(struct Define));

    // Skip #define
    (*tokenIndex)++;

    if (*tokenIndex < *num_tokens) {
        define->name = strdup((*tokens)[*tokenIndex]->lexeme);
        // You might want to handle macro replacement and parameters here
        (*tokenIndex)++;  // Move to the next token after the identifier

        // Parse the statement associated with #define
        define->stmt = parseStatement(tokens, num_tokens, tokenIndex);

        return define;
    } else {
        fprintf(stderr, "Error: Unexpected end of tokens after #define\n");
        free(define);
        return NULL;
    }
}

// Function to parse #endif statement
struct EndIf* parseEndIf(struct Token*** tokens, int* num_tokens, int* tokenIndex) {
    struct EndIf* endIf = malloc(sizeof(struct EndIf));

    // Skip #endif
    (*tokenIndex)++;

    // Parse the corresponding #ifdef
    endIf->ifdef = parseStatement(tokens, num_tokens, tokenIndex);

    return endIf;
}

struct Symbol** parseParams(struct Token*** tokens, int* num_tokens, int* tokenIndex) {
    // Initialize with a small capacity
    size_t capacity = 5;
    struct Symbol** parameters = malloc(capacity * sizeof(struct Param*));

    int paramIndex = 0;

    while (*num_tokens > *tokenIndex) {
        // Check if the array needs resizing
        if (paramIndex >= capacity - 1) {
            // Double the capacity
            capacity *= 2;
            parameters = realloc(parameters, capacity * sizeof(struct Param*));

            if (!parameters) {
                fprintf(stderr, "Error: Memory reallocation failed\n");
                exit(EXIT_FAILURE);  // Handle the error as appropriate for your parser
            }
        }

        // Parse the type and name of the parameter
        // Skip the , or (
        (*tokenIndex)++;
        struct Symbol* paramSymbol = parseSymbolAndType(tokens, num_tokens, tokenIndex);

        // Check if parseSymbolAndType succeeded
        if (!paramSymbol) {
            // Handle the error as appropriate for your parser
            break;
        }
        // Add the parameter to the array
        parameters[paramIndex] = paramSymbol;

        // Increment the parameter index
        paramIndex++;

        // Check for a comma (',') indicating more parameters
        if ((*tokens)[*tokenIndex]->type == TOK_PUNCT_COMMA) {}
        else if ((*tokens)[*tokenIndex]->type == TOK_PUNCT_CLOSE_PAREN) {
            // skip )
            (*tokenIndex)++;
            break;
        } else {
            // Error: Expected comma (',') or close parenthesis (')') after parameter
            fprintf(stderr, "Error: Expected comma (',') or close parenthesis (')') after parameter\n");
            // Handle the error as appropriate for your parser
            break;
        }
    }

    // Add a NULL at the end of the array to indicate the end of parameters
    parameters[paramIndex] = NULL;

    return parameters;
}



// Symbol contains Type, but Symbol->symbol_name could be NULL !!!
//
struct Symbol* parseSymbolAndType(struct Token*** tokens, int* num_tokens, int* tokenIndex) {
    if(!isalpha((*tokens)[*tokenIndex]->lexeme[0])) {
        return NULL;
    }

    struct Type* type = malloc(sizeof(struct Type));
    struct Symbol* symbol = malloc(sizeof(struct Symbol));
    type->type_kw = 0;
    symbol->type = type;

    struct Token* token_arr[20];
    int pos = 0;
    struct Token* current_token = (*tokens)[*tokenIndex];
    while((*num_tokens) > (*tokenIndex)  && current_token->type != TOK_PUNCT_SEMICOLON && current_token->type != TOK_PUNCT_OPEN_PAREN && current_token->type != TOK_PUNCT_OPEN_BRACE && current_token->type != TOK_OP_EQUALS && current_token->type != TOK_PUNCT_COMMA && current_token->type != TOK_PUNCT_CLOSE_PAREN) {
        if(current_token->type == TOK_KW_CONST) {
            type->type_kw |= CONST;
            (*tokenIndex)++;
            current_token = (*tokens)[*tokenIndex];
            continue;
        }
        else if(current_token->type == TOK_KW_STATIC) {
            type->type_kw |= STATIC;
            (*tokenIndex)++;
            current_token = (*tokens)[*tokenIndex];
            continue;
        }
        else if(current_token->type == TOK_KW_TYPEDEF) {
            type->type_kw |= TYPEDEF;
            (*tokenIndex)++;
            current_token = (*tokens)[*tokenIndex];
            continue;
        }
        else if(current_token->type == TOK_KW_ENUM) {
            type->type_kw |= ENUM;
            (*tokenIndex)++;
            current_token = (*tokens)[*tokenIndex];
            continue;
        }

        if(current_token->type == TOK_PUNCT_NEWLINE) {
            (*tokenIndex)++;
            current_token = (*tokens)[*tokenIndex];
            continue;
        }
        current_token = (*tokens)[*tokenIndex];
        token_arr[pos] = current_token;
        pos++;
        (*tokenIndex)++;
    }
    pos -= 2; // the stop token and one more in cycle
    (*tokenIndex)--;

    if(current_token->type == TOK_PUNCT_SEMICOLON) {
        if(pos == 0) {
            // varname;
            symbol->symbol_name = current_token->lexeme;
        }
        else {
            // from the end
            symbol->symbol_name = token_arr[pos]->lexeme;
            pos--;
            if(pos > 0 && token_arr[pos]->type == TOK_OP_MULTIPLY) {
                // memory
                symbol->type->op = POINTER;
                while(token_arr[pos]->type == TOK_OP_MULTIPLY) {
                    symbol->type->num_op++;
                    pos--;
                }
            }
            if(pos < 0) {
                fprintf(stderr, "ERROR pos < 0   !");
            }
            if(pos == 0) {
                symbol->type->type_name = token_arr[pos]->lexeme;
            }
            else {
                fprintf(stderr, "still ERROR about pos !");
            }
        }
    }
    else if(current_token->type == TOK_OP_EQUALS) {
        // from the end
        symbol->symbol_name = token_arr[pos]->lexeme;
        pos--;
        if(pos > 0 && token_arr[pos]->type == TOK_OP_MULTIPLY) {
            // memory
            symbol->type->op = POINTER;
            while(token_arr[pos]->type == TOK_OP_MULTIPLY) {
                symbol->type->num_op++;
                pos--;
            }
        }
        if(pos < 0) {
            fprintf(stderr, "ERROR pos < 0   !");
        }
        if(pos == 0) {
            symbol->type->type_name = token_arr[pos]->lexeme;
        }
        else {
            fprintf(stderr, "still ERROR about pos !");
        }
    }
    else if(current_token->type == TOK_PUNCT_OPEN_PAREN) {
        // Handle open parenthesis
        symbol->symbol_name = token_arr[pos]->lexeme;
        pos--;
        if(pos > 0 && token_arr[pos]->type == TOK_OP_MULTIPLY) {
            // memory
            type->op = POINTER;
            while(token_arr[pos]->type == TOK_OP_MULTIPLY) {
                type->num_op++;
                pos--;
            }
        }
        if(pos < 0) {
            fprintf(stderr, "ERROR pos < 0   !");
        }
        if(pos == 0) {
            type->type_name = token_arr[pos]->lexeme;
        }
    }
    // casting -> only type info   |  param -> depends
    // TODO:    if(pos == 0) {
    //              only varname but in what case ???
    // }
    else if(current_token->type == TOK_PUNCT_CLOSE_PAREN) {
        // Handle open parenthesis
        symbol->symbol_name = token_arr[pos]->lexeme;
        pos--;
        if(pos > 0 && token_arr[pos]->type == TOK_OP_MULTIPLY) {
            // memory
            type->op = POINTER;
            while(token_arr[pos]->type == TOK_OP_MULTIPLY) {
                type->num_op++;
                pos--;
            }
        }
        if(pos < 0) {
            fprintf(stderr, "ERROR pos < 0   !");
        }
        if(pos == 0) {
            type->type_name = token_arr[pos]->lexeme;
        }
    }
    else if(current_token->type == TOK_PUNCT_COMMA) {
        // Handle open parenthesis
        symbol->symbol_name = token_arr[pos]->lexeme;
        pos--;
        if(pos > 0 && token_arr[pos]->type == TOK_OP_MULTIPLY) {
            // memory
            type->op = POINTER;
            while(token_arr[pos]->type == TOK_OP_MULTIPLY) {
                type->num_op++;
                pos--;
            }
        }
        if(pos < 0) {
            fprintf(stderr, "ERROR pos < 0   !");
        }
        if(pos == 0) {
            type->type_name = token_arr[pos]->lexeme;
        }
    }
    else if(current_token->type == TOK_PUNCT_OPEN_BRACE) {

    }
    else {
        fprintf(stderr, "ERROR: Unexpected token type in parseString\n");
    }

    return symbol;
}

// operates on the left side of any expression(statement) only !!!,
// e.g. in call  -> not having type info this could misinterpret fucntion name for type...
void parseStringLeftSide(struct Token*** tokens, int* num_tokens, int* tokenIndex, struct Statement** stmt, struct Type** type) {
    struct Symbol* symbol = parseSymbolAndType(tokens, num_tokens, tokenIndex);
    if(!symbol) {
        fprintf(stderr, "ERROR !!!!");
        return;
    }
    struct Token* current_token = (*tokens)[*tokenIndex];

    if(current_token->type == TOK_PUNCT_SEMICOLON) {
        struct Assign* decl = malloc(sizeof(struct Assign));
        decl->left = symbol;
        decl->right = NULL;
        (*stmt)->stmt = decl;
        (*stmt)->stmt_type = Stmt_Assign;
        //(*tokenIndex)++;

    }
    else if(current_token->type == TOK_OP_EQUALS) {
        struct Assign* decl = malloc(sizeof(struct Assign));
        decl->left = symbol;
        decl->left_ = Stmt_Symbol;
        (*stmt)->stmt = decl;
        (*stmt)->stmt_type = Stmt_Assign;

        decl->right = NULL;
        //(*tokenIndex)++;
    }
    else if(current_token->type == TOK_PUNCT_OPEN_PAREN) {
        // Handle open parenthesis
        struct FunctionDeclaration* funDecl = malloc(sizeof(struct FunctionDeclaration));
        funDecl->symbol = symbol;
        struct Symbol** parameters = parseParams(tokens, num_tokens, tokenIndex);
        funDecl->parameters = parameters;

        current_token = (*tokens)[(*tokenIndex)];
        if(current_token->type == TOK_PUNCT_SEMICOLON) {
            (*tokenIndex)++;
        }

        (*stmt)->stmt = funDecl;
        (*stmt)->stmt_type = Stmt_FunctionDeclaration;
        // Skip the open parenthesis
        //(*tokenIndex)++;
    }
    else if(current_token->type == TOK_PUNCT_OPEN_BRACE) {

    }
    else {
        fprintf(stderr, "ERROR: Unexpected token type in parseString\n");
    }
}

struct Comment* parseComment(struct Token*** tokens, int* num_tokens, int* tokenIndex) {
    struct Comment* cmnt = malloc(sizeof(Comment));
    char* comment = malloc(1000);
    while((*tokenIndex) > (*num_tokens) && ((*tokens)[(*tokenIndex)+1]->type != TOK_OP_MULTIPLY && (*tokens)[(*tokenIndex)+1]->type == TOK_OP_DIVIDE)) {
        strcat(comment, (*tokens)[(*tokenIndex)]->lexeme);
    }
    cmnt->comment = comment;
    printf("Comment: %s", comment);

    return cmnt;
}

struct Statement* parseStatement(struct Token*** tokens, int* num_tokens, int* tokenIndex) {
    struct Statement* stmt = malloc(sizeof(Statement));
    struct Token* tokenbuf[20]; // const static extern struct ************** varname ;
    int tb = 0;
    for(int* i=tokenIndex; *i < (*num_tokens); (*i)++) {
        const struct Token* current_token = (*tokens)[*i];
        if(current_token->type == TOK_PUNCT_NEWLINE) {
            (*i)++;
        }
        else if(current_token->type == TOK_PUNCT_HASH) {
            // preprocessor stmt
            if(((*tokens)[(*i)+1])->type == TOK_KW_INCLUDE) {
                (*i) += 2;
                stmt->stmt = parseInclude(tokens, num_tokens, i);
                stmt->stmt_type = Stmt_Include;
                return stmt;
            }
            else if(((*tokens)[(*i)+1])->type == TOK_KW_IFDEF) {
                (*i) += 2;
                stmt->stmt = parseIfDef(tokens, num_tokens, i);

                return stmt;
            }
            else if(((*tokens)[(*i)+1])->type == TOK_KW_DEFINE) {
                (*i) += 2;
                stmt->stmt = parseDefine(tokens, num_tokens, i);

                return stmt;
            }
            else if(((*tokens)[(*i)+1])->type == TOK_KW_ENDIF) {
                (*i) += 2;
                stmt->stmt = parseEndIf(tokens, num_tokens, i);

                return stmt;
            }
        }


        else if(current_token->type == TOK_KW_TYPEDEF) {
            (*i)++;
            //stmt->stmt = parseIf(tokens, num_tokens, i);
            //stmt->stmt_type = Stmt_If;
            return stmt;
        }
        else if(current_token->type == TOK_KW_IF) {
            (*i)++;
            stmt->stmt = parseIf(tokens, num_tokens, i);
            stmt->stmt_type = Stmt_If;
            return stmt;
        }
        else if(current_token->type == TOK_KW_FOR) {
            (*i)++;
            stmt->stmt = parseFor(tokens, num_tokens, i);
            stmt->stmt_type = Stmt_For;
            return stmt;
        }
        else if((current_token->type == TOK_OP_DIVIDE && (*tokens)[(*i)+1]->type == TOK_OP_DIVIDE) ||
                (current_token->type == TOK_OP_DIVIDE && (*tokens)[(*i)+1]->type == TOK_OP_MULTIPLY)) {
            parseComment(tokens, num_tokens, i);
            (*i) += 2;
        }


        // no keyword
        else if(current_token->type == TOK_STRING || current_token->type == TOK_KW_CONST || current_token->type == TOK_KW_STATIC) {
            // (*i)++;
            struct Type* type = malloc(sizeof(struct Type));
            parseStringLeftSide(tokens, num_tokens, i, &stmt, &type);
            return stmt;
        }
        else if(current_token->type == TOK_OP_MULTIPLY) {
            // (*i)++;
            // e.g. *varname .......
            return stmt;
        }
        else {
            // e.g. compound statement: (...)
        }

        /*
        // must be type or definition, sooo -> type or known variable name or compound
        else if(current_token->type == TOK_STRING || current_token->type == TOK_OP_MULTIPLY || current_token->type == TOK_OP_AND) {
            i++;
            struct Constant* c = malloc(sizeof(struct Constant*));
            char* s = malloc(strlen((*tokens)[(*tokenIndex)]->lexeme));
            c->builtin_type = s;
            c->btype = STRING;

            //parseFor(tokens, num_tokens, i);
        }
        */
    }
    stmt->stmt = NULL;
    return stmt;
}


struct TranslationUnit* parseTranslationUnit(struct Token*** tokens, int* num_tokens) {
    struct TranslationUnit* TU = malloc(sizeof(struct TranslationUnit));
    TU->body = NULL; // Initialize body to NULL
    TU->count = 0;   // Initialize count to 0

    for (int i = 0; i < *num_tokens; i++) {
        TU->body = realloc(TU->body, sizeof(Statement*) * (TU->count + 1));
        TU->body[TU->count] = parseStatement(tokens, num_tokens, &i);
        TU->count++;
    }

    return TU;
}


// recurse to free memory
void freeTranslationUnit(struct TranslationUnit** TU) {
    for (int i = 0; i < (*TU)->count; i++) {
        free(((*TU)->body)[i]);
    }
}

// Example usage
int main() {
    int num_tokens = 1000;
    int tokenIndex = 0;

    struct Token** tokens = malloc(sizeof(struct Token*) * num_tokens);

    //char* content = readFile("/home/adam/Plocha/C_PARSER_INTERPRETER/C_P_I/main.c");
    //printf("%s", content);

    const char* content = "const int x;int x(const int a, char b, int** c); static const char** g;";
    //struct Token* tokens = lex(input);

    lex(content, &num_tokens, &tokenIndex, &tokens);
    struct TranslationUnit* TU = parseTranslationUnit(&tokens, &tokenIndex);

    // Process or print tokens as needed
    for (int i = 0; i < tokenIndex; ++i) {
        printf("Type: %d, Lexeme: %s\n", tokens[i]->type, tokens[i]->lexeme);
    }

    for (int i = 0; i < tokenIndex; ++i) {
        if(tokens[i]->lexeme == NULL || tokens[i] == NULL) {
            fprintf(stderr, "NULL: %d \n", i);
        }
    }


    // Free allocated memory
    //for (int i = 0; tokens[i].type != 0; ++i) {
    //    free(tokens[i].lexeme);
    //}
    //free(content);

    for(int i=0; i< tokenIndex; i++) {
        free(tokens[i]->lexeme);
        free(tokens[i]);
    }
    free(tokens);

    //freeTranslationUnit(&TU);
    //free(TU);

    return 0;
}
