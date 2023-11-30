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

    unsigned int linecolor, samebitcounter = 1;
    bool lastbit;
    QString framecode;
    qsizetype indexofheader;

    if((frame.pixel(frame.width()/2, 0)&0xFF) > selectionthreshold)
        linecolor = 0xFFFFFFFF;
    else
        linecolor = 0;

    for(int i=0; i<frame.width() ;i++)
        frame.setPixelColor(i, 0, linecolor);

    lastbit = linecolor;

    for(int j=1; j<frame.height() ;j++){
        if((frame.pixel(frame.width()/2, j)&0xFF) > selectionthreshold)
            linecolor = 0xFFFFFFFF;
        else
            linecolor = 0;

        for(int i=0; i<frame.width() ;i++)
            frame.setPixelColor(i, j, linecolor);

        if(lastbit != ((bool)linecolor)){
            if(lastbit)
                samebitcounter = qRound((double)samebitcounter/(double)whiteheight);
            else
                samebitcounter = qRound((double)samebitcounter/(double)blackheight);

            framecode.append(QString(QChar('0'+lastbit)).repeated(samebitcounter));

            lastbit = linecolor;
            samebitcounter = 0;
        }
        samebitcounter++;
    }

    if(samebitcounter > 1){
        if(lastbit)
            samebitcounter = qRound((double)samebitcounter/(double)whiteheight);
        else
            samebitcounter = qRound((double)samebitcounter/(double)blackheight);

        framecode.append(QString(QChar('0'+lastbit)).repeated(samebitcounter));
    }

    if(!topbottom)
        ReverseStr(framecode);

    indexofheader = framecode.indexOf("11100110");

    if(debugmode){
        emit Message(framecode);
        if(indexofheader != -1){
            emit Message("\t Detectado: ");
            framecode.remove(0, indexofheader+8);
            framecode.remove(8, framecode.size());
            emit Message(QChar(framecode.toInt(nullptr, 2)));
        }
        emit Message("\n");
    }else{
        if((indexofheader != -1) && codebegun){
            framecode.remove(0, indexofheader+8);
            framecode.remove(8, framecode.size());
            if(framecode.toInt(nullptr, 2) == lastsymbol){
                symbolcounter++;
            }else{
                if(lastsymbol == 0xFFFF){
                    emit Message("\n");
                }else if(qRound((double)symbolcounter/(double)frameavg) > 0){
                    emit Message(QString(QChar(lastsymbol)).repeated(qRound((double)symbolcounter/(double)frameavg)));
                }
                lastsymbol = framecode.toInt(nullptr, 2);
                symbolcounter = 1;
            }
        }else{

            if(codebegun && lastsymbol != 0xFFFF)
                emit Message(QString(QChar(lastsymbol)).repeated(qRound((double)symbolcounter/(double)frameavg)));

            if(framecode.indexOf("11111111111111111111111111") != -1){
                codebegun = true;
                lastsymbol = 0xFFFF;
            }
        }
    }
}

void WorkerCamera::ReverseStr(QString &str){
    QChar charaux;
    int begin=0, end=(str.size()-1);

    while(begin<end){
        charaux = str[begin];
        str[begin] = str[end];
        str[end] = charaux;

        begin++;
        end--;
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
    }else
        codebegun = false;

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

void WorkerCamera::StartCoding(bool istopbottom, unsigned int frameaverage, unsigned char threshold, unsigned int blacksize, unsigned int whitesize){
    coding = !coding;

    topbottom = istopbottom;
    frameavg = frameaverage;
    selectionthreshold = threshold;
    blackheight = blacksize;
    whiteheight = whitesize;
}

void WorkerCamera::SwapCamera(){

    const QList<QCameraDevice> availablecameras = QMediaDevices::videoInputs();

    if(availablecameras.size() < 2){
        if(!availablecameras.size()){
            emit Message("Não existe câmera disponível");
            return;
        }

        if(camera->cameraDevice() == availablecameras[0]){
            emit Message("Não existe outra câmera disponível");
            return;
        }
        camera->setCameraDevice(availablecameras[0]);
    }else if(camera->cameraDevice() == availablecameras[0]){
        camera->setCameraDevice(availablecameras[1]);
    }else{
        camera->setCameraDevice(availablecameras[0]);
    }
}

void WorkerCamera::CameraSetTopBottom(bool istopbottom){
    topbottom = istopbottom;
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

void WorkerCamera::CameraSetCalibration(bool value){
    debugmode = value;
}
