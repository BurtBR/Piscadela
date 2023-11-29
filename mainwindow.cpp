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
    connect(ui->spinFrameAvg, &QSpinBox::valueChanged, this, &MainWindow::On_spinFrameAvg_valueChanged);
    connect(ui->spinThreshold, &QSpinBox::valueChanged, this, &MainWindow::On_spinThreshold_valueChanged);
    connect(ui->spinBlackheight, &QSpinBox::valueChanged, this, &MainWindow::On_spinBlackheight_valueChanged);
    connect(ui->spinWhiteheight, &QSpinBox::valueChanged, this, &MainWindow::On_spinWhiteheight_valueChanged);
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
    connect(worker, &WorkerCamera::Message, this, &MainWindow::WorkerMessage);
    connect(this, &MainWindow::StartCamera, worker, &WorkerCamera::StartCamera);
    connect(this, &MainWindow::StartCoding, worker, &WorkerCamera::StartCoding);

    connect(this, &MainWindow::CameraSetFrameaverage, worker, &WorkerCamera::CameraSetFrameaverage);
    connect(this, &MainWindow::CameraSetThreshold, worker, &WorkerCamera::CameraSetThreshold);
    connect(this, &MainWindow::CameraSetBlacksize, worker, &WorkerCamera::CameraSetBlacksize);
    connect(this, &MainWindow::CameraSetWhitesize, worker, &WorkerCamera::CameraSetWhitesize);

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

void MainWindow::WorkerMessage(QString message){
    PrintMessage(message);
}

void MainWindow::On_buttonSwapCamera_clicked(){
    PrintMessage("Troca troca de câmera\n");
}

void MainWindow::On_buttonDecode_clicked(){
    emit StartCoding(ui->spinFrameAvg->value(),
                     ui->spinThreshold->value(),
                     ui->spinBlackheight->value(),
                     ui->spinWhiteheight->value());
}

void MainWindow::On_textCoderUser_textChanged(){

}

void MainWindow::On_spinFrameAvg_valueChanged(int newvalue){
    emit CameraSetFrameaverage(newvalue);
}

void MainWindow::On_spinThreshold_valueChanged(int newvalue){
    emit CameraSetThreshold(newvalue);
}

void MainWindow::On_spinBlackheight_valueChanged(int newvalue){
    emit CameraSetBlacksize(newvalue);
}

void MainWindow::On_spinWhiteheight_valueChanged(int newvalue){
    emit CameraSetWhitesize(newvalue);
}
