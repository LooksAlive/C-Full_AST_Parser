#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "ast_dumper.h"
#include "generic_vector.h"
#include "lexer.h"


void jumpOverNewLines(struct Token*** tokens, int* num_tokens, int* tokenIndex) {
    for(int*i = tokenIndex; (*tokens)[(*tokenIndex)]->type == TOK_PUNCT_NEWLINE; (*i)++) {
        (*i)++;
    }

    //assert(1 && "ahoj");
    //static_assert(true);
}

struct Identifier* init_identifier(const char* name) {
    struct Identifier* id = malloc(sizeof(struct Identifier));
    id->name = malloc(strlen(name));
    strcpy(id->name, name);

    return id;
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
        fprintf(stderr, "error in parsing include Statement!");
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

struct Compare* parseCompare(struct Token* tokens, int* num_tokens, int* tokenIndex) {
    struct Compare* cmp = malloc(sizeof(struct Compare));
    struct Token tok = tokens[(*tokenIndex)];
    // Compare
    if(tokens[(*tokenIndex)].type != TOK_PUNCT_OPEN_PAREN) {
        (*tokenIndex)++;
    }
    while(1) {
        tok = tokens[(*tokenIndex)];
        //struct Expression* expr = parseExpression(tokens, num_tokens, tokenIndex);

    }
    return cmp;
}

struct Statement* parseElse(struct Token*** tokens, int* num_tokens, int* tokenIndex) {
    struct Statement* stmt = malloc(sizeof(struct Statement));
    stmt->stmt_type = Stmt_Else;

    struct Else* else_stmt = malloc(sizeof(struct Else));
    else_stmt->bcount = 0;
    else_stmt->body = NULL;

    stmt->stmt = else_stmt;


    /* body */
    while((*tokens)[(*tokenIndex)]->type == TOK_PUNCT_OPEN_BRACE || (*tokens)[(*tokenIndex)]->type == TOK_PUNCT_NEWLINE) {
        (*tokenIndex)++;
    }

    while((*tokens)[(*tokenIndex)]->type != TOK_PUNCT_CLOSE_BRACE) {
        struct Statement* s = parseStatement(tokens, num_tokens, tokenIndex);

        else_stmt->body = realloc(else_stmt->body, sizeof(struct Statement*) * (else_stmt->bcount + 1));
        else_stmt->body[else_stmt->bcount] = s;
        else_stmt->bcount++;
    }
    (*tokenIndex)++; // jump over TOK_PUNCT_CLOSE_BRACE in while

}

// open parenthesis ( is jumped over from parseStatement()
struct Statement* parseIf(struct Token*** tokens, int* num_tokens, int* tokenIndex) {
    jumpOverNewLines(tokens, num_tokens, tokenIndex);
    if((*tokens)[(*tokenIndex)]->type == TOK_PUNCT_OPEN_PAREN) { (*tokenIndex)++; };

    struct Statement* stmt = malloc(sizeof(struct Statement));
    stmt->stmt_type = Stmt_If;

    struct If* if_stmt = malloc(sizeof(struct If));
    if_stmt->bcount = 0;
    if_stmt->body = NULL;
    if_stmt->else_part = NULL;

    stmt->stmt = if_stmt;

    //if((*tokens)[(*tokenIndex)]->type != TOK_PUNCT_OPEN_BRACKET) {};
    // TODO: comparation
    if_stmt->compare = parseCompare(tokens, num_tokens, tokenIndex);
    if((*tokens)[(*tokenIndex)]->type == TOK_PUNCT_CLOSE_PAREN) { (*tokenIndex)++; };

    /* body */
    while((*tokens)[(*tokenIndex)]->type == TOK_PUNCT_OPEN_BRACE || (*tokens)[(*tokenIndex)]->type == TOK_PUNCT_NEWLINE) {
        (*tokenIndex)++;
    }

    while((*tokens)[(*tokenIndex)]->type != TOK_PUNCT_CLOSE_BRACE) {
        struct Statement* s = parseStatement(tokens, num_tokens, tokenIndex);

        if_stmt->body = realloc(if_stmt->body, sizeof(struct Statement*) * (if_stmt->bcount + 1));
        if_stmt->body[if_stmt->bcount] = s;
        if_stmt->bcount++;
    }
    (*tokenIndex)++; // jump over TOK_PUNCT_CLOSE_BRACE in while
    /* else / else if */
    if((*tokens)[(*tokenIndex)]->type != TOK_KW_ELSE) {
        if((*tokens)[(*tokenIndex)]->type != TOK_KW_IF) {
            (*tokenIndex) += 2;
            (*tokenIndex)++; /* ( */
            if_stmt->else_part = parseIf(tokens, num_tokens, tokenIndex);
            if_stmt->else_part->stmt_type = Stmt_ElseIf;
            // NOTE: If and ElseIf structs are the same.
        }
        else {
            if_stmt->else_part = parseElse(tokens, num_tokens, tokenIndex);
        }
    }

    return stmt;
}


struct TypeSpecList* init_type_spec_list() {
    struct TypeSpecList* type_spec_list = malloc(sizeof(TypeSpecList));
    type_spec_list->type_spec = TypeSpecifier_NONE;
    type_spec_list->next = NULL;
    return type_spec_list;
}

struct BaseType* init_base_type(struct TypeSpecList** type_spec_list) {
    struct BaseType* baseType = malloc(sizeof(struct BaseType));
    baseType->type_spec_list = (*type_spec_list);

    baseType->qualtype = QualType_NONE;
    baseType->storage_spec = StorageClassSpecifier_NONE;
    baseType->op = MemoryOperator_NONE;
    baseType->num_op = 0;

    return baseType;
}

void add_type_spec_to_list(const char* name /* if typedefname */, enum TypeSpecifier_ typespec, struct TypeSpecList** list) {
    struct Identifier* id = malloc(sizeof(Identifier));
    if(name) {
        id->name = malloc(strlen(name));
        strcpy(id->name, name);
    }

    ///struct TypeSpecList* l = (*list);

    if((*list)->type_spec == TypeSpecifier_NONE) {
        (*list)->id = id;
        (*list)->type_spec = typespec;
        //(*list) = (*list)->next;
    }
    else {
        struct TypeSpecList* type_name_list_next = malloc(sizeof(struct TypeSpecList));
        type_name_list_next->id = id;
        type_name_list_next->type_spec = typespec;
        type_name_list_next->next = NULL;

        (*list)->next = type_name_list_next;
        (*list) = type_name_list_next;
    }
}

enum QualType_ isQuatType(const Token* tok) {
    switch(tok->type) {
        case TOK_KW_CONST:
            return QualType_CONST;
        case TOK_TYPESPEC_Atomic:
            return QualType_ATOMIC;
        default:
            return QualType_NONE;
    }
}

enum StorageClassSpecifier_ isStorageClassSpecifier(const Token* tok) {
    switch(tok->type) {
        case TOK_KW_EXTERN:
            return StorageClassSpecifier_Extern;
        case TOK_KW_STATIC:
            return StorageClassSpecifier_Static;
        default:
            return StorageClassSpecifier_NONE;
    }
}

enum TypeSpecifier_ isTypeSpecifier(const Token* tok) {
    switch (tok->type) {
        case TOK_TYPESPEC_Void:
            return TypeSpecifier_Void;
        case TOK_TYPESPEC_Char:
            return TypeSpecifier_Char;
        case TOK_TYPESPEC_Short:
            return TypeSpecifier_Short;
        case TOK_TYPESPEC_Int:
            return TypeSpecifier_Int;
        case TOK_TYPESPEC_Long:
            return TypeSpecifier_Long;
        case TOK_TYPESPEC_Float:
            return TypeSpecifier_Float;
        case TOK_TYPESPEC_Double:
            return TypeSpecifier_Double;
        case TOK_TYPESPEC_Signed:
            return TypeSpecifier_Signed;
        case TOK_TYPESPEC_Unsigned:
            return TypeSpecifier_Unsigned;
        case TOK_TYPESPEC_Bool:
            return TypeSpecifier_Bool;
        case TOK_TYPESPEC_Complex:
            return TypeSpecifier_Complex;
        case TOK_TYPESPEC_M128:
            return TypeSpecifier_M128;
        case TOK_TYPESPEC_M128D:
            return TypeSpecifier_M128D;
        case TOK_TYPESPEC_M128I:
            return TypeSpecifier_M128I;
        case TOK_TYPESPEC_M128Extension:
            return TypeSpecifier_M128Extension;
        case TOK_TYPESPEC_Atomic:
            return TypeSpecifier_Atomic;
        case TOK_KW_STRUCT:
            return TypeSpecifier_Struct;
        case TOK_KW_UNION:
            return TypeSpecifier_Union;
        case TOK_KW_ENUM:
            return TypeSpecifier_Enum;
        case TOK_TYPESPEC_TypeofExtension:
            return TypeSpecifier_TypeofExtension;
        default:
            return TypeSpecifier_NONE;
    }
}

enum FunctionSpecifier_ isFunctionSpecifier(const Token* tok) {
    switch(tok->type) {
        case TOK_KW_INLINE:
            return FunctionSpecifier_Inline;
        default:
            return FunctionSpecifier_NONE;
    }
}

struct Identifier* isTypedefName(const Token** tok, const struct MacrosAndTypeDefNames** macros_and_tpd_names) {
    for(int i = 0; i < (*macros_and_tpd_names)->typedefnames_size; i++) {
        if(strcmp((*tok)->lexeme, (*macros_and_tpd_names)->typedefnames[i]) == 0) {
            return init_identifier((*tok)->lexeme);
        }
    }

