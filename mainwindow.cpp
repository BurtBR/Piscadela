#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    this->setWindowTitle(this->windowTitle() + " (" + QString(GIT_COMMIT_HASH) + ")");
}

MainWindow::~MainWindow(){
    delete ui;
}

