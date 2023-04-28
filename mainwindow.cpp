#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QPainter"
#include "QThread"
#include "QTimer"
#include "QKeyEvent"
#include "QFileDialog"
#include "QDebug"
#include "rtweekend.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    int myWidth=960,myHeight=960;
    ui->setupUi(this);
    this->resize(myWidth,myHeight);
    setFixedSize(myWidth,myHeight);
    sc=new scene();
    //sc->model_scene((double)myWidth/myHeight);
    //sc->test((double)myWidth/myHeight);
    //sc->random_scene((double)myWidth/myHeight);
    sc->cornell_scene((double)myWidth/myHeight);
    loop=new RenderRoute(width(),height(),nullptr,sc);
    loopThread=new QThread(this);
    loop->moveToThread(loopThread);
    connect(loopThread,&QThread::finished,loop, &RenderRoute::deleteLater);
    connect(loopThread,&QThread::started,loop,&RenderRoute::loop);
    connect(loop,&RenderRoute::frameOut,this,&MainWindow::receiveFrame);
    loopThread->start();
}

MainWindow::~MainWindow()
{
    delete ui;
    loopThread->quit();
    loopThread->wait();
    if(canvas)delete canvas;
    if(loopThread)delete loopThread;
    loop=nullptr;
    canvas=nullptr;
    loopThread=nullptr;
}

void MainWindow::receiveFrame(unsigned char *data)
{
    //if(canvas != nullptr) delete canvas;
    canvas = new QImage(data, width(), height(), QImage::Format_RGBA8888);
    update();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    if(canvas)
    {
        QPainter painter(this);
        painter.drawImage(0,0,*canvas);
    }
    QWidget::paintEvent(event);
}
