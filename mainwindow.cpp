#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

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

void MainWindow::on_action_about_invisibility_cloak_triggered()
{
    QMessageBox::about(this, "About Invisibility Cloak", "An Invisibility Cloak Implementation\n"
                                                         "You are free to modify as you like\n\n"
                                                         "Muhammed Emin ÖMÜR - Software Engineer");
}
