#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

// See www.asciitable.com
#define ESCAPE_KEY (27)
#define SYSTEM_ERROR (-1)
#define ROIX (300)
#define ROIY (50)
#define SRCX (1280)
#define SRCY (720)

int main(int argc, char** argv)
{
   const char* default_file = "0";
   const char* filename = argc >=2 ? argv[1] : default_file;
   VideoCapture cap;
   if (*filename == '0') {
     cap.open(0);
   } else {
     cap.open(filename);
   }
   bool canView = 0;
   namedWindow(filename);
   char winInput;

   VideoWriter vout;
    Size S = Size((int) cap.get(CAP_PROP_FRAME_WIDTH),    // Acquire input size
                  (int) cap.get(CAP_PROP_FRAME_HEIGHT));

   vout.open("lines.mp4", VideoWriter::fourcc('H','2','6','4'), cap.get(CAP_PROP_FPS), S, true);


   printf("Press 's' to write screen capture, 'c' to switch views or 'esc' to exit\n");
   int cnt = 0;

   while (1)
   {
      Mat src;
      cnt++;
      
      cap.read(src);
      if(src.empty()){
        break;
      }

      Mat roisrc, dst;

      roisrc = src(Rect(ROIX,ROIY,SRCX-ROIX,SRCY-ROIY)); 

      // Edge detection with thresholds of param3 to param4 intensity for hysteresis with param5xparam5 sobel and default l1 norm 
      Canny(roisrc, dst, 50, 150, 3);

      // Probabilistic Line Transform
      vector<Vec4i> linesP; // will hold the results of the detection
      HoughLinesP(dst, linesP, 1, CV_PI/180, 120, 75, 10 ); // runs the actual detection
      // Draw the lines
      for( size_t i = 0; i < linesP.size(); i++ )
      {
        Vec4i l = linesP[i];
        line( src, Point(l[0]+ROIX, l[1]+ROIY), Point(l[2]+ROIX, l[3]+ROIY), Scalar(0,0,255), 3, LINE_AA);
      }

      if (canView)
      {
        imshow(filename, dst);
      } else {
        imshow(filename,src);
      }

      vout.write(src);

      if ((winInput = waitKey(1)) == ESCAPE_KEY)
      {
          break;
      }
      else if(winInput == 's') 
      {
          String imgname = to_string(cnt) + ".png";
          imwrite(imgname, src);
      }
      else if(winInput == 'c') 
      {
          canView = canView ? 0 : 1; //flip flag to show canny or normal
      }

   }
   destroyWindow(filename); 
}
