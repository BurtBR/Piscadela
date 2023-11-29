#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
    Q_OBJECT

private:
    Ui::MainWindow *ui;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void PrintMessage(QString text);
    void On_buttonSwapCamera_clicked();
    void On_buttonDecode_clicked();
    void On_textCoderUser_textChanged();
};
#endif // MAINWINDOW_H
