#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[]){
    QApplication *a;
    MainWindow *w;

    try{
        a = new QApplication(argc, argv);
    }catch(...){
        QMessageBox(QMessageBox::Warning, "Erro", "Falha ao inicializar").exec();
        return 0;
    }

    try{
        w = new MainWindow();
    }catch(...){
        QMessageBox(QMessageBox::Warning, "Erro", "Falha ao inicializar").exec();
        return 0;
    }

    w->showMaximized();
    a->exec();

    delete a;
    delete w;

    return 0;
}
