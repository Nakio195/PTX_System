#include "PTX_Core.h"

PTX_Core::PTX_Core()
{
    Cam = new PTX_Cam();

    if(!Cam->isAvailable())
    {
        std::cout << "Fatal error : Unable to find camera with ID :" << Cam->getCameraID();
        exit(EXIT_NO_AVAILABLE_CAMERA);
    }

    DetectedColors.resize(4);

    DetectedColors[0] = new PTX_Color(0);
    DetectedColors[1] = new PTX_Color(60);
    DetectedColors[2] = new PTX_Color(30);
    DetectedColors[3] = new PTX_Color(115);

    Raw = cv::Mat::zeros(static_cast<int>(Cam->Height), static_cast<int>(Cam->Width), CV_8UC3);
    ColorWheel = cv::Mat::zeros(static_cast<int>(Cam->Height), static_cast<int>(Cam->Width), CV_8UC3);
    HSV = cv::Mat::zeros(static_cast<int>(Cam->Height), static_cast<int>(Cam->Width), CV_8UC3);
    Filtered = cv::Mat::zeros(static_cast<int>(Cam->Height), static_cast<int>(Cam->Width), CV_8UC3);
    Objects = cv::Mat::zeros(static_cast<int>(Cam->Height), static_cast<int>(Cam->Width), CV_8UC3);


    Gain_R = 100.0f;
    Gain_G = 100.0f;
    Gain_B = 100.0f;

    LowSaturation = 60;
    HighSaturation = 255;   // HSV interval Saturation settings
    LowValue = 0;
    HighValue = 255;   // HSV interval Value settings

    Threshold = 10;  // Minimal size for contours
    ObjectSelect = 0;
    ColorSelection = 0;

    // OpenCV windows handling

    RGBCorrectionWindow = cv::String("RGB Correction");
    isRGBActive = true;
    cv::namedWindow(RGBCorrectionWindow);
    cv::createTrackbar("Red Gain", RGBCorrectionWindow, &Gain_R, 1000);
    cv::createTrackbar("Green Gain", RGBCorrectionWindow, &Gain_G, 1000);
    cv::createTrackbar("Blue Gain", RGBCorrectionWindow, &Gain_B, 1000);

    FilteringWindow = cv::String("Filtered view");
    isFilteringActive = true;
    cv::namedWindow(FilteringWindow);
    // Trackbars to set thresholds for HSV values
    cv::createTrackbar("Low S", FilteringWindow, &LowSaturation, 255);
    cv::createTrackbar("High S", FilteringWindow, &HighSaturation, 255);
    cv::createTrackbar("Low V", FilteringWindow, &LowValue, 255);
    cv::createTrackbar("High V", FilteringWindow, &HighValue, 255);

    ColorWheelWindow = cv::String("ColorWheel");
    isColorWheelActive = true;
    cv::namedWindow(ColorWheelWindow);

    for(unsigned int i = 0; i < DetectedColors.size(); i++)
    {
        cv::createTrackbar(cv::String("Low ")+cv::String(std::to_string(i+1)), ColorWheelWindow, &DetectedColors[i]->LowHue, 180);
        cv::createTrackbar(cv::String("High ")+cv::String(std::to_string(i+1)), ColorWheelWindow, &DetectedColors[i]->HighHue, 180);
    }

    ObjectsDetectionWindow = cv::String("Objects Detection");
    isObjectsDetectionActive = true;
    cv::namedWindow(ObjectsDetectionWindow);
    cv::createTrackbar("Color Selection", ObjectsDetectionWindow, &ColorSelection, 2);
    cv::createTrackbar("Threshold", ObjectsDetectionWindow, &Threshold, 255);
    cv::createTrackbar("Contour Select", ObjectsDetectionWindow, &ObjectSelect, 25);


}

bool PTX_Core::UpdateCamera(int newID)
{
    if(newID != -1)
        Cam->SelectCamera(static_cast<unsigned int>(newID));

    bool State = Cam->Update();

    if(State)
        //Raw = Cam->getRawFrameData();
    Raw = cv::imread("ObjectDetection.png", cv::IMREAD_COLOR);

    return State;
}

void PTX_Core::Scan()
{
    RGB_Correction();
    FilterColors();
    UpdateColorWheel();
    FindObjects();
}



unsigned int PTX_Core::getColorsNumber()
{
    return DetectedColors.size();
}

void PTX_Core::RGB_Correction()
{
    // RGB on the fly correction
    float R_Correction = (Gain_R)/100.0f;
    float G_Correction = (Gain_G)/100.0f;
    float B_Correction = (Gain_B)/100.0f;

    for(int r = 0; r < Raw.rows; r++)
    {
        for(int c = 0; c < Raw.cols; c++)
        {
            Raw.at<cv::Vec3b>(r, c).val[0] *= B_Correction;
            Raw.at<cv::Vec3b>(r, c).val[1] *= G_Correction;
            Raw.at<cv::Vec3b>(r, c).val[2] *= R_Correction;
        }
    }

}

