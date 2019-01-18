#include <iostream>
#include <list>
#include <SFML/Graphics.hpp>

#include <opencv2/videoio/videoio.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <Box2D/Box2D.h>

#include "PTX_System/PTX_Cam.h"
#include "PTX_System/PTX_Core.h"

using namespace std;

int main()
{
    //sf::RenderWindow window(sf::VideoMode(1280, 480), "TestBed");

    PTX_Core Core;
    sf::Sprite RawFrame;
    sf::Sprite ROIFrame;
    bool Continue = true;

    sf::Image temp;
    sf::Texture tex_temp;
    sf::Sprite spr_temp;
    ROIFrame.setPosition(640, 0);

    while (Continue)
    {
        if(Core.UpdateCamera())
        {
            //Camera.FindCorners(200);
            Core.Scan();
            //Core.showSettingWindows();
        }
        char key = static_cast<char>(cv::waitKey(30));
        if (key == 'q' || key == 27)
            Continue = false;
    }

    return 0;
}

























//            //cv::Mat Img = Camera.getRawFrameData();
//            cv::Mat Img = cv::imread("ObjectDetection.png", cv::IMREAD_COLOR);

//            cv::Mat ColorWheel = cv::Mat::zeros(Img.rows, Img.cols, CV_8UC3);
//            cv::Mat HSV = cv::Mat::zeros( Img.size(), CV_8UC3 );
//            cv::Mat Filtered = cv::Mat::zeros(Img.rows, Img.cols, CV_8UC3);

//            cv::Mat Red = cv::Mat::zeros(Img.rows, Img.cols, CV_8UC1);
//            cv::Mat Green = cv::Mat::zeros(Img.rows, Img.cols, CV_8UC1);
//            cv::Mat Blue = cv::Mat::zeros(Img.rows, Img.cols, CV_8UC1);

//            cv::Mat RedContours = cv::Mat::zeros( Img.size(), CV_8UC3 );
//            cv::Mat GreenContours = cv::Mat::zeros( Img.size(), CV_8UC3 );
//            cv::Mat BlueContours = cv::Mat::zeros( Img.size(), CV_8UC3 );

//            std::vector<cv::Vec4i> hierarchy;
//            std::vector<std::vector<cv::Point> > RedPolygons;
//            std::vector<std::vector<cv::Point> > GreenPolygons;
//            std::vector<std::vector<cv::Point> > BluePolygons;
//            std::vector<std::vector<unsigned int>> RedObjects;
//            std::vector<std::vector<unsigned int>> GreenObjects;
//            std::vector<std::vector<unsigned int>> BlueObjects;

//            float Hist[256] = {0};



//// RGB on the fly correction
//            float R_Correction = (Gain_R)/1000.0f;
//            float G_Correction = (Gain_G)/1000.0f;
//            float B_Correction = (Gain_B)/1000.0f;

//            for(int r = 0; r < Img.rows; r++)
//            {
//                for(int c = 0; c < Img.cols; c++)
//                {
//                    Img.at<cv::Vec3b>(r, c).val[0] *= B_Correction;
//                    Img.at<cv::Vec3b>(r, c).val[1] *= G_Correction;
//                    Img.at<cv::Vec3b>(r, c).val[2] *= R_Correction;
//                }
//            }

//// Color filtering and separation
//            cv::cvtColor(Img, HSV, cv::COLOR_RGB2HSV);

//            cv::MatIterator_<cv::Vec3b> it, end;

//            for(int r = 0; r < HSV.rows; r++)
//            {
//                for(int c = 0; c < HSV.cols; c++)
//                {
//                    if(HSV.at<cv::Vec3b>(r, c).val[1] >= low_S && HSV.at<cv::Vec3b>(r, c).val[1] <= high_S && HSV.at<cv::Vec3b>(r, c).val[2] >= low_V && HSV.at<cv::Vec3b>(r, c).val[2] <= high_V)
//                    {
//                        int Hue = HSV.at<cv::Vec3b>(r, c).val[0];
//                        Hist[Hue]++;

//                        /********************       RED      **********************/
//                        if(l_R < h_R)
//                        {
//                            // Positive RED
//                            if((Hue > l_R && Hue < h_R))
//                            {
//                                Filtered.at<cv::Vec3b>(r, c).val[0] = 0;
//                                Filtered.at<cv::Vec3b>(r, c).val[1] = 0;
//                                Filtered.at<cv::Vec3b>(r, c).val[2] = 255;
//                                Red.at<unsigned char>(r, c) = 255;
//                            }
//                        }

//                        else
//                        {
//                            //Negative RED
//                            if((Hue > l_R || Hue < h_R))
//                            {
//                                Filtered.at<cv::Vec3b>(r, c).val[0] = 0;
//                                Filtered.at<cv::Vec3b>(r, c).val[1] = 0;
//                                Filtered.at<cv::Vec3b>(r, c).val[2] = 255;
//                                Red.at<unsigned char>(r, c) = 255;
//                            }
//                        }
//                        /********************       RED      **********************/

//                        /********************       GREEN      ********************/
//                        if(l_G < h_G)
//                        {
//                            // Positive GREEN
//                            if((Hue > l_G && Hue < h_G))
//                            {
//                                Filtered.at<cv::Vec3b>(r, c).val[0] = 0;
//                                Filtered.at<cv::Vec3b>(r, c).val[1] = 255;
//                                Filtered.at<cv::Vec3b>(r, c).val[2] = 0;
//                                Green.at<unsigned char>(r, c) = 255;
//                            }
//                        }

