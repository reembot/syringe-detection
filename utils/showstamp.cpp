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
#include <sstream>

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
   bool hasPropofol=0, hasRocuronium=0, hasPhenylephrine=0, hasLidocaine=0, hasOdanestron=0, hasDexamethasone=0;
   const char* input_file = "../data/vout1.csv";
   const char* input_vfile = "../data/vout1.mp4";
   const char* output_vfile = "../data/vout1stamped.mp4";
   const char* in_filename = argc >=2 ? argv[1] : input_file;
   const char* in_vfilename = argc >=3 ? argv[2] : input_vfile;
   const char* out_vfilename = argc >=4 ? argv[3] : output_vfile;
   String med, medtime;
   ifstream infile;
   infile.open(in_filename);
   VideoCapture cap(in_vfilename);
   namedWindow(in_vfilename);
   char winInput;
   Mat src;
   int waitval = 100;

   cap.set(CAP_PROP_FRAME_WIDTH, 1280);
   cap.set(CAP_PROP_FRAME_HEIGHT, 720);

//   VideoWriter vout;
//    Size S = Size((int) cap.get(CAP_PROP_FRAME_WIDTH),    // Acquire input size
//                  (int) cap.get(CAP_PROP_FRAME_HEIGHT));

//   vout.open(out_vfilename, VideoWriter::fourcc('H','2','6','4'), cap.get(CAP_PROP_FPS), S, true);

   if (infile.good()) {
     std::getline(infile,med,',');
     std::getline(infile,medtime);
   }

   while (1)
   {
      cap.read(src);
      if (to_string(int(cap.get(CAP_PROP_POS_MSEC))) == medtime)
      {
        if(med == "Propofol")
        {
          hasPropofol = hasPropofol ? 0 : 1;
        } 
        else if (med == "Rocuronium") 
        {
          hasRocuronium = hasRocuronium ? 0 : 1;
        }
        else if (med == "Phenylephrine") 
        {
          hasPhenylephrine = hasPhenylephrine ? 0 : 1;
        }
        else if (med == "Lidocaine") 
        {
          hasLidocaine = hasLidocaine ? 0 : 1;
        }
        else if (med == "Odanestron") 
        {
          hasOdanestron = hasOdanestron ? 0 : 1;
        }
        else if (med == "Dexamethasone") 
        {
          hasDexamethasone = hasDexamethasone ? 0 : 1;
        }
        if (infile.good()) {
          std::getline(infile,med,',');
          std::getline(infile,medtime);
        }
      }
      if(hasPropofol) { putText(src, "Propofol", Point(30,50), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0,0,255), 1, LINE_AA); }
      if(hasRocuronium) { putText(src, "Rocuronium", Point(30,100), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0,0,255), 1, LINE_AA); }
      if(hasPhenylephrine) { putText(src, "Phenylephrine", Point(30,150), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0,0,255), 1, LINE_AA); }
      if(hasLidocaine) { putText(src, "Lidocaine", Point(30,200), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0,0,255), 1, LINE_AA); }
      if(hasOdanestron) { putText(src, "Odanestron", Point(30,250), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0,0,255), 1, LINE_AA); }
      if(hasDexamethasone) { putText(src, "Dexamethasone", Point(30,300), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0,0,255), 1, LINE_AA); }

      if (src.empty())
      {
        break;
      }
      imshow(in_vfilename, src);
//      vout.write(src);
      if ((winInput = waitKey(waitval)) == ESCAPE_KEY)
      {
        break;
      }
   }

   destroyWindow(in_vfilename); 
   infile.close();
   std::cout << "Wrote " << out_vfilename << "." << endl;
};
