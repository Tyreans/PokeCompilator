#include "dialog.h"
#include "ui_dialog.h"
#include <filesystem>
#include <QListWidgetItem>
#include <QIcon>
#include <QMessageBox>
#include <QFile>
#include <QFontDatabase>
#include <QFont>
#include <QDebug>
#include <QInputDialog>
#include <QTextStream>
#include <QDir>
#include <QFileDialog>
#include "RegistradorConsola.h"
#include "Lexico.h"
#include "Sintactico.h"
#include "TablaSimbolos.h"
#include "Pila.h"

namespace fs = std::filesystem;

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    changeBackground(true);
    ui->setupUi(this);
    int fontId = QFontDatabase::addApplicationFont(":/fonts/FONTS/Minecraft.ttf");
    if(fontId != -1){
        QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
        QFont pixelFont(fontFamily, 8);
        ui->listWidget->setFont(pixelFont);
        ui->plainTextEdit->setFont(pixelFont);
        ui->label->setFont(pixelFont);
        ui->btnRoute->setFont(pixelFont);
        ui->btnNewFile->setFont(pixelFont);
    }else{
        qDebug() << "NU, ERROR CON LA FUENTE";
    }
    cargarDirectorioVisual(carpetaRaiz);
    ui->plainTextEdit->setReadOnly(true);
    coloresTipado = new resaltador(ui->plainTextEdit->document());
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::cargarDirectorioVisual(const QString& rutaCarpeta){
    ui->listWidget->clear();
    ui->listWidget->setIconSize(QSize(24,24));

    try{
        for (const auto& entrada: fs::directory_iterator(rutaCarpeta.toStdString())){
            if(entrada.is_regular_file()){
                fs::path archivo = entrada.path();
                std::string extension = archivo.extension().string();
                std::string nombreArchivo = archivo.filename().string();

                QIcon icono;
                bool esArchivoValido = false;

                if(extension == ".pie"){
                    icono = QIcon(":/images/IMAGES/pokemon.png");
                    esArchivoValido = true;
                }else if(extension == ".prod"){
                    icono = QIcon(":/images/IMAGES/avatar.png");
                    esArchivoValido = true;
                }

                if(esArchivoValido){
                    QListWidgetItem* item = new QListWidgetItem(icono, QString::fromStdString(nombreArchivo));
                    item->setData(Qt::UserRole, QString::fromStdString(archivo.string()));
                    ui->listWidget->addItem(item);
                }
            }
        }
    }catch(const fs::filesystem_error& e){
        QMessageBox::critical(this, "Error", "No se pudo abrir la carpeta");
    }
}

void Dialog::changeBackground(bool b)
{
    if(b){
        this->setStyleSheet("Dialog { background-image: url(:/images/IMAGES/MAIN.png); }");
    }else{
        this->setStyleSheet("Dialog { background-image: url(:/images/IMAGES/MAIN2.png); }");
    }
}

void Dialog::on_listWidget_itemClicked(QListWidgetItem *item){
    rutaArchivoAbierto = item->data(Qt::UserRole).toString();
    QFile archivo(rutaArchivoAbierto);
    if(!archivo.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::warning(this, "Error", "No se pudo abrir el archivo: "+archivo.errorString());
        return;
    }
    QTextStream entrada(&archivo);
    entrada.setEncoding(QStringConverter::Encoding::Utf8);
    QString contenidoCompleto = entrada.readAll();
    archivo.close();
    ui->plainTextEdit->setPlainText(contenidoCompleto);
    if(rutaArchivoAbierto.endsWith(".pie")){
        ui->plainTextEdit->setReadOnly(false);
        coloresTipado->activar(true);
    }else if(rutaArchivoAbierto.endsWith(".prod")){
        ui->plainTextEdit->setReadOnly(true);
        coloresTipado->activar(false);
    }
    changeBackground(true);
}

void Dialog::on_btnSalir_clicked()
{
    exit(0);
}


