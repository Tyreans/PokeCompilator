#include "Sintactico.h"
#include "RegistradorConsola.h"
#include <iostream>
#include <algorithm>

Sintactico::Sintactico(TablaSimbolos &tabla, RegistradorConsola &registrador)
    : tokens(nullptr), posicion(0), error(false), tabla(tabla), registrador(registrador)
{
}

Token Sintactico::tokenActual() const
{
    if(!tokens || posicion >= tokens->size())
        return {TokenType::END, "@"};
    return (*tokens)[posicion];
}

void Sintactico::avanzar()
{
    if(!finArchivo())
        posicion++;
}

bool Sintactico::finArchivo() const
{
    return tokenActual().type == TokenType::END;
}

bool Sintactico::aceptar(TokenType type)
{
    if(tokenActual().type == type) {
        avanzar();
        return true;
    }
    return false;
}

bool Sintactico::esperar(TokenType type, const QString &mensaje)
{
    if(aceptar(type))
        return true;
    reportarError(mensaje);
    return false;
}

void Sintactico::reportarError(const QString &mensaje)
{
    QString output = QString("Error de sintaxis: %1 (token actual: %2)")
    .arg(mensaje)
        .arg(QString::fromLatin1(tokenTypeToString(tokenActual().type)));
    registrador.appendLine(output);
    error = true;
}

void Sintactico::imprimirProduccion(const QString &produccion)
{
    registrador.appendLine(produccion);
}

bool Sintactico::tuvoError() const
{
    return error;
}

bool Sintactico::analizar(const std::vector<Token> &inputTokens)
{
    tokens = &inputTokens;
    posicion = 0;
    error = false;

    registrador.appendLine("- - - ANALIZADOR SINTACTICO - Inicio - - -");
    bool result = analizarPrograma();
    if(!finArchivo() && !error)
        reportarError("Se esperaba el fin del archivo");
    if(result && !error)
        registrador.appendLine("Cadena ACEPTADA");
    return result && !error;
}

bool Sintactico::analizarPrograma()
{
    imprimirProduccion("<PROGRAMA> -> <INSTRUCCION> <PROGRAMA> | @");
    while(!finArchivo()) {
        if(tokenActual().type == TokenType::SALTO) {
            avanzar();
            continue;
        }
        if(!analizarInstruccion())
            return false;
    }
    return true;
}

bool Sintactico::analizarInstruccion()
{
    imprimirProduccion("<INSTRUCCION> -> DIM | IF | WHILE | PRINT | ASIGNACION");
    TokenType type = tokenActual().type;
    if(type == TokenType::DIM)
        return analizarDim();
    if(type == TokenType::IF)
        return analizarSi();
    if(type == TokenType::WHILE)
        return analizarMientras();
    if(type == TokenType::PRINT)
        return analizarImprimir();
    if(esIdentificador(type))
        return analizarAsignacion();

    reportarError("Instruccion invalida");
    return false;
}

bool Sintactico::analizarDim()
{
    imprimirProduccion("<DIM> -> DIM <IDENTIFICADOR> ( ENTERO ) SALTO");
    avanzar();
    Token id = tokenActual();
    if(!esIdentificador(id.type)) {
        reportarError("Se esperaba un identificador despues de DIM");
        return false;
    }
    avanzar();
    if(!esperar(TokenType::LPAREN, "Se esperaba '('") ||
        !esperar(TokenType::ENTERO, "Se esperaba el tipo ENTERO dentro de parentesis") ||
        !esperar(TokenType::RPAREN, "Se esperaba ')'") ||
        !esperar(TokenType::SALTO, "Se esperaba SALTO al final de DIM"))
        return false;

    std::string typeName = nombreTipoIdentificador(id.type);
    if(!tabla.declarar(id.lexeme, typeName)) {
        reportarError(QString("Variable ya declarada: %1").arg(QString::fromStdString(id.lexeme)));
        return false;
    }
    return true;
}

bool Sintactico::analizarAsignacion()
{
    imprimirProduccion("<ASIGNACION> -> <IDENTIFICADOR> = <EXPRESION> <OPERACION_ARITMETICA> SALTO");
    Token id = tokenActual();
    TokenType idType = id.type;
    avanzar();
    if(!esperar(TokenType::ASSIGN, "Se esperaba '=' en la asignacion"))
        return false;
    if(!analizarExpresion())
        return false;
    if(!analizarOperacionAritmetica())
        return false;
    if(!esperar(TokenType::SALTO, "Se esperaba SALTO al final de la asignacion"))
        return false;

    if(!tabla.estaDeclarada(id.lexeme)) {
        tabla.declarar(id.lexeme, nombreTipoIdentificador(idType));
    }
    return true;
}

bool Sintactico::analizarImprimir()
{
    imprimirProduccion("<PRINT> -> PRINT <EXPRESION> (CONCAT <EXPRESION>)* SALTO");
    avanzar();
    if(!analizarExpresion())
        return false;
    while(aceptar(TokenType::CONCAT)) {
        if(!analizarExpresion())
            return false;
    }
    return esperar(TokenType::SALTO, "Se esperaba SALTO al final de PRINT");
}

