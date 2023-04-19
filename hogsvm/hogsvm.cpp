#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/objdetect.hpp"
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
   const char* output_file = "vout.csv";
   const char* filename = argc >=2 ? argv[1] : default_file;
   const char* out_filename = argc >=3 ? argv[2] : output_file;
   ofstream outfile;
   outfile.open(out_filename);
   VideoCapture cap;
   int waitval = 100;
   bool origView=1;
   if (*filename == '0') {
     cap.open(0);
   } else {
     cap.open(filename);
   }
   namedWindow(filename);
   char winInput;
   int cnt=0;

   Mat orig,src;

   HOGDescriptor hog;
   hog.load( "syringe.yml" );


//   VideoWriter vout;
//   Size S = Size((int) cap.get(CAP_PROP_FRAME_WIDTH),    // Acquire input size
//                  (int) cap.get(CAP_PROP_FRAME_HEIGHT));

//   vout.open("colors.mp4", VideoWriter::fourcc('H','2','6','4'), cap.get(CAP_PROP_FPS), S, true);

   printf("Press space to pause and unpause, 's' to step through each frame, 'c' to switch views or 'esc' to exit\n");

   while (1)
   {
      cnt++;
      cap.read(orig);
      if(orig.empty())
      {
        break;
      }
        resize(orig, src, Size(320,240), INTER_LINEAR);
        vector< Rect > detections;
        vector< double > foundWeights;
        hog.detectMultiScale( src, detections, foundWeights );
        for ( size_t j = 0; j < detections.size(); j++ )
        {
//            std::cout << "Frame " << cnt << " HOG #" << j << " Weight " << foundWeights[j] << endl;
          if(foundWeights[j] > 0.4) {
            Point rtl = detections[j].tl();
            Point rbr = detections[j].br();
            rtl*=4;rbr*=4;
            Scalar color = Scalar( 0, foundWeights[j] * foundWeights[j] * 200, 0 );
            rectangle( orig, rtl, rbr, color, src.cols / 400 + 1 );
          }
        }

      if (origView)
      {
        imshow(filename,orig);
      } else {
        imshow(filename,src);
      }

//      vout.write(src);


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
