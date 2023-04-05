/*
 *
 *  Example by Sam Siewert 
 *
 *  Created for OpenCV 4.x for Jetson Nano 2g, based upon
 *  https://docs.opencv.org/4.1.1
 *
 *  Tested with JetPack 4.6 which installs OpenCV 4.1.1
 *  (https://developer.nvidia.com/embedded/jetpack)
 *
 *  Based upon earlier simpler-capture examples created
 *  for OpenCV 2.x and 3.x (C and C++ mixed API) which show
 *  how to use OpenCV instead of lower level V4L2 API for the
 *  Linux UVC driver.
 *
 *  Verify your hardware and OS configuration with:
 *  1) lsusb
 *  2) ls -l /dev/video*
 *  3) dmesg | grep UVC
 *
 *  Note that OpenCV 4.x only supports the C++ API
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <opencv2/videoio.hpp>  // Video write

using namespace cv;
using namespace std;

// See www.asciitable.com
#define ESCAPE_KEY (27)
#define SYSTEM_ERROR (-1)

int main(int argc, char** argv)
{
   const char* default_file = "0";
   const char* filename = argc >=2 ? argv[1] : default_file;
   VideoCapture cap;
   int cnt = 0;
   if (*filename == '0') {
     cap.open(0);
   } else {
     cap.open(filename);
   }

   printf("Press 's' to write screen capture or 'esc' to exit\n");

   namedWindow("skeleton");
   char winInput;
   Mat src;

//   cap.set(CAP_PROP_FRAME_WIDTH, 640);
//   cap.set(CAP_PROP_FRAME_HEIGHT, 480);
   cap.read(src);

   VideoWriter vout;
    Size S = Size((int) cap.get(CAP_PROP_FRAME_WIDTH),    // Acquire input size
                  (int) cap.get(CAP_PROP_FRAME_HEIGHT));

   vout.open("skel.mp4", VideoWriter::fourcc('H','2','6','4'), cap.get(CAP_PROP_FPS), S, true);


   while (1)
   {
      cnt++;
      cap.read(src);
      Mat gray, binary, mfblur;


      cvtColor(src, gray, COLOR_BGR2GRAY);

 // Use 70 negative for Moose, 150 positive for hand
 // 
 // To improve, compute a histogram here and set threshold to first peak
 //
 // For now, histogram analysis was done with GIMP
 //
      threshold(gray, binary, 150, 255, THRESH_BINARY);
      binary = 255 - binary;

 // To remove median filter, just replace blurr value with 1
      medianBlur(binary, mfblur, 1);

 // This section of code was adapted from the following post, which was
 // based in turn on the Wikipedia description of a morphological skeleton
 //
 // http://felix.abecassis.me/2011/09/opencv-morphological-skeleton/
 //
      Mat skel(mfblur.size(), CV_8UC1, Scalar(0));
      Mat temp;
      Mat eroded;
      Mat element = getStructuringElement(MORPH_CROSS, Size(3, 3));
      bool done;
      int iterations=0;

      do
      {
        erode(mfblur, eroded, element);
        dilate(eroded, temp, element);
        subtract(mfblur, temp, temp);
        bitwise_or(skel, temp, skel);
        eroded.copyTo(mfblur);

        done = (countNonZero(mfblur) == 0);
        iterations++;
 
      } while (!done && (iterations < 100));

      cvtColor(skel, skel, COLOR_GRAY2RGB);
      imshow("skeleton", skel);
      vout.write(skel);


      if ((winInput = waitKey(1)) == ESCAPE_KEY)
      {
          break;
      }
      else if(winInput == 's') 
      {
          String imgname = to_string(cnt) + ".png";
          String skelimgname = to_string(cnt) + "skel.png";
          imwrite(imgname, src);
          imwrite(skelimgname, skel);
      }
      
   }

   destroyWindow("skeleton"); 
};
