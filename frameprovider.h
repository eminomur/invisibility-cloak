#ifndef FRAMEPROVIDER_H
#define FRAMEPROVIDER_H

#include <QWidget>
#include <QThread>

#include <exception>

#include <opencv2/videoio.hpp>
#include <opencv2/core.hpp>

class CameraNotFoundException : public std::exception
{
public:
    virtual const char *what() const noexcept override
    {
        return "No camera found for given index";
    }
};

class FrameProvider : public QWidget
{
    Q_OBJECT
public:
    explicit FrameProvider(int index = 0, QWidget *parent = nullptr);
    ~FrameProvider();
signals:
    void new_frame_ready(cv::Mat&);
private:
    QThread *frame_provider_thread;
    cv::VideoCapture video_capture;
    bool shooting_flag;
};

#endif // FRAMEPROVIDER_H