    return NULL;
}

struct Identifier* isMacroName(Token** tok, const struct MacrosAndTypeDefNames** macros_and_tpd_names) {
    for(int i = 0; i < (*macros_and_tpd_names)->macros_size; i++) {
        if(strcmp((*tok)->lexeme, (*macros_and_tpd_names)->macros[i]) == 0) {
            return init_identifier((*tok)->lexeme);
        }
    }

    return NULL;
}

// stops at any punct that does not belong to grammar
// @param fromTypeDef ->
struct Type* parseType(struct Token*** tokens, int* num_tokens, int* tokenIndex, bool fromTypeDef) {
    jumpOverNewLines(tokens, num_tokens, tokenIndex);
    struct Type* type = malloc(sizeof(struct Type));

    type->ttype = TYPE_BASE;

    // figure out what kind of type is there.
    struct TypeSpecList* type_spec_list = init_type_spec_list();
    struct BaseType* base_type = init_base_type(&type_spec_list);
    type->type.base_type = base_type;
    enum QualType_ qualtype = QualType_NONE;
    enum StorageClassSpecifier_ storage_spec_type = StorageClassSpecifier_NONE;
    enum TypeSpecifier_ type_spec = TypeSpecifier_NONE;

    storage_spec_type = isStorageClassSpecifier((*tokens)[*tokenIndex]);
    if(storage_spec_type) { (*tokenIndex)++; };
    qualtype = isQuatType((*tokens)[*tokenIndex]);
    if(qualtype) { (*tokenIndex)++; };
    type_spec = isTypeSpecifier((*tokens)[*tokenIndex]);
    if(type_spec) { (*tokenIndex)++; };
    if(qualtype || storage_spec_type || type_spec) {
        base_type->qualtype = qualtype;
        base_type->storage_spec = storage_spec_type;
        base_type->type_spec_list = type_spec_list;
        struct TypeSpecList* type_name_list_current = type_spec_list;
        char* name = NULL;
        add_type_spec_to_list(name, type_spec, &type_name_list_current);
        for(int* i=tokenIndex; isTypeSpecifier((*tokens)[*tokenIndex]) != TypeSpecifier_NONE; (*i)++) {
            type_spec = isTypeSpecifier((*tokens)[*tokenIndex]);
            add_type_spec_to_list(name, type_spec, &type_name_list_current);
        }

        if((*tokens)[*tokenIndex]->type == TOK_OP_MULTIPLY) {
            base_type->op = MemoryOperator_POINTER;
            while((*tokens)[*tokenIndex]->type == TOK_OP_MULTIPLY) {
                base_type->num_op += 1;
                (*tokenIndex)++;
            }
            // Handle pointer type
            base_type->pointer_type_qualifier = isQuatType((*tokens)[*tokenIndex]);
            if(base_type->pointer_type_qualifier) { (*tokenIndex)++; };
        }
        else if((*tokens)[*tokenIndex]->type == TOK_OP_AND) {
            base_type->op = MemoryOperator_REFERENCE;
            while((*tokens)[*tokenIndex]->type == TOK_OP_AND) {
                base_type->num_op += 1;
                (*tokenIndex)++;
            }
        }
        else if((*tokens)[*tokenIndex]->type == TOK_STRING) {
            //Identifier*id = isTypedefName();
            return type;
        }
        else if((*tokens)[*tokenIndex]->type == TOK_PUNCT_CLOSE_PAREN) {
            //isTypedefName();
            return type;
        }
        else if ((*tokens)[*tokenIndex]->type == TOK_PUNCT_OPEN_BRACKET) {
            // Handle array type
            struct ArrayType* array_type = malloc(sizeof(struct ArrayType));
            array_type->base_type = base_type;
            array_type->bracket_statement = NULL;
            array_type->initializer = NULL;  // Initialize to NULL, you can update this based on your requirements

            // Parse array size (if specified)
            (*tokenIndex)++;

            /*
            if ((*tokens)[*tokenIndex]->type == TOK_INTEGER) {
                array_type->size = atoi((*tokens)[*tokenIndex]->lexeme);
                (*tokenIndex)++;
            } else if ((*tokens)[*tokenIndex]->type == TOK_STRING) {
                // Check if the size is specified as a macro
                struct Identifier* macroIdentifier = isMacroName(&(*tokens)[*tokenIndex], NULL);
                if (macroIdentifier != NULL) {
                    // Handle macro-based size (you might want to replace this with your macro resolution logic)
                    //array_type->size = resolveMacro(macroIdentifier); // Replace with your macro resolution logic
                    (*tokenIndex)++;
                } else {
                    fprintf(stderr, "Error: Expected a constant integer or macro for array size\n");
                    // Handle error as needed
                }
                */

            array_type->bracket_statement = parseStatement(tokens, num_tokens, tokenIndex);

            // Parse the closing bracket
            if ((*tokens)[*tokenIndex]->type == TOK_PUNCT_CLOSE_BRACKET) {
                (*tokenIndex)++;
            } else {
                fprintf(stderr, "Error: Expected ']' in array type\n");
                // Handle error as needed
            }

            // Update the type to be an array type
            type->ttype = TYPE_ARRAY;
            type->type.array_type = array_type;
        }
        else {
            fprintf(stderr, "ERROR IN PARSING TYPE !!!");
        }
        return type;
    }

    free(type_spec_list);
    free(base_type);
    free(type);

    return NULL;
}

// jumped over (
struct For* parseFor(struct Token*** tokens, int* num_tokens, int* tokenIndex) {
    jumpOverNewLines(tokens, num_tokens, tokenIndex);
    if((*tokens)[(*tokenIndex)]->type == TOK_PUNCT_OPEN_PAREN) { (*tokenIndex)++; };

