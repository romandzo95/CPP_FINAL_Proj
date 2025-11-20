#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <chrono>
#include <QTimer>
#include <future>
#include <kollatz.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->threadSlider->setMinimum(1);
    ui->threadSlider->setMaximum(std::thread::hardware_concurrency());
    ui->textEdit2->setText(QString::number(ui->threadSlider->value()));
    connect(ui->threadSlider, &QSlider::valueChanged, this, [=](int value) {
        ui->textEdit2->setText(QString::number(value));
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_StartButton_clicked(){
    ui->StartButton->setEnabled(false);
    ui->StopButton->setEnabled(true);
    ui->textEdit->clear();
    stopRequested=false;
    unsigned long long n = ui->spinBox->value();
    int threadsCount = ui->threadSlider->value();
    std::thread([this, n, threadsCount]() { runKollatz(n, threadsCount); }).detach();
}

void MainWindow::on_StopButton_clicked(){
    stopRequested=true;
    ui->StartButton->setEnabled(true);
    ui->StopButton->setEnabled(false);
}

void MainWindow::updateOutput(const QString& text){
    ui->textEdit->append(text);
}

void MainWindow::runKollatz(unsigned long long n, int threadsCount){
    globalCounter.store(1);
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::future<KollatzRes>> futures;
    for (int i = 0; i < threadsCount; i++)
    {
        futures.push_back(std::async(std::launch::async,KollatzFunc, n , std::ref(stopRequested)));
    }
    KollatzRes maxRes{0, 0};
    bool overflowError=false;
    for (auto &f : futures)
    {
        if (stopRequested.load())
        {
            break;
        }
        KollatzRes res = f.get();
        if(res.length==0){
            overflowError=true;
            stopRequested=true;
            break;
        }
        if (res.length > maxRes.length)
        {
            maxRes = res;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    QString output;
    if(overflowError){
        output = "ERROR: the number is bigger than ull max!";
    }
    else if (stopRequested.load())
    {
        output = "Calculations are stopped!";
    }
    else
    {
        output = "Number with the longest sequence: " + QString::number(maxRes.num) +
                 "\nLength: " + QString::number(maxRes.length) +
                 "\nTime: " + QString::number(duration.count()) + " ms";
    }

    QMetaObject::invokeMethod(this, "updateOutput", Qt::QueuedConnection, Q_ARG(QString, output));

    QTimer::singleShot(0, this, [=]() {
        ui->StartButton->setEnabled(true);
        ui->StopButton->setEnabled(false);
    });
}
