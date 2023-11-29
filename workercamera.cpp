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
}

void WorkerCamera::StartCamera(){
    timer1S->start(1000);
}

void WorkerCamera::FrameReady(QImage frame){

}

void WorkerCamera::Timer1STimeout(){
    fps = framecounter;
    framecounter = 0;

    emit PresentFrame(QPixmap::fromImage(_debugFrameTest()));
}

QImage WorkerCamera::_debugFrameTest(){
    QImage frame = QImage(1280, 720, QImage::Format_RGB32);

    for(int j=0; j<720 ;j++){
        for(int i=0; i<1280 ;i++){
            frame.setPixel(i, j, qRgb(i%255, j%255, (i*j)%255));
        }
    }

    return frame;
}
