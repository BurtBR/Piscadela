#include "workercamera.h"

WorkerCamera::WorkerCamera(QObject *parent) : QObject{parent}{

    timer1S = new QTimer(this);
    timer1S->setTimerType(Qt::PreciseTimer);

    connect(timer1S, &QTimer::timeout, this, &WorkerCamera::Timer1STimeout);
}

WorkerCamera::~WorkerCamera(){
    if(timer1S){
        delete timer1S;
        timer1S = nullptr;
    }

    if(mediasession){
        delete mediasession;
        mediasession = nullptr;
    }

    if(camera){
        delete camera;
        camera = nullptr;
    }
}

void WorkerCamera::StartCamera(){

    QVideoSink *sink = nullptr;

    try{
        mediasession = new QMediaCaptureSession(this);
    }catch(...){
        emit Message("Failed to start camera");
        return;
    }

    try{
        sink = new QVideoSink(this);
    }catch(...){
        delete mediasession;
        mediasession = nullptr;
        emit Message("Failed to start camera");
        return;
    }

    try{
        camera = new QCamera(QCameraDevice::Position::FrontFace);
    }catch(...){
        delete mediasession;
        mediasession = nullptr;
        delete sink;
        sink = nullptr;
        emit Message("Failed to start camera");
        return;
    }

    mediasession->setVideoSink(sink);
    mediasession->setCamera(camera);

    camera->start();

    connect(mediasession, &QMediaCaptureSession::destroyed, sink, &QVideoSink::deleteLater);
    connect(sink, &QVideoSink::videoFrameChanged, this, &WorkerCamera::FrameReady);

    timer1S->start(1000);
}

void WorkerCamera::FrameReady(QVideoFrame frame){

    QImage frameimage = frame.toImage();
    QPainter p;

    if(p.begin(&frameimage)){
        p.setPen(QPen(Qt::red));
        p.setFont(QFont("Times", 30));
        p.drawText(10, 40, "FPS: " + QString::number(fps));
        p.end();
    }

    framecounter++;

    emit PresentFrame(QPixmap::fromImage(frameimage));
}

void WorkerCamera::Timer1STimeout(){
    fps = framecounter;
    framecounter = 0;
}

QImage WorkerCamera::_debugFrameTest(){
    QImage frame = QImage(1280, 720, QImage::Format_RGB32);

    for(int j=0; j<720 ;j++){
        for(int i=0; i<1280 ;i++){
            frame.setPixel(i, j, qRgb((i+j)%256, (i-j)%256, (i*j)%256));
        }
    }

    framecounter++;

    return frame;
}
