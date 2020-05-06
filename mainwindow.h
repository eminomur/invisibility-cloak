#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "frameprovider.h"

#include <opencv2/core.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void get_new_frame(const cv::Mat&);

private:
    Ui::MainWindow *ui;

    FrameProvider *frame_provider;
};
#endif // MAINWINDOW_H
