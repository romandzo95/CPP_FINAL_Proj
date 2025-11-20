#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <thread>
#include <atomic>
#include "kollatz.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    std::atomic<bool> stopRequested{false};
    void runKollatz(unsigned long long n, int threadsCount);

public slots:
    void on_StartButton_clicked();
    void on_StopButton_clicked();
    void updateOutput(const QString& text);
};
#endif // MAINWINDOW_H
