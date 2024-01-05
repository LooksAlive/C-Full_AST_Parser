#ifndef MAIN_H
#define MAIN_H

// void* data in every node
// void* data can be used for example for references and more data associating with given node by user needs
// e.g. strcut UserData { Statement** refs; }; -> for Statement, according to type of Node we can set more or do it different way
//#define user_node_void_data_ptr




// by default positions are not recorded in nodes
//#define node_record_positions


#include <stdbool.h>
#include <assert.h>

//#include <json-c/json.h>
#include "lexer.h"

enum EncodingPrefix_ {
    u8,
    u,
    U,
    L
};

enum CompareOperation_ {
    CompareOperation_NONE,
    Gt,
    Lt,
    GtE,
    LtE,
    Eq,
    NotEq
};

enum assignmentOperation_ {
    assignmentOperation_NONE,
    EQ,
    MULTIPLY_EQ,
    DIVIDE_EQ,
    MODULO_EQ,
    PLUS_EQ,
    MINUS_EQ,
    LEFTSHIFT_EQ,
    RIGHTSHIFT_EQ,
    AND_EQ,
    OR_EQ,
    XOR_EQ // ^=
};

enum UnaryOperation_ {
    UnaryOperation_NONE,
    ADD,
    SUB,
    MUL,
    DIV,
    AND,    // &
    NOT,    // !
    TILDA   // ~     AltGr + A

};

enum AccessType_ {
    AccessType_DOT,
    AccessType_ARROW
};

struct Position {
    int line;
    int col;
    int offset;
} Position;

struct Macro {
    struct Identifier* macro_id;
    struct Token* tokens;
} Macro;

struct ExpandedMacro {
    struct Macro* macro;
    struct Statement* stmt;
} ExpandedMacro;



// required for coorect determination of symbols
// char* are enough -> from pre-processing, we still end up with just single TU.
struct MacrosAndTypeDefNames {
    char** macros;
    unsigned macros_size;
    unsigned macros_capacity;

    char** typedefnames;
    unsigned typedefnames_size;
    unsigned typedefnames_capacity;
} MacrosAndTypeDefNames;



struct Comment {
    char* comment;
    #ifdef user_node_void_data_ptr
    void* data;
    #endif
    #ifdef node_record_positions
    unsigned line;
    unsigned col;
    unsigned offset;
    #endif
    // Add more members as needed
} Comment;

struct Call {
    struct Identifier* id;
    struct Statement** arguments; // Assuming you have an Expression structure defined
    unsigned acount;
    // Add more members as needed
} Call;

struct If {
    struct Position* pos;
    struct Compare* compare;
    struct Statement** body;
    unsigned bcount;
    struct Statement* else_part;
} If;

struct ElseIf {
    struct Position* pos;
    struct Compare* compare;
    struct Statement** body;
    unsigned bcount;
    struct Statement* else_part;
} ElseIf;

struct Else {
    struct Position* pos;
    struct Statement** body;
    unsigned bcount;
} Else;

struct For {
    struct Statement* initialization;
    struct Statement* condition;
    struct Statement* iteration;

    struct Statement** body;
    unsigned bcount;
} For;

struct While {
    struct Statement* condition;
    struct Statement** body;
    unsigned bcount;
} While;

struct DoWhile {
    struct Statement* condition;
    struct Statement** body;
    unsigned bcount;
} DoWhile;


enum IncludeType_ {
    STANDARD,   // <>
    NONSTANDARD // ""
};
struct Include {
    char* relative_included_path;
    char* absolute_path;
    enum IncludeType_ include_type;
} Include;

enum BuiltinType_ {
    BuiltinType_INT,
    BuiltinType_CHAR,
    BuiltinType_STRING, // char* e.g. "xxxxx"
    BuiltinType_CHAR_PTR,
    BuiltinType_DOUBLE,
    BuiltinType_FLOAT,

};

struct Constant {
    void* builtin_type;
    enum BuiltinType_ btype;
} Constant;


enum Statement_ {
    Stmt_NONE,
    Stmt_Identifier,
    Stmt_Symbol,
    Stmt_Call,
    Stmt_For,
    Stmt_If,
    Stmt_ElseIf,
    Stmt_Else,
    Stmt_While,
    Stmt_Do_While,
    Stmt_Enum,
    Stmt_SizeOf,
    Stmt_TypeDef,
    Stmt_FunctionDecl, //  char f();
    Stmt_FunctionDef,
    Stmt_Include,
    Stmt_FunctionDeclaration,
    Stmt_VarDecl, // ends with ; e.g. int x;
    Stmt_Assign, // decl with = opeartor is assign  -> var1 = var2; ...
    //Stmt_VarDef,
    Stmt_SymbolName,
    Stmt_Constant,

    Stmt_Break,
    Stmt_Continue,
    Stmt_Switch,
    Stmt_Case,
    Stmt_Goto,
    Stmt_Label,
    Stmt_Alignas,
    Stmt_Default,