bool Sintactico::analizarSi()
{
    imprimirProduccion("<E_IF> -> IF <EXPRESION> <OP_LOGICO> <EXPRESION> THEN SALTO <M_INSTRUCCION> <C_IF> ENDIF SALTO");
    avanzar();
    if(!analizarExpresion() || !analizarOperadorLogico() || !analizarExpresion())
        return false;
    if(!esperar(TokenType::THEN, "Se esperaba THEN") ||
        !esperar(TokenType::SALTO, "Se esperaba SALTO despues de THEN"))
        return false;

    if(!analizarListaInstruccionesHasta({TokenType::ELSEIF, TokenType::ELSE, TokenType::ENDIF}))
        return false;

    while(aceptar(TokenType::ELSEIF)) {
        if(!analizarExpresion() || !analizarOperadorLogico() || !analizarExpresion())
            return false;
        if(!esperar(TokenType::THEN, "Se esperaba THEN") ||
            !esperar(TokenType::SALTO, "Se esperaba SALTO despues de THEN"))
            return false;
        if(!analizarListaInstruccionesHasta({TokenType::ELSEIF, TokenType::ELSE, TokenType::ENDIF}))
            return false;
    }

    if(aceptar(TokenType::ELSE)) {
        if(!esperar(TokenType::SALTO, "Se esperaba SALTO despues de ELSE"))
            return false;
        if(!analizarListaInstruccionesHasta({TokenType::ENDIF}))
            return false;
    }

    if(!esperar(TokenType::ENDIF, "Se esperaba ENDIF") ||
        !esperar(TokenType::SALTO, "Se esperaba SALTO despues de ENDIF"))
        return false;

    return true;
}

bool Sintactico::analizarMientras()
{
    imprimirProduccion("<E_BUCLE> -> WHILE <EXPRESION> <OP_LOGICO> <EXPRESION> SALTO <M_INSTRUCCION> WEND SALTO");
    avanzar();
    if(!analizarExpresion() || !analizarOperadorLogico() || !analizarExpresion())
        return false;
    if(!esperar(TokenType::SALTO, "Se esperaba SALTO despues de la condicion WHILE"))
        return false;

    if(!analizarListaInstruccionesHasta({TokenType::WEND}))
        return false;

    if(!esperar(TokenType::WEND, "Se esperaba WEND") ||
        !esperar(TokenType::SALTO, "Se esperaba SALTO despues de WEND"))
        return false;

    return true;
}

bool Sintactico::analizarExpresion()
{
    TokenType type = tokenActual().type;
    if(esIdentificador(type)) {
        if(!tabla.estaDeclarada(tokenActual().lexeme)) {
            QString warning = QString("Advertencia: variable no declarada %1").arg(QString::fromStdString(tokenActual().lexeme));
            registrador.appendLine(warning);
        }
        avanzar();
        return true;
    }
    if(type == TokenType::CADENA_TEXTO || type == TokenType::ENTERO || type == TokenType::FLOAT || type == TokenType::BOOLEANO) {
        avanzar();
        return true;
    }
    reportarError("Se esperaba una expresion");
    return false;
}

bool Sintactico::analizarOperacionAritmetica()
{
    if(aceptar(TokenType::PLUS) || aceptar(TokenType::MINUS) || aceptar(TokenType::MUL) || aceptar(TokenType::DIV) || aceptar(TokenType::MOD)) {
        return analizarExpresion();
    }
    return true;
}

bool Sintactico::analizarOperadorLogico()
{
    if(aceptar(TokenType::GT) || aceptar(TokenType::LT) || aceptar(TokenType::GE) || aceptar(TokenType::LE) || aceptar(TokenType::NE) || aceptar(TokenType::EQ))
        return true;
    reportarError("Se esperaba un operador logico");
    return false;
}

bool Sintactico::analizarListaInstruccionesHasta(const std::vector<TokenType> &terminadores)
{
    while(!finArchivo()) {
        if(std::find(terminadores.begin(), terminadores.end(), tokenActual().type) != terminadores.end())
            return true;
        if(tokenActual().type == TokenType::SALTO) {
            avanzar();
            continue;
        }
        if(!analizarInstruccion())
            return false;
    }
    reportarError("Se esperaba un terminador en la lista de instrucciones");
    return false;
}

bool Sintactico::esIdentificador(TokenType type) const
{
    return type == TokenType::ID_CADENA || type == TokenType::ID_ENTERO || type == TokenType::ID_FLOAT || type == TokenType::ID_BOOLEANO;
}

std::string Sintactico::nombreTipoIdentificador(TokenType type) const
{
    switch(type) {
    case TokenType::ID_CADENA: return "CADENA";
    case TokenType::ID_ENTERO: return "ENTERO";
    case TokenType::ID_FLOAT: return "FLOAT";
    case TokenType::ID_BOOLEANO: return "BOOLEANO";
    default: return "DESCONOCIDO";
    }
}
