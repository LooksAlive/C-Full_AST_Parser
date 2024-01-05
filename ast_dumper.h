#ifndef AST_DUMPER_H
#define AST_DUMPER_H

#include <stdio.h>
#include <stdlib.h>


#include "main.h"
#include "generic_vector.h"

#include <json-c/json.h>


char* getTypeName(enum Type_ type) {
    switch (type) {
        case TYPE_BASE:
            return "TYPE_BASE";
        case TYPE_POINTER:
            return "TYPE_POINTER";
        case TYPE_ARRAY:
            return "TYPE_ARRAY";
        case TYPE_FUNCTION_PTR:
            return "TYPE_FUNCTION_PTR";
        default:
            return "UNKNOWN_TYPE";
    }
}

char* getMemoryOperatorName(enum MemoryOperator_ op) {
    switch (op) {
        case MemoryOperator_NONE:
            return "MemoryOperator_NONE";
        case MemoryOperator_POINTER:
            return "MemoryOperator_POINTER";
        case MemoryOperator_REFERENCE:
            return "MemoryOperator_REFERENCE";
        default:
            return "UNKNOWN_OPERATOR";
    }
}

char* getQualTypeName(enum QualType_ qual) {
    switch (qual) {
        case QualType_NONE:
            return "QualType_NONE";
        case QualType_CONST:
            return "QualType_CONST";
        case QualType_VOLATILE:
            return "QualType_VOLATILE";
        case QualType_RESTRICT:
            return "QualType_RESTRICT";
        case QualType_ATOMIC:
            return "QualType_ATOMIC";
        default:
            return "UNKNOWN_QUALIFIER";
    }
}

char* getFunctionSpecifierName(enum FunctionSpecifier_ specifier) {
    switch (specifier) {
        case FunctionSpecifier_NONE:
            return "FunctionSpecifier_NONE";
        case FunctionSpecifier_Inline:
            return "FunctionSpecifier_Inline";
        case FunctionSpecifier__inline__:
            return "FunctionSpecifier__inline__";
        case FunctionSpecifier_Noreturn:
            return "FunctionSpecifier_Noreturn";
        case FunctionSpecifier__stdcall:
            return "FunctionSpecifier__stdcall";
        case FunctionSpecifier_gccAttributeSpecifier:
            return "FunctionSpecifier_gccAttributeSpecifier";
        case FunctionSpecifier__declspec:
            return "FunctionSpecifier__declspec";
        default:
            return "UNKNOWN_FUNCTION_SPECIFIER";
    }
}

char* getStorageClassSpecifierName(enum StorageClassSpecifier_ specifier) {
    switch (specifier) {
        case StorageClassSpecifier_NONE:
            return "StorageClassSpecifier_NONE";
        case StorageClassSpecifier_Extern:
            return "StorageClassSpecifier_Extern";
        case StorageClassSpecifier_Auto:
            return "StorageClassSpecifier_Auto";
        case StorageClassSpecifier_Static:
            return "StorageClassSpecifier_Static";
        case StorageClassSpecifier_Register:
            return "StorageClassSpecifier_Register";
        case StorageClassSpecifier__Thread_local:
            return "StorageClassSpecifier__Thread_local";
        default:
            return "UNKNOWN_STORAGE_CLASS_SPECIFIER";
    }
}

char* getTypeSpecifierName(enum TypeSpecifier_ specifier) {
    switch (specifier) {
        case TypeSpecifier_NONE:
            return "TypeSpecifier_NONE";
        case TypeSpecifier_Void:
            return "TypeSpecifier_Void";
        case TypeSpecifier_Char:
            return "TypeSpecifier_Char";
        case TypeSpecifier_Short:
            return "TypeSpecifier_Short";
        case TypeSpecifier_Int:
            return "TypeSpecifier_Int";
        case TypeSpecifier_Long:
            return "TypeSpecifier_Long";
        case TypeSpecifier_Float:
            return "TypeSpecifier_Float";
        case TypeSpecifier_Double:
            return "TypeSpecifier_Double";
        case TypeSpecifier_Signed:
            return "TypeSpecifier_Signed";
        case TypeSpecifier_Unsigned:
            return "TypeSpecifier_Unsigned";
        case TypeSpecifier_Bool:
            return "TypeSpecifier_Bool";
        case TypeSpecifier_Complex:
            return "TypeSpecifier_Complex";
        case TypeSpecifier_M128:
            return "TypeSpecifier_M128";
        case TypeSpecifier_M128D:
            return "TypeSpecifier_M128D";
        case TypeSpecifier_M128I:
            return "TypeSpecifier_M128I";
        case TypeSpecifier_M128Extension:
            return "TypeSpecifier_M128Extension";
        case TypeSpecifier_Atomic:
            return "TypeSpecifier_Atomic";
        case TypeSpecifier_Struct:
            return "TypeSpecifier_Struct";
        case TypeSpecifier_Union:
            return "TypeSpecifier_Union";
        case TypeSpecifier_Enum:
            return "TypeSpecifier_Enum";
        case TypeSpecifier_TypedefName:
            return "TypeSpecifier_TypedefName";
        case TypeSpecifier_TypeofExtension:
            return "TypeSpecifier_TypeofExtension";
        default:
            return "UNKNOWN_TYPE_SPECIFIER";
    }
}