    Stmt_Struct,
    Stmt_Union,

    Stmt_Define,
    Stmt_IfDef,
    Stmt_IfnDef,
    Stmt_EndIf,

};

struct Statement {
    void* stmt;
    enum Statement_ stmt_type;
} Statement;


enum Type_ {
    TYPE_BASE,
    TYPE_POINTER,
    TYPE_ARRAY,
    TYPE_FUNCTION_PTR
};

enum MemoryOperator_ {
    MemoryOperator_NONE,
    MemoryOperator_POINTER,
    MemoryOperator_REFERENCE
};

enum QualType_ {
    QualType_NONE,
    QualType_CONST,
    QualType_VOLATILE,
    QualType_RESTRICT,
    QualType_ATOMIC,
};

enum FunctionSpecifier_ {
    FunctionSpecifier_NONE,
    FunctionSpecifier_Inline,
    FunctionSpecifier__inline__, // GNU extension
    FunctionSpecifier_Noreturn,
    FunctionSpecifier__stdcall,
    FunctionSpecifier_gccAttributeSpecifier,
    FunctionSpecifier__declspec // identifier
};

enum StorageClassSpecifier_ {
    StorageClassSpecifier_NONE,
    //StorageClassSpecifier_Typedef,
    StorageClassSpecifier_Extern,
    StorageClassSpecifier_Auto,
    StorageClassSpecifier_Static,
    StorageClassSpecifier_Register,
    StorageClassSpecifier__Thread_local,
};

// type basic specifiers, not definition or declaration of that Statement -> its type not Statement !
enum TypeSpecifier_ {
    TypeSpecifier_NONE,
    TypeSpecifier_Void,
    TypeSpecifier_Char,
    TypeSpecifier_Short,
    TypeSpecifier_Int,
    TypeSpecifier_Long,
    TypeSpecifier_Float,
    TypeSpecifier_Double,
    TypeSpecifier_Signed,
    TypeSpecifier_Unsigned,
    TypeSpecifier_Bool,
    TypeSpecifier_Complex,
    TypeSpecifier_M128,
    TypeSpecifier_M128D,
    TypeSpecifier_M128I,
    TypeSpecifier_M128Extension,
    TypeSpecifier_Atomic,
    TypeSpecifier_Struct,
    TypeSpecifier_Union,
    TypeSpecifier_Enum,
    TypeSpecifier_TypedefName,
    TypeSpecifier_TypeofExtension,
};




struct Identifier {
    char* name;
} Identifier;


enum CompountType_ {
    PAREN, // ()
    BRACE, // {}
    BRACKET, // []
};

struct Compound {
    struct Statement* expr;
    enum CompountType_ compound_type;
} Compound;

struct Compare {
    struct Statement* left;
    struct Statement* right;
    enum CompareOperation_ cmp_op;
} Compare;

struct BinaryOperation {
    struct Statement* left;
    struct Statement* right;
    enum UnaryOperation_ assign_op;
} BinaryOperation;

// when we hit = sign left could be anything: expr or just varDecl and right as well
struct Assign {
    struct Statement* left;
    struct Statement* right;
    enum assignmentOperation_ assign_op;
} Assign;

struct Symbol {
    struct Identifier* id; // name of given symbol
    enum MemoryOperator_ op;
    unsigned num_op;

    // access operator to next statement e.g. x->y.z
    enum AccessType_ access_type;
    struct Statement* next;
} Symbol;

struct ArrayAccess {
    struct Symbol* id; // name of given symbol
    struct Statement* array_expr; // between []  - constant, another expr, ...
    enum MemoryOperator_ op;
    unsigned num_op;
} ArrayAccess;


// multiple types e.g. signed short int
struct TypeSpecList {
    enum TypeSpecifier_ type_spec;
    struct Identifier* id; // if type_spec == TypeSpecifier_TypedefName
    struct TypeSpecList* next;
} TypeSpecList;

struct BaseType {
    // not pre-processing getting names like enum, void, short, unsigned int, long long,
    // macros etc in form of char* in linked list
    struct TypeSpecList* type_spec_list;

    struct Identifier* id;
    enum QualType_ qualtype;
    enum StorageClassSpecifier_ storage_spec;
    // &, *
    enum MemoryOperator_ op;
    unsigned short num_op;

    enum QualType_ pointer_type_qualifier;

} BaseType;

// Forward declaration for recursive structure
struct InitializerList;

struct SingleInitializer {
    struct Expression* expr; // The expression for a single initializer
    struct InitializerList* next; // Pointer to the next initializer in the list
} SingleInitializer;

struct DesignatedInitializer {
    struct Designator* designator; // Designator for the designated initializer
    struct InitializerList* initializer; // Pointer to the initializer for the designated field
} DesignatedInitializer;

