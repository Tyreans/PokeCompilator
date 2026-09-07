#include "TablaSimbolos.h"

bool TablaSimbolos::declarar(const std::string &nombre, const std::string &tipo)
{
    auto it = m_tabla.find(nombre);
    if(it != m_tabla.end()) {
        return false;
    }
    m_tabla[nombre] = tipo;
    return true;
}

bool TablaSimbolos::estaDeclarada(const std::string &nombre) const
{
    return m_tabla.find(nombre) != m_tabla.end();
}

std::string TablaSimbolos::tipoDe(const std::string &nombre) const
{
    auto it = m_tabla.find(nombre);
    return it != m_tabla.end() ? it->second : "";
}

std::vector<std::pair<std::string, std::string>> TablaSimbolos::entradas() const
{
    std::vector<std::pair<std::string, std::string>> result;
    for(const auto &item : m_tabla)
        result.emplace_back(item.first, item.second);
    return result;
}
