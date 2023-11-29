#ifndef WORKERCAMERA_H
#define WORKERCAMERA_H

#include <QObject>
#include <QCamera>
#include <QMediaCaptureSession>
#include <QVideoSink>
#include <QImage>
#include <QPixmap>
#include <QTimer>

class WorkerCamera : public QObject{
    Q_OBJECT
private:
    QMediaCaptureSession *mediasession = nullptr;
    QTimer *timer1S = nullptr;
    unsigned int framecounter = 0;
    unsigned int fps = 0;

    // Provate methods
    QImage _debugFrameTest();

public:
    explicit WorkerCamera(QObject *parent = nullptr);
    ~WorkerCamera();

signals:
    void PresentFrame(QPixmap frame);

private slots:
    void FrameReady(QVideoFrame frame);
    void Timer1STimeout();

public slots:
    void StartCamera();
};

#endif // WORKERCAMERA_H