void Dialog::on_btnProd_clicked()
{
    if(rutaArchivoAbierto.isEmpty()){
        ui->label->setText("No se ha seleccionado\nningun .pie");
        cargarDirectorioVisual(carpetaRaiz);
        return;
    }
    RegistradorConsola logs;
    logs.appendLine("PokeCompilator - Analizador para Basic pie");
    logs.appendLine("Basic Modificado para aceptar operadores logicos modernos");
    logs.appendLine("Tabla_M y Gramatica creada por LuisPies y Tyreans");
    QString texto_a_analizar = ui->plainTextEdit->toPlainText();
    Lexico lexico;
    lexico.analizar(texto_a_analizar.toStdString(), &logs);
    const auto &tokens = lexico.tokens();
    logs.appendLine(QString("\nTOKENS RECONOCIDOS (%1)").arg(tokens.size()));
    for(size_t i = 0; i < tokens.size(); ++i)
        logs.appendLine(QString(" [%1] %2").arg(static_cast<int>(i)).arg(QString::fromStdString(tokenTypeToString(tokens[i].type))));

    TablaSimbolos tabla;
    Sintactico sintactico(tabla, logs);
    if(sintactico.analizar(tokens)) {
        logs.appendLine("\nTABLA DE SIMBOLOS:");
        for(const auto &entry : tabla.entradas())
            logs.appendLine(QString("  %1 : %2").arg(QString::fromStdString(entry.first)).arg(QString::fromStdString(entry.second)));
    }


    QString fileName = rutaArchivoAbierto.chopped(4);
    fileName += ".prod";

    QString rutaCompleta = QDir(carpetaRaiz).filePath(fileName);
    QFile archivo(rutaCompleta);

    if (archivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream salida(&archivo);
        salida.setEncoding(QStringConverter::Encoding::Utf8);

        salida << logs.texto();

        archivo.close();

        ui->label->setText(fileName+"\ncreado exitosamente");
        cargarDirectorioVisual(carpetaRaiz);

    } else {
        ui->label->setText("Ups! No se pudo crear el archivo");
    }

}


void Dialog::on_btnSave_clicked()
{
    if(rutaArchivoAbierto.isEmpty()){
        ui->label->setText("No hay un archivo seleccionado para guardar");
        return;
    }
    if(rutaArchivoAbierto.endsWith(".prod")){
        QMessageBox::warning(this, "Error", "Sin permisos de modificar las producciones del PokeCompilator");
        return;
    }
    QFile archivo(rutaArchivoAbierto);
    if(!archivo.open(QIODevice::WriteOnly | QIODevice::Text)){
        ui->label->setText("HOLA MUNDO");
        return;
    }
    QTextStream salida(&archivo);
    salida.setEncoding(QStringConverter::Encoding::Utf8);
    salida << ui->plainTextEdit->toPlainText();
    archivo.close();
    QString saveString= "Archivo Guardado en \n" + rutaArchivoAbierto;
    ui->label->setText(saveString);
    changeBackground(true);
}


void Dialog::on_btnNewFile_clicked()
{
    bool ok;
    QString fileName = QInputDialog::getText(this,
                                             "Nuevo Archivo",
                                             "Ingrese el nombre del archivo",
                                             QLineEdit::Normal,
                                             "nuevo",
                                             &ok);
    if(ok && !fileName.isEmpty()){
        if(!fileName.endsWith(".pie")){
            fileName += ".pie";
        }

        QString rutaCompleta = QDir(carpetaRaiz).filePath(fileName);
        QFile archivo(rutaCompleta);

        if (archivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream salida(&archivo);
            salida.setEncoding(QStringConverter::Encoding::Utf8);

            salida << "STRING$ = \"HOLA MUNDO\"\nPRINT \"IMPRIMIENDO: \" ; STRING$";

            archivo.close();

            ui->label->setText(fileName+" creado exitosamente");
            cargarDirectorioVisual(carpetaRaiz);

        } else {
            ui->label->setText("Ups! No se pudo crear el archivo");
        }
    }
}


void Dialog::on_btnRoute_clicked()
{
    QString dirSeleccionado  = QFileDialog::getExistingDirectory(this,
                                                            tr("Seleccione una carpeta"),
                                                            "home/tyreans",
                                                            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
                                                            );
    if(!dirSeleccionado.isEmpty()){
        if(!dirSeleccionado.endsWith("/")){
            dirSeleccionado += "/";
        }
        this->carpetaRaiz = dirSeleccionado;
        cargarDirectorioVisual(carpetaRaiz);
        ui->label->setText("Carpeta\n"+carpetaRaiz+"\nseleccionada");
    }
}


void Dialog::on_plainTextEdit_textChanged()
{
    changeBackground(false);
}

