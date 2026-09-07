#pragma once
#include <string>

enum class TokenType {
    ID_CADENA, ID_ENTERO, ID_FLOAT, ID_BOOLEANO,
    CADENA_TEXTO, ENTERO, FLOAT, BOOLEANO,
    IF, THEN, ELSE, ELSEIF, WHILE, ENDIF, WEND,
    PRINT, DIM,
    PLUS, MINUS, MUL, DIV, MOD,
    ASSIGN, GT, LT, GE, LE, NE, EQ,
    SALTO, CONCAT, LPAREN, RPAREN,
    END, UNKNOWN
};

struct Token {
    TokenType type;
    std::string lexeme;
};

inline const char* tokenTypeToString(TokenType type)
{
    switch(type) {
    case TokenType::ID_CADENA: return "ID_CADENA";
    case TokenType::ID_ENTERO: return "ID_ENTERO";
    case TokenType::ID_FLOAT: return "ID_FLOAT";
    case TokenType::ID_BOOLEANO: return "ID_BOOLEANO";
    case TokenType::CADENA_TEXTO: return "CADENA_TEXTO";
    case TokenType::ENTERO: return "ENTERO";
    case TokenType::FLOAT: return "FLOAT";
    case TokenType::BOOLEANO: return "BOOLEANO";
    case TokenType::IF: return "IF";
    case TokenType::THEN: return "THEN";
    case TokenType::ELSE: return "ELSE";
    case TokenType::ELSEIF: return "ELSEIF";
    case TokenType::WHILE: return "WHILE";
    case TokenType::ENDIF: return "ENDIF";
    case TokenType::WEND: return "WEND";
    case TokenType::PRINT: return "PRINT";
    case TokenType::DIM: return "DIM";
    case TokenType::PLUS: return "+";
    case TokenType::MINUS: return "-";
    case TokenType::MUL: return "*";
    case TokenType::DIV: return "/";
    case TokenType::MOD: return "%";
    case TokenType::ASSIGN: return "=";
    case TokenType::GT: return ">";
    case TokenType::LT: return "<";
    case TokenType::GE: return ">=";
    case TokenType::LE: return "<=";
    case TokenType::NE: return "!=";
    case TokenType::EQ: return "==";
    case TokenType::SALTO: return "SALTO";
    case TokenType::CONCAT: return "CONCATENACION";
    case TokenType::LPAREN: return "(";
    case TokenType::RPAREN: return ")";
    case TokenType::END: return "@";
    default: return "UNKNOWN";
    }
}
