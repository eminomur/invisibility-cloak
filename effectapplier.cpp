#include "effectapplier.h"

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "app_config.h"

EffectApplier::EffectApplier()
    : min_max_initialized_flag(false),
      bg_initialized_flag(false)
{}

void EffectApplier::set_min_max(const cv::Mat& roi)
{
    init_min_max();
    for (int i = 0, num_of_rows = roi.rows; i < num_of_rows; ++i) {
        for (int j = 0, num_of_cols = roi.cols; j < num_of_cols; ++j) {
            const cv::Vec3b& current_pixel = roi.at<cv::Vec3b>(i, j);
            if (current_pixel[0] < min[0] &&
                current_pixel[1] < min[1] &&
                current_pixel[2] < min[2]) {
                min[0] = current_pixel[0];
                min[1] = current_pixel[1];
                min[2] = current_pixel[2];
            }
            if (current_pixel[0] > max[0] &&
                current_pixel[1] > max[1] &&
                current_pixel[2] > max[2]) {
                max[0] = current_pixel[0];
                max[1] = current_pixel[1];
                max[2] = current_pixel[2];
            }
        }
    }
    for (auto& a : min) {
        if (a <= MAX_THRESHOLD) {
            a = 0;
        } else {
            a -= MAX_THRESHOLD;
        }
    }
    for (auto& a : max) {
        if (255 - a <= MAX_THRESHOLD) {
            a = 255;
        } else {
            a += MAX_THRESHOLD;
        }
    }
    min_max_initialized_flag = true;
}

void EffectApplier::set_background(const std::string& file_name)
{
    cv::Mat bg = cv::imread(file_name);
    cv::resize(bg, background, cv::Size(FRAME_WIDTH, FRAME_HEIGHT), 0, 0, cv::INTER_AREA);
    bg_initialized_flag = true;
}

bool EffectApplier::is_initialized() const
{
    return min_max_initialized_flag && bg_initialized_flag;
}

void EffectApplier::apply(cv::Mat& frame)
{
    for (int i = 0; i < FRAME_HEIGHT; ++i) {
        for (int j = 0; j < FRAME_WIDTH; ++j) {
            cv::Vec3b& current_pixel = frame.at<cv::Vec3b>(i, j);
            if ((min[0] <= current_pixel[0]) && (current_pixel[0] <= max[0]) &&
                (min[1] <= current_pixel[1]) && (current_pixel[1] <= max[1]) &&
                (min[2] <= current_pixel[2]) && (current_pixel[2] <= max[2])) {
                current_pixel = background.at<cv::Vec3b>(i, j);
            }
        }
    }
}

void EffectApplier::init_min_max()
{
    for (int i = 0, arr_size = min.size(); i < arr_size; ++i) {
        min[i] = 255;
        max[i] = 0;
    }
}