void PTX_Core::FilterColors()
{
    // Color filtering and separation
    cv::cvtColor(Raw, HSV, cv::COLOR_BGR2HSV);

    for(unsigned int i = 0; i < DetectedColors.size(); i++)
    {
        DetectedColors[i]->ColorMatrix = cv::Mat::zeros(static_cast<int>(Cam->Height), static_cast<int>(Cam->Width), CV_8UC1);
    }

    Filtered = cv::Mat::zeros(static_cast<int>(Cam->Height), static_cast<int>(Cam->Width), CV_8UC3);

    for(unsigned int i = 0; i < 180; i++)
        Hist[i] = 0;

    for(int r = 0; r < HSV.rows; r++)
    {
        for(int c = 0; c < HSV.cols; c++)
        {
            int Saturation = HSV.at<cv::Vec3b>(r, c).val[1];
            int Value = HSV.at<cv::Vec3b>(r, c).val[2];

            if(Saturation >= LowSaturation && Saturation <= HighSaturation && Value >= LowValue && Value <= HighValue)
            {
                int Hue = HSV.at<cv::Vec3b>(r, c).val[0];
                Hist[Hue]++;

                for(unsigned int i = 0; i < DetectedColors.size(); i++)
                {
                    if(DetectedColors[i]->FitHueInterval(Hue))
                    {
                        Filtered.at<cv::Vec3b>(r, c).val[0] = DetectedColors[i]->MainHue;
                        Filtered.at<cv::Vec3b>(r, c).val[1] = 255;
                        Filtered.at<cv::Vec3b>(r, c).val[2] = 255;

                        DetectedColors[i]->ColorMatrix.at<unsigned char>(r, c) = 255;
                        break;
                    }
                }
            }
        }
    }

    cv::cvtColor(Filtered, Filtered, cv::COLOR_HSV2BGR);
}

void PTX_Core::UpdateColorWheel()
{
    int MaxHue = 1;
    ColorWheel = cv::Mat::zeros(static_cast<int>(Cam->Height), static_cast<int>(Cam->Width), CV_8UC3);

    for(int i = 0; i < 180; i++)
    {
        if(Hist[i] > MaxHue)
            MaxHue = static_cast<int>(Hist[i]);
    }

    for(int i = 0; i < 180; i++)
    {
        Hist[i] = Hist[i]/(MaxHue) * ColorWheel.rows; //Normalize Hist to fit size
        cv::line(ColorWheel, cv::Point(3*i, 480), cv::Point(3*i, ColorWheel.rows-static_cast<int>(Hist[i])), cv::Scalar(i, 255, 255), 3);
    }

    for(unsigned int i = 0; i < DetectedColors.size(); i++)
    {
        cv::line(ColorWheel, cv::Point(3*DetectedColors[i]->LowHue, 480), cv::Point(3*DetectedColors[i]->LowHue, 0), cv::Scalar(DetectedColors[i]->MainHue, 255, 255));
        cv::line(ColorWheel, cv::Point(3*DetectedColors[i]->HighHue, 480), cv::Point(3*DetectedColors[i]->HighHue, 0), cv::Scalar(DetectedColors[i]->MainHue, 255, 255));
    }

    cv::cvtColor(ColorWheel, ColorWheel, cv::COLOR_HSV2BGR);
    //cv::cvtColor(ColorWheel, ColorWheel, cv::COLOR_BGR2RGB);
}

void PTX_Core::FindObjects()
{
    Objects = cv::Mat::zeros(static_cast<int>(Cam->Height), static_cast<int>(Cam->Width), CV_8UC3);

    for(unsigned int i = 0; i < DetectedColors.size(); i++)
    {
        std::vector<cv::Vec4i> Hierarchy;
        DetectedColors[i]->Objects.clear();
        DetectedColors[i]->Polygons.clear();
        unsigned int ObjectsCount = 0;

        cv::findContours(DetectedColors[i]->ColorMatrix, DetectedColors[i]->Polygons, Hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);

        for (unsigned int ParentID = 0; ParentID < DetectedColors[i]->Polygons.size(); ParentID++)
        {
            if(Hierarchy[ParentID][3] == -1 && DetectedColors[i]->Polygons[ParentID].size() > static_cast<unsigned int>(Threshold))
            {
                ObjectsCount++;
                DetectedColors[i]->Objects.resize(ObjectsCount);
                DetectedColors[i]->Objects[ObjectsCount-1].push_back(ParentID);

                for(unsigned int ChildID = 0; ChildID < DetectedColors[i]->Polygons.size(); ChildID++)
                {
                    if(Hierarchy[ChildID][3] == static_cast<int>(ParentID) && DetectedColors[i]->Polygons[ChildID].size() > static_cast<unsigned int>(Threshold))
                        DetectedColors[i]->Objects[ObjectsCount-1].push_back(ChildID);
                }

            }
        }

        if(DetectedColors[i]->Objects.size() > 0 && DetectedColors[i]->Polygons.size() > 0)
        {
            std::system("cls");
            if(static_cast<unsigned int>(ObjectSelect) < DetectedColors[i]->Objects.size())
            {
                for(unsigned int j = 0; j < DetectedColors[i]->Objects[static_cast<unsigned int>(ObjectSelect)].size(); j++)
                {
                    std::cout << static_cast<int>(DetectedColors[i]->Objects[static_cast<unsigned int>(ObjectSelect)][j]) << std::endl;
                    cv::Scalar color = cv::Scalar(DetectedColors[i]->MainHue, 255, 255);
                    cv::drawContours(Objects, DetectedColors[i]->Polygons, static_cast<int>(DetectedColors[i]->Objects[static_cast<unsigned int>(ObjectSelect)][j]), color);
                }
            }
        }
    }

    cv::cvtColor(Objects, Objects, cv::COLOR_HSV2BGR);
}

void PTX_Core::showSettingWindows()
{
    if(isRGBActive)
        cv::imshow(RGBCorrectionWindow, Raw);

    if(isFilteringActive)
        cv::imshow(FilteringWindow, Filtered);

    if(isColorWheelActive)
        cv::imshow(ColorWheelWindow, ColorWheel);

    if(isObjectsDetectionActive)
        cv::imshow(ObjectsDetectionWindow, Objects);
}
