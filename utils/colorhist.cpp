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
   String out_filename=filename, out8_filename=filename, outper_filename=filename,outmm_filename=filename,outmmper_filename=filename;
   out_filename += ".colorhist.csv";
   out8_filename += ".color8hist.csv";
   outper_filename += ".colorperhist.csv";
   outmm_filename += ".colormmhist.csv";
   outmmper_filename += ".colormmperhist.csv";
   ofstream outfile,out4file,outperfile,outmmfile,outmmperfile;
/*
   outfile.open(out_filename);
   out4file.open(out8_filename);
   outperfile.open(outper_filename);
   outmmfile.open(outmm_filename);
*/
   outmmperfile.open(outmmper_filename);
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

   Mat presize,src;

//   printf("Press space to pause and unpause, 's' to step through each frame, or 'esc' to exit\n");

   while (1)
   {
      cap.read(presize);
      if(presize.empty())
      {
        break;
      }
      resize(presize, src, Size(175,50), INTER_LINEAR);
      cnt++;
      int mmhist[512] = {0};
      int bval=0,gval=0,rval=0,b8val=0,g8val=0,r8val=0,mmval=0;
      int bhist[256] = {0}, ghist[256] = {0}, rhist[256] = {0};
      int b8hist[8] = {0}, g8hist[8] = {0}, r8hist[8] = {0};
      int blo=0,bhi=0,glo=0,ghi=0,rlo=0,rhi=0;
      for(int j=0;j<src.rows;j++)
      {
        for(int i=0;i<src.cols;i++)
        {
           bval = src.at<Vec3b>(j,i)[0];           
           gval = src.at<Vec3b>(j,i)[1];           
           rval = src.at<Vec3b>(j,i)[2];           
           b8val = bval / 32;
           g8val = gval / 32;
           r8val = rval / 32;
           bhist[bval]++;
           ghist[gval]++;
           rhist[rval]++;
           b8hist[b8val]++;
           g8hist[g8val]++;
           r8hist[r8val]++;
           mmval=b8val*64+g8val*8+r8val;           
           mmhist[mmval]++;
/*           if(src.at<Vec3b>(j,i)[0]<32){b8hist[0]++;}else if(src.at<Vec3b>(j,i)[0]<64){b8hist[1]++;}else if(src.at<Vec3b>(j,i)[0]<96){b8hist[2]++;}else if(src.at<Vec3b>(j,i)[0]<128){b8hist[3]++;}
           else if(src.at<Vec3b>(j,i)[0]<160){b8hist[4]++;}else if(src.at<Vec3b>(j,i)[0]<192){b8hist[5]++;}else if(src.at<Vec3b>(j,i)[0]<224){b8hist[6]++;}else{b8hist[7]++;}
           ghist[src.at<Vec3b>(j,i)[1]]++;
           if(src.at<Vec3b>(j,i)[1]<32){g8hist[0]++;}else if(src.at<Vec3b>(j,i)[1]<64){g8hist[1]++;}else if(src.at<Vec3b>(j,i)[1]<96){g8hist[2]++;}else if(src.at<Vec3b>(j,i)[1]<128){g8hist[3]++;}
           else if(src.at<Vec3b>(j,i)[1]<160){g8hist[4]++;}else if(src.at<Vec3b>(j,i)[1]<192){g8hist[5]++;}else if(src.at<Vec3b>(j,i)[1]<224){g8hist[6]++;}else{g8hist[7]++;}
           rhist[src.at<Vec3b>(j,i)[2]]++;
           if(src.at<Vec3b>(j,i)[2]<32){r8hist[0]++;}else if(src.at<Vec3b>(j,i)[2]<64){r8hist[1]++;}else if(src.at<Vec3b>(j,i)[2]<96){r8hist[2]++;}else if(src.at<Vec3b>(j,i)[2]<128){r8hist[3]++;}
           else if(src.at<Vec3b>(j,i)[2]<160){r8hist[4]++;}else if(src.at<Vec3b>(j,i)[2]<192){r8hist[5]++;}else if(src.at<Vec3b>(j,i)[2]<224){r8hist[6]++;}else{r8hist[7]++;}
*/
        }
      }
      for(int i=0;i<256;i++){
//      outfile << cnt << "," << i << "," << bhist[i] << "," << ghist[i] << "," << rhist[i] << endl;
      }
      int totalp = src.rows*src.cols;
      for(int i=0;i<8;i++){
//        out4file << cnt << "," << i << "," << b8hist[i] << "," << g8hist[i] << "," << r8hist[i] << endl;
//        outperfile << cnt << "," << i << "," << to_string((float)b8hist[i]/totalp) << "," << to_string((float)g8hist[i]/totalp) << "," << to_string((float)r8hist[i]/totalp) << endl;
      }
      for(int i=0;i<512;i++){
        float mmper = (float)mmhist[i]/totalp;
        if(mmper > 0.02) {
          bval = i / 64;
          gval = (i % 64) / 8;
          rval = i % 8;
          blo = bval * 32;
          bhi = (bval + 1) * 32;
          glo = gval * 32;
          ghi = (gval + 1) * 32;
          rlo = rval * 32;
          rhi = (rval + 1) * 32;           
//          outmmfile << cnt << ",(B " << blo << "-" << bhi << " G " << glo << "-" << ghi << " R " << rlo << "-" << rhi << ")," << mmhist[i] << endl;
          outmmperfile << cnt << ",(B " << blo << "-" << bhi << " G " << glo << "-" << ghi << " R " << rlo << "-" << rhi << ")," << to_string((float)mmhist[i]/totalp) << endl;
        }
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
   outmmfile.close();
   outmmperfile.close();


}
