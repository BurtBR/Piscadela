#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    this->setWindowTitle(this->windowTitle() + " (" + QString(GIT_COMMIT_HASH) + ")");

    try{
        if(!StartThreadCamera()){
            throw;
            return;
        }
    }catch(...){
        throw "Failed to allocate memory for threadCamera";
        return;
    }

    connect(ui->buttonDecode, &QPushButton::clicked, this, &MainWindow::On_buttonDecode_clicked);
    connect(ui->buttonSwapCamera, &QToolButton::clicked, this, &MainWindow::On_buttonSwapCamera_clicked);
    connect(ui->textCoderUser, &QTextEdit::textChanged, this, &MainWindow::On_textCoderUser_textChanged);
}

MainWindow::~MainWindow(){
    delete ui;

    if(threadCamera){
        threadCamera->quit();
        if(!threadCamera->wait(5000)){
            threadCamera->terminate();
            threadCamera->wait();
        }
        delete threadCamera;
        threadCamera = nullptr;
    }
}

bool MainWindow::StartThreadCamera(){

    if(threadCamera)
        return false;

    WorkerCamera *worker = nullptr;

    try{
        worker = new WorkerCamera;
    }catch(...){
        throw "Failed to allocate memory for threadCamera";
        return false;
    }

    try{
        threadCamera = new QThread(this);
    }catch(...){
        delete worker;
        throw "Failed to allocate memory for threadCamera";
        return false;
    }

    connect(threadCamera, &QThread::finished, worker, &WorkerCamera::deleteLater);
    connect(worker, &WorkerCamera::PresentFrame, this, &MainWindow::PresentFrame);
    connect(this, &MainWindow::StartCamera, worker, &WorkerCamera::StartCamera);

    worker->moveToThread(threadCamera);
    threadCamera->start();

    emit StartCamera();

    return true;
}

void MainWindow::PrintMessage(QString text){
    ui->textConsole->moveCursor (QTextCursor::End);
    ui->textConsole->insertPlainText(text);
}

void MainWindow::PresentFrame(QPixmap frame){
    int w = ui->labelImage->width()-10;
    int h = ui->labelImage->height()-10;

    ui->labelImage->setPixmap(frame.scaled(w, h, Qt::KeepAspectRatio));
}

void MainWindow::On_buttonSwapCamera_clicked(){
    PrintMessage("Troca troca de câmera\n");
}

void MainWindow::On_buttonDecode_clicked(){
    PrintMessage("Pi pi pi po po zaum\n");
}

void MainWindow::On_textCoderUser_textChanged(){

}
