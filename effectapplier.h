#ifndef EFFECTAPPLIER_H
#define EFFECTAPPLIER_H

#include <opencv2/core.hpp>

#include <array>
#include <string>

class EffectApplier
{
public:
    EffectApplier();

    void set_min_max(const cv::Mat&);
    void set_background(const std::string&);

    bool is_initialized() const;

    void apply(cv::Mat&);
private:
    cv::Mat background;

    // Minimum and maximum values to decide if corresponding pixel of the image
    // stored in background member variable should be used
    std::array<short, 3> min;
    std::array<short, 3> max;

    bool min_max_initialized_flag;
    bool bg_initialized_flag;

    void init_min_max();
};

#endif // EFFECTAPPLIER_H
