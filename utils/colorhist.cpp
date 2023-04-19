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
   const char* output_file = "vout.csv";
   const char* filename = argc >=2 ? argv[1] : default_file;
   String out_filename=filename, out4_filename=filename, outper_filename=filename;
   out_filename += ".colorhist.csv";
   out4_filename += ".color4hist.csv";
   outper_filename += ".colorperhist.csv";
   ofstream outfile,out4file,outperfile;
   outfile.open(out_filename);
   out4file.open(out4_filename);
   outperfile.open(outper_filename);
   VideoCapture cap;
   int waitval = 0;
   if (*filename == '0') {
     cap.open(0);
   } else {
     cap.open(filename);
   }
   namedWindow(filename);
   char winInput;
   int cnt=0;

   Mat src;

//   printf("Press space to pause and unpause, 's' to step through each frame, or 'esc' to exit\n");

   while (1)
   {
      cap.read(src);
      if(src.empty())
      {
        break;
      }
      cnt++;
      int bhist[256] = {0}, ghist[256] = {0}, rhist[256] = {0};
      int b4hist[4] = {0}, g4hist[4] = {0}, r4hist[4] = {0};
      for(int j=0;j<src.rows;j++)
      {
        for(int i=0;i<src.cols;i++)
        {
           bhist[src.at<Vec3b>(j,i)[0]]++;
           if(src.at<Vec3b>(j,i)[0]<64){b4hist[0]++;}else if(src.at<Vec3b>(j,i)[0]<128){b4hist[1]++;}else if(src.at<Vec3b>(j,i)[0]<192){b4hist[2]++;}else{b4hist[3]++;}
           ghist[src.at<Vec3b>(j,i)[1]]++;
           if(src.at<Vec3b>(j,i)[1]<64){g4hist[0]++;}else if(src.at<Vec3b>(j,i)[1]<128){g4hist[1]++;}else if(src.at<Vec3b>(j,i)[1]<192){g4hist[2]++;}else{g4hist[3]++;}
           rhist[src.at<Vec3b>(j,i)[2]]++;
           if(src.at<Vec3b>(j,i)[2]<64){r4hist[0]++;}else if(src.at<Vec3b>(j,i)[2]<128){r4hist[1]++;}else if(src.at<Vec3b>(j,i)[2]<192){r4hist[2]++;}else{r4hist[3]++;}
        }
      }
      for(int i=0;i<256;i++){
      outfile << cnt << "," << i << "," << bhist[i] << "," << ghist[i] << "," << rhist[i] << endl;
      }
      int totalp = src.rows*src.cols;
      for(int i=0;i<4;i++){
      out4file << cnt << "," << i << "," << b4hist[i] << "," << g4hist[i] << "," << r4hist[i] << endl;
      outperfile << cnt << "," << i << "," << to_string((float)b4hist[i]/totalp) << "," << to_string((float)g4hist[i]/totalp) << "," << to_string((float)r4hist[i]/totalp) << endl;
      }
      imshow(filename,src);

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

   }
   std::cout << "Histogram csvs written." << endl;
   destroyWindow(filename); 
   outfile.close();
   out4file.close();
   outperfile.close();
}
