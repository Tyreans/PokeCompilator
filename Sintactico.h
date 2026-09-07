#pragma once

#include <QString>
#include <string>
#include <vector>
#include "Token.h"
#include "TablaSimbolos.h"
#include "RegistradorConsola.h"

class Sintactico {
public:
    Sintactico(TablaSimbolos &tabla, RegistradorConsola &registrador);
    bool analizar(const std::vector<Token> &tokens);
    bool tuvoError() const;

private:
    const std::vector<Token> *tokens;
    size_t posicion;
    bool error;
    TablaSimbolos &tabla;
    RegistradorConsola &registrador;

    Token tokenActual() const;
    void avanzar();
    bool finArchivo() const;
    bool aceptar(TokenType type);
    bool esperar(TokenType type, const QString &mensaje);
    void reportarError(const QString &mensaje);
    void imprimirProduccion(const QString &produccion);

    bool analizarPrograma();
    bool analizarInstruccion();
    bool analizarDim();
    bool analizarAsignacion();
    bool analizarImprimir();
    bool analizarSi();
    bool analizarMientras();
    bool analizarExpresion();
    bool analizarOperacionAritmetica();
    bool analizarOperadorLogico();
    bool analizarListaInstruccionesHasta(const std::vector<TokenType> &terminadores);
    bool esIdentificador(TokenType type) const;
    std::string nombreTipoIdentificador(TokenType type) const;
};
