#include "PTX_Cam.h"

PTX_Cam::PTX_Cam(unsigned int CamID)
{
    ValidCamera = false;
    CurrentCamID = CamID;

    Camera = new cv::VideoCapture(static_cast<int>(CamID));

    if(Camera->isOpened())
    {
        ValidCamera = true;
        Width = static_cast<unsigned int>(Camera->get(cv::CAP_PROP_FRAME_WIDTH));
        Height = static_cast<unsigned int>(Camera->get(cv::CAP_PROP_FRAME_HEIGHT));
    }

    else
    {
        ValidCamera = false;
        Width = 0;
        Height = 0;
    }

    ROI[0] = cv::Point2f(0, 0);
    ROI[1] = cv::Point2f(Width, 0);
    ROI[2] = cv::Point2f(0, Height);
    ROI[3] = cv::Point2f(Width, Height);
}

bool PTX_Cam::SelectCamera(unsigned int CamID)
{
    if(Camera->isOpened())
        Camera->release();

    Camera->open(static_cast<int>(CamID));

    if(Camera->isOpened())
    {
        ValidCamera = true;
        Width = static_cast<unsigned int>(Camera->get(cv::CAP_PROP_FRAME_WIDTH));
        Height = static_cast<unsigned int>(Camera->get(cv::CAP_PROP_FRAME_HEIGHT));
    }

    else
    {
        ValidCamera = false;
        Width = 0;
        Height = 0;
    }

    return ValidCamera;

}

unsigned int PTX_Cam::getCameraID()
{
    return CurrentCamID;
}

bool PTX_Cam::isAvailable()
{
    return ValidCamera;
}

/* Take a new frame from the camera and produce both entire frame and ROI frame
 *
 * returns true if the new frame is valid
 *
 */

bool PTX_Cam::Update(bool ComputeROI)
{
    if(ValidCamera)
    {
        if(Camera->read(Frame))
        {
            if(Frame.rows > 0 && Frame.cols > 0)
            {
                if(ComputeROI)
                {
                    cv::Point2f WorkArea[4];
                    WorkArea[0] = cv::Point2f(0, 0);
                    WorkArea[1] = cv::Point2f(Width, 0);
                    WorkArea[2] = cv::Point2f(0, Height);
                    WorkArea[3] = cv::Point2f(Width, Height);

                    cv::Mat Transforms = cv::getPerspectiveTransform(ROI, WorkArea);
                    cv::warpPerspective(Frame, ROIFrame, Transforms, cv::Size(300, 300));
                }
                return true;
            }
        }

        else
            ValidCamera = false;
    }

    else    // Try to open camera
    {
        return SelectCamera(CurrentCamID);
    }

    return ValidCamera;
}

cv::Mat PTX_Cam::getRawFrameData()
{
    return Frame;
}

sf::Texture* PTX_Cam::getRawFrame()
{
    sf::Image TempImage;
    cv::Mat tempMat;
    cv::cvtColor(Frame, tempMat, cv::COLOR_BGR2RGBA);
    TempImage.create(static_cast<unsigned int>(tempMat.cols), static_cast<unsigned int>(tempMat.rows), tempMat.ptr());
    Texture.loadFromImage(TempImage);
    return &Texture;
}

cv::Mat PTX_Cam::getROIFrameData()
{
    return ROIFrame;
}

sf::Texture* PTX_Cam::getROIFrame()
{
    sf::Image TempImage;
    cv::Mat tempMat;
    cv::cvtColor(Frame, tempMat, cv::COLOR_BGR2RGBA);
    TempImage.create(static_cast<unsigned int>(tempMat.cols), static_cast<unsigned int>(tempMat.rows), tempMat.ptr());
    ROITexture.loadFromImage(TempImage);
    return &ROITexture;
}

void PTX_Cam::FindCorners(int Threshold, bool Feedback)
{
    cv::Mat Gray, Harris, Harris_Normed;                            // Initializing context
    Harris = cv::Mat::zeros(Frame.size(), CV_32FC1);
    Corners.clear();
    CornersFiltered.clear();

    cv::cvtColor(Frame, Gray, cv::COLOR_RGBA2GRAY);  // Working on gray image

    int blockSize = 4;
    int apertureSize = 3;
    double k = 0.04;

    cornerHarris( Gray, Harris, blockSize, apertureSize, k, cv::BORDER_DEFAULT );       //Harris detection
    normalize( Harris, Harris_Normed, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat() );

    int MaxCorners = 300;   // To limit artifacts and time consumation in case of wrong source
    for( int j = 0; j < Harris_Normed.rows ; j++ )
    {
        for( int i = 0; i < Harris_Normed.cols; i++ )
        {
            if( static_cast<int>(Harris_Normed.at<float>(j,i)) > Threshold && MaxCorners > 0)
            {
                MaxCorners--;
                Corners.push_back(sf::Vector2i(i, j));
            }
        }
    }

    // Gathering multiples points by averaging through a 20px window
    std::list<sf::Vector2i>::iterator it = Corners.begin();
    while(it != Corners.end())
    {
        sf::Vector2i CurrentPoint = *it;

        sf::Rect<int> Window(sf::Vector2i(CurrentPoint.x-10, CurrentPoint.y-10), sf::Vector2i(20, 20));

        std::list<sf::Vector2i>::iterator jt = Corners.begin();
        while(jt != Corners.end())
        {
            if(jt != it)
            {
                sf::Vector2i ComparedPoint = *jt;

                if(Window.contains(ComparedPoint))
                {
                    CurrentPoint.x = (CurrentPoint.x+ComparedPoint.x)/2;
                    CurrentPoint.y = (CurrentPoint.y+ComparedPoint.y)/2;
                    jt = Corners.erase(jt);
                }
                else
                    ++jt;
            }

            else
                ++jt;
        }

        CornersFiltered.push_back(CurrentPoint);
        ++it;
    }

    if(Feedback)
    {
        for(auto Corner : CornersFiltered)
        {
            cv::circle(Frame, cv::Point( Corner.x, Corner.y ), 5,  cv::Scalar(255, 255, 0), 2, 8, 0 );
        }
    }
}


std::vector<sf::Vector2i> PTX_Cam::getCorners()
{
    return CornersFiltered;
}
