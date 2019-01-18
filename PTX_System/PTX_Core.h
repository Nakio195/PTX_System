#ifndef PTX_CORE_H
#define PTX_CORE_H

#include <iostream>
#include <vector>
#include "opencv2/opencv.hpp"
#include "PTX_Color.h"
#include "PTX_Cam.h"

enum ExitStatus{EXIT_NO_AVAILABLE_CAMERA = 100};

class PTX_Core
{
    public:
        PTX_Core();

        bool UpdateCamera(int newID = -1);
        void Scan();

        unsigned int getColorsNumber();

        void showSettingWindows();
        void showRGB_CorrectionWindow();
        void showFilteringWindow();
        void showColorWheel();
        void showObjectsDetectionWindow();

    private:
        void RGB_Correction();
        void FilterColors();
        void UpdateColorWheel();
        void FindObjects();

    private:
        PTX_Cam *Cam;
        std::vector<PTX_Color*> DetectedColors;

        cv::Mat Raw;
        cv::Mat HSV;
        cv::Mat ColorWheel;
        cv::Mat Filtered;
        cv::Mat Objects;
        float Hist[180];

        // OpenCV windows handling
        cv::String RGBCorrectionWindow;
        bool isRGBActive;

        cv::String FilteringWindow;
        bool isFilteringActive;

        cv::String ColorWheelWindow;
        bool isColorWheelActive;

        cv::String ObjectsDetectionWindow;
        bool isObjectsDetectionActive;

        int Gain_R, Gain_G, Gain_B; // RGB Correction gains

        int LowSaturation, HighSaturation;   // HSV interval Saturation settings
        int LowValue, HighValue;   // HSV interval Value settings

        int Threshold;  // Minimal size for contours

        int ObjectSelect;
        int ColorSelection;
};

#endif // PTX_CORE_H
