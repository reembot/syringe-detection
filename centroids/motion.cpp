#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

// See www.asciitable.com
#define ESCAPE_KEY (27)
#define SYSTEM_ERROR (-1)

class bbox {
  public:
    int x_min,x_max,y_min,y_max;
    bbox(int rows, int cols)
    {
      x_min=cols;
      x_max=0;
      y_min=cols;
      y_max=0;
    }
    int area();
    int h();
    int w();
};

int bbox::h() { return x_max > x_min ? y_max - y_min : 0; }
int bbox::w() { return x_max > x_min ? x_max - x_min : 0; }
int bbox::area() { return x_max > x_min ? h() * w() : 0; }

int main(int argc, char** argv)
{
   const char* default_file = "0";
   const char* filename = argc >=2 ? argv[1] : default_file;
   VideoCapture cap;
   int waitval = 100;
   if (*filename == '0') {
     cap.open(0);
   } else {
     cap.open(filename);
   }
   bool origView = 1;
   namedWindow(filename);
   char winInput;

//mats for original capture, grayscale version, previous frame and frame difference
   Mat src, srcGray, srcPrev, srcDiff;

   cap.read(src);
   if(src.empty())
   {
     return 0;
   }

//uncomment for video out
/*
   VideoWriter vout;
   Size S = Size((int) cap.get(CAP_PROP_FRAME_WIDTH),    // Acquire input size
                  (int) cap.get(CAP_PROP_FRAME_HEIGHT));

   vout.open("motion.mp4", VideoWriter::fourcc('H','2','6','4'), cap.get(CAP_PROP_FPS), S, true);
*/

   vector<bbox> bhistory;

// default method will simply compare change in intensity on single channel image (grayscale)
   cv::cvtColor(src, srcGray, COLOR_BGR2GRAY);
   srcPrev = srcGray.clone();
   srcDiff = srcGray.clone();

   printf("Press space to pause and unpause, 's' to step through each frame, 'c' to switch views or 'esc' to exit\n");
   int cnt = 0;


   while (1)
   {
      cnt++;
      
      cap.read(src);
      if(src.empty())
      {
        break;
      }

      cv::cvtColor(src, srcGray, COLOR_BGR2GRAY);

      absdiff(srcPrev, srcGray, srcDiff);

//Alternatively we could threshold values, changing everything to 0 or 255 with THRESH_BINARY or only set values under param3 to 0 with THRESH_TOZERO
//      threshold(srcDiff,srcDiff,100,255,THRESH_BINARY);
      
       srcPrev = srcGray.clone();       
       int totalp = 0;
       bbox bounds(src.rows,src.cols);

       for(int j=0;j<src.rows-2;j++) //due to intrinsic camera issue skipping last few rows as 718 and 719 always vary in intensity
       {
         for(int i=0;i<src.cols;i++)
         {
             if(int(srcDiff.at<uchar>(j,i)) > 50)
             {
                 totalp++;
                 if(bounds.x_min > i)
                 {
                   bounds.x_min = i;
                 }
                 if(bounds.x_max < i)
                 {
                   bounds.x_max = i;
                 }
                 if(bounds.y_min > j)
                 {
                   bounds.y_min = j;
                 }
                 if(bounds.y_max < j)
                 { 
                   bounds.y_max = j;
                 }
             }
         }
       }

       if(bounds.x_max > bounds.x_min)
       {
         bhistory.push_back(bounds);
         rectangle(src,Point(bounds.x_min,bounds.y_min),Point(bounds.x_max,bounds.y_max),Scalar(0,255,0),5,0);
         rectangle(srcDiff,Point(bounds.x_min,bounds.y_min),Point(bounds.x_max,bounds.y_max),Scalar(255),5,0);
       }

      putText(src, "Movement Area: " + to_string(bounds.area()), Point(1000,50), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0,255,0), 1, LINE_AA);
      if (bhistory.size() > 2) 
      {
        int areachange = int((float)bhistory[bhistory.size()-1].area()/(float)bhistory[bhistory.size()-2].area()*100);
        putText(src, "Change: " + to_string(areachange) + " % " + (areachange > 100 ? "(Grew)" : "(Shrank)"), Point(1000,100), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0,255,0), 1, LINE_AA);
        if(bhistory[bhistory.size()-1].x_max > bhistory[bhistory.size()-2].x_max)
        {
          putText(src, "Moving Right", Point(1000,150), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0,255,0), 1, LINE_AA);
        } 
        else
        {
          putText(src, "Moving Left", Point(1000,150), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0,255,0), 1, LINE_AA);         
        }
        if(bhistory[bhistory.size()-1].y_max < bhistory[bhistory.size()-2].y_max)
        {
          putText(src, "Moving Up", Point(1000,200), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0,255,0), 1, LINE_AA);
        } 
        else
        {
          putText(src, "Moving Down", Point(1000,200), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0,255,0), 1, LINE_AA);         
        }
      }

      if (origView)
      {
        imshow(filename,src);
      } else {
        imshow(filename,srcDiff);
      }

//uncomment for video out
//      vout.write(src);

      if ((winInput = waitKey(waitval)) == ESCAPE_KEY)
      {
          break;
      }
      else if(winInput == 's') 
      {
          waitval = 0;
      }
      else if(winInput == ' ') 
      {
          waitval = (waitval == 100) ? 0 : 100;
      }
      else if(winInput == 'c') 
      {
          origView = origView ? 0 : 1; //flip flag to show original or difference
      }

   }
   destroyWindow(filename); 
}
