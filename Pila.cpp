#include "Pila.h"

Pila::Pila() : cima(-1) {
}

void Pila::insertapila(const std::string &elem) {
    if(cima == -1) {
        cima = 0;
        pilac[cima] = elem;
    }
    else {
        if(cima == MAX - 1) {
            std::cout << "Pila Llena" << std::endl;
        }
        else {
            cima++;
            pilac[cima] = elem;
        }
    }
}

void Pila::eliminapila() {
    if(cima == -1) {
        std::cout << "Pila Vacia" << std::endl;
    }
    else {
        pilac[cima] = "";
        cima--;
    }
}

std::string Pila::obtenerCima() const {
    if(cima == -1) {
        return "";
    }
    return pilac[cima];
}

bool Pila::estaVacia() const {
    return cima == -1;
}

bool Pila::estaLlena() const {
    return cima == MAX - 1;
}

int Pila::obtenerTamano() const {
    return cima + 1;
}

void Pila::vaciar() {
    cima = -1;
}
