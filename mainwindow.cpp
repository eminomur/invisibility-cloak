#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "app_config.h"

#include <QMessageBox>
#include <QFileDialog>

#include <opencv2/highgui.hpp>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      effect_apply_flag(false),
      save_background_flag(false),
      roi_flag(false)
{
    ui->setupUi(this);
    qRegisterMetaType<cv::Mat>("cv::Mat&");
    frame_provider = new FrameProvider(0, this);
    effect_applier = new EffectApplier;
    connect(frame_provider, &FrameProvider::new_frame_ready, this, &MainWindow::get_new_frame);
}

MainWindow::~MainWindow()
{
    delete effect_applier;
    delete ui;
}

void MainWindow::get_new_frame(cv::Mat& new_frame)
{
    if (effect_apply_flag) {
        effect_applier->apply(new_frame);
    } else if (roi_flag) {
        roi_flag = false;
        cv::Mat frame_for_roi_operation = new_frame.clone();
        cv::namedWindow("Choose ROI and Press Enter");
        cv::Rect roi;
        while ((roi = cv::selectROI("Choose ROI and Press Enter", frame_for_roi_operation)).empty() == true);
        cv::destroyWindow("Choose ROI and Press Enter");
        effect_applier->set_min_max(frame_for_roi_operation(roi));
        ui->choose_roi_push_button->setEnabled(true);
        ui->start_stop_push_button->setEnabled(true);
    } else if (save_background_flag) {
        save_background_flag = false;
        cv::imwrite("bg.jpg", new_frame);
        ui->save_bg_push_button->setEnabled(true);
    }
    ui->frame_label->setPixmap(QPixmap::fromImage(QImage(new_frame.data,
                                                         FRAME_WIDTH, FRAME_HEIGHT, QImage::Format_BGR888)));
}

void MainWindow::on_action_about_invisibility_cloak_triggered()
{
    QMessageBox::about(this, "About Invisibility Cloak", "An Invisibility Cloak Implementation.\n"
                                                         "You are free to modify as you like.\n\n"
                                                         "Icon made by Freepik from www.flaticon.com\n\n"
                                                         "Muhammed Emin ÖMÜR - Software Engineer");
}

void MainWindow::on_start_stop_push_button_clicked()
{
    if (!roi_flag && effect_applier->is_initialized()) {
        effect_apply_flag = !effect_apply_flag;
    } else {
        QMessageBox::critical(this, "Not Initialized", "A background image and roi must be provided to do that.");
    }
}

void MainWindow::on_choose_roi_push_button_clicked()
{
    ui->choose_roi_push_button->setEnabled(false);
    ui->start_stop_push_button->setEnabled(false);
    effect_apply_flag = false;
    roi_flag = true;
}

void MainWindow::on_load_bg_push_button_clicked()
{
    effect_apply_flag = false;
    QString file_filter = "Image Files (*bmp *.dib *.jpeg *.jpg *.jpe *.jp2 *.png *.pbm *.pgm *.ppm *.sr *.ras *.tiff)";
    QString image_file_name = QFileDialog::getOpenFileName(this, "Select a background image",
                                                           QDir::homePath(), file_filter);
    if (!image_file_name.isEmpty()) {
        effect_applier->set_background(image_file_name.toStdString());
    } else {
        QMessageBox::critical(this, "Empty", "Background image must be selected!");
    }
}

void MainWindow::on_save_bg_push_button_clicked()
{
    ui->save_bg_push_button->setEnabled(false);
    QTimer::singleShot(SAVE_BACKGROUND_TIMEOUT_MILLISECONDS, this, [this]() {
        save_background_flag = true;
    });
}
