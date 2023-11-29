#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    this->setWindowTitle(this->windowTitle() + " (" + QString(GIT_COMMIT_HASH) + ")");

    connect(ui->buttonDecode, &QPushButton::clicked, this, &MainWindow::On_buttonDecode_clicked);
    connect(ui->buttonSwapCamera, &QToolButton::clicked, this, &MainWindow::On_buttonSwapCamera_clicked);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::On_buttonSwapCamera_clicked(){
    ui->textConsole->append("Troca troca de câmera");
}

void MainWindow::On_buttonDecode_clicked(){
    ui->textConsole->append("Decodificamento");
}