json_object* printVarDeclJson(struct VarDecl* varDecl);
json_object* printAssignJson(struct Assign* assign);
json_object* printStatementJson(struct Statement* stmt);

json_object* printParamDeclJson(struct ParamDecl* paramDecl) {
    if (!paramDecl) {
        return NULL;
    }

    // Create a JSON object for ParamDecl
    json_object* paramDeclObject = json_object_new_object();

    // Add symbol name
    json_object_object_add(paramDeclObject, "symbol_name", json_object_new_string(paramDecl->id->name));

    // Add type information
    json_object_object_add(paramDeclObject, "type", json_object_new_string("ParamDecl"));

    return paramDeclObject;
}

json_object* printFunctionDeclJson(struct FunctionDecl* funcDecl) {
    if (!funcDecl) {
        return NULL;
    }

    // Create a JSON object for FunctionDecl
    json_object* funcDeclObject = json_object_new_object();

    // Add type information
    json_object_object_add(funcDeclObject, "type", json_object_new_string("FunctionDecl"));

    // Add symbol name
    json_object_object_add(funcDeclObject, "symbol_name", json_object_new_string(funcDecl->id->name));

    // Add function specifier
    json_object_object_add(funcDeclObject, "function_specifier", json_object_new_int(funcDecl->functionspec));

    // Add parameters
    json_object* paramsArray = json_object_new_array();
    for (int i = 0; funcDecl->parameters && funcDecl->parameters[i]; ++i) {
        json_object* paramObject = printParamDeclJson(funcDecl->parameters[i]);
        if (paramObject) {
            json_object_array_add(paramsArray, paramObject);
        }
    }
    json_object_object_add(funcDeclObject, "parameters", paramsArray);

    return funcDeclObject;
}

json_object* printBreakJson(struct Break* breakStmt) {
    if (!breakStmt) {
        return NULL;
    }

    // Create a JSON object for Break
    json_object* breakObject = json_object_new_object();

    // Add type information
    json_object_object_add(breakObject, "type", json_object_new_string("Break"));

    return breakObject;
}

json_object* printContinueJson(struct Continue* continueStmt) {
    if (!continueStmt) {
        return NULL;
    }

    // Create a JSON object for Continue
    json_object* continueObject = json_object_new_object();

    // Add type information
    json_object_object_add(continueObject, "type", json_object_new_string("Continue"));

    return continueObject;
}





json_object* printSwitchCaseJson(struct SwitchCase* switchCase) {
    if (!switchCase) {
        return NULL;
    }

    // Create a JSON object for SwitchCase
    json_object* switchCaseObject = json_object_new_object();

    // Add type information
    json_object_object_add(switchCaseObject, "type", json_object_new_string("SwitchCase"));

    // Add condition
    json_object_object_add(switchCaseObject, "condition", printStatementJson(switchCase->condition)); // You may need to update this based on your requirements

    // Add body
    json_object* bodyArray = json_object_new_array();
    for (int i = 0; switchCase->body && switchCase->body[i]; ++i) {
        json_object* statementObject = printStatementJson(switchCase->body[i]);
        if (statementObject) {
            json_object_array_add(bodyArray, statementObject);
        }
    }
    json_object_object_add(switchCaseObject, "body", bodyArray);

    return switchCaseObject;
}

