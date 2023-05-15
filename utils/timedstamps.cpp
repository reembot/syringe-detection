#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

// See www.asciitable.com
#define ESCAPE_KEY (27)
#define SYSTEM_ERROR (-1)

int main(int argc, char** argv)
{
   const char* input_file = "../data/voutPr.csv";
   const char* input_tfile = "../explore/voutPrcolor1.csv";
   const char* in_filename = argc >=2 ? argv[1] : input_file;
   const char* in_tfilename = argc >=3 ? argv[2] : input_tfile;

   std:string med, medtime, testmed, testtime, oldmedtime,oldtesttime;
   ifstream infile, intfile;
   infile.open(in_filename);
   intfile.open(in_tfilename);
   int msec = 0;
   int meddetect=0,testdetect=0;

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
        meddetect++;
        if (infile.good()) 
        {
          std::getline(infile,med,',');
          oldmedtime = medtime;
          std::getline(infile,medtime);
        }
      }
      if (to_string(msec) == testtime)
      {
        do 
        {
        testdetect++;
          if (intfile.good()) 
          {
            std::getline(intfile,testmed,',');
            oldtesttime = testtime;
            std::getline(intfile,testtime);
          }
        } 
        while(testtime==oldtesttime && intfile.eof() != 1); //corner case for white labels
      }



      if(infile.eof() && intfile.eof()) { break; }
      if(msec > 59000) { break; }
      if(med == "" && testmed == "") { break; }

      if(oldtesttime == testtime && oldmedtime == medtime) { break; }
   }

   infile.close();
   intfile.close();

   int medresponse=60000/meddetect;
   int testresponse=60000/testdetect;
   int responsediff=abs(medresponse-testresponse);
//   cout << meddetect << " " << medresponse << " " << testdetect << " " << testresponse << " " << responsediff << endl;
   cout << responsediff;

};