// Type representing the initializer lists
struct InitializerList {
    union {
        struct {
            struct Expression* expr; // Expression for a single initializer
            struct InitializerList* next; // Pointer to the next initializer in the list
        } single;
        struct {
            struct Designator* designator; // Designator for the designated initializer
            struct InitializerList* initializer; // Pointer to the initializer for the designated field
        } designated;
    } data;
    int is_single; // Flag to indicate whether it's a single or designated initializer
    struct InitializerList* next; // Pointer to the next initializer in the list
} InitializerList;

// Type representing designators for designated initializers
struct Designator {
    enum {
        Designator_Field, // Designator for struct field
        Designator_Index, // Designator for array index
    } kind;
    union {
        char* field_name; // Field name for struct initializer
        struct Expression* index; // Expression for array index
    } data;
} Designator;

struct ArrayType {
    struct BaseType* base_type;
    struct Statement* bracket_statement; // constant | macro->constant | NULL
    struct InitializerList* initializer; // Add this field
} ArrayType;

// Type representing parameters of a function type
struct ParameterList {
    struct Type* type; // Type of the parameter
    struct ParameterList* next; // Pointer to the next parameter in the list
} ParameterList;

// Type representing a function type
struct FunctionPtrType {
    struct Type* returntype; // Return type of the function
    struct ParameterList* parameters; // List of parameters
} FunctionPtrType;

struct Type {
    // what kind of type we have here.
    enum Type_ ttype;
    union {
        struct BaseType* base_type;
        struct ArrayType* array_type;
        struct FunctionPtrType* function_ptr_type;
    } type;
} Type;


struct VarDecl {
    struct Identifier* id;
    struct Type* type;
} VarDecl;

struct VarDef {
    struct Identifier* id;
    struct Type* type;
    struct Statement* right_side;
} VarDef;

struct ParamDecl {
    struct Identifier* id;
    struct Type* type;
} ParamDecl;

struct FunctionDecl {
    //struct Type* type;
    //struct Identifier* id;
    struct Identifier* id;
    struct Type* type;
    enum FunctionSpecifier_ functionspec;

    struct ParamDecl** parameters;
} FunctionDecl;

struct FunctionDef {
    //struct Type* type;
    //struct Identifier* id;
    struct Identifier* id;
    struct Type* type;
    enum FunctionSpecifier_ functionspec;

    struct ParamDecl** parameters;
    struct Statement** body;
    unsigned bcount;
} FunctionDef;




struct TranslationUnit {
    struct Statement** body;
    unsigned count;
} TranslationUnit;

struct Define {
    struct Identifier* id;
    struct Statement* stmt; // Constant or some macro definition
} Define;

struct IfDef {
    struct Identifier* id;
    struct Statement** body;
    unsigned bcount;
} IfDef;

struct IfnDef {
    struct Identifier* id;
    struct Statement** body;
    unsigned bcount;
} IfnDef;

// appropriate IfDef
struct EndIf {
    void* ifdef_or_ifndef; // filled when returned to Ifxxxx pre-processor Statement
} EndIf;

struct Break {
    struct Position* pos;
} Break;
struct Continue {
    struct Position* pos;
} Continue;
struct SwitchCase {
    struct Position* pos;
    struct Statement* condition;
    struct Statement** body;
    unsigned bcount;
} SwitchCase;
struct SwitchDefault {
    struct Position* pos;
    struct Statement** body;
    unsigned bcount;
} SwitchDefault;
struct Switch {
    struct Position* pos;
    struct Statement* condition;
    struct SwitchCase** cases;
    unsigned bccount;
    struct SwitchDefault* default_case;
} Switch;

struct Goto {
    struct Position* pos;
} Goto;
struct Label {
    struct Position* pos;
} Label;
struct Alignas {
    struct Position* pos;
} Alignas;
struct Enum {
    //struct Position* pos;
    struct Identifier* id;

    struct Statement** body; // each Statement could have expression.
    unsigned bcount;
} Enum;

enum StructOrUnion_ {
    StructOrUnion_Struct,
    StructOrUnion_Union
};
enum Typedef_ {
    Typedef_Typedef,
    Typedef_StructOrEnum
};

struct StructOrUnion {
    struct Identifier* id;
    enum StructOrUnion_ s_or_u;
    //struct Position* pos;
    struct Statement** body;
    unsigned bcount;
} StructOrUnion;

struct TypeDef {
    //struct Position* pos;
    struct Identifier* id; // typedef int [myint]    --- in struct_or_union it is NULL
    enum Typedef_ t_or_soru; // typedef or StructOrEnum
    union {
        struct Type* targetType;      // For simple types
        struct StructOrUnion* structOrUnionType; // For structs or unions
    } typedef_type;
};



struct SizeOf {
    struct Type* type;
} SizeOf;


struct Statement* parseStatement(struct Token*** tokens, int* num_tokens, int* tokenIndex);
struct Statement* decideWhatToParse(struct Token*** tokens, int* num_tokens, int* tokenIndex);

#endif // MAIN_H
