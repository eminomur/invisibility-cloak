#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qRegisterMetaType<cv::Mat>("const cv::Mat&");
    frame_provider = new FrameProvider(0, this);

    connect(frame_provider, &FrameProvider::new_frame_ready, this, &MainWindow::get_new_frame);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::get_new_frame(const cv::Mat& new_frame)
{
    ui->frame_label->setPixmap(QPixmap::fromImage(QImage(new_frame.clone().data, new_frame.cols, new_frame.rows, QImage::Format_BGR888)));
}
