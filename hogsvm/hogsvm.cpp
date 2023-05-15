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

#include "../utils/labelColors.h"

// See www.asciitable.com
#define ESCAPE_KEY (27)
#define SYSTEM_ERROR (-1)

int main(int argc, char** argv)
{
//setup standard OpenCV capture and output file
   const char* default_file = "0";
   const char* output_file = "vout.csv";
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
   namedWindow(filename);
   char winInput;
   int cnt=0;

//setup original frame and resized to be src for hogsvm
   Mat orig,src;

//flags to keep track of labels
  bool hadPr=0,hadR=0,hadPh=0,hadL=0,hadO=0,hadD=0;


//load model trained with train_HOG.cpp
   HOGDescriptor hog;
   hog.load( "syringe.yml" );

   while (1)
   {
      cnt++;
      cap.read(orig);
      if(orig.empty())
      {
        break;
      }

//resize frame to 1/4 size to increase detection speed
        resize(orig, src, Size(320,240), INTER_LINEAR);
        vector< Rect > detections;
        vector< double > foundWeights;
//perform detections
        hog.detectMultiScale( src, detections, foundWeights );
        bool hasPr=0,hasR=0,hasPh=0,hasL=0,hasO=0,hasD=0;
        for ( size_t j = 0; j < detections.size(); j++ )
        {
//draw rectangle around detected syringe areas
            Point rtl = detections[j].tl();
            Point rbr = detections[j].br();
            rtl*=4;rbr*=4;
            Scalar color = Scalar ( 0,0,255);
            if(rtl.x+350 < orig.rows-100 && rtl.y+100 < orig.cols-50){
              rectangle( orig, Point(rtl.x+150,rtl.y+50), Point(rtl.x+400,rtl.y+150), color, src.cols / 400 + 1 );
              Mat syringeLabel = orig(Rect(rtl.x,rtl.y,400,100));
//call labelColors function from include file, works similar to apps in ocr folder
              switch(labelColors(syringeLabel,1,1,1,1,1,1))
              {
                case 1: hasPr=1; break;
                case 2: hasR=1; break;
                case 3: hasPh=1; break;
                case 4: hasL=1; break;
                case 5: hasD=1; break;
                case 6: hasO=1; break;
              }
            }
        }

//put name of medications detected
      if(hasR) {
        putText(orig, "Rocuronium", Point(30,100), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0,0,255), 1, LINE_AA);
      }
      if(hasPr) {
        putText(orig, "Propofol", Point(30,50), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0,0,255), 1, LINE_AA);
      }
      if(hasPh) {
        putText(orig, "Phenylephrine", Point(30,150), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0,0,255), 1, LINE_AA);
      }

      if(hasL) { 
        putText(orig, "Lidocaine", Point(30,200), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0,0,255), 1, LINE_AA); 
      }

      if(hasO) { 
        putText(orig, "Odanestron", Point(30,250), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0,0,255), 1, LINE_AA); 
      }

      if(hasD) { 
        putText(orig, "Dexamethasone", Point(30,300), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0,0,255), 1, LINE_AA); 
      }

//add timestamp if medication was previously detected but now is not
      std:string msecs = to_string(int(cap.get(CAP_PROP_POS_MSEC)));
      if(hasR!=hadR)
      {
        hadR=hasR;
        outfile << "Rocuronium" << "," << msecs << endl;      
      }
      if(hasPr!=hadPr)
      {
        hadPr=hasPr;
        outfile << "Propofol" << "," << msecs << endl;      
      }
      if(hasPh!=hadPh)
      {
        hadPh=hasPh;
        outfile << "Phenylephrine" << "," << msecs << endl;      
      }
      if(hasL!=hadL)
      {
        hadL=hasL;
        outfile << "Lidocaine" << "," << msecs << endl;      
      }
      if(hasO!=hadO)
      {
        hadO=hasO;
        outfile << "Odanestron" << "," << msecs << endl;      
      }
      if(hasD!=hadD)
      {
        hadD=hasD;
        outfile << "Dexamethasone" << "," << msecs << endl;      
      }
      
//show image
      imshow(filename,orig);

//capture key to escape,step through next frame or pause
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
   destroyWindow(filename); 
   outfile.close();
}
