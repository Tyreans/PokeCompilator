#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <string>
#include "resaltador.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

    void cargarDirectorioVisual(const QString& rutaCarpeta);
    void changeBackground(bool b);
private slots:
    void on_listWidget_itemClicked(class QListWidgetItem *item);

    void on_btnSalir_clicked();

    void on_btnProd_clicked();

    void on_btnSave_clicked();

    void on_btnNewFile_clicked();

    void on_btnRoute_clicked();

    void on_plainTextEdit_textChanged();

private:
    Ui::Dialog *ui;
    QString rutaArchivoAbierto = "";
    QString carpetaRaiz = "/home/tyreans/binarios/";
    resaltador *coloresTipado = nullptr;
};
#endif // DIALOG_H
