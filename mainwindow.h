#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "frameprovider.h"
#include "effectapplier.h"

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
    void get_new_frame(cv::Mat&);
    void on_action_about_invisibility_cloak_triggered();
    void on_start_stop_push_button_clicked();
    void on_choose_roi_push_button_clicked();
    void on_load_bg_push_button_clicked();
    void on_save_bg_push_button_clicked();
private:
    Ui::MainWindow *ui;

    FrameProvider *frame_provider;
    EffectApplier *effect_applier;

    bool effect_apply_flag;
    bool save_background_flag;
    bool roi_flag;
};
#endif // MAINWINDOW_H
