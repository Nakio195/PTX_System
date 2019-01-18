#ifndef PTX_CAM_H
#define PTX_CAM_H

#include <opencv2/opencv.hpp>
#include <SFML/Graphics.hpp>

class PTX_Cam
{
    public:
        PTX_Cam(unsigned int CamID = 0);

        bool SelectCamera(unsigned int CamID);
        bool isAvailable();
        unsigned int getCameraID();
        bool Update(bool ComputeROI = true);

        cv::Mat getRawFrameData();
        sf::Texture* getRawFrame();

        cv::Mat getROIFrameData();
        sf::Texture* getROIFrame();

        void FindCorners(int Threshold = 128, bool Feedback = false);
        std::vector<sf::Vector2i> getCorners();

    public:
        unsigned int Width;
        unsigned int Height;

    private:
        cv::VideoCapture *Camera;

        cv::Point2f ROI[4];

        bool ValidCamera;
        unsigned int CurrentCamID;

        cv::Mat Frame;
        sf::Texture Texture;

        cv::Mat ROIFrame;
        sf::Texture ROITexture;

        std::list<sf::Vector2i> Corners;
        std::vector<sf::Vector2i> CornersFiltered;


};

#endif // PTX_CAM_H
