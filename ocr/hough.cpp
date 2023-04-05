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

int main(int argc, char** argv)
{
   const char* default_file = "22400003.AVI";
   const char* filename = argc >=2 ? argv[1] : default_file;
   VideoCapture cam0;
   bool prob = 0;
   bool crop = 0;
   if (*filename == '0') {
     cam0.open(0);
   } else {
     cam0.open(filename);
   }
   namedWindow("Source");
   char winInput;

   cam0.set(CAP_PROP_FRAME_WIDTH, 640);
   cam0.set(CAP_PROP_FRAME_HEIGHT, 480);
   printf("Press 's' to write screen capture or 'esc' to exit\n");
   int cnt = 0;

   while (1)
   {
      Mat src;
      cnt++;
      
      cam0.read(src);
      Mat dst, roidst;
      // Loads an image
      //src = imread( filename, IMREAD_GRAYSCALE );
      // Check if image is loaded fine
      if(src.empty()){
          printf(" Error opening file\n");
          printf(" Program Arguments: [file_name -- 0 for default camera or if no arg passed default %s] \n", default_file);
          return -1;
      }
      // Edge detection with thresholds of 50 to 200 intensity for hysteresis with 3x3 sobel and default l1 norm 
      Canny(src, dst, 50, 200, 3);

      if(crop) {
      //setup region of interest if looking at car
        roidst = dst(Rect(0,0,1280,600));      
      } else {
        roidst = dst;
      }

      if (!prob) {

      // Standard Hough Line Transform
      vector<Vec2f> lines; // will hold the results of the detection
      HoughLines(roidst, lines, 1, CV_PI/180, 150, 0, 0 ); // runs the actual detection
      // Draw the lines
      for( size_t i = 0; i < lines.size(); i++ )
      {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        line( src, pt1, pt2, Scalar(0,0,255), 3, LINE_AA);
      }

      } else {

      // Probabilistic Line Transform
      vector<Vec4i> linesP; // will hold the results of the detection
      HoughLinesP(roidst, linesP, 1, CV_PI/180, 150, 200, 10 ); // runs the actual detection
      // Draw the lines
      for( size_t i = 0; i < linesP.size(); i++ )
      {
        Vec4i l = linesP[i];
        line( src, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, LINE_AA);
      }

      }

      // Show results
      imshow("Source", src);
      if ((winInput = waitKey(1)) == ESCAPE_KEY)
      {
          break;
      } else {
        if(winInput == 's') {
          String imgname = to_string(cnt) + ".png";
          imwrite(imgname, src);
        }
        if(winInput == 'p') {
          if(!prob) {
            printf("Switching to Probabilistic Hough\n");
          } else {
            printf("Switching to Standard Hough\n");
          }
          prob = prob ? 0 : 1;
        }
        if(winInput == 'c') {
          if(!crop) {
            printf("Cropping Bottom Region (car hood)\n");
          } else {
            printf("Using full image\n");
          }
          crop = crop ? 0 : 1;
        }
      }
   }
   destroyWindow("Source"); 
}
