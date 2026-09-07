#pragma once

#include <string>
#include <map>
#include <vector>

class TablaSimbolos {
public:
    bool declarar(const std::string &nombre, const std::string &tipo);
    bool estaDeclarada(const std::string &nombre) const;
    std::string tipoDe(const std::string &nombre) const;
    std::vector<std::pair<std::string, std::string>> entradas() const;

private:
    std::map<std::string, std::string> m_tabla;
};
