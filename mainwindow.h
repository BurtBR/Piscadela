#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include "workercamera.h"
#include "workercoder.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
    Q_OBJECT

private:
    // Private Attributes
    Ui::MainWindow *ui;
    QThread *threadCamera= nullptr;
    QThread *threadCoder = nullptr;

    // Private Methods
    bool StartThreadCamera();
    bool StartThreadCoder();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void StartCamera();
    void StartCoding(bool istopbottom, unsigned int frameaverage, unsigned char threshold, unsigned int blacksize, unsigned int whitesize);
    void CameraSwap();
    void CameraSetTopBottom(bool istopbottom = true);
    void CameraSetFrameaverage(unsigned int value);
    void CameraSetThreshold(unsigned int value);
    void CameraSetBlacksize(unsigned int value);
    void CameraSetWhitesize(unsigned int value);
    void Translate(QString text);
    void SetInsertBeginFrame(bool value);
    void SetInsertHeader(bool value);
    void SetInsert0b(bool value);
    void SetInsertComma(bool value);
    void SetCalibration(bool value);

private slots:
    void PrintMessage(QString text);
    void PresentFrame(QPixmap frame);
    void WorkerMessage(QString message);
    void WorkerTranslated(QString text);
    void On_buttonSwapCamera_clicked();
    void On_buttonDecode_clicked();
    void On_buttonClearConsole_clicked();
    void On_textCoderUser_textChanged();
    void On_comboBottomUp_selectionChanged();
    void On_spinFrameAvg_valueChanged(int newvalue);
    void On_spinThreshold_valueChanged(int newvalue);
    void On_spinBlackheight_valueChanged(int newvalue);
    void On_spinWhiteheight_valueChanged(int newvalue);
    void On_checkInsertBeginFrame_Toggled(int value);
    void On_checkInsertHeader_Toggled(int value);
    void On_checkInsert0b_Toggled(int value);
    void On_checkInsertComma_Toggled(int value);
    void On_checkCalibration_Toggled(int value);
};

#endif // MAINWINDOW_H
