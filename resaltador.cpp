#include "resaltador.h"

resaltador::resaltador(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    // Identificadores de variables (a$, b%, c!, d~) -> Color Naranja
    formatoIdentificadores.setForeground(QColor("#0934B8"));
    formatoIdentificadores.setFontWeight(QFont::Bold);

    // Valores asignados (Números, Strings entre comillas, booleanos)
    formatoNumeros.setForeground(QColor("#06D64B")); // Verde
    formatoCadenas.setForeground(QColor("#A31707")); // Rojo Oscuro

    // Funcionalidades / Palabras Clave (IF, WHILE, PRINT, etc.) -> Color Púrpura
    formatoPalabrasClave.setForeground(QColor("#B51EBA"));
    formatoPalabrasClave.setFontWeight(QFont::Bold);

    // Operadores Matemáticos y Lógicos (+, -, ==, >=, etc.) -> Negro
    formatoOperadores.setForeground(QColor("#000000"));

    // Otros caracteres (; , parentesis) -> Negro
    formatoOtros.setForeground(QColor("#000000"));

    // ==========================================
    // 2. DEFINICIÓN DE REGLAS CON REGEX
    // ==========================================
    ReglaResaltado regla;

    // ID_CADENA_TEXTO: letras seguidas de $
    regla.patron = QRegularExpression("\\b[A-Za-z]+\\$");
    regla.formato = formatoIdentificadores;
    reglasResaltado.append(regla);

    // ID_ENTERO: letras seguidas de %
    regla.patron = QRegularExpression("\\b[A-Za-z]+\\%");
    regla.formato = formatoIdentificadores;
    reglasResaltado.append(regla);

    // ID_FLOAT: letras seguidas de !
    regla.patron = QRegularExpression("\\b[A-Za-z]+\\!");
    regla.formato = formatoIdentificadores;
    reglasResaltado.append(regla);

    // ID_BOOLEANO: letras seguidas de ~
    regla.patron = QRegularExpression("\\b[A-Za-z]+~");
    regla.formato = formatoIdentificadores;
    reglasResaltado.append(regla);


    // CADENA_TEXTO: texto entre comillas dobles
    regla.patron = QRegularExpression("\".*?\"");
    regla.formato = formatoCadenas;
    reglasResaltado.append(regla);

    // FLOAT: números con punto decimal
    regla.patron = QRegularExpression("\\b[0-9]+\\.[0-9]+\\b");
    regla.formato = formatoNumeros;
    reglasResaltado.append(regla);

    // ENTERO: números puros
    regla.patron = QRegularExpression("\\b[0-9]+\\b");
    regla.formato = formatoNumeros;
    reglasResaltado.append(regla);

    // BOOLEANO: TRUE, FALSE, true, false
    regla.patron = QRegularExpression("\\b(TRUE|FALSE|true|false)\\b");
    regla.formato = formatoIdentificadores;
    reglasResaltado.append(regla);


    QStringList palabrasClave;
    palabrasClave << "\\bIF\\b" << "\\bTHEN\\b" << "\\bELSE\\b" << "\\bELSEIF\\b"
                  << "\\bWHILE\\b" << "\\bENDIF\\b" << "\\bWEND\\b" << "\\bPRINT\\b" << "\\bDIM\\b";

    for (const QString &palabra : palabrasClave) {
        regla.patron = QRegularExpression(palabra);
        regla.formato = formatoPalabrasClave;
        reglasResaltado.append(regla);
    }

    QStringList operadores;
    operadores << "\\+" << "-" << "\\*" << "/" << "==" << "!=" << ">=" << "<=" << "=" << "%" << "!" << ">" << "<";

    for (const QString &op : operadores) {
        regla.patron = QRegularExpression(op);
        regla.formato = formatoOperadores;
        reglasResaltado.append(regla);
    }

    // Concatenación (;) y Paréntesis ( )
    regla.patron = QRegularExpression(";");
    regla.formato = formatoOtros;
    reglasResaltado.append(regla);

    regla.patron = QRegularExpression("[()]");
    regla.formato = formatoOtros;
    reglasResaltado.append(regla);
}

void resaltador::highlightBlock(const QString &text)
{
    if (!habilitado) {
        return;
    }

    for (const ReglaResaltado &regla : reglasResaltado) {
        QRegularExpressionMatchIterator matchIterator = regla.patron.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), regla.formato);
        }
    }
}
