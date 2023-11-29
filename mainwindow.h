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

private slots:
    void PrintMessage(QString text);
    void PresentFrame(QPixmap frame);
    void WorkerMessage(QString message);
    void On_buttonSwapCamera_clicked();
    void On_buttonDecode_clicked();
    void On_textCoderUser_textChanged();
};

#endif // MAINWINDOW_H