//                        else
//                        {
//                            //Negative GREEN
//                            if((Hue > l_G || Hue < h_G))
//                            {
//                                Filtered.at<cv::Vec3b>(r, c).val[0] = 0;
//                                Filtered.at<cv::Vec3b>(r, c).val[1] = 255;
//                                Filtered.at<cv::Vec3b>(r, c).val[2] = 0;
//                                Green.at<unsigned char>(r, c) = 255;
//                            }
//                        }
//                        /********************       GREEN      ********************/



//                        /********************       BLUE      *********************/
//                        if(l_B < h_B)
//                        {
//                            // Positive BLUE
//                            if((Hue > l_B && Hue < h_B))
//                            {
//                                Filtered.at<cv::Vec3b>(r, c).val[0] = 255;
//                                Filtered.at<cv::Vec3b>(r, c).val[1] = 0;
//                                Filtered.at<cv::Vec3b>(r, c).val[2] = 0;
//                                Red.at<unsigned char>(r, c) = 255;
//                            }
//                        }

//                        else
//                        {
//                            //Negative BLUE
//                            if((Hue > l_B || Hue < h_B))
//                            {
//                                Filtered.at<cv::Vec3b>(r, c).val[0] = 255;
//                                Filtered.at<cv::Vec3b>(r, c).val[1] = 0;
//                                Filtered.at<cv::Vec3b>(r, c).val[2] = 0;
//                                Blue.at<unsigned char>(r, c) = 255;
//                            }
//                        }

//                        /********************       BLUE      *********************/
//                    }


//                }
//            }


//// Color Hist painting

//            int MaxHue = 1;

//            for(int i = 0; i < 180; i++)
//            {
//                if(Hist[i] > MaxHue)
//                    MaxHue = static_cast<int>(Hist[i]);
//            }

//            for(int i = 0; i < 180; i++)
//            {
//                Hist[i] = Hist[i]/(MaxHue) * 480; //Normalize Hist to 480
//                cv::line(ColorWheel, cv::Point(3*i, 480), cv::Point(3*i, 480-static_cast<int>(Hist[i])), cv::Scalar(i, 255, 255), 3);
//            }

//            cv::line(ColorWheel, cv::Point(3*l_R, 480), cv::Point(3*l_R, 0), cv::Scalar(126, 255, 255));
//            cv::line(ColorWheel, cv::Point(3*h_R, 480), cv::Point(3*h_R, 0), cv::Scalar(126, 255, 255));
//            cv::line(ColorWheel, cv::Point(3*l_G, 480), cv::Point(3*l_G, 0), cv::Scalar(63, 255, 255));
//            cv::line(ColorWheel, cv::Point(3*h_G, 480), cv::Point(3*h_G, 0), cv::Scalar(63, 255, 255));
//            cv::line(ColorWheel, cv::Point(3*l_B, 480), cv::Point(3*l_B, 0), cv::Scalar(0, 255, 255));
//            cv::line(ColorWheel, cv::Point(3*h_B, 480), cv::Point(3*h_B, 0), cv::Scalar(0, 255, 255));

//            cv::cvtColor(ColorWheel, ColorWheel, cv::COLOR_HSV2BGR);
//            cv::cvtColor(ColorWheel, ColorWheel, cv::COLOR_BGR2RGB);

//// Finding objects
//            cv::findContours(Red, RedPolygons, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);


//            unsigned int ObjectsCount = 0;

//            for (unsigned int ParentID = 0; ParentID < RedPolygons.size(); ParentID++)
//            {
//                if(hierarchy[ParentID][3] == -1)
//                {
//                    ObjectsCount++;
//                    RedObjects.resize(ObjectsCount);
//                    RedObjects[ObjectsCount-1].push_back(ParentID);

//                    for(unsigned int ChildID = 0; ChildID < RedPolygons.size(); ChildID++)
//                    {
//                        if(hierarchy[ChildID][3] == ParentID)
//                            RedObjects[ObjectsCount-1].push_back(ChildID);
//                    }
//                }
//            }

//            if(RedObjects.size() > 0)
//            {
//                if(ContourSelect < RedObjects.size())
//                {
//                    for(unsigned int i = 0; i < RedObjects[ContourSelect].size(); i++)
//                    {
//                        cv::Scalar color = cv::Scalar(0, 0, 255);
//                        cv::drawContours(RedContours, RedPolygons, RedObjects[ContourSelect][i], color);
//                    }
//                }
//            }


//            cv::findContours(Green, GreenPolygons, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_TC89_KCOS);


//            for(unsigned int i = 0; i < GreenPolygons.size(); i++ )
//            {
//                if(GreenPolygons[i].size() > Threshold)
//                {
//                    cv::Scalar color = cv::Scalar(0, 255, 0);
//                    cv::drawContours(GreenContours, GreenPolygons, i, color);
//                }
//            }

//            cv::findContours(Blue, BluePolygons, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_TC89_L1);
//            for(unsigned  int i = 0; i < BluePolygons.size(); i++ )
//            {
//                if(BluePolygons[i].size() > Threshold)
//                {
//                    cv::Scalar color = cv::Scalar(255, 0, 0);
//                    cv::drawContours(BlueContours, BluePolygons, i, color);
//                }
//            }


//            cv::imshow(window_capture_name, Img);
//            cv::imshow(window_setting_name, Filtered);
//            cv::imshow(window_detection_name, ColorWheel);

//            switch(ColorSelection)
//            {
//                case 0:
//                    cv::imshow("Contour detection", RedContours);
//                    break;
//                case 1:
//                    cv::imshow("Contour detection", GreenContours);
//                    break;
//                case 2:
//                    cv::imshow("Contour detection", BlueContours);
//                    break;
//            }
//        //}