    struct For* for_stmt = malloc(sizeof(struct For));
    for_stmt->bcount = 0;
    for_stmt->body = NULL;

    for_stmt->initialization = parseStatement(tokens, num_tokens, tokenIndex);
    if((*tokens)[(*tokenIndex)]->type == TOK_PUNCT_SEMICOLON) { (*tokenIndex)++; }
    for_stmt->condition = parseStatement(tokens, num_tokens, tokenIndex);
    if((*tokens)[(*tokenIndex)]->type == TOK_PUNCT_SEMICOLON) { (*tokenIndex)++; }
    for_stmt->iteration = parseStatement(tokens, num_tokens, tokenIndex);
    if((*tokens)[(*tokenIndex)]->type == TOK_PUNCT_CLOSE_PAREN) { (*tokenIndex)++; }

    /* body */
    while((*tokens)[(*tokenIndex)]->type == TOK_PUNCT_OPEN_BRACE || (*tokens)[(*tokenIndex)]->type == TOK_PUNCT_NEWLINE) {
        (*tokenIndex)++;
    }


    while((*tokens)[(*tokenIndex)]->type != TOK_PUNCT_CLOSE_BRACE) {
        if((*tokens)[(*tokenIndex)]->type == TOK_PUNCT_NEWLINE) {
            (*tokenIndex)++;
            continue;
        }
        struct Statement* s = parseStatement(tokens, num_tokens, tokenIndex);

        for_stmt->body = realloc(for_stmt->body, sizeof(struct Statement*) * (for_stmt->bcount + 1));
        for_stmt->body[for_stmt->bcount] = s;
        for_stmt->bcount++;

        if(*tokenIndex >= *num_tokens) {
            break;
        }
    }

    (*tokenIndex)++; // jump over TOK_PUNCT_CLOSE_BRACE in while


    return for_stmt;
}

// jumped over (
struct While* parseWhile(struct Token*** tokens, int* num_tokens, int* tokenIndex) {
    jumpOverNewLines(tokens, num_tokens, tokenIndex);
    if((*tokens)[(*tokenIndex)]->type == TOK_PUNCT_OPEN_PAREN) { (*tokenIndex)++; };

    struct While* while_stmt = malloc(sizeof(struct While));
    while_stmt->bcount = 0;
    while_stmt->body = NULL;

    while_stmt->condition = parseStatement(tokens, num_tokens, tokenIndex);
    if((*tokens)[(*tokenIndex)]->type != TOK_PUNCT_CLOSE_PAREN) { (*tokenIndex)++; }
    /* body */
    while((*tokens)[(*tokenIndex)]->type == TOK_PUNCT_OPEN_BRACE || (*tokens)[(*tokenIndex)]->type == TOK_PUNCT_NEWLINE) {
        (*tokenIndex)++;
    }
    while((*tokens)[(*tokenIndex)]->type != TOK_PUNCT_CLOSE_BRACE) {
        if((*tokens)[(*tokenIndex)]->type == TOK_PUNCT_NEWLINE) {
            (*tokenIndex)++;
            continue;
        }
        struct Statement* s = parseStatement(tokens, num_tokens, tokenIndex);

        while_stmt->body = realloc(while_stmt->body, sizeof(struct Statement*) * (while_stmt->bcount + 1));
        while_stmt->body[while_stmt->bcount] = s;
        while_stmt->bcount++;

        if(*tokenIndex >= *num_tokens) {
            break;
        }
    }

    (*tokenIndex)++; // jump over TOK_PUNCT_CLOSE_BRACE in while


    return while_stmt;
}

struct StructOrUnion* parseStructOrUnion(struct Token*** tokens, int* num_tokens, int* tokenIndex, const bool fromtypeDef, const enum TokenType type /* struct or union  */) {
    struct StructOrUnion* s_or_u = malloc(sizeof(struct StructOrUnion));
    s_or_u->bcount = 0;
    s_or_u->body = NULL;

    jumpOverNewLines(tokens, num_tokens, tokenIndex);
    if((*tokens)[(*tokenIndex)]->type == TOK_STRING) {
        s_or_u->id = init_identifier((*tokens)[(*tokenIndex)]->lexeme);
        (*tokenIndex)++;
    }

    jumpOverNewLines(tokens, num_tokens, tokenIndex);
    if((*tokens)[(*tokenIndex)]->type == TOK_PUNCT_OPEN_BRACE) {
        (*tokenIndex)++;
    }


    if(type == TOK_KW_STRUCT) {
        s_or_u->s_or_u = StructOrUnion_Struct;
    }
    else if(type == TOK_KW_UNION) {
        s_or_u->s_or_u = StructOrUnion_Union;
    }

     while((*tokens)[(*tokenIndex)]->type != TOK_PUNCT_CLOSE_BRACE) {
        struct Statement* stmt = parseStatement(tokens, num_tokens, tokenIndex);
        s_or_u->body = realloc(s_or_u->body, sizeof(struct Statement*) * (s_or_u->bcount + 1));
        s_or_u->body[s_or_u->bcount] = stmt;
        s_or_u->bcount++;

        if(*tokenIndex >= *num_tokens) {
            break;
        }
    }
     (*tokenIndex)++;
     if((*tokens)[(*tokenIndex)]->type == TOK_PUNCT_SEMICOLON) {
         (*tokenIndex)++;
     }

    return s_or_u;
}

struct TypeDefName* parseTypeDef(struct Token*** tokens, int* num_tokens, int* tokenIndex) {
    jumpOverNewLines(tokens, num_tokens, tokenIndex);
    struct TypeDef* td = malloc(sizeof(struct TypeDef));

    if((*tokens)[(*tokenIndex)]->type == TOK_KW_STRUCT) {
        td->t_or_soru = Typedef_StructOrEnum;
        (*tokenIndex)++;
        struct StructOrUnion* s_or_u = parseStructOrUnion(tokens, num_tokens, tokenIndex, true, TOK_KW_STRUCT);

        jumpOverNewLines(tokens, num_tokens, tokenIndex);
        // TypeDef name ended with }:
        if((*tokens)[(*tokenIndex)]->type == TOK_STRING) {
            td->id = init_identifier((*tokens)[(*tokenIndex)]->lexeme);
            (*tokenIndex)++;
            jumpOverNewLines(tokens, num_tokens, tokenIndex);
            if((*tokens)[(*tokenIndex)]->type == TOK_PUNCT_SEMICOLON) {
                (*tokenIndex)++;
            }
        }
        td->typedef_type.structOrUnionType = s_or_u;
    }
    else if((*tokens)[(*tokenIndex)]->type == TOK_KW_UNION) {
        td->t_or_soru = Typedef_StructOrEnum;
        (*tokenIndex)++;
        struct StructOrUnion* s_or_u = parseStructOrUnion(tokens, num_tokens, tokenIndex, true, TOK_KW_UNION);

        jumpOverNewLines(tokens, num_tokens, tokenIndex);
        // TypeDef name ended with }:
        if((*tokens)[(*tokenIndex)]->type == TOK_STRING) {
            jumpOverNewLines(tokens, num_tokens, tokenIndex);
            td->id = init_identifier((*tokens)[(*tokenIndex)]->lexeme);
            (*tokenIndex)++;
            if((*tokens)[(*tokenIndex)]->type == TOK_PUNCT_SEMICOLON) {
                (*tokenIndex)++;
            }
        }
        td->typedef_type.structOrUnionType = s_or_u;
    }

