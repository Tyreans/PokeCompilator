/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QPlainTextEdit *plainTextEdit;
    QListWidget *listWidget;
    QPushButton *btnSalir;
    QPushButton *btnProd;
    QPushButton *btnSave;
    QLabel *label;
    QPushButton *btnRoute;
    QPushButton *btnNewFile;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName("Dialog");
        Dialog->resize(748, 543);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/IMAGES/juego.png"), QSize(), QIcon::Mode::Active, QIcon::State::On);
        Dialog->setWindowIcon(icon);
        Dialog->setStyleSheet(QString::fromUtf8("Dialog { background-image: url(:/images/IMAGES/MAIN.png); }"));
        plainTextEdit = new QPlainTextEdit(Dialog);
        plainTextEdit->setObjectName("plainTextEdit");
        plainTextEdit->setGeometry(QRect(60, 160, 251, 291));
        QFont font;
        font.setFamilies({QString::fromUtf8("Titillium Web")});
        font.setPointSize(8);
        plainTextEdit->setFont(font);
        plainTextEdit->setStyleSheet(QString::fromUtf8("background-image: url(:/images/IMAGES/blanco.jpg);\n"
"self.mi_plaintextedit.setStyleSheet(\"QPlainTextEdit { border: 0px solid transparent; }\")"));
        plainTextEdit->setFrameShape(QFrame::Shape::NoFrame);
        plainTextEdit->setFrameShadow(QFrame::Shadow::Plain);
        listWidget = new QListWidget(Dialog);
        listWidget->setObjectName("listWidget");
        listWidget->setGeometry(QRect(430, 160, 291, 271));
        listWidget->setStyleSheet(QString::fromUtf8("background-image: url(:/images/IMAGES/NOWP.png);"));
        listWidget->setFrameShape(QFrame::Shape::NoFrame);
        listWidget->setFrameShadow(QFrame::Shadow::Plain);
        btnSalir = new QPushButton(Dialog);
        btnSalir->setObjectName("btnSalir");
        btnSalir->setGeometry(QRect(116, 10, 20, 21));
        btnSalir->setStyleSheet(QString::fromUtf8("background-image: url(:/images/IMAGES/NOWP.png);"));
        btnSalir->setFlat(true);
        btnProd = new QPushButton(Dialog);
        btnProd->setObjectName("btnProd");
        btnProd->setGeometry(QRect(150, 10, 20, 21));
        btnProd->setStyleSheet(QString::fromUtf8("background-image: url(:/images/IMAGES/NOWP.png);"));
        btnProd->setFlat(true);
        btnSave = new QPushButton(Dialog);
        btnSave->setObjectName("btnSave");
        btnSave->setGeometry(QRect(180, 10, 20, 21));
        btnSave->setStyleSheet(QString::fromUtf8("background-image: url(:/images/IMAGES/NOWP.png);"));
        btnSave->setFlat(true);
        label = new QLabel(Dialog);
        label->setObjectName("label");
        label->setGeometry(QRect(430, 430, 281, 31));
        label->setStyleSheet(QString::fromUtf8("background-image: url(:/images/IMAGES/NOWP.png);"));
        btnRoute = new QPushButton(Dialog);
        btnRoute->setObjectName("btnRoute");
        btnRoute->setGeometry(QRect(580, 470, 111, 41));
        btnRoute->setStyleSheet(QString::fromUtf8("background-image: url(:/images/IMAGES/NOWP.png);"));
        btnRoute->setFlat(true);
        btnNewFile = new QPushButton(Dialog);
        btnNewFile->setObjectName("btnNewFile");
        btnNewFile->setGeometry(QRect(460, 470, 91, 41));
        btnNewFile->setStyleSheet(QString::fromUtf8("background-image: url(:/images/IMAGES/NOWP.png);"));
        btnNewFile->setFlat(true);

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "PokeCompilator", nullptr));
#if QT_CONFIG(tooltip)
        btnSalir->setToolTip(QCoreApplication::translate("Dialog", "Salir del Programa", nullptr));
#endif // QT_CONFIG(tooltip)
        btnSalir->setText(QString());
#if QT_CONFIG(tooltip)
        btnProd->setToolTip(QCoreApplication::translate("Dialog", "Crear archivo de produccion", nullptr));
#endif // QT_CONFIG(tooltip)
        btnProd->setText(QString());
#if QT_CONFIG(tooltip)
        btnSave->setToolTip(QCoreApplication::translate("Dialog", "Guardar el archivo abierto", nullptr));
#endif // QT_CONFIG(tooltip)
        btnSave->setText(QString());
        label->setText(QString());
#if QT_CONFIG(tooltip)
        btnRoute->setToolTip(QString());
#endif // QT_CONFIG(tooltip)
        btnRoute->setText(QCoreApplication::translate("Dialog", "Seleccionar \n"
"diectorio", nullptr));
        btnNewFile->setText(QCoreApplication::translate("Dialog", "Crear nuevo \n"
"archivo .pie", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
