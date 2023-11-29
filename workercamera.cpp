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

void WorkerCamera::ProcessFrame(QImage &frame){

    unsigned int linecolor;

    for(int j=0; j<frame.height() ;j++){
        if((frame.pixel(frame.width()/2, j)&0xFF) > selectionthreshold)
            linecolor = 0xFFFFFFFF;
        else
            linecolor = 0;

        for(int i=0; i<frame.width() ;i++)
            frame.setPixelColor(i, j, linecolor);
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

    if(coding){
        frameimage.convertTo(QImage::Format_Grayscale8);
        ProcessFrame(frameimage);
    }

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

void WorkerCamera::StartCoding(unsigned int frameaverage, unsigned char threshold, unsigned int blacksize, unsigned int whitesize){
    coding = !coding;

    frameavg = frameaverage;
    selectionthreshold = threshold;
    blackheight = blacksize;
    whiteheight = whitesize;
}

void WorkerCamera::CameraSetFrameaverage(unsigned int value){
    frameavg = value;
}

void WorkerCamera::CameraSetThreshold(unsigned int value){
    selectionthreshold = value;
}

void WorkerCamera::CameraSetBlacksize(unsigned int value){
    blackheight = value;
}

void WorkerCamera::CameraSetWhitesize(unsigned int value){
    whiteheight = value;
}
