#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
//#include <stdbool.h>



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



char getToken(const char* content, int* c_pos) {
    if(strlen(content) > (*c_pos)) {
        char c = content[(*c_pos)++];
        return c;
    }
    else {
        return 0;
    }
}

void skipSimpleChars(const char* content, int* c_pos) {
    char c;
    while ((c = content[*c_pos]) != '\0' && (c == ' ' || c == '\t' || c == '\n' || c == '\r')) {
        (*c_pos)++;
    }
}

char* parsePath(const char* content, int* c_pos, char stop_char) {
    char c;
    int start_pos = *c_pos;
    while ((c = getToken(content, c_pos))) {
        if (c == stop_char) {
            int length = *c_pos - start_pos - 1;
            char* path = malloc(length + 1);
            strncpy(path, content + start_pos, length);
            path[length] = '\0';
            return path;
        }
    }
    return NULL;  // Handle error: unexpected end of input
}

// ' ' is mostly jumped over autoamatically but not from start of statement parsing, also we need it to check syntax
static const char stop_punct[] = { ' ', '(', ')', '{', '}', '+', '-', '=', '!', '\n', '\t', ';', ',', ':', '*', '/', '&', '|', '<', '>', '\"', '\'', '?', '\r', '^', '%' }; // not #

char* parsePathStopPunct(const char* content, int* c_pos, char* stop_char) {
    char c;
    const int start_pos = *c_pos;
    while ((c = getToken(content, c_pos))) {
        if(isalpha(c) || isalnum(c)) {
            continue;
        }
        else {
            char eq = 0;
            for(int i=0; i < sizeof(stop_punct); i++) {
                if(isalpha(c) || isalnum(c)) {
                    *stop_char = stop_punct[i];
                    break;
                }
                else if(c == stop_punct[i]) {
                    eq = stop_punct[i];
                    *stop_char = stop_punct[i];
                    break;
                }
            }
            if (eq) {
                int length = *c_pos - start_pos - 1;
                if(length > 0) {
                    char* path = malloc(length + 1);
                    strncpy(path, content + start_pos, length);
                    path[length] = '\0';
                    return path;
                }
                else {
                    *stop_char = 0;
                    return NULL;
                }
            }
        }
    }
    *stop_char = 0;
    return NULL;  // Handle error: unexpected end of input
}

struct Include* parseInclude(const char* content, int* c_pos) {
    (*c_pos)++;  // Skip '#'
    char c;
    struct Include* include = malloc(sizeof(struct Include));

    // Skip whitespace after '#'
    skipSimpleChars(content, c_pos);

    // Check if it's an include directive
    if (strncmp(content + *c_pos, "include", 7) == 0) {
        (*c_pos) += strlen("include");  // Skip "include"

        // Skip whitespace after 'include'
        skipSimpleChars(content, c_pos);

        c = content[*c_pos];
        if (c == '<') {
            (*c_pos)++;
            include->include_type = STANDARD;
            include->relative_included_path = parsePath(content, c_pos, '>');
        }
        else if (c == '\"') {
            (*c_pos)++;
            include->include_type = NONSTANDARD;
            include->relative_included_path = parsePath(content, c_pos, '\"');
        }

        // Now 'include' structure is populated with type and path
        // You can use include->include_type and include->path as needed

        // Clean up - free memory when done with the include structure
        printf("%s", include->relative_included_path);
        //free(include->relative_included_path);
        //free(include);
    }
    skipSimpleChars(content, c_pos);

    return include;
}

/*
struct ParsedStruct parseKeywordStruct(const char* content, int* c_pos, const char* kw) {
    struct ParsedStruct parsedStruct = { NULL, NULL };

    if (strcmp(kw, "struct") == 0) {
        skipSimpleChars(content, c_pos);

        // Continue parsing for struct name and modifiers
        char stop_char;
        parsedStruct.name = parsePathStopPunct(content, c_pos, &stop_char);

        // Loop to handle * or & modifiers
        while (stop_char == '*' || stop_char == '&') {
            // Handle modifier
            if (parsedStruct.modifier == NULL) {
                parsedStruct.modifier = malloc(2);
                parsedStruct.modifier[0] = stop_char;
                parsedStruct.modifier[1] = '\0';
            } else {
                // Handle additional modifiers if needed
                // Note: This is a simple example; in a real-world scenario, you might want a more sophisticated approach
            }

            // Continue parsing for struct name
            parsedStruct.name = parsePathStopPunct(content, c_pos, &stop_char);
        }
    }

    return parsedStruct;
}
*/

