#include "stdio.h"
#include<iostream> 
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace std;
using namespace cv;
const int kThreashold = 125;
const int kMaxVal = 255;
const Size kGaussianBlueSize = Size(5, 5);
int main()
{
    VideoCapture video;
    //读入视频
    video.open("C:/Users/Lenovo/Desktop/识别红方.mp4");
    if (!video.isOpened())
    {
        return 0;
    }

    Mat frame,binary,Gaussian,dst;
    vector<Mat>channels;
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    Rect boundRect;
    RotatedRect box;
    vector<Point2f> boxPts(4);
    
    while(1) {
        Rect point_array[20];
        video >> frame;
        if (frame.empty()) {
            break;
        }
        split(frame,channels);
        dst = channels.at(2) - channels.at(0);
        threshold(dst, binary, kThreashold, kMaxVal, 0);
        GaussianBlur(binary, Gaussian, kGaussianBlueSize, 0);
        findContours(Gaussian, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);
        int index = 0;

               //循环次数为查找出轮廓的个数
        for (int i = 0; i < contours.size(); i++) {
          
         
            boundRect = boundingRect(Mat(contours[i]));
            //rectangle(frame, boundRect.tl(), boundRect.br(), (0, 255, 0), 2,8 ,0);
            try
            {
                if (double(boundRect.height / boundRect.width) >= 2&& boundRect.height > 10 && boundRect.height > 15) {//
                    point_array[index] = boundRect;
                    index++;
                }
            }
            catch (const char* msg)
            {
                cout << printf(msg) << endl;
                //continue;
            }
        }        
        //相似矩形
        int point_near[2] = {0,0};
        int min = 10000;
        for (int i = 0; i < index-1; i++)
        {
            for (int j = i + 1; j < index; j++) {
                int value = abs(point_array[i].area() - point_array[j].area());
                if (value < min)
                {
                    min = value;
                    point_near[0] = i;
                    point_near[1] = j;
                }
            }
        }   
        try
        {
           
            Rect rectangle_1 = point_array[point_near[0]];
            Rect rectangle_2 = point_array[point_near[1]];
            if (rectangle_2.x == 0 || rectangle_1.x == 0) {
                //throw "not enough points";
            }
            Point point1 = Point(rectangle_1.x + rectangle_1.width / 2, rectangle_1.y);
            Point point2 = Point(rectangle_1.x + rectangle_1.width / 2, rectangle_1.y + rectangle_1.height);
            Point point3 = Point(rectangle_2.x + rectangle_2.width / 2, rectangle_2.y);
            Point point4 = Point(rectangle_2.x + rectangle_2.width / 2, rectangle_2.y + rectangle_2.height);
            Point p[4] = { point1,point2,point4,point3 };
  
            for (int i = 0; i < 4; i++) {
                line(frame, p[i%4], p[(i+1)%4], Scalar(0, 255, 0), 2);
            }           
        }
        catch (const char* msg)
        {
            cout << msg << endl;
            //continue;
        }
        imshow("video", frame);
        if (waitKey(10) >= 0) {
            break;
        }
    }
    video.release();
    cv::destroyAllWindows();
    return 0;
}