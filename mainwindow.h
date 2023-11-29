#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include "workercamera.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
    Q_OBJECT

private:
    // Private Attributes
    Ui::MainWindow *ui;
    QThread *threadCamera= nullptr;

    // Private Methods
    bool StartThreadCamera();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void StartCamera();
    void StartCoding(unsigned int frameaverage, unsigned char threshold, unsigned int blacksize, unsigned int whitesize);
    void CameraSetFrameaverage(unsigned int value);
    void CameraSetThreshold(unsigned int value);
    void CameraSetBlacksize(unsigned int value);
    void CameraSetWhitesize(unsigned int value);

private slots:
    void PrintMessage(QString text);
    void PresentFrame(QPixmap frame);
    void WorkerMessage(QString message);
    void On_buttonSwapCamera_clicked();
    void On_buttonDecode_clicked();
    void On_textCoderUser_textChanged();
    void On_spinFrameAvg_valueChanged(int newvalue);
    void On_spinThreshold_valueChanged(int newvalue);
    void On_spinBlackheight_valueChanged(int newvalue);
    void On_spinWhiteheight_valueChanged(int newvalue);
};

#endif // MAINWINDOW_H