int parseKeywordTypedef(const char* content, int* c_pos, const char* kw, struct Symbol** sym) {
    if (strcmp(kw, "typedef") == 0) {
        (*sym)->type_kw |= TYPEDEF;
        return 1;
    }
    return 0;
}
int parseKeywordStruct(const char* content, int* c_pos, const char* kw, struct Symbol** sym) {
    if (strcmp(kw, "struct") == 0) {
        (*sym)->type_kw |= STRUCT;
        return 1;
    }
    return 0;
}
int parseKeywordExtern(const char* content, int* c_pos, const char* kw, struct Symbol** sym) {
    if (strcmp(kw, "extern") == 0) {
        (*sym)->type_kw |= EXTERN;
        return 1;
    }
    return 0;
}
int parseKeywordEnum(const char* content, int* c_pos, const char* kw, struct Symbol** sym) {
    if (strcmp(kw, "enum") == 0) {
        (*sym)->type_kw |= ENUM;
        return 1;
    }
    return 0;
}
int parseKeywordIf(const char* content, int* c_pos, const char* kw, struct Symbol** sym) {
    if (strcmp(kw, "if") == 0) {

    }
    return 1;
}
int parseKeywordElse(const char* content, int* c_pos, const char* kw, struct Symbol** sym) {
    if (strcmp(kw, "else") == 0) {

    }
    return 1;
}
int parseKeywordWhile(const char* content, int* c_pos, const char* kw, struct Symbol** sym) {
    if (strcmp(kw, "while") == 0) {

    }
    return 1;
}
int parseKeywordReturn(const char* content, int* c_pos, const char* kw, struct Symbol** sym) {
    if (strcmp(kw, "return") == 0) {

    }
    return 1;
}
int parseKeywordStatic(const char* content, int* c_pos, const char* kw, struct Symbol** sym) {
    if (strcmp(kw, "static") == 0) {
        (*sym)->type_kw |= STATIC;
        return 1;
    }
    return 0;
}
int parseKeywordConst(const char* content, int* c_pos, const char* kw, struct Symbol** sym) {
    if (strcmp(kw, "const") == 0) {
        (*sym)->type_kw |= CONST;
        return 1;
    }
    return 0;
}

int parseKeywordFor(const char* content, int* c_pos, const char* kw, struct Symbol** sym) {
    if (strcmp(kw, "for") == 0) {
        return 1;
    }
    return 0;
}

int parseAssignRightSideExpression(const char* content, int* c_pos) {
    char stop_char;
    const char* x = parsePathStopPunct(content, c_pos, &stop_char);



}

