#ifndef LEXER_H
#define LEXER_H





// Example token types
enum TokenType {
    TOK_KW_IF,
    TOK_KW_ELSE,
    TOK_KW_WHILE,
    TOK_KW_FOR,
    TOK_KW_RETURN,
    TOK_KW_INCLUDE,
    TOK_KW_EXTERN,
    TOK_KW_IFDEF,
    TOK_KW_IFNDEF,
    TOK_KW_UNDEF,
    TOK_KW_ENDIF,
    TOK_KW_PRAGMA,
    TOK_KW_DEFINE,
    TOK_KW_REGISTER,
    TOK_KW_TYPEDEF,
    TOK_KW_STRUCT,
    TOK_KW_UNION,
    TOK_KW_SIZEOF,

    TOK_KW_INLINE,

    TOK_KW_ENUM,
    TOK_KW_STATIC,
    TOK_KW_CONST,

    TOK_KW_CASE,
    TOK_KW_DEFAULT,
    TOK_KW_SWITCH,


    TOK_OP_PLUS,
    TOK_OP_MINUS,
    TOK_OP_MULTIPLY,
    TOK_OP_DIVIDE,
    TOK_OP_EQUALS,
    TOK_OP_AND,
    TOK_OP_OR,
    TOK_OP_NOT,
    TOK_OP_XOR,
    TOK_OP_SHIFT_LEFT,
    TOK_OP_SHIFT_RIGHT,
    TOK_OP_BIGGER,
    TOK_OP_LOWER,
    TOK_OP_ARROW,
    TOK_OP_PLUSEQUALS,
    TOK_OP_MINUSEQUALS,
    TOK_OP_MULEQUALS,
    TOK_OP_DIVEQUALS,

    TOK_OP_EQUALSQUALS,
    TOK_OP_BIGGER_EQUALS,
    TOK_OP_LOWER_EQUALS,
    TOK_OP_NOTEQUALS,
    TOK_OP_ANDAND,
    TOK_OP_OROR,

    TOK_OP_MODULO,
    TOK_OP_MODULOEQUALS,

    TOK_PUNCT_OPEN_PAREN,
    TOK_PUNCT_CLOSE_PAREN,
    TOK_PUNCT_SEMICOLON,
    TOK_PUNCT_OPEN_BRACE,
    TOK_PUNCT_CLOSE_BRACE,
    TOK_PUNCT_OPEN_BRACKET,
    TOK_PUNCT_CLOSE_BRACKET,
    TOK_PUNCT_COMMA,
    TOK_PUNCT_HASH,
    TOK_PUNCT_DOUBLEQUOTE,
    TOK_PUNCT_SINGLEQUOTE,
    TOK_PUNCT_DOT,
    TOK_PUNCT_BACKSLASH,
    TOK_PUNCT_COLON,
    TOK_PUNCT_QUESTIONMARK,
    TOK_PUNCT_NEWLINE,


    TOK_TYPESPEC_Void,
    TOK_TYPESPEC_Char,
    TOK_TYPESPEC_Short,
    TOK_TYPESPEC_Int,
    TOK_TYPESPEC_Long,
    TOK_TYPESPEC_Float,
    TOK_TYPESPEC_Double,
    TOK_TYPESPEC_Signed,
    TOK_TYPESPEC_Unsigned,
    TOK_TYPESPEC_Bool,
    TOK_TYPESPEC_Complex,
    TOK_TYPESPEC_M128,
    TOK_TYPESPEC_M128D,
    TOK_TYPESPEC_M128I,
    TOK_TYPESPEC_M128Extension,
    TOK_TYPESPEC_Atomic,
    TOK_TYPESPEC_TypeofExtension, // __typeof__



    TOK_INTEGER,
    TOK_STRING,
    // Add more as needed
};



// Token structure
typedef struct Token {
    enum TokenType type;
    char* lexeme; // The actual token text
    // Add more fields as needed (line number, position, etc.)
} Token;


// Constants related to keywords
#define KW_IF "if"
#define KW_ELSE "else"
#define KW_WHILE "while"
#define KW_FOR "for"
#define KW_INT "int"
#define KW_RETURN "return"
#define KW_INCLUDE "include"
#define KW_EXTERN "extern"
#define KW_IFDEF "ifdef"
#define KW_IFNDEF "ifndef"
#define KW_ENDIF "endif"
#define KW_UNDEF "undef"
#define KW_PRAGMA "pragma"
#define KW_DEFINE "define"
#define KW_REGISTER "register"
#define KW_TYPEDEF "typedef"
#define KW_STRUCT "struct"
#define KW_UNION "union"
#define KW_SIZEOF "sizeof"
#define KW_ENUM "enum"
#define KW_STATIC "static"
#define KW_CONST "const"

#define KW_CASE "case"
#define KW_DEFAULT "default"
#define KW_SWITCH "switch"

#define KW_INLINE "inline"

#define TYPESPEC_Void "void"
#define TYPESPEC_Char "char"
#define TYPESPEC_Short "short"
#define TYPESPEC_Int "int"
#define TYPESPEC_Long "long"
#define TYPESPEC_Float "float"
#define TYPESPEC_Double "double"
#define TYPESPEC_Signed "signed"
#define TYPESPEC_Unsigned "unsigned"
#define TYPESPEC_Bool "_Bool"
#define TYPESPEC_Complex "_Complex"
#define TYPESPEC_Atomic "atomic"
#define TYPESPEC_M128 "m128"
#define TYPESPEC_M128D "m128D"
#define TYPESPEC_M128I "m128I"
#define TYPESPEC_M128Extension "__extension__"
#define TYPESPEC_TypeofExtension " __typeof__"


// Constants related to operators
#define OP_PLUS "+"
#define OP_MINUS "-"
#define OP_MULTIPLY "*"
#define OP_DIVIDE "/"
#define OP_EQUALS "="
#define OP_MODULO "%"
#define OP_MODULOEQUALS "%"
#define OP_EQUALSEQUALS "=="
#define OP_NOTEQUALS "!="
#define OP_BIGGER_EQUALS ">="
#define OP_LOWER_EQUALS "<="
#define OP_AND "&"
#define OP_ANDAND "&&"
#define OP_OR "|"
#define OP_OROR "||"
#define OP_NOT "!"
#define OP_XOR "^"
#define OP_SHIFT_LEFT "<<"
#define OP_SHIFT_RIGHT ">>"
#define OP_BIGGER ">"
#define OP_LOWER "<"
#define OP_ARROW "->"

#define OP_MINUSEQUALS "-="
#define OP_PLUSEQUALS "+="
#define OP_MULEQUALS "*="
#define OP_DIVEQUALS "/="



// Constants related to punctuation
#define PUNCT_OPEN_PAREN "("
#define PUNCT_CLOSE_PAREN ")"
#define PUNCT_SEMICOLON ";"
#define PUNCT_OPEN_BRACE "{"
#define PUNCT_CLOSE_BRACE "}"
#define PUNCT_OPEN_BRACKET "["
#define PUNCT_CLOSE_BRACKET "]"
#define PUNCT_COMMA ","
#define PUNCT_HASH "#"
#define PUNCT_DOUBLEQUOTE "\""
#define PUNCT_SINGLEQUOTE "\'"
#define PUNCT_SPACE " "
#define PUNCT_TAB "\t"
#define PUNCT_DOT "."
#define PUNCT_BACKSLASH "\\"
#define PUNCT_COLON ":"
#define PUNCT_QUESTIONMARK "?"
#define PUNCT_NEWLINE "\n"




void lex(const char* input, int* num_tokens, int* tokenIndex, struct Token*** tokens);




#endif // LEXER_H
