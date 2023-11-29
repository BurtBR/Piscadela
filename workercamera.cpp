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
}

void WorkerCamera::StartCamera(){

    QCamera *camera = nullptr;
    QVideoSink *sink = nullptr;

    try{
        mediasession = new QMediaCaptureSession(this);
    }catch(...){
        return;
    }

    try{
        sink = new QVideoSink(this);
    }catch(...){
        delete mediasession;
        mediasession = nullptr;
    }

    try{
        camera = new QCamera(QCameraDevice::Position::FrontFace);
    }catch(...){
        delete mediasession;
        mediasession = nullptr;
        delete sink;
        sink = nullptr;
        return;
    }

    mediasession->setVideoSink(sink);
    mediasession->setCamera(camera);

    camera->start();

    connect(mediasession, &QMediaCaptureSession::destroyed, camera, &QCamera::deleteLater);
    connect(mediasession, &QMediaCaptureSession::destroyed, sink, &QVideoSink::deleteLater);
    connect(sink, &QVideoSink::videoFrameChanged, this, &WorkerCamera::FrameReady);

    timer1S->start(1000);
}

void WorkerCamera::FrameReady(QVideoFrame frame){

    emit PresentFrame(QPixmap::fromImage(frame.toImage()));
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
