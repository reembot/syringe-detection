#include <opencv2/opencv.hpp>
//#include <vector>

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

#include "../utils/labelColors.h"

int main(int argc, char **argv)
{
//standard main OpenCV capture and output file setup
    const char *default_file = "0";
    const char *filename = argc >= 2 ? argv[1] : default_file;
    const char* output_file = "vout.csv";
    const char* out_filename = argc >=3 ? argv[2] : output_file;
    ofstream outfile;
    outfile.open(out_filename);
    VideoCapture cap;
    if (*filename == '0')
    {
        cap.open(0);
    }
    else
    {
        cap.open(filename);
    }
//setup mats for transformations
    int cnt=0;
    Mat gray, blurred, threshed, edges;
//lines to be stored
    vector<Vec4i> top_lines, last_lines;
//flags for detections
    bool hadPr=0,hadR=0,hadPh=0,hadL=0,hadO=0,hadD=0;


    while (true)
    {
        // Read a frame from the video capture device
        Mat frame;
        cap.read(frame);
        if (frame.empty())
        {
            break;
        }
        cnt++;
        // Apply pre-processing to the frame
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        GaussianBlur(gray, blurred, Size(3, 3), 0);
        int maxthresh = threshold(blurred,threshed,0,255,THRESH_BINARY + THRESH_OTSU);
        int minthresh = maxthresh / 2;
        maxthresh = (maxthresh < 10) ? 255: maxthresh;
        minthresh = (maxthresh == 255) ? 255: minthresh;
        Canny(blurred, edges, minthresh, maxthresh);

        // Detect lines in the image using HoughLinesP
        vector<Vec4i> lines;
        HoughLinesP(edges, lines, 1, CV_PI / 180, 100, 175, 30);

        // Filter the lines to keep only those that are roughly horizontal
        vector<Vec4i> filtered_lines;
        double angle_threshold = 15; // degrees
        int hmax = 80,hmin = 40,wmax=300,wmin=175;
        int ymin[6] = {960,960,960,960,960,960};
        for (size_t i = 0; i < lines.size(); i++)
        {
            Vec4i l = lines[i];
            double dx = l[2] - l[0];
            double dy = l[3] - l[1];
            double angle = abs(atan2(dy, dx) * 180 / CV_PI);
            if (angle < angle_threshold || angle > 180 - angle_threshold)
            {
              if ( (dx >= wmin && dx <= wmax))
               {
                    filtered_lines.push_back(l);
               }
            }
        }

        last_lines = top_lines;
        top_lines.clear();

        for (int i = 0; i < 7; i++)
        {
          for (size_t j = 0; j < filtered_lines.size(); j++)
          {
             Vec4i l = filtered_lines[j];
             if(i == 0) 
             {
                  if(ymin[0] > l[1])
                  {
                    ymin[0]=l[1];
                  }
              } 
              else
              { 
                if(l[1] == ymin[i-1])
                {
                    top_lines.push_back(l);
                }
                if(ymin[i] > l[1] && l[1] > ymin[i-1] + 100 && i != 6) 
                {
                    ymin[i]=l[1];
                 }
              }
          }
        }
        bool hasPr=0,hasR=0,hasPh=0,hasL=0,hasO=0,hasD=0;
        for(size_t i = 0; i < top_lines.size(); i++)
        {
          Vec4i l = top_lines[i];
          int x1,x2,y1,y2;

          if(top_lines.size()==last_lines.size())
          {
            Vec4i l2 = last_lines[i];
            x1 = (l[0]-100+l2[0]-100)/2;
            x2 = (l[0]+300+l2[0]+300)/2;
            y1 = (l[1]-25+l2[1]-25)/2;
            y2 = (l[1]+75+l2[1]+75)/2;
          } else {

            x1 = l[0]-100;
            x2 = l[0]+300;
            y1 = l[1]-25;
            y2 = l[1]+75;
          }

          if(frame.cols - x1 > 400 && x1 > 100 && frame.rows - y1 > 100 && y1 > 25)
          {
            Mat syringeLabel = frame(Rect(x1,y1,400,100));
            switch(labelColors(syringeLabel,1,1,1,1,1,1))
            {
              case 1: hasPr=1; break;
              case 2: hasR=1; break;
              case 3: hasPh=1; break;
              case 4: hasL=1; break;
              case 5: hasD=1; break;
              case 6: hasO=1; break;
            }
          }


        }

      std:string msecs = to_string(int(cap.get(CAP_PROP_POS_MSEC)));
      if(hasR!=hadR)
      {
        hadR=hasR;
        outfile << "Rocuronium" << "," << msecs << endl;      
      }
      if(hasPr!=hadPr)
      {
        hadPr=hasPr;
        outfile << "Propofol" << "," << msecs << endl;      
      }
      if(hasPh!=hadPh)
      {
        hadPh=hasPh;
        outfile << "Phenylephrine" << "," << msecs << endl;      
      }
      if(hasL!=hadL)
      {
        hadL=hasL;
        outfile << "Lidocaine" << "," << msecs << endl;      
      }
      if(hasO!=hadO)
      {
        hadO=hasO;
        outfile << "Odanestron" << "," << msecs << endl;      
      }
      if(hasD!=hadD)
      {
        hadD=hasD;
        outfile << "Dexamethasone" << "," << msecs << endl;      
      }

    }
    outfile.close();
    return 0;
}