    else {
        td->t_or_soru = Typedef_Typedef;
        struct Type* type = parseType(tokens, num_tokens, tokenIndex, true);
        if((*tokens)[(*tokenIndex)]->type == TOK_STRING) {
            // typedef name;
            td->id = init_identifier((*tokens)[(*tokenIndex)]->lexeme);
            (*tokenIndex)++;
            td->typedef_type.targetType = type;

            jumpOverNewLines(tokens, num_tokens, tokenIndex);
            if((*tokens)[(*tokenIndex)]->type == TOK_PUNCT_SEMICOLON) {
                (*tokenIndex)++;
            }
        }
    }

    return td;
}

struct SizeOf* parseSizeOf(struct Token*** tokens, int* num_tokens, int* tokenIndex) {
    jumpOverNewLines(tokens, num_tokens, tokenIndex);
    if((*tokens)[(*tokenIndex)]->type == TOK_PUNCT_OPEN_PAREN) { (*tokenIndex)++; };

    struct SizeOf* sz_of = malloc(sizeof(struct SizeOf));

    sz_of->type = parseType(tokens, num_tokens, tokenIndex, false);

    if((*tokens)[(*tokenIndex)]->type == TOK_PUNCT_CLOSE_PAREN) {
        (*tokenIndex)++;
    }
    if((*tokens)[(*tokenIndex)]->type == TOK_PUNCT_SEMICOLON) {
        (*tokenIndex)++;
    }


    return sz_of;
}

struct IfDef* parseIfDef(struct Token*** tokens, int* num_tokens, int* tokenIndex) {
    struct IfDef* ifdef = malloc(sizeof(struct IfDef));

    if((*tokens)[(*tokenIndex)+1]->type == TOK_STRING) {
        ifdef->id = init_identifier((*tokens)[(*tokenIndex)+1]->lexeme);
    }
    (*tokenIndex)++;

    while (1) {
        struct Statement* stmt = parseStatement(tokens, num_tokens, tokenIndex);
        if(stmt->stmt_type == Stmt_EndIf) {
            ((struct EndIf*)(stmt->stmt))->ifdef_or_ifndef = ifdef;
            return ifdef;
        }
        else {
            ifdef->body = realloc(ifdef->body, sizeof(struct Statement*) * (ifdef->bcount + 1));
            ifdef->body[ifdef->bcount] = stmt;
            ifdef->bcount++;
        }
        if(*tokenIndex >= *num_tokens) {
            break;
        }
    }

    return ifdef;
}

struct IfnDef* parseIfnDef(struct Token*** tokens, int* num_tokens, int* tokenIndex) {
    struct IfnDef* ifndef = malloc(sizeof(struct IfnDef));

    if((*tokens)[(*tokenIndex)+1]->type == TOK_STRING) {
        ifndef->id = init_identifier((*tokens)[(*tokenIndex)+1]->lexeme);
    }
    (*tokenIndex)++;

    while (1) {
        struct Statement* stmt = parseStatement(tokens, num_tokens, tokenIndex);
        if(stmt->stmt_type == Stmt_EndIf) {
            ((struct EndIf*)(stmt->stmt))->ifdef_or_ifndef = ifndef;
            return ifndef;
        }
        else {
            ifndef->body = realloc(ifndef->body, sizeof(struct Statement*) * (ifndef->bcount + 1));
            ifndef->body[ifndef->bcount] = stmt;
            ifndef->bcount++;
        }
        if(*tokenIndex >= *num_tokens) {
            break;
        }
    }

    return ifndef;
}

struct Define* parseDefine(struct Token*** tokens, int* num_tokens, int* tokenIndex) {
    struct Define* define = malloc(sizeof(struct Define));

    /* ID */
    if((*tokens)[(*tokenIndex)+1]->type == TOK_STRING) {
        define->id = init_identifier((*tokens)[(*tokenIndex)+1]->lexeme);
    }
    (*tokenIndex)++;
    /* VALUE */
    if(((*tokens)[(*tokenIndex)]->type == TOK_STRING || (*tokens)[(*tokenIndex)]->type == TOK_INTEGER) && (*tokens)[(*tokenIndex)+1]->type != TOK_PUNCT_BACKSLASH) {
        define->stmt = parseStatement(tokens, num_tokens, tokenIndex);
    }
    (*tokenIndex)++;

    return define;
}

// Function to parse #endif Statement
struct EndIf* parseEndIf(struct Token*** tokens, int* num_tokens, int* tokenIndex) {
    struct EndIf* endIf = malloc(sizeof(struct EndIf));
    // Parse the corresponding #ifdef
    endIf->ifdef_or_ifndef = NULL;

    return endIf;
}

struct Comment* parseComment(struct Token*** tokens, int* num_tokens, int* tokenIndex, const bool simpleComment /* // */ ) {
    struct Comment* cmnt = malloc(sizeof(Comment));
    char* comment = malloc(1000);
    if(simpleComment) {
        while((*tokenIndex) > (*num_tokens) && ((*tokens)[(*tokenIndex)+1]->type != TOK_PUNCT_NEWLINE)) {
            strcat(comment, (*tokens)[(*tokenIndex)]->lexeme);
        }
    }
    else {
        while((*tokenIndex) > (*num_tokens) && ((*tokens)[(*tokenIndex)+1]->type != TOK_OP_MULTIPLY && (*tokens)[(*tokenIndex)+2]->type == TOK_OP_DIVIDE)) {
            strcat(comment, (*tokens)[(*tokenIndex)]->lexeme);
        }
    }
    cmnt->comment = comment;
    printf("Comment: %s", comment);

    return cmnt;
}

struct ParamDecl** parseParams(struct Token*** tokens, int* num_tokens, int* tokenIndex, int* ParamDeclSize) {
    if((*tokens)[*tokenIndex]->type == TOK_PUNCT_CLOSE_PAREN) {
        *ParamDeclSize = 0;
        return NULL;
    }

    if((*tokens)[*tokenIndex]->type == TOK_PUNCT_OPEN_PAREN) {
        fprintf(stderr, "1 - this should not hit.");
    }

    // Initialize with a small capacity
    struct ParamDecl** parameters = malloc(*ParamDeclSize * sizeof(struct ParamDecl*));

    if(!parameters) {
        fprintf(stderr, "CANNOT ALLOCATE parameters !!!");
    }

    int paramIndex = 0;

    while ((*tokens)[*tokenIndex]->type != TOK_PUNCT_CLOSE_PAREN) {
        // Check if the array needs resizing
        if (paramIndex >= *ParamDeclSize) {
            // Double the capacity
            (*ParamDeclSize)++;
            parameters = realloc(parameters, *ParamDeclSize * sizeof(struct ParamDecl));

            if (!parameters) {
                fprintf(stderr, "Error: Memory reallocation failed\n");
                exit(EXIT_FAILURE);  // Handle the error as appropriate for your parser
            }
        }

        struct Type* type = parseType(tokens, num_tokens, tokenIndex, false);
        if(!type) {
            fprintf(stderr, "ERROR PARSING TYPE !!!");
        }

        parameters[*ParamDeclSize - 1] = malloc(sizeof(struct ParamDecl*));
        parameters[*ParamDeclSize - 1]->type = type;
        struct Token* tok = (*tokens)[*tokenIndex];
        if((*tokens)[*tokenIndex]->type == TOK_STRING) {
            struct Identifier* id = init_identifier((*tokens)[*tokenIndex]->lexeme);
            parameters[*ParamDeclSize - 1]->id = id;
            (*tokenIndex)++;
        }

        if((*tokens)[*tokenIndex]->type == TOK_PUNCT_COMMA ) {
            (*tokenIndex)++;
        }
        paramIndex++;
    }
    (*tokenIndex)++; /* TOK_PUNCT_CLOSE_PAREN */

