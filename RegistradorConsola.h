#pragma once

#include <QString>
#include <iostream>

class RegistradorConsola {
public:
    void append(const QString &texto) {
        m_texto += texto;
        std::cout << texto.toStdString() << std::flush;
    }

    void appendLine(const QString &texto = QString()) {
        m_texto += texto;
        m_texto += '\n';
        std::cout << texto.toStdString() << std::endl;
    }

    const QString &texto() const { return m_texto; }

private:
    QString m_texto;
};
