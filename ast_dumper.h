#ifndef AST_DUMPER_H
#define AST_DUMPER_H

#include <stdio.h>
#include <stdlib.h>


#include "main.h"
#include "generic_vector.h"



// Struct to store the JSON result
typedef struct JsonResult {
    char* data;
    size_t capacity;
    size_t length;
} JsonResult;

void printStatementJson(JsonResult* result, struct Statement* stmt, int indent);

// Initialize the JSON result
void initJsonResult(JsonResult* result) {
    result->data = malloc(sizeof(char) * 1000);
    //result->data[result->capacity] = '\0';
    result->capacity = 1000;
    result->length = 0;
}

// Append a string to the JSON result
void appendJsonString(JsonResult* result, const char* str) {
    size_t strLen = strlen(str);

    // Ensure enough capacity
    if (result->length + strLen + 1 > result->capacity) {
        result->capacity = result->length + strLen + 1;
        result->data = realloc(result->data, result->capacity);
    }

    // Append the string
    strcat(result->data, str);
    result->length += strLen;
}

// Append an indented string to the JSON result
void appendIndentedJsonString(JsonResult* result, const char* key, const char* value, int indent) {
    char indentedStr[256];
    snprintf(indentedStr, sizeof(indentedStr), "\"%s\": \"%s\",\n", key, value);

    // Add indentation
    char indents[256];
    snprintf(indents, sizeof(indents), "%*s", indent * 2, " ");

    // Append the indented string
    appendJsonString(result, indents);
    appendJsonString(result, indentedStr);
}

// Append an indented integer to the JSON result
void appendIndentedJsonInt(JsonResult* result, const char* key, int value, int indent) {
    char indentedStr[256];
    snprintf(indentedStr, sizeof(indentedStr), "\"%s\": %d,\n", key, value);

    // Add indentation
    char indents[256];
    snprintf(indents, sizeof(indents), "%*s", indent * 2, " ");

    // Append the indented string
    appendJsonString(result, indents);
    appendJsonString(result, indentedStr);
}

// Start an indented JSON object
void startIndentedJsonObject(JsonResult* result, int indent) {
    char indents[256];
    snprintf(indents, sizeof(indents), "%*s{\n", indent * 2, " ");

    // Append the indented string
    appendJsonString(result, indents);
}

// End an indented JSON object
void endIndentedJsonObject(JsonResult* result, int indent) {
    char indents[256];
    snprintf(indents, sizeof(indents), "%*s},\n", indent * 2, " ");

    // Append the indented string
    appendJsonString(result, indents);
}

// Start an indented JSON array
void startIndentedJsonArray(JsonResult* result, int indent) {
    char indents[256];
    snprintf(indents, sizeof(indents), "%*s[\n", indent * 2, " ");

    // Append the indented string
    appendJsonString(result, indents);
}

// End an indented JSON array
void endIndentedJsonArray(JsonResult* result, int indent) {
    char indents[256];
    snprintf(indents, sizeof(indents), "%*s],\n", indent * 2, " ");

    // Append the indented string
    appendJsonString(result, indents);
}

// Print the JSON result
void printJsonResult(const JsonResult* result) {
    if (result->data) {
        printf("%s", result->data);
    }
}

// Free the memory used by the JSON result
void freeJsonResult(JsonResult* result) {
    free(result->data);
    result->data = NULL;
    result->capacity = 0;
    result->length = 0;
}

void printVarDeclJson(JsonResult* result, struct VarDecl* varDecl, int indent) {
    startIndentedJsonObject(result, indent);
    appendIndentedJsonString(result, "type", "VarDecl", indent + 1);
    appendIndentedJsonString(result, "symbol_name", varDecl->id, indent + 1);
    //appendIndentedJsonString(result, "type_name", varDecl->type->type_name, indent + 1);
    endIndentedJsonObject(result, indent);
}

void printAssignJson(JsonResult* result, struct Assign* assign, int indent) {
    startIndentedJsonObject(result, indent);
    appendIndentedJsonString(result, "type", "Assign", indent + 1);

    // Print left side
    appendIndentedJsonString(result, "left", "", indent + 1);
    if(assign->left) {
        printStatementJson(result, assign->left, indent + 2);
    }

    // Print right side
    appendIndentedJsonString(result, "right", "", indent + 1);
    if(assign->right) {
        printStatementJson(result, assign->right, indent + 2);
    }

    endIndentedJsonObject(result, indent);
}

void printStatementJson(JsonResult* result, struct Statement* stmt, int indent) {
    if (!stmt || !stmt->stmt_type || !stmt->stmt) {
        return;
    }

    const int* const* s;

    switch (stmt->stmt_type) {
        case Stmt_VarDecl:
            printVarDeclJson(result, (struct VarDecl*)stmt->stmt, indent);
            break;
        case Stmt_Assign:
            printAssignJson(result, (struct Assign*)stmt->stmt, indent);
            break;
        // Add more cases for other statement types as needed
    }
}

void printTranslationUnitJson(struct TranslationUnit* translationUnit, int indent) {
    if (!translationUnit) {
        return;
    }

    JsonResult result;
    initJsonResult(&result);

    startIndentedJsonObject(&result, indent);
    appendIndentedJsonString(&result, "type", "TranslationUnit", indent + 1);
    startIndentedJsonArray(&result, indent + 1);

    for (unsigned i = 0; i < translationUnit->count; ++i) {
        printStatementJson(&result, translationUnit->body[i], indent + 2);
    }

    endIndentedJsonArray(&result, indent + 1);
    endIndentedJsonObject(&result, indent);


    printJsonResult(&result);
    freeJsonResult(&result);
}






#endif // AST_DUMPER_H