    return parameters;
}

//struct s {int a;};
//typedef struct ss ss;

enum assignmentOperation_ isAssignmentOperation(struct Token*** tokens, int* num_tokens, int* tokenIndex) {
    // Helper function to check if the next token matches a given type

    // Check for various assignment operations
    switch((*tokens)[*tokenIndex]->type) {
        case TOK_OP_EQUALS:
            return EQ;
        case TOK_OP_PLUSEQUALS:
            return PLUS_EQ;
        case TOK_OP_MINUSEQUALS:
            return MINUS_EQ;
        case TOK_OP_MULEQUALS:
            return MULTIPLY_EQ;
        case TOK_OP_DIVEQUALS:
            return DIVIDE_EQ;
        case TOK_OP_MODULOEQUALS:
            return MODULO_EQ;
        case TOK_OP_SHIFT_LEFT:
            return LEFTSHIFT_EQ;
        case TOK_OP_SHIFT_RIGHT:
            return RIGHTSHIFT_EQ;
        case TOK_OP_AND:
            return AND_EQ;
        case TOK_OP_OR:
            return OR_EQ;
        case TOK_OP_XOR:
            return XOR_EQ;
        default:
            return assignmentOperation_NONE;
    }
}

enum UnaryOperation_ isUnaryOperation(struct Token*** tokens, int* num_tokens, int* tokenIndex) {
    // Helper function to check if the next token matches a given type

    // Check for various unary operations
    switch((*tokens)[*tokenIndex]->type) {
        case TOK_OP_PLUS:
            return ADD;
        case TOK_OP_MINUS:
            return SUB;
        case TOK_OP_MULTIPLY:
            return MUL;
        case TOK_OP_DIVIDE:
            return DIV;
        case TOK_OP_AND:
            return AND;
        case TOK_OP_NOT:
            return NOT;
        //case TOK_OP_TILDA:
        //    return TILDA;
        default:
            return UnaryOperation_NONE;
    }
}

enum CompareOperation_ isCompareOperation(struct Token*** tokens, int* num_tokens, int* tokenIndex) {
    // Helper function to check if the next token matches a given type

    // Check for various compare operations
    switch((*tokens)[*tokenIndex]->type) {
        case TOK_OP_BIGGER:
            return Gt;
        case TOK_OP_LOWER:
            return Lt;
        case TOK_OP_BIGGER_EQUALS:
            return GtE;
        case TOK_OP_LOWER_EQUALS:
            return LtE;
        case TOK_OP_EQUALSQUALS:
            return Eq;
        case TOK_OP_NOTEQUALS:
            return NotEq;
        default:
            return CompareOperation_NONE;
    }

    // If none of the above, return a default value (you may want to handle this differently based on your parser logic)
    return Gt; // Default to Gt, adjust as needed
}



enum ExpressionFlag_ {
    ExpressionFlag_NONE,
    ExpressionFlag_EnumElement,
    ExpressionFlag_SWITCH_CASE,

};

// TODO: consider fromEnum flag
struct Statement* parseExpression(struct Token*** tokens, int* num_tokens, int* tokenIndex, const enum ExpressionFlag_ expr_flag) {
    jumpOverNewLines(tokens, num_tokens, tokenIndex);
    struct Statement* stmt = malloc(sizeof(struct Statement));
    stmt->stmt_type = 0;
    stmt->stmt = NULL;

    if((*tokens)[*tokenIndex]->type == TOK_STRING) {
        struct Identifier* id = init_identifier((*tokens)[*tokenIndex]->lexeme);
        (*tokenIndex)++;
        // not a type:  varname; || varname = ...   ,;
        if((*tokens)[*tokenIndex]->type == TOK_PUNCT_SEMICOLON) {
            // varname;
            stmt->stmt = id;
            stmt->stmt_type = Stmt_Identifier;
        }
        else if((*tokens)[*tokenIndex]->type == TOK_PUNCT_OPEN_PAREN) {
            (*tokenIndex)++;
            // CALL EXPRESSION
            struct Call* call = malloc(sizeof(struct Call));
            call->arguments = NULL;
            call->acount = 0;
            call->id = id;
            while((*tokens)[*tokenIndex]->type != TOK_PUNCT_CLOSE_PAREN) {
                struct Statement* s = parseExpression(tokens, num_tokens, tokenIndex, ExpressionFlag_NONE);
                call->arguments = realloc(call->arguments, sizeof(struct Statement*) * (call->acount + 1));
                call->arguments[call->acount] = s;
                call->acount++;

                if(*tokenIndex >= *num_tokens) {
                    break;
                }
            }
            (*tokenIndex)++; // }
            if((*tokens)[*tokenIndex]->type == TOK_PUNCT_SEMICOLON) {
                (*tokenIndex)++; // ;
            }

            stmt->stmt = call;
            stmt->stmt_type = Stmt_Call;
        }
        else if((*tokens)[*tokenIndex]->type == TOK_PUNCT_OPEN_BRACKET) {
            // array access
            (*tokenIndex)++; // [
            struct Symbol* sym = malloc(sizeof(struct Symbol));
            sym->op = MemoryOperator_POINTER;
            sym->num_op = 1;
            sym->next = NULL;
            sym->id = id;
            struct ArrayAccess* arracc = malloc(sizeof(struct ArrayAccess));
            arracc->op = MemoryOperator_NONE;
            arracc->num_op = 0;
            arracc->id = sym;
            arracc->array_expr = parseExpression(tokens, num_tokens, tokenIndex, ExpressionFlag_NONE);
            //(*tokenIndex)++;  //  ] is soncumed by parseExpression()
        }
        else if((*tokens)[*tokenIndex]->type == TOK_PUNCT_COMMA) {
            if(expr_flag == ExpressionFlag_EnumElement) {

            }
            stmt->stmt_type = Stmt_Identifier;
            stmt->stmt = id;
            (*tokenIndex)++;
        }
        else if((*tokens)[*tokenIndex]->type == TOK_PUNCT_COLON) {
            stmt->stmt_type = Stmt_Identifier;
            stmt->stmt = id;
            if(expr_flag == ExpressionFlag_SWITCH_CASE) {}
            else {
                (*tokenIndex)++;
            }
        }
        // from end of call )
        else if((*tokens)[*tokenIndex]->type == TOK_PUNCT_CLOSE_PAREN) {
            stmt->stmt_type = Stmt_Identifier;
            stmt->stmt = id;
            if(expr_flag == ExpressionFlag_SWITCH_CASE) {}
            else {
                //(*tokenIndex)++;
            }
        }
        else if((*tokens)[*tokenIndex]->type == TOK_PUNCT_CLOSE_BRACE) {
            stmt->stmt_type = Stmt_Identifier;
            stmt->stmt = id;
            if(expr_flag == ExpressionFlag_SWITCH_CASE) {}
            else {
                //(*tokenIndex)++;
            }
        }
    }
    else if((*tokens)[*tokenIndex]->type == TOK_PUNCT_OPEN_PAREN) {

    }
    else if((*tokens)[*tokenIndex]->type == TOK_PUNCT_COMMA) {

    }
    else if((*tokens)[*tokenIndex]->type == TOK_OP_MULTIPLY) {
        struct Symbol* sym = malloc(sizeof(struct Symbol));
        sym->op = MemoryOperator_POINTER;
        sym->num_op = 1;
        sym->next = NULL;
        (*tokenIndex)++;
        if((*tokens)[*tokenIndex]->type == TOK_STRING) {
            struct Identifier* id = init_identifier((*tokens)[*tokenIndex]->lexeme);
            sym->id = id;
            if((*tokens)[*tokenIndex]->type == TOK_PUNCT_DOT) {
                sym->access_type = AccessType_DOT;
                (*tokenIndex)++;
                sym->next = parseExpression(tokens, num_tokens, tokenIndex, ExpressionFlag_NONE);
            }
            if((*tokens)[*tokenIndex]->type == TOK_OP_ARROW) {
                sym->access_type = AccessType_ARROW;
                (*tokenIndex)++;
                sym->next = parseExpression(tokens, num_tokens, tokenIndex, ExpressionFlag_NONE);
            }
            stmt->stmt_type = Stmt_Symbol;
            stmt->stmt = sym;
        }
        else {
            free(sym);
        }
    }
    else if((*tokens)[*tokenIndex]->type == TOK_OP_AND) {
        struct Symbol* sym = malloc(sizeof(struct Symbol));
        sym->op = MemoryOperator_REFERENCE;
        sym->num_op = 1;
        sym->next = NULL;
        (*tokenIndex)++;
        if((*tokens)[*tokenIndex]->type == TOK_STRING) {
            struct Identifier* id = init_identifier((*tokens)[*tokenIndex]->lexeme);
            sym->id = id;
            if((*tokens)[*tokenIndex]->type == TOK_PUNCT_DOT) {
                sym->access_type = AccessType_DOT;
                (*tokenIndex)++;
                sym->next = parseExpression(tokens, num_tokens, tokenIndex, ExpressionFlag_NONE);
            }
            if((*tokens)[*tokenIndex]->type == TOK_OP_ARROW) {
                sym->access_type = AccessType_ARROW;
                (*tokenIndex)++;
                sym->next = parseExpression(tokens, num_tokens, tokenIndex, ExpressionFlag_NONE);
            }
            stmt->stmt_type = Stmt_Symbol;
            stmt->stmt = sym;
        }
        else {
            free(sym);
        }
    }



