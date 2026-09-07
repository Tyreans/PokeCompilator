#include "Lexico.h"
#include "RegistradorConsola.h"
#include <cctype>
#include <iostream>

static std::string toUpper(const std::string &text)
{
    std::string result;
    result.reserve(text.size());
    for(char c : text)
        result += std::toupper((unsigned char)c);
    return result;
}

TokenType Lexico::palabraReservada(const std::string &lex) const
{
    std::string up = toUpper(lex);
    if(up == "IF") return TokenType::IF;
    if(up == "THEN") return TokenType::THEN;
    if(up == "ELSE") return TokenType::ELSE;
    if(up == "ELSEIF") return TokenType::ELSEIF;
    if(up == "WHILE") return TokenType::WHILE;
    if(up == "ENDIF") return TokenType::ENDIF;
    if(up == "WEND") return TokenType::WEND;
    if(up == "PRINT") return TokenType::PRINT;
    if(up == "DIM") return TokenType::DIM;
    if(up == "TRUE" || up == "FALSE") return TokenType::BOOLEANO;
    return TokenType::UNKNOWN;
}

TokenType Lexico::tipoIdentificador(const std::string &lex) const
{
    if(lex.empty()) return TokenType::ID_ENTERO;
    char ultimo = lex.back();
    switch(ultimo) {
    case '$': return TokenType::ID_CADENA;
    case '%': return TokenType::ID_ENTERO;
    case '!': return TokenType::ID_FLOAT;
    case '~': return TokenType::ID_BOOLEANO;
    default: return TokenType::ID_ENTERO;
    }
}

void Lexico::agregarToken(TokenType type, const std::string &lexema)
{
    m_tokens.push_back({type, lexema});
}

bool Lexico::analizar(const std::string &src, RegistradorConsola *registrador)
{
    m_tokens.clear();
    const char *s = src.c_str();
    int len = static_cast<int>(src.size());
    int i = 0;

    while(i < len)
    {
        char c = s[i];
        if(c == ' ' || c == '\t') { i++; continue; }

        if(c == '\r' || c == '\n') {
            if(m_tokens.empty() || m_tokens.back().type != TokenType::SALTO)
                agregarToken(TokenType::SALTO, "SALTO");
            i++;
            continue;
        }

        if(std::toupper((unsigned char)c) == 'R' && i + 2 < len && std::toupper((unsigned char)s[i+1]) == 'E' && std::toupper((unsigned char)s[i+2]) == 'M') {
            while(i < len && s[i] != '\n') i++;
            continue;
        }

        if(c == '"') {
            int start = ++i;
            while(i < len && s[i] != '"' && s[i] != '\n') i++;
            std::string literal(s + start, s + i);
            if(i < len && s[i] == '"') i++;
            agregarToken(TokenType::CADENA_TEXTO, literal);
            continue;
        }

        if(std::isdigit((unsigned char)c)) {
            int start = i;
            bool isFloat = false;
            while(i < len && std::isdigit((unsigned char)s[i])) i++;
            if(i < len && s[i] == '.') {
                isFloat = true;
                i++;
                while(i < len && std::isdigit((unsigned char)s[i])) i++;
            }
            agregarToken(isFloat ? TokenType::FLOAT : TokenType::ENTERO, std::string(s + start, s + i));
            continue;
        }

        if(std::isalpha((unsigned char)c) || c == '_') {
            int start = i;
            while(i < len && (std::isalnum((unsigned char)s[i]) || s[i] == '_' || s[i] == '$' || s[i] == '%' || s[i] == '!' || s[i] == '~'))
                i++;
            std::string lexema(s + start, s + i);
            TokenType reservado = palabraReservada(lexema);
            if(reservado != TokenType::UNKNOWN)
                agregarToken(reservado, lexema);
            else
                agregarToken(tipoIdentificador(lexema), lexema);
            continue;
        }

        if(c == '>' && i + 1 < len && s[i+1] == '=') { agregarToken(TokenType::GE, ">="); i += 2; continue; }
        if(c == '<' && i + 1 < len && s[i+1] == '=') { agregarToken(TokenType::LE, "<="); i += 2; continue; }
        if(c == '!' && i + 1 < len && s[i+1] == '=') { agregarToken(TokenType::NE, "!="); i += 2; continue; }
        if(c == '=' && i + 1 < len && s[i+1] == '=') { agregarToken(TokenType::EQ, "=="); i += 2; continue; }

        switch(c) {
        case '+': agregarToken(TokenType::PLUS, "+"); break;
        case '-': agregarToken(TokenType::MINUS, "-"); break;
        case '*': agregarToken(TokenType::MUL, "*"); break;
        case '/': agregarToken(TokenType::DIV, "/"); break;
        case '%': agregarToken(TokenType::MOD, "%"); break;
        case '=': agregarToken(TokenType::ASSIGN, "="); break;
        case '>': agregarToken(TokenType::GT, ">"); break;
        case '<': agregarToken(TokenType::LT, "<"); break;
        case '(': agregarToken(TokenType::LPAREN, "("); break;
        case ')': agregarToken(TokenType::RPAREN, ")"); break;
        case ';': agregarToken(TokenType::CONCAT, "CONCATENACION"); break;
        default:
            if(registrador)
                registrador->appendLine(QString("[Lexico] Caracter no reconocido: '%1'").arg(QChar(c)));
            else
                std::cerr << "[Lexico] Caracter no reconocido: '" << c << "'\n";
            break;
        }
        i++;
    }

    if(m_tokens.empty() || m_tokens.back().type != TokenType::SALTO)
        agregarToken(TokenType::SALTO, "SALTO");
    agregarToken(TokenType::END, "@");
    return true;
}
