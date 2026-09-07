#pragma once

#include <string>
#include <vector>
#include "Token.h"
#include "RegistradorConsola.h"

class Lexico {
public:
    Lexico() = default;
    bool analizar(const std::string &src, RegistradorConsola *registrador = nullptr);
    const std::vector<Token>& tokens() const { return m_tokens; }
    void limpiar() { m_tokens.clear(); }

private:
    std::vector<Token> m_tokens;
    TokenType palabraReservada(const std::string &lex) const;
    TokenType tipoIdentificador(const std::string &lex) const;
    void agregarToken(TokenType type, const std::string &lexema = "");
};