    else if((*tokens)[*tokenIndex]->type == TOK_INTEGER) {
        struct Constant* c = malloc(sizeof(struct Constant));

        c->btype = BuiltinType_INT;
        int* intValue = malloc(sizeof(int));
        if(intValue != NULL) {
            *intValue = atoi((*tokens)[*tokenIndex]->lexeme);
            c->builtin_type = intValue;
        }
        else {
            fprintf(stderr, "Error: Unable to allocate memory for the string value\n");
            free(c);
        }
        (*tokenIndex)++;
        stmt->stmt_type = Stmt_Constant;
        stmt->stmt = c;
    }
    else if ((*tokens)[*tokenIndex]->type == TOK_PUNCT_DOUBLEQUOTE) {
        struct Constant* c = malloc(sizeof(struct Constant));

        c->btype = BuiltinType_STRING;

        // Initialize an empty string
        char* stringValue = strdup("");

        // Check if strdup succeeded
        if (stringValue != NULL) {
            // Iterate through tokens until the closing double quote is found
            (*tokenIndex)++;  // Move to the next token after the opening double quote
            while ((*tokens)[*tokenIndex]->type != TOK_PUNCT_DOUBLEQUOTE) {
                // Concatenate the lexeme of the current token to the string
                char* combinedString = malloc(strlen(stringValue) + strlen((*tokens)[*tokenIndex]->lexeme) + 1);
                strcpy(combinedString, stringValue);
                strcat(combinedString, (*tokens)[*tokenIndex]->lexeme);
                free(stringValue);
                stringValue = combinedString;

                (*tokenIndex)++;  // Move to the next token
            }

            // Move to the next token after the closing double quote
            (*tokenIndex)++;

            // Use the string value
            printf("String value: %s\n", stringValue);

            // Assign the final string to the struct
            c->builtin_type = stringValue;
        } else {
            fprintf(stderr, "Error: Unable to allocate memory for the string value\n");
            free(c);
        }
        (*tokenIndex)++;
        stmt->stmt_type = Stmt_Constant;
        stmt->stmt = c;
    }

    return stmt;
}

// TODO: test switch !
struct SwitchCase* parseSwitchCase(struct Token*** tokens, int* num_tokens, int* tokenIndex) {
    struct SwitchCase* swc = malloc(sizeof(struct SwitchCase));
    swc->bcount = 0;
    swc->body = NULL;

    swc->condition = parseExpression(tokens, num_tokens, tokenIndex, ExpressionFlag_SWITCH_CASE);

    if((*tokens)[*tokenIndex]->type == TOK_PUNCT_COLON) {
        (*tokenIndex)++;
    }

    while((*tokens)[*tokenIndex]->type != TOK_KW_DEFAULT) {
        struct Statement* s = parseStatement(tokens, num_tokens, tokenIndex);
        swc->body = realloc(swc->body, sizeof(struct Statement*) * (swc->bcount + 1));
        swc->body[swc->bcount] = s;
        swc->bcount++;

        if(*tokenIndex >= *num_tokens) {
            break;
        }
    }
    //(*tokenIndex)++; // }

    return swc;
}

struct SwitchDefault* parseSwitchDefault(struct Token*** tokens, int* num_tokens, int* tokenIndex) {
    struct SwitchDefault* swd = malloc(sizeof(struct SwitchDefault));
    swd->bcount = 0;

    if((*tokens)[*tokenIndex]->type == TOK_PUNCT_COLON) {
        (*tokenIndex)++;
    }

    while((*tokens)[*tokenIndex]->type != TOK_PUNCT_CLOSE_BRACE) {
        struct Statement* s = parseStatement(tokens, num_tokens, tokenIndex);
        swd->body = realloc(swd->body, sizeof(struct Statement*) * (swd->bcount + 1));
        swd->body[swd->bcount] = s;
        swd->bcount++;

        if(*tokenIndex >= *num_tokens) {
            break;
        }
    }
    (*tokenIndex)++; // }

    return swd;
}

struct Switch* parseSwitch(struct Token*** tokens, int* num_tokens, int* tokenIndex) {
    struct Switch* sw = malloc(sizeof(struct Switch));
    sw->bccount = 0;

    if((*tokens)[*tokenIndex]->type == TOK_PUNCT_OPEN_PAREN) {
        (*tokenIndex)++;
    }

    sw->condition = parseExpression(tokens, num_tokens, tokenIndex, ExpressionFlag_NONE);

    if((*tokens)[*tokenIndex]->type == TOK_PUNCT_CLOSE_PAREN) {
        (*tokenIndex)++;
    }

    /* body */
    if((*tokens)[*tokenIndex]->type == TOK_PUNCT_OPEN_BRACE) {
        (*tokenIndex)++;
    }
    while(1) {
        if((*tokens)[*tokenIndex]->type == TOK_KW_CASE) {
            (*tokenIndex)++; // jump [case]
            struct SwitchCase* s = parseSwitchCase(tokens, num_tokens, tokenIndex);
            sw->cases = realloc(sw->cases, sizeof(struct Statement*) * (sw->bccount + 1));
            sw->cases[sw->bccount] = s;
            sw->bccount++;
        }
        else if((*tokens)[*tokenIndex]->type == TOK_KW_DEFAULT) {
            (*tokenIndex)++; // default, parseSwitchDefault() jump over last }
            sw->default_case = parseSwitchDefault(tokens, num_tokens, tokenIndex);
            break;
        }
        else {
            fprintf(stderr, "ERROR IN SWITCH - should not be hut.");
        }

        if(*tokenIndex >= *num_tokens) {
            break;
        }
    }
    //(*tokenIndex)++; // }
    struct Token* tok = (*tokens)[*tokenIndex];


    return sw;
}

struct Break* parseBreak(struct Token*** tokens, int* num_tokens, int* tokenIndex) {
    struct Break* b = malloc(sizeof(struct Break));

