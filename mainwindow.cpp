#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    this->setWindowTitle(this->windowTitle() + " (" + QString(GIT_COMMIT_HASH) + ")");

    connect(ui->buttonDecode, &QPushButton::clicked, this, &MainWindow::On_buttonDecode_clicked);
    connect(ui->buttonSwapCamera, &QToolButton::clicked, this, &MainWindow::On_buttonSwapCamera_clicked);
    connect(ui->textCoderUser, &QTextEdit::textChanged, this, &MainWindow::On_textCoderUser_textChanged);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::PrintMessage(QString text){
    ui->textConsole->moveCursor (QTextCursor::End);
    ui->textConsole->insertPlainText(text);
}

void MainWindow::On_buttonSwapCamera_clicked(){
    PrintMessage("Troca troca de câmera\n");
}

void MainWindow::On_buttonDecode_clicked(){
    PrintMessage("Pi pi pi po po zaum\n");
}

void MainWindow::On_textCoderUser_textChanged(){

}
