#pragma once

#include <string>
#include <iostream>

const int MAX = 100;

class Pila {
public:
    Pila();
    void insertapila(const std::string &elem);
    void eliminapila();
    std::string obtenerCima() const;
    bool estaVacia() const;
    bool estaLlena() const;
    int obtenerTamano() const;
    void vaciar();

private:
    std::string pilac[MAX];
    int cima;
};