    if((*tokens)[*tokenIndex]->type == TOK_PUNCT_SEMICOLON) {
        (*tokenIndex)++;
    }

    return b;
}

struct Continue* parseContinue(struct Token*** tokens, int* num_tokens, int* tokenIndex) {
    struct Continue* c = malloc(sizeof(struct Continue));

    if((*tokens)[*tokenIndex]->type == TOK_PUNCT_SEMICOLON) {
        (*tokenIndex)++;
    }

    return c;
}

// enum kw is jumped over.
struct Enum* parseEnum(struct Token*** tokens, int* num_tokens, int* tokenIndex) {
    jumpOverNewLines(tokens, num_tokens, tokenIndex);

    struct Enum* e = malloc(sizeof(struct Enum));

    if((*tokens)[*tokenIndex]->type == TOK_STRING) {
        e->id = init_identifier((*tokens)[*tokenIndex]->lexeme);
        (*tokenIndex)++;
    }

    /* body */
    struct Token* tok = (*tokens)[(*tokenIndex)];
    if((*tokens)[(*tokenIndex)]->type == TOK_PUNCT_OPEN_BRACE) {
        (*tokenIndex)++;
    }

    while((*tokens)[(*tokenIndex)]->type != TOK_PUNCT_CLOSE_BRACE) {
        if((*tokens)[(*tokenIndex)]->type == TOK_PUNCT_NEWLINE) {
            (*tokenIndex)++;
            continue;
        }
        struct Statement* s = parseExpression(tokens, num_tokens, tokenIndex, ExpressionFlag_EnumElement);
        // decideWhatToParse(tokens, num_tokens, tokenIndex);
        // parseStatement(tokens, num_tokens, tokenIndex);

        e->body = realloc(e->body, sizeof(struct Statement*) * (e->bcount + 1));
        e->body[e->bcount] = s;
        e->bcount++;

        if(*tokenIndex >= *num_tokens) {
            break;
        }
    }
    (*tokenIndex)++; // jump over TOK_PUNCT_CLOSE_BRACE in while
    if((*tokens)[(*tokenIndex)]->type == TOK_PUNCT_SEMICOLON) {
        (*tokenIndex)++;
    }

    return e;
}




struct Statement* decideWhatToParse(struct Token*** tokens, int* num_tokens, int* tokenIndex) {
    struct Statement* stmt = malloc(sizeof(struct Statement));
    stmt->stmt = NULL;
    stmt->stmt_type = 0;

    const enum FunctionSpecifier_ fs = isFunctionSpecifier((*tokens)[*tokenIndex]);

    if(fs) {
        // function for sure.
    }

    struct Type* type = parseType(tokens, num_tokens, tokenIndex, false);
    if(type) {
        Token* tok = (*tokens)[*tokenIndex + 1];
        // type parsed successfully, we can continue.
        if((*tokens)[*tokenIndex]->type == TOK_STRING) {
            tok = (*tokens)[*tokenIndex];
            if((*tokens)[*tokenIndex + 1]->type == TOK_PUNCT_SEMICOLON ||
                    (*tokens)[*tokenIndex + 1]->type == TOK_PUNCT_CLOSE_PAREN ||
                    (*tokens)[*tokenIndex + 1]->type == TOK_PUNCT_CLOSE_BRACE)
            {
                struct VarDecl* vardecl = malloc(sizeof(struct VarDecl));
                struct Identifier* id = init_identifier((*tokens)[*tokenIndex]->lexeme);
                vardecl->id = id;
                *tokenIndex += 2; /* varname, ; ) } */
                vardecl->type = type;

                stmt->stmt_type = Stmt_VarDecl;
                stmt->stmt = vardecl;
            }
            else if((*tokens)[*tokenIndex + 1]->type == TOK_OP_EQUALS) {
                struct VarDef* vardef = malloc(sizeof(struct VarDef));
                struct Identifier* id = init_identifier((*tokens)[*tokenIndex]->lexeme);
                vardef->id = id;
                (*tokenIndex)++;
                vardef->right_side = parseExpression(tokens, num_tokens, tokenIndex, ExpressionFlag_NONE);
            }
            else if((*tokens)[*tokenIndex + 1]->type == TOK_PUNCT_OPEN_PAREN) {
                // function def or decl without enum FunctionSpecifier_
                struct Identifier* id = init_identifier((*tokens)[*tokenIndex]->lexeme);
                (*tokenIndex)++; // function name
                (*tokenIndex)++; // (
                int ParamDeclSize = 1;
                tok = (*tokens)[*tokenIndex];
                struct ParamDecl** parameters = parseParams(tokens, num_tokens, tokenIndex, &ParamDeclSize);
                if((*tokens)[*tokenIndex]->type == TOK_PUNCT_SEMICOLON) {
                    struct FunctionDecl* fundecl = malloc(sizeof(struct FunctionDecl));
                    fundecl->functionspec = fs;
                    fundecl->id = id;
                    fundecl->parameters = parameters;
                    stmt->stmt_type = Stmt_FunctionDecl;
                    stmt->stmt = fundecl;
                    (*tokenIndex)++;
                }
                else if((*tokens)[*tokenIndex]->type == TOK_PUNCT_OPEN_BRACE) {
                    struct FunctionDef* fundef = malloc(sizeof(struct FunctionDef));
                    fundef->functionspec = fs;
                    fundef->id = id;
                    fundef->parameters = parameters;
                    (*tokenIndex)++;
                    while((*tokens)[*tokenIndex]->type != TOK_PUNCT_CLOSE_BRACE) {
                        (*tokenIndex)++; // jump [case]
                        struct Statement* s = parseStatement(tokens, num_tokens, tokenIndex);
                        fundef->body = realloc(fundef->body, sizeof(struct Statement*) * (fundef->bcount + 1));
                        fundef->body[fundef->bcount] = s;
                        fundef->bcount++;

                        if(*tokenIndex >= *num_tokens) {
                            break;
                        }
                    }
                    (*tokenIndex)++; // }
                    stmt->stmt_type = Stmt_FunctionDef;
                    stmt->stmt = fundef;

                }
                else {
                    free(id);
                    fprintf(stderr, "ERROR PARSING FUNCTIONDEF .");
                }
            }
        }
        else {
            fprintf(stderr, "NOT IMPLEMENTED YET, decideWhatToParse() !!!, %s ", (*tokens)[*tokenIndex]->lexeme);
        }
    }
    else {
        stmt = parseExpression(tokens, num_tokens, tokenIndex, ExpressionFlag_NONE);
    }

    return stmt; // Unable to decide what to parse
}

struct Statement* parseStatement(struct Token*** tokens, int* num_tokens, int* tokenIndex) {
    jumpOverNewLines(tokens, num_tokens, tokenIndex);

    const struct Token* current_token = (*tokens)[*tokenIndex];

    struct Statement* stmt = malloc(sizeof(Statement));
    stmt->stmt = NULL;
    stmt->stmt_type = 0;

