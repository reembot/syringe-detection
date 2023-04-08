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
   bool testPropofol=0, testRocuronium=0, testPhenylephrine=0, testLidocaine=0, testOdanestron=0, testDexamethasone=0;
   int tp=0,tn=0,fp=0,fn=0;
   const char* input_file = "../data/voutPr.csv";
   const char* input_tfile = "../explore/voutPrcolor1.csv";
   const char* in_filename = argc >=2 ? argv[1] : input_file;
   const char* in_tfilename = argc >=3 ? argv[2] : input_tfile;
   String med, medtime, testmed, testtime;
   ifstream infile, intfile;
   infile.open(in_filename);
   intfile.open(in_tfilename);
   int msec = 0;

   if (infile.good()) {
     std::getline(infile,med,',');
     std::getline(infile,medtime);
   }

   if (intfile.good()) {
     std::getline(intfile,testmed,',');
     std::getline(intfile,testtime);
   }

   while (1)
   {
      msec+=100;
      if (to_string(msec) == medtime)
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
      if (to_string(msec) == testtime)
      {
        if(testmed == "Propofol")
        {
          testPropofol = testPropofol ? 0 : 1;
        } 
        else if (testmed == "Rocuronium") 
        {
          testRocuronium = testRocuronium ? 0 : 1;
        }
        else if (testmed == "Phenylephrine") 
        {
          testPhenylephrine = testPhenylephrine ? 0 : 1;
        }
        else if (testmed == "Lidocaine") 
        {
          testLidocaine = testLidocaine ? 0 : 1;
        }
        else if (testmed == "Odanestron") 
        {
          testOdanestron = testOdanestron ? 0 : 1;
        }
        else if (testmed == "Dexamethasone") 
        {
          testDexamethasone = testDexamethasone ? 0 : 1;
        }
        if (intfile.good()) {
          std::getline(intfile,testmed,',');
          std::getline(intfile,testtime);
        }
      }

      if(hasPropofol && testPropofol) { tp++; }
      if(hasRocuronium && testRocuronium) { tp++; }
      if(hasPhenylephrine && testPhenylephrine) { tp++; } 
      if(hasLidocaine && testLidocaine) { tp++; }
      if(hasOdanestron && testOdanestron) { tp++; }
      if(hasDexamethasone && testDexamethasone) { tp++; }

      if(!hasPropofol && !testPropofol) { tn++; }
      if(!hasRocuronium && !testRocuronium) { tn++; }
      if(!hasPhenylephrine && !testPhenylephrine) { tn++; } 
      if(!hasLidocaine && !testLidocaine) { tn++; }
      if(!hasOdanestron && !testOdanestron) { tn++; }
      if(!hasDexamethasone && !testDexamethasone) { tn++; }

      if(!hasPropofol && testPropofol) { fp++; }
      if(!hasRocuronium && testRocuronium) { fp++; }
      if(!hasPhenylephrine && testPhenylephrine) { fp++; } 
      if(!hasLidocaine && testLidocaine) { fp++; }
      if(!hasOdanestron && testOdanestron) { fp++; }
      if(!hasDexamethasone && testDexamethasone) { fp++; }

      if(hasPropofol && !testPropofol) { fn++; }
      if(hasRocuronium && !testRocuronium) { fn++; }
      if(hasPhenylephrine && !testPhenylephrine) { fn++; } 
      if(hasLidocaine && !testLidocaine) { fn++; }
      if(hasOdanestron && !testOdanestron) { fn++; }
      if(hasDexamethasone && !testDexamethasone) { fn++; }

      if(infile.eof() && intfile.eof()) { break; }
      if(msec > 120000) { break; }
      if(med == "" && testmed == "") { break; }

   }
   infile.close();
   intfile.close();
   std::cout << "Total Time: " << msec << endl;
   std::cout << "Frames:" << msec/100 << endl;
   std::cout << "Conditions (Frames x Categories): " << (msec/100) * 6 << endl;
   std::cout << "TP: " << to_string(tp) << " FP: " << to_string(fp) << " TN: " << to_string(tn) << " FN: " << to_string(fn) << " Total: " << to_string(tp+fp+tn+fn) << endl;
   std::cout << "TPR: " << ( (float) tp / ( (float) tp + (float) fn ) * 100 ) << endl; 
   std::cout << "FPR: " << ( (float) fp / ( (float) fp + (float) tn ) * 100 ) << endl; 
   std::cout << "PPR: " << ( (float) tp / ( (float) tp + (float) fp ) * 100 ) << endl; 
   std::cout << "ACC: " << ( ( (float) tp + (float) tn ) / ( (float) tp + (float) tn + (float) fp + (float) fn) * 100 ) << endl; 


};
