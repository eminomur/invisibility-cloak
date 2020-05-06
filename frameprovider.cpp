#include "frameprovider.h"

#include <opencv2/imgproc.hpp>

FrameProvider::FrameProvider(int index, QWidget *parent)
    : QWidget(parent), video_capture(index), shooting_flag(true)
{
    if (!video_capture.isOpened()) {
        throw CameraNotFoundException();
    }

    frame_provider_thread = QThread::create([&]() {
        cv::Mat frame;
        while (shooting_flag) {
            video_capture >> frame;
            cv::resize(frame, frame, cv::Size(600, 600), 0, 0, cv::INTER_AREA);
            cv::flip(frame, frame, 1);
            emit new_frame_ready(frame);
        }
    });

    connect(frame_provider_thread, &QThread::finished, frame_provider_thread, &QThread::deleteLater);
    frame_provider_thread->start();
}

FrameProvider::~FrameProvider()
{
    shooting_flag = false;
    frame_provider_thread->wait();
}
