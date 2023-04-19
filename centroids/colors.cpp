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

int main(int argc, char** argv)
{
   const char* default_file = "0";
   const char* output_file = "../data/vout.csv";
   const char* filename = argc >=2 ? argv[1] : default_file;
   const char* out_filename = argc >=3 ? argv[2] : output_file;
   ofstream outfile;
   outfile.open(out_filename);
   VideoCapture cap;
   int waitval = 100;
   if (*filename == '0') {
     cap.open(0);
   } else {
     cap.open(filename);
   }
   bool origView=1,hasRoc=0,hadRoc=0,hasProp=0,hadProp=0,hasPhen=0,hadPhen=0;
   int prevStamp=0;
   namedWindow(filename);
   char winInput;

   Mat src, srcSeg;

   cap.read(src);
   if(src.empty())
   {
     return 0;
   }
   srcSeg = src.clone();

   VideoWriter vout;
   Size S = Size((int) cap.get(CAP_PROP_FRAME_WIDTH),    // Acquire input size
                  (int) cap.get(CAP_PROP_FRAME_HEIGHT));

   vout.open("colors.mp4", VideoWriter::fourcc('H','2','6','4'), cap.get(CAP_PROP_FPS), S, true);

   printf("Press space to pause and unpause, 's' to step through each frame, 'c' to switch views or 'esc' to exit\n");

   while (1)
   {
      cap.read(src);
      if(src.empty())
      {
        break;
      }
      if(int(cap.get(CAP_PROP_POS_MSEC)) - prevStamp > 1500) {
        hadRoc = hasRoc;
        inRange(src,Scalar(0,0,225),Scalar(150,150,255),srcSeg);
        hasRoc = (sum(srcSeg)[0] > 1000);
        if(hadRoc != hasRoc) {
          outfile << "Rocuronium" << "," << cap.get(CAP_PROP_POS_MSEC) << endl;
          prevStamp = int(cap.get(CAP_PROP_POS_MSEC));      
        } else {
        hadProp = hasProp;
        inRange(src,Scalar(0,240,240),Scalar(200,255,255),srcSeg);
        hasProp = (sum(srcSeg)[0] > 1000);
        if(hadProp != hasProp) {
          outfile << "Propofol" << "," << cap.get(CAP_PROP_POS_MSEC) << endl;
          prevStamp = int(cap.get(CAP_PROP_POS_MSEC));
        } else {
        hadPhen = hasPhen;
        inRange(src,Scalar(200,0,200),Scalar(255,200,255),srcSeg);
        std::cout << sum(srcSeg)[0] << endl;
        hasPhen = (sum(srcSeg)[0] > 75000);
        if(hadPhen != hasPhen) {
          outfile << "Phenylephrine" << "," << cap.get(CAP_PROP_POS_MSEC) << endl;
          prevStamp = int(cap.get(CAP_PROP_POS_MSEC));
        }
        }
        }
      }
      if(hasRoc) {
        putText(src, "Rocuronium", Point(30,100), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0,0,255), 1, LINE_AA);
      }
      if(hasProp) {
        putText(src, "Propofol", Point(30,50), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0,0,255), 1, LINE_AA);
      }
      if(hasPhen) {
        putText(src, "Phenylephrine", Point(30,150), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0,0,255), 1, LINE_AA);
      }


      if (origView)
      {
        imshow(filename,src);
      } else {
        imshow(filename,srcSeg);
      }

      vout.write(src);

      if ((winInput = waitKey(waitval)) == ESCAPE_KEY)
      {
          break;
      }
      else if(winInput == 's') 
      {
          waitval=0;
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
   outfile.close();
}
