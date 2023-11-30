#ifndef WORKERCAMERA_H
#define WORKERCAMERA_H

#include <QObject>
#include <QCamera>
#include <QMediaCaptureSession>
#include <QMediaDevices>
#include <QVideoSink>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QTimer>

// TEMPORARIO
#include <QDebug>
// TEMPORARIO

class WorkerCamera : public QObject{
    Q_OBJECT
private:
    QMediaCaptureSession *mediasession = nullptr;
    QCamera *camera = nullptr;
    QTimer *timer1S = nullptr;
    uint16_t lastsymbol;
    unsigned int framecounter = 0;
    unsigned int fps = 0;
    unsigned int symbolcounter = 0;
    bool coding = false;
    bool topbottom = true;
    bool debugmode = false;
    bool codebegun = false;
    unsigned char selectionthreshold = 70;
    unsigned int frameavg = 3;
    unsigned int blackheight = 15;
    unsigned int whiteheight = 18;

    // Provate methods
    void ProcessFrame(QImage &frame);
    void ReverseStr(QString &str);

public:
    explicit WorkerCamera(QObject *parent = nullptr);
    ~WorkerCamera();

signals:
    void PresentFrame(QPixmap frame);
    void Message(QString message);

private slots:
    void FrameReady(QVideoFrame frame);
    void Timer1STimeout();

public slots:
    void StartCamera();
    void StartCoding(bool istopbottom = true, unsigned int frameaverage = 3, unsigned char threshold = 70, unsigned int blacksize = 13, unsigned int whitesize = 18);
    void SwapCamera();
    void CameraSetTopBottom(bool istopbottom = true);
    void CameraSetFrameaverage(unsigned int value);
    void CameraSetThreshold(unsigned int value);
    void CameraSetBlacksize(unsigned int value);
    void CameraSetWhitesize(unsigned int value);
    void CameraSetCalibration(bool value);
};

#endif // WORKERCAMERA_H