    if(current_token->type == TOK_PUNCT_HASH) {
        // preprocessor stmt
        if(((*tokens)[(*tokenIndex)+1])->type == TOK_KW_INCLUDE) {
            (*tokenIndex) += 2;
            stmt->stmt = parseInclude(tokens, num_tokens, tokenIndex);
            stmt->stmt_type = Stmt_Include;
            return stmt;
        }
        else if(((*tokens)[(*tokenIndex)+1])->type == TOK_KW_IFDEF) {
            (*tokenIndex) += 2;
            stmt->stmt = parseIfDef(tokens, num_tokens, tokenIndex);
            stmt->stmt_type = Stmt_IfDef;
            return stmt;
        }
        else if(((*tokens)[(*tokenIndex)+1])->type == TOK_KW_IFNDEF) {
            (*tokenIndex) += 2;
            stmt->stmt = parseIfnDef(tokens, num_tokens, tokenIndex);
            stmt->stmt_type = Stmt_IfnDef;
            return stmt;
        }
        else if(((*tokens)[(*tokenIndex)+1])->type == TOK_KW_DEFINE) {
            (*tokenIndex) += 2;
            stmt->stmt = parseDefine(tokens, num_tokens, tokenIndex);
            stmt->stmt_type = Stmt_Define;
            return stmt;
        }
        else if(((*tokens)[(*tokenIndex)+1])->type == TOK_KW_ENDIF) {
            (*tokenIndex) += 2;
            stmt->stmt = parseEndIf(tokens, num_tokens, tokenIndex);
            stmt->stmt_type = Stmt_EndIf;
            return stmt;
        }
    }
    else if(current_token->type == TOK_KW_TYPEDEF) {
        (*tokenIndex)++;
        stmt->stmt = parseTypeDef(tokens, num_tokens, tokenIndex);
        stmt->stmt_type = Stmt_TypeDef;
    }
    else if(current_token->type == TOK_KW_IF) {
        (*tokenIndex)++;
        stmt = parseIf(tokens, num_tokens, tokenIndex);
    }
    else if(current_token->type == TOK_KW_FOR) {
        (*tokenIndex)++;
        stmt->stmt = parseFor(tokens, num_tokens, tokenIndex);
        stmt->stmt_type = Stmt_For;
    }
    else if(current_token->type == TOK_KW_WHILE) {
        (*tokenIndex)++;
        stmt->stmt = parseWhile(tokens, num_tokens, tokenIndex);
        stmt->stmt_type = Stmt_While;
    }
    else if(current_token->type == TOK_KW_ENUM) {
        (*tokenIndex)++;
        stmt->stmt = parseEnum(tokens, num_tokens, tokenIndex);
        stmt->stmt_type = Stmt_Enum;
    }
    else if(current_token->type == TOK_KW_STRUCT) {
        (*tokenIndex)++;
        stmt->stmt = parseStructOrUnion(tokens, num_tokens, tokenIndex, false, TOK_KW_STRUCT);
        stmt->stmt_type = Stmt_Struct;
    }
    else if(current_token->type == TOK_KW_UNION) {
        (*tokenIndex)++;
        stmt->stmt = parseStructOrUnion(tokens, num_tokens, tokenIndex, false, TOK_KW_UNION);
        stmt->stmt_type = Stmt_Union;
    }
    else if(current_token->type == TOK_KW_SIZEOF) {
        (*tokenIndex)++;
        // TODO: parse with only type flag.
        stmt->stmt = parseSizeOf(tokens, num_tokens, tokenIndex);
        stmt->stmt_type = Stmt_SizeOf;
    }
    else if(current_token->type == TOK_KW_SWITCH) {
        (*tokenIndex)++;
        // TODO: parse with only type flag.
        stmt->stmt = parseSwitch(tokens, num_tokens, tokenIndex);
        stmt->stmt_type = Stmt_Switch;
    }
    else if(current_token->type == TOK_KW_BREAK) {
        (*tokenIndex)++;
        // TODO: parse with only type flag.
        stmt->stmt = parseBreak(tokens, num_tokens, tokenIndex);
        stmt->stmt_type = Stmt_Break;
    }
    else if(current_token->type == TOK_KW_CONTINUE) {
        (*tokenIndex)++;
        // TODO: parse with only type flag.
        stmt->stmt = parseContinue(tokens, num_tokens, tokenIndex);
        stmt->stmt_type = Stmt_Continue;
    }
    else if(current_token->type == TOK_OP_DIVIDE) {
        if((*tokens)[(*tokenIndex)+1]->type == TOK_OP_DIVIDE) {
            parseComment(tokens, num_tokens, tokenIndex, true);
        }
        else if((*tokens)[(*tokenIndex)+1]->type == TOK_OP_MULTIPLY) {
            parseComment(tokens, num_tokens, tokenIndex, false);
        }
        (*tokenIndex) += 2;
    }
    // no keyword
    else {
        if(current_token->type == TOK_STRING || current_token->type == TOK_KW_CONST || current_token->type == TOK_KW_STATIC) {}
        // (*tokenIndex)++;
        stmt = decideWhatToParse(tokens, num_tokens, tokenIndex);
    }

    if(!stmt || !stmt->stmt) {
        fprintf(stderr, "if(!stmt || !stmt->stmt) error !!!");
        free(stmt);
    }

    return stmt;
}


struct TranslationUnit* parseTranslationUnit(struct Token*** tokens, int* num_tokens, struct MacrosAndTypeDefNames** macros_and_tdf_names) {
    struct TranslationUnit* TU = malloc(sizeof(struct TranslationUnit));
    TU->body = NULL; // Initialize body to NULL
    TU->count = 0;   // Initialize count to 0
    int tokenIndex = 0;
    while (1) {
        struct Statement* stmt = parseStatement(tokens, num_tokens, &tokenIndex);
        if(stmt != NULL && stmt->stmt != NULL) {
            TU->body = realloc(TU->body, sizeof(struct Statement*) * (TU->count + 1));
            TU->body[TU->count] = stmt;
            TU->count++;
        }
        else {
            fprintf(stderr, "parseTranslationUnit() smtm is NULL");
        }
        if(tokenIndex >= *num_tokens) {
            break;
        }
        /*
        const enum TokenType t = (*tokens)[tokenIndex]->type;
        if(t == TOK_PUNCT_SEMICOLON || t == TOK_PUNCT_CLOSE_PAREN) {
            Token* tok = (*tokens)[tokenIndex];
            tokenIndex++;
        }
        */
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
    int num_tokens = 100; // TODO: when small number realloc messing data, how to solve ?
    int tokenIndex = 0;

    struct Token** tokens = malloc(sizeof(struct Token*) * num_tokens);

    struct MacrosAndTypeDefNames* macros_and_tdf_names = malloc(sizeof(struct MacrosAndTypeDefNames));

    //char* content = readFile("/home/adam/Plocha/C_PARSER_INTERPRETER/C_P_I/main.c");
    //printf("%s", content);

    const char* content = "const signed int xxx; \nswitch(mycall(a,b)){case xxx: int a;break; default: int b; break;} sizeof(int); \nmycall(a,b,c,d,e); enum X {asdf, dasjf, adsfds,dsaf}; typedef struct XXX {char c;} XXX; typedef const int* XX; struct X {int a;}; const int f(const int** b, signed char& c);\n";
    //struct Token* tokens = lex(input);

    lex(content, &num_tokens, &tokenIndex, &tokens);

    // Process or print tokens as needed
    for (int i = 0; i < tokenIndex; i++) {
        Token *tok = tokens[i];
        //int x = 0;
        printf("Type: %d, Lexeme: %s\n", tokens[i]->type, tokens[i]->lexeme);
    }

    for (int i = 0; i < tokenIndex; ++i) {
        if(tokens[i]->lexeme == NULL) {
            fprintf(stderr, "NULL: %d \n", i);
        }
    }

    struct TranslationUnit* TU = parseTranslationUnit(&tokens, &tokenIndex, &macros_and_tdf_names);

    // Free allocated memory
    //for (int i = 0; tokens[i].type != 0; ++i) {
    //    free(tokens[i].lexeme);
    //}
    //free(content);

    for(int i=0; i< tokenIndex; i++) {
        free(tokens[i]->lexeme);
        //free(tokens);
    }

    free(tokens);

    printTranslationUnitJson(TU);

    //freeTranslationUnit(&TU);
    //free(TU);

    return 0;
}
