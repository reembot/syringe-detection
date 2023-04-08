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
   const char* default_file = "vout.mp4";
   const char* filename = argc >=2 ? argv[1] : default_file;
   VideoCapture cap(0);
   namedWindow(filename);
   char winInput;
   Mat src;
   int cnt=0;

   cap.set(CAP_PROP_FRAME_WIDTH, 1280);
   cap.set(CAP_PROP_FRAME_HEIGHT, 720);
   cap.read(src);

   VideoWriter vout;
    Size S = Size((int) cap.get(CAP_PROP_FRAME_WIDTH),    // Acquire input size
                  (int) cap.get(CAP_PROP_FRAME_HEIGHT));

   vout.open(filename, VideoWriter::fourcc('H','2','6','4'), cap.get(CAP_PROP_FPS), S, true);


   while (1)
   {
      cnt++;
      cap.read(src);
      imshow(filename, src);
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
      
   }

   destroyWindow(filename); 
};