// sym should be empty or sym->name set to typename
// if empty we try to parse typename and varname, if sym->name set we parsing only varname.
void parseTypeOrVarname(const char* content, int* c_pos, struct Statement** s, struct Symbol** sym) {
    // sym already contains
    if((*sym) == NULL) {
        struct Symbol* sym = malloc(sizeof(struct Symbol));
        fprintf(stderr, "should never reach this.");
    }


    skipSimpleChars(content, c_pos);

    char stop_char = 0;
    const char* p = parsePathStopPunct(content, c_pos, &stop_char);


    if((*sym)->type_kw == 0 && (*sym)->name) {
        // type is set parse identifiers and varname

        // this one could only be a type (int, char, float, ... builtin types), follows either ***** or varname
        if(stop_char == '*') {
            (*sym)->op = POINTER;
            (*sym)->num_op = 1;
            // could have more ** in declarations
            if(content[(*c_pos)+1] == '*') {
                (*c_pos)++;
                while(content[(*c_pos)] == '*') {
                    (*sym)->num_op++;
                    (*c_pos)++;
                }
                // varname

            }
        }
        else if(stop_char == ';') {
            struct Symbol* sym_name = malloc(sizeof(struct Symbol));
            sym_name->name = (char*)p;
            struct Declaration* decl = malloc(sizeof(struct Declaration));
            decl->type_sym = (*sym);
            decl->sym_name = sym_name;

            (*s)->stmt = decl;
            (*s)->stmt_type = Stmt_Declaration;
        }
    }
    else if ((*sym)->type_kw != 0) {
        // type is not set parse all

    }


    else if (stop_char == '=') {
        (*c_pos)++;
        skipSimpleChars(content, c_pos);

        parseAssignRightSideExpression(content, c_pos);
    }
    else if(stop_char == ')' || stop_char == '}') {

    }
    else {
        fprintf(stderr, "should never reach this with stop_char %c", stop_char);
    }
}

void parseStatement_(const char* content, int* c_pos, struct Statement** s) {
    // we are at the begining.
    // Assuming a simple variable declaration for now
    char c, stop_char = 0;
    skipSimpleChars(content, c_pos);

    char* p = parsePathStopPunct(content, c_pos, &stop_char);

    if(!p && stop_char == 0) {
        free((*s));
        (*s) = NULL;
        return;
    }

    struct Symbol* sym = malloc(sizeof(struct Symbol));
    sym->type_kw = 0;
    if(stop_char == ' ') {
        int kw_found = 0;
        while(kw_found) {
            // keywords for type
            kw_found = parseKeywordConst(content, c_pos, p, &sym);
            kw_found = parseKeywordStruct(content, c_pos, p, &sym);
            kw_found = parseKeywordStatic(content, c_pos, p, &sym);
            kw_found = parseKeywordExtern(content, c_pos, p, &sym);
            kw_found = parseKeywordEnum(content, c_pos, p, &sym);
            kw_found = parseKeywordTypedef(content, c_pos, p, &sym);
        }
        if(sym->type_kw == 0) {
            sym->name = (char*)p;
            // no type here possibly keyword, or else some typename/varname - later
            parseTypeOrVarname(content, c_pos, s, &sym);
        }
        else {
            parseTypeOrVarname(content, c_pos, s, &sym);
        }

        // varname, typename - same we do not differ between types

    }
    //varname=
    else if(stop_char == '=') {
        sym->name = (char*)p;
        parseAssignRightSideExpression(content, c_pos);
    }
    // case varname/typename ;
    else if(stop_char == ';') {
        fprintf(stderr, "Expression does nothing\n");
        (*c_pos)++;
        sym->name = (char*)p;
    }
    else {
        fprintf(stderr, "Should never be there\n");
        free((char*)p);
        free(sym);
    }
}

struct Statement* parseStatement(const char* content, int* pos) {
    struct Statement* s = malloc(sizeof(struct Statement));
    char c = content[*pos];
    if(c == '#') {
        struct Include* i = parseInclude(content, pos);
        s->stmt = i;
        s->stmt_type = Stmt_Include;
    }
    else {
        parseStatement_(content, pos, &s);
    }

    return s;
}

void parseC(const char* content) {
    int pos = 0;
    char c;

    struct TranslationUnit* TU = malloc(sizeof(struct TranslationUnit));
    TU->body = NULL;  // Initialize to NULL initially
    TU->count = 0;

    while(content[pos] != '\0') {
        Statement* s = parseStatement(content, &pos);
        TU->body = realloc(TU->body, sizeof(Statement*) * TU->count + 1);
        TU->body[TU->count] = s;
        TU->count++;
    }

    free(TU);
}




// Example usage
int main() {
    //char* content = readFile("/home/adam/Plocha/C_PARSER_INTERPRETER/C_P_I/main.c");
    //printf("%s", content);

    const char* content = "#include <stdio.h>\nx; "; // main() { int x; return 0; }

    parseC(content);

    return 0;
}