json_object* printSwitchDefaultJson(struct SwitchDefault* switchDefault) {
    if (!switchDefault) {
        return NULL;
    }

    // Create a JSON object for SwitchDefault
    json_object* switchDefaultObject = json_object_new_object();

    // Add type information
    json_object_object_add(switchDefaultObject, "type", json_object_new_string("SwitchDefault"));

    // Add body
    json_object* bodyArray = json_object_new_array();
    for (int i = 0; switchDefault->body && switchDefault->body[i]; ++i) {
        json_object* statementObject = printStatementJson(switchDefault->body[i]);
        if (statementObject) {
            json_object_array_add(bodyArray, statementObject);
        }
    }
    json_object_object_add(switchDefaultObject, "body", bodyArray);

    return switchDefaultObject;
}

json_object* printSwitchJson(struct Switch* switchStmt) {
    if (!switchStmt) {
        return NULL;
    }

    // Create a JSON object for Switch
    json_object* switchObject = json_object_new_object();

    // Add type information
    json_object_object_add(switchObject, "type", json_object_new_string("Switch"));

    // Add condition
    json_object_object_add(switchObject, "condition", printStatementJson(switchStmt->condition)); // You may need to update this based on your requirements

    // Add cases
    json_object* casesArray = json_object_new_array();
    for (int i = 0; switchStmt->cases && switchStmt->cases[i]; ++i) {
        json_object* switchCaseObject = printSwitchCaseJson(switchStmt->cases[i]);
        if (switchCaseObject) {
            json_object_array_add(casesArray, switchCaseObject);
        }
    }
    json_object_object_add(switchObject, "cases", casesArray);

    // Add default case
    if (switchStmt->default_case) {
        json_object* switchDefaultObject = printSwitchDefaultJson(switchStmt->default_case);
        if (switchDefaultObject) {
            json_object_object_add(switchObject, "default_case", switchDefaultObject);
        }
    }

    return switchObject;
}

json_object* printStructOrUnionJson(struct StructOrUnion* structOrUnion) {
    if (!structOrUnion) {
        return NULL;
    }

    // Create a JSON object for StructOrUnion
    json_object* structOrUnionObject = json_object_new_object();

    // Add type information
    json_object_object_add(structOrUnionObject, "type", json_object_new_string("StructOrUnion"));

    // Add symbol name
    json_object_object_add(structOrUnionObject, "symbol_name", json_object_new_string(structOrUnion->id->name));

    // Add struct or union information
    json_object_object_add(structOrUnionObject, "s_or_u", json_object_new_int(structOrUnion->s_or_u));

    // Add body
    json_object* bodyArray = json_object_new_array();
    for (int i = 0; structOrUnion->body && structOrUnion->body[i]; ++i) {
        json_object* statementObject = printStatementJson(structOrUnion->body[i]);
        if (statementObject) {
            json_object_array_add(bodyArray, statementObject);
        }
    }
    json_object_object_add(structOrUnionObject, "body", bodyArray);

    return structOrUnionObject;
}


json_object* printBaseTypeJson(struct BaseType* baseType) {
    if (!baseType) {
        return NULL;
    }

    // Create a JSON object for BaseType
    json_object* baseTypeObject = json_object_new_object();

    // Add type specifier list
    json_object* typeSpecListArray = json_object_new_array();
    struct TypeSpecList* typeSpecListNode = baseType->type_spec_list;

    while (typeSpecListNode) {
        json_object* typeSpecObject = json_object_new_object();
        json_object_object_add(typeSpecObject, "type_spec", json_object_new_string(getTypeSpecifierName(typeSpecListNode->type_spec)));

        // If it's a typedef name, add its name
        if (typeSpecListNode->type_spec == TypeSpecifier_TypedefName) {
            json_object_object_add(typeSpecObject, "typedef_name", json_object_new_string(typeSpecListNode->id->name));
        }

        json_object_array_add(typeSpecListArray, typeSpecObject);

        typeSpecListNode = typeSpecListNode->next;
    }

    json_object_object_add(baseTypeObject, "type_spec_list", typeSpecListArray);

    // Add other base type information
    json_object_object_add(baseTypeObject, "qualtype", json_object_new_string(getQualTypeName(baseType->qualtype)));
    json_object_object_add(baseTypeObject, "storage_spec", json_object_new_string(getStorageClassSpecifierName(baseType->storage_spec)));
    json_object_object_add(baseTypeObject, "op", json_object_new_string(getMemoryOperatorName(baseType->op)));
    json_object_object_add(baseTypeObject, "num_op", json_object_new_int(baseType->num_op));
    json_object_object_add(baseTypeObject, "pointer_type_qualifier", json_object_new_string(getQualTypeName(baseType->pointer_type_qualifier)));

    return baseTypeObject;
}

