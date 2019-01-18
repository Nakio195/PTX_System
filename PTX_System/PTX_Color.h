#ifndef PTX_COLOR_H
#define PTX_COLOR_H

#include <opencv2/opencv.hpp>
#include <vector>

class PTX_Color
{
    public:
        PTX_Color(unsigned char Hue);

        bool FitHueInterval(int Hue);

    public:
        int LowHue, HighHue;
        unsigned char MainHue;
        std::vector<std::vector<cv::Point>> Polygons;
        std::vector<std::vector<unsigned int>> Objects;
        cv::Mat ColorMatrix;
};

#endif // PTX_COLOR_H
