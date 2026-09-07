#ifndef RESALTADOR_H
#define RESALTADOR_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QVector>

class resaltador : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    void activar(bool estado){habilitado = estado; rehighlight();}
    explicit resaltador(QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct ReglaResaltado{
        QRegularExpression patron;
        QTextCharFormat formato;
    };

    QVector<ReglaResaltado> reglasResaltado;

    QTextCharFormat formatoIdentificadores;
    QTextCharFormat formatoNumeros;
    QTextCharFormat formatoCadenas;
    QTextCharFormat formatoBools;
    QTextCharFormat formatoPalabrasClave;
    QTextCharFormat formatoOperadores;
    QTextCharFormat formatoOtros;
    bool habilitado = true;
};

#endif // RESALTADOR_H
