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
#include <fstream>
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
   const char* input_file = "../data/vout5.mp4";
   const char* output_file = "../data/vout5.csv";
   const char* in_filename = argc >=2 ? argv[1] : input_file;
   const char* out_filename = argc >=3 ? argv[2] : output_file;
   ofstream outfile;
   outfile.open(out_filename);
   VideoCapture cap(in_filename);
   namedWindow(in_filename);
   char winInput;
   Mat src;
   int waitval = 100;

//   cap.set(CAP_PROP_FRAME_WIDTH, 1280);
//   cap.set(CAP_PROP_FRAME_HEIGHT, 720);

//   std::cout << "FPS: " << cap.get(CAP_PROP_FPS) << endl;

   std::cout << "Starting video display.  Press p to pause. " << endl;

   while (1)
   {
      cap.read(src);
      if(src.empty())
      {
        break;
      }
      imshow(in_filename, src);

      if ((winInput = waitKey(waitval)) == ESCAPE_KEY)
      {
        break;
      }
      else if(winInput == 'u' && waitval == 0)
      {
        waitval = 100;
        std::cout << "Unpaused at " << cap.get(CAP_PROP_POS_MSEC) << " msecs" << endl;
        std::cout << "Press p to pause." << endl;
      }
      else if(winInput == 'p' || waitval == 0)
      {
        std::cout << "Paused at " << cap.get(CAP_PROP_POS_MSEC) << " msecs" << endl;
        if(winInput == 'p')
        {
          std::cout << "Enter label number or any other key to skip labeling." << endl;
          winInput = waitKey(0);
        }
        waitval = 0;
        if (winInput == '1' || winInput == '2' || winInput == '3' || winInput == '4' || winInput == '5' || winInput == '6') 
        {
          if(winInput == '1') 
          {
            std::cout << "Writing Propofol at " << cap.get(CAP_PROP_POS_MSEC) << " msecs" << endl;
            outfile << "Propofol" << "," << cap.get(CAP_PROP_POS_MSEC) << endl;
          }
          else if(winInput == '2') 
          {
            std::cout << "Writing Rocuronium at " << cap.get(CAP_PROP_POS_MSEC) << " msecs" << endl;
            outfile << "Rocuronium" << "," << cap.get(CAP_PROP_POS_MSEC) << endl;
          }
          else if(winInput == '3') 
          {
            std::cout << "Writing Phenylephrine at " << cap.get(CAP_PROP_POS_MSEC) << " msecs" << endl;
            outfile << "Phenylephrine" << "," << cap.get(CAP_PROP_POS_MSEC) << endl;
          }
          else if(winInput == '4') 
          {
            std::cout << "Writing Lidocaine at " << cap.get(CAP_PROP_POS_MSEC) << " msecs" << endl;
            outfile << "Lidocaine" << "," << cap.get(CAP_PROP_POS_MSEC) << endl;
          }
          else if(winInput == '5') 
          {
            std::cout << "Writing Odanestron at " << cap.get(CAP_PROP_POS_MSEC) << " msecs" << endl;
            outfile << "Odanestron" << "," << cap.get(CAP_PROP_POS_MSEC) << endl;
          }
          else if(winInput == '6') 
          {
            std::cout << "Writing Dexamethasone at " << cap.get(CAP_PROP_POS_MSEC) << " msecs" << endl;
            outfile << "Dexamethasone" << "," << cap.get(CAP_PROP_POS_MSEC) << endl;
          }
          std::cout << "Press u to unpause or any other key to move to next frame." << endl;
          if ((winInput = waitKey(0)) == 'u') 
          {
            waitval = 100;
            std::cout << "Unpaused at " << cap.get(CAP_PROP_POS_MSEC) << " msecs" << endl;
            std::cout << "Press p to pause."  << endl;
          }
          else
          {
          std::cout << "Moving to next frame. " << endl;
          std::cout << "Press u to unpause, enter label number, or any other key to skip label." << endl;
          }
        }
        else 
        {
          std::cout << "Skipped labeling and moving to next frame. " << endl;
          std::cout << "Press u to unpause, enter label number, or any other key to skip label." << endl;
        }
      }
   }

   destroyWindow(in_filename); 
   outfile.close();
};