json_object* printTypeJson(struct Type* type) {
    // Handle BaseType
    json_object* typeObject = json_object_new_object();
    if (type->ttype == TYPE_BASE) {
        json_object* baseTypeObject = printBaseTypeJson(type->type.base_type);
        if (baseTypeObject) {
            json_object_object_add(typeObject, "base_type", baseTypeObject);
        }
    }

    return typeObject;
}

json_object* printSizeOfJson(struct SizeOf* sizeofStmt) {
    if (!sizeofStmt) {
        return NULL;
    }

    // Create a JSON object for SizeOf
    json_object* sizeofObject = json_object_new_object();

    // Add type information
    json_object_object_add(sizeofObject, "type", json_object_new_string("SizeOf"));

    // Add sizeof type information
    json_object_object_add(sizeofObject, "sizeof_type", printTypeJson(sizeofStmt->type));

    return sizeofObject;
}

json_object* printForJson(struct For* forStmt) {
    if (!forStmt) {
        return NULL;
    }

    // Create a JSON object for For
    json_object* forObject = json_object_new_object();

    // Add type information
    json_object_object_add(forObject, "type", json_object_new_string("For"));

    // Add initialization
    json_object_object_add(forObject, "initialization", json_object_new_string("")); // You may need to update this based on your requirements

    // Add condition
    json_object_object_add(forObject, "condition", json_object_new_string("")); // You may need to update this based on your requirements

    // Add iteration
    json_object_object_add(forObject, "iteration", json_object_new_string("")); // You may need to update this based on your requirements

    // Add body
    json_object_object_add(forObject, "body", json_object_new_array());
    json_object* bodyArray = json_object_object_get(forObject, "body");
    for (int i = 0; forStmt->body && forStmt->body[i]; ++i) {
        json_object* statementObject = printStatementJson(forStmt->body[i]);
        if (statementObject) {
            json_object_array_add(bodyArray, statementObject);
        }
    }

    return forObject;
}

json_object* printWhileJson(struct While* whileStmt) {
    if (!whileStmt) {
        return NULL;
    }

    // Create a JSON object for While
    json_object* whileObject = json_object_new_object();

    // Add type information
    json_object_object_add(whileObject, "type", json_object_new_string("While"));

    // Add condition
    json_object_object_add(whileObject, "condition", json_object_new_string("")); // You may need to update this based on your requirements

    // Add body
    json_object_object_add(whileObject, "body", json_object_new_array());
    json_object* bodyArray = json_object_object_get(whileObject, "body");
    for (int i = 0; whileStmt->body && whileStmt->body[i]; ++i) {
        json_object* statementObject = printStatementJson(whileStmt->body[i]);
        if (statementObject) {
            json_object_array_add(bodyArray, statementObject);
        }
    }

    return whileObject;
}

json_object* printIncludeJson(struct Include* includeStmt) {
    if (!includeStmt) {
        return NULL;
    }

    // Create a JSON object for Include
    json_object* includeObject = json_object_new_object();

    // Add type information
    json_object_object_add(includeObject, "type", json_object_new_string("Include"));

    // Add relative included path
    json_object_object_add(includeObject, "relative_included_path", json_object_new_string(includeStmt->relative_included_path));

    // Add absolute path
    json_object_object_add(includeObject, "absolute_path", json_object_new_string(includeStmt->absolute_path));

    // Add include type
    json_object_object_add(includeObject, "include_type", json_object_new_int(includeStmt->include_type));

    return includeObject;
}

json_object* printVarDeclJson(struct VarDecl* varDecl) {
    // Create a JSON object for VarDecl
    json_object* varDeclObject = json_object_new_object();

    // Add type information
    json_object_object_add(varDeclObject, "type", json_object_new_string("VarDecl"));

    // Add symbol name
    json_object_object_add(varDeclObject, "symbol_name", json_object_new_string(varDecl->id->name));

    // Add base type information
    if (varDecl->type && varDecl->type->ttype == TYPE_BASE) {
        json_object_object_add(varDeclObject, "type", printTypeJson(varDecl->type));
    }

    return varDeclObject;
}

json_object* printAssignJson(struct Assign* assign) {
    json_object* assignObject = json_object_new_object();
    json_object_object_add(assignObject, "type", json_object_new_string("Assign"));

    // Print left side
    json_object_object_add(assignObject, "left", json_object_new_object());
    if (assign->left) {
        json_object* leftObject = printStatementJson(assign->left + 2);
        if (leftObject) {
            json_object_object_add(assignObject, "left", leftObject);
        }
    }

    // Print right side
    json_object_object_add(assignObject, "right", json_object_new_object());
    if (assign->right) {
        json_object* rightObject = printStatementJson(assign->right + 2);
        if (rightObject) {
            json_object_object_add(assignObject, "right", rightObject);
        }
    }

    return assignObject;
}

json_object* printIdentifierJson(struct Identifier* identifier) {
    if (!identifier) {
        return NULL;
    }

    // Create a JSON object for Identifier
    json_object* identifierObject = json_object_new_object();

    // Add type information
    json_object_object_add(identifierObject, "type", json_object_new_string("Identifier"));

    // Add name
    json_object_object_add(identifierObject, "name", json_object_new_string(identifier->name));

    return identifierObject;
}

const char* getAccessType(enum AccessType_ accessType) {
    switch (accessType) {
        case AccessType_DOT:
            return "DOT";
        case AccessType_ARROW:
            return "ARROW";
        default:
            return "UNKNOWN_ACCESS_TYPE";
    }
}

json_object* printSymbolJson(struct Symbol* symbol) {
    if (!symbol) {
        return NULL;
    }

    // Create a JSON object for Symbol
    json_object* symbolObject = json_object_new_object();

    // Add type information
    json_object_object_add(symbolObject, "type", json_object_new_string("Symbol"));

    // Add identifier
    json_object_object_add(symbolObject, "identifier", json_object_new_object());
    if (symbol->id) {
        json_object* identifierObject = printIdentifierJson(symbol->id);
        if (identifierObject) {
            json_object_object_add(symbolObject, "identifier", identifierObject);
        }
    }

    // Add memory operator information
    json_object_object_add(symbolObject, "memory_operator", json_object_new_string(getMemoryOperatorName(symbol->op)));
    json_object_object_add(symbolObject, "num_op", json_object_new_int(symbol->num_op));

    // Add access type information
    json_object_object_add(symbolObject, "access_type", json_object_new_string(getAccessType(symbol->access_type)));

    // Add next statement
    json_object_object_add(symbolObject, "next", json_object_new_object());
    if (symbol->next) {
        json_object* nextObject = printStatementJson(symbol->next);
        if (nextObject) {
            json_object_object_add(symbolObject, "next", nextObject);
        }
    }

    return symbolObject;
}

// Function to print Call information to a JSON object
json_object* printCallJson(struct Call* call) {
    if (!call) {
        return NULL;
    }

    // Create a JSON object for Call
    json_object* callObject = json_object_new_object();

    // Add type information
    json_object_object_add(callObject, "type", json_object_new_string("Call"));

    // Add identifier
    if (call->id) {
        json_object_object_add(callObject, "identifier", json_object_new_string(call->id->name));
    }

    // Add arguments
    json_object* argumentsArray = json_object_new_array();
    for (int i = 0; i < call->acount; i++) {
        json_object* callObject = printStatementJson(call->arguments[i]);
        json_object_array_add(argumentsArray, callObject);
    }
    json_object_object_add(callObject, "arguments", argumentsArray);

    return callObject;
}

// Function to print Enum information to a JSON object
json_object* printEnumJson(struct Enum* enumStmt) {
    if (!enumStmt) {
        return NULL;
    }

    // Create a JSON object for Enum
    json_object* enumObject = json_object_new_object();

    // Add type information
    json_object_object_add(enumObject, "type", json_object_new_string("Enum"));

    // Add identifier
    if (enumStmt->id) {
        json_object_object_add(enumObject, "identifier", json_object_new_string(enumStmt->id->name));
    }

    // Add body
    json_object* bodyArray = json_object_new_array();
    for (int i = 0; enumStmt->body && enumStmt->body[i]; ++i) {
        json_object* enumElement = printStatementJson(enumStmt->body[i]);
        json_object_array_add(bodyArray, enumElement);
    }
    json_object_object_add(enumObject, "body", bodyArray);

    return enumObject;
}

json_object* printTypedefJson(struct TypeDef* typedefStmt) {
    if (!typedefStmt) {
        return NULL;
    }

    // Create a JSON object for Typedef
    json_object* typedefObject = json_object_new_object();

    // Add type information
    json_object_object_add(typedefObject, "type", json_object_new_string("Typedef"));

    // Add symbol name
    if (typedefStmt->id) {
        json_object_object_add(typedefObject, "id", printStatementJson(typedefStmt->id));
    }

    // Add typedef or struct/union information
    json_object_object_add(typedefObject, "t_or_soru", json_object_new_int(typedefStmt->t_or_soru));

    // Add typedef type information
    if (typedefStmt->t_or_soru == Typedef_Typedef) {
        json_object_object_add(typedefObject, "typedef_type", printTypeJson(typedefStmt->typedef_type.targetType));
    } else if (typedefStmt->t_or_soru == Typedef_StructOrEnum) {
        json_object_object_add(typedefObject, "struct_or_enum", printStructOrUnionJson(typedefStmt->typedef_type.structOrUnionType));
    }

    return typedefObject;
}


// Modify the function definition
json_object* printStatementJson(struct Statement* stmt) {
    if (!stmt || !stmt->stmt_type || !stmt->stmt) {
        return NULL;
    }

    json_object* result = NULL;

    switch (stmt->stmt_type) {
        case Stmt_Identifier:
            result = printIdentifierJson(stmt->stmt);
            break;
        case Stmt_Switch:
            result = printSwitchJson(stmt->stmt);
            break;
        case Stmt_Case:
            result = printSwitchCaseJson(stmt->stmt);
            break;
        case Stmt_Default:
            result = printSwitchDefaultJson(stmt->stmt);
            break;
        case Stmt_Struct:
            result = printStructOrUnionJson(stmt->stmt);
            break;
        case Stmt_Symbol:
            result = printSymbolJson(stmt->stmt);
            break;
        case Stmt_Call:
            result = printCallJson(stmt->stmt);
            break;
        case Stmt_For:
            result = printForJson(stmt->stmt);
            break;
        case Stmt_If:
            // Handle Stmt_If
            break;
        case Stmt_ElseIf:
            // Handle Stmt_ElseIf
            break;
        case Stmt_Else:
            // Handle Stmt_Else
            break;
        case Stmt_While:
            result = printWhileJson(stmt->stmt);
            break;
        case Stmt_Do_While:
            // Handle Stmt_Do_While
            break;
        case Stmt_Enum:
            result = printEnumJson(stmt->stmt);
            break;
        case Stmt_SizeOf:
            result = printSizeOfJson(stmt->stmt);
            break;
        case Stmt_TypeDef:
            result = printTypedefJson(stmt->stmt);
            break;
        case Stmt_FunctionDecl:
            // Handle Stmt_FunctionDecl
            break;
        case Stmt_FunctionDef:
            // Handle Stmt_FunctionDef
        case Stmt_Break:
            result = printBreakJson(stmt->stmt);
            break;
        case Stmt_Include:
            result = printIncludeJson(stmt->stmt);
            break;
        case Stmt_FunctionDeclaration:
            result = printFunctionDeclJson(stmt->stmt);
            break;
        case Stmt_VarDecl:
            result = printVarDeclJson(stmt->stmt);
            break;
        case Stmt_Assign:
            result = printAssignJson(stmt->stmt);
            break;
        // Add more cases for other statement types as needed
        default:
            fprintf(stderr, "Error: Unknown statement type\n %d", stmt->stmt_type);
            // Handle error as needed
            break;
    }

    return result;
}

void printTranslationUnitJson(const struct TranslationUnit* translationUnit) {
    if (!translationUnit) {
        return;
    }

    json_object* jsonObject = json_object_new_object();
    json_object_object_add(jsonObject, "type", json_object_new_string("TranslationUnit"));
    json_object_object_add(jsonObject, "body", json_object_new_array());

    for (unsigned i = 0; i < translationUnit->count; ++i) {
        json_object* stmtObject = printStatementJson(translationUnit->body[i]);
        json_object_array_add(json_object_object_get(jsonObject, "body"), stmtObject);
    }

    printf("%s\n", json_object_to_json_string_ext(jsonObject, JSON_C_TO_STRING_PRETTY));
}

#endif // AST_DUMPER_H
