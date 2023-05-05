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
   bool hasPropofol=0, hasRocuronium=0, hasPhenylephrine=0, hasLidocaine=0, hasOdanestron=0, hasDexamethasone=0;
   bool testPropofol=0, testRocuronium=0, testPhenylephrine=0, testLidocaine=0, testOdanestron=0, testDexamethasone=0;
   int tp=0,tn=0,fp=0,fn=0;
   int Prtp=0,Prtn=0,Prfp=0,Prfn=0;
   int Rtp=0,Rtn=0,Rfp=0,Rfn=0;
   int Phtp=0,Phtn=0,Phfp=0,Phfn=0;
   int Ltp=0,Ltn=0,Lfp=0,Lfn=0;
   int Otp=0,Otn=0,Ofp=0,Ofn=0;
   int Dtp=0,Dtn=0,Dfp=0,Dfn=0;
   const char* input_file = "../data/voutPr.csv";
   const char* input_tfile = "../explore/voutPrcolor1.csv";
   const char* in_filename = argc >=2 ? argv[1] : input_file;
   const char* in_tfilename = argc >=3 ? argv[2] : input_tfile;
   std:string med, medtime, testmed, testtime, oldmedtime,oldtesttime;
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

//   cout << "test1" << endl;

   while (1)
   {
//      std::cout << testtime << " " << testmed << medtime << med << endl;
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
        if (intfile.good()) 
        {
          std::getline(intfile,testmed,',');
          oldtesttime = testtime;
          std::getline(intfile,testtime);
        }
//        cout << "testmed " << testmed << " testtime " << testtime << " oldtesttime " << oldtesttime << " eof " << intfile.eof() << endl;
        } 
        while(testtime==oldtesttime && intfile.eof() != 1); //corner case for white labels
      }

      if(hasPropofol && testPropofol) { tp++; Prtp++; }
      if(hasRocuronium && testRocuronium) { tp++; Rtp++; }
      if(hasPhenylephrine && testPhenylephrine) { tp++; Phtp++; } 
      if(hasLidocaine && testLidocaine) { tp++; Ltp++; }
      if(hasOdanestron && testOdanestron) { tp++; Otp++; }
      if(hasDexamethasone && testDexamethasone) { tp++; Dtp++; }

      if(!hasPropofol && !testPropofol) { tn++; Prtn++; }
      if(!hasRocuronium && !testRocuronium) { tn++; Rtn++; }
      if(!hasPhenylephrine && !testPhenylephrine) { tn++; Phtn++; } 
      if(!hasLidocaine && !testLidocaine) { tn++; Ltn++; }
      if(!hasOdanestron && !testOdanestron) { tn++; Otn++; }
      if(!hasDexamethasone && !testDexamethasone) { tn++; Dtn++; }

      if(!hasPropofol && testPropofol) { fp++; Prfp++; }
      if(!hasRocuronium && testRocuronium) { fp++; Rfp++; }
      if(!hasPhenylephrine && testPhenylephrine) { fp++; Phfp++; } 
      if(!hasLidocaine && testLidocaine) { fp++; Lfp++; }
      if(!hasOdanestron && testOdanestron) { fp++; Ofp++; }
      if(!hasDexamethasone && testDexamethasone) { fp++; Dfp++; }

      if(hasPropofol && !testPropofol) { fn++; Prfn++; }
      if(hasRocuronium && !testRocuronium) { fn++; Rfn++; }
      if(hasPhenylephrine && !testPhenylephrine) { fn++; Phfn++; } 
      if(hasLidocaine && !testLidocaine) { fn++; Lfn++; }
      if(hasOdanestron && !testOdanestron) { fn++; Ofn++; }
      if(hasDexamethasone && !testDexamethasone) { fn++; Dfn++; }

      if(infile.eof() && intfile.eof()) { break; }
      if(msec > 59000) { break; }
      if(med == "" && testmed == "") { break; }
/*
      std::cout << med << endl;
      std::cout << testmed << endl;
      std::cout << oldtesttime << " vs " << testtime << endl;
      std::cout << oldmedtime << " vs " << medtime << endl;
      std::cout << med << endl;
*/
      if(oldtesttime == testtime && oldmedtime == medtime) { break; }
   }
   infile.close();
   intfile.close();
   int catnum = 0;
   if(Prtp+Prfp+Prfn > 0) {
/*
   std::cout << "Propofol" << endl;
   std::cout << "TP: " << to_string(Prtp) << " FP: " << to_string(Prfp) << " TN: " << to_string(Prtn) << " FN: " << to_string(Prfn) << " Total: " << to_string(Prtp+Prfp+Prtn+Prfn) << endl;
   std::cout << "TPR: " << ( (float) Prtp / ( (float) Prtp + (float) Prfn ) * 100 ) << endl; 
   std::cout << "FPR: " << ( (float) Prfp / ( (float) Prfp + (float) Prtn ) * 100 ) << endl; 
   std::cout << "PPR: " << ( (float) Prtp / ( (float) Prtp + (float) Prfp ) * 100 ) << endl; 
   std::cout << "ACC: " << ( ( (float) Prtp + (float) Prtn ) / ( (float) Prtp + (float) Prtn + (float) Prfp + (float) Prfn) * 100 ) << endl; 
   std::cout << "F1: " << ( ( 2 * (float) Prtp) / ( 2 * (float) Prtp + (float) Prfp + (float) Prfn) * 100 ) << endl; 
*/
   std::cout << in_filename << ",";
   std::cout << in_tfilename << ",";
   std::cout << "Propofol";
   std::cout << "," << to_string(Prtp) << "," << to_string(Prfp) << "," << to_string(Prtn) << "," << to_string(Prfn) << "," << to_string(Prtp+Prfp+Prtn+Prfn);
   std::cout << "," << ( (float) Prtp / ( (float) Prtp + (float) Prfn ) * 100 ); 
   std::cout << "," << ( (float) Prfp / ( (float) Prfp + (float) Prtn ) * 100 ); 
   std::cout << "," << ( (float) Prtp / ( (float) Prtp + (float) Prfp ) * 100 ); 
   std::cout << "," << ( ( (float) Prtp + (float) Prtn ) / ( (float) Prtp + (float) Prtn + (float) Prfp + (float) Prfn) * 100 ); 
   std::cout << "," << ( ( 2 * (float) Prtp) / ( 2 * (float) Prtp + (float) Prfp + (float) Prfn) * 100 ) << endl; 
   catnum++;
   }
   if(Rtp+Rfp+Rfn > 0) {
   std::cout << in_filename << ",";
   std::cout << in_tfilename << ",";
   std::cout << "Rocuronium";
   std::cout << "," << to_string(Rtp) << "," << to_string(Rfp) << "," << to_string(Rtn) << "," << to_string(Rfn) << "," << to_string(Rtp+Rfp+Rtn+Rfn);
   std::cout << "," << ( (float) Rtp / ( (float) Rtp + (float) Rfn ) * 100 ); 
   std::cout << "," << ( (float) Rfp / ( (float) Rfp + (float) Rtn ) * 100 ); 
   std::cout << "," << ( (float) Rtp / ( (float) Rtp + (float) Rfp ) * 100 ); 
   std::cout << "," << ( ( (float) Rtp + (float) Rtn ) / ( (float) Rtp + (float) Rtn + (float) Rfp + (float) Rfn) * 100 ); 
   std::cout << "," << ( ( 2 * (float) Rtp) / ( 2 * (float) Rtp + (float) Rfp + (float) Rfn) * 100 ) << endl; 
/*
   std::cout << "TP: " << to_string(Rtp) << " FP: " << to_string(Rfp) << " TN: " << to_string(Rtn) << " FN: " << to_string(Rfn) << " Total: " << to_string(Rtp+Rfp+Rtn+Rfn) << endl;
   std::cout << "TPR: " << ( (float) Rtp / ( (float) Rtp + (float) Rfn ) * 100 ) << endl; 
   std::cout << "FPR: " << ( (float) Rfp / ( (float) Rfp + (float) Rtn ) * 100 ) << endl; 
   std::cout << "PPR: " << ( (float) Rtp / ( (float) Rtp + (float) Rfp ) * 100 ) << endl; 
   std::cout << "ACC: " << ( ( (float) Rtp + (float) Rtn ) / ( (float) Rtp + (float) Rtn + (float) Rfp + (float) Rfn) * 100 ) << endl; 
   std::cout << "F1: " << ( ( 2 * (float) Rtp) / ( 2 * (float) Rtp + (float) Rfp + (float) Rfn) * 100 ) << endl; 
*/
   catnum++;
   }
   if(Phtp+Phfp+Phfn > 0) {
   std::cout << in_filename << ",";
   std::cout << in_tfilename << ",";
   std::cout << "Phenylephrine";
   std::cout << "," << to_string(Phtp) << "," << to_string(Phfp) << "," << to_string(Phtn) << "," << to_string(Phfn) << "," << to_string(Phtp+Phfp+Phtn+Phfn);
   std::cout << "," << ( (float) Phtp / ( (float) Phtp + (float) Phfn ) * 100 ); 
   std::cout << "," << ( (float) Phfp / ( (float) Phfp + (float) Phtn ) * 100 ); 
   std::cout << "," << ( (float) Phtp / ( (float) Phtp + (float) Phfp ) * 100 ); 
   std::cout << "," << ( ( (float) Phtp + (float) Phtn ) / ( (float) Phtp + (float) Phtn + (float) Phfp + (float) Phfn) * 100 ); 
   std::cout << "," << ( ( 2 * (float) Phtp) / ( 2 * (float) Phtp + (float) Phfp + (float) Phfn) * 100 ) << endl; /*
   std::cout << "TP: " << to_string(Phtp) << " FP: " << to_string(Phfp) << " TN: " << to_string(Phtn) << " FN: " << to_string(Phfn) << " Total: " << to_string(Phtp+Phfp+Phtn+Phfn) << endl;
   std::cout << "TPR: " << ( (float) Phtp / ( (float) Phtp + (float) Phfn ) * 100 ) << endl; 
   std::cout << "FPR: " << ( (float) Phfp / ( (float) Phfp + (float) Phtn ) * 100 ) << endl; 
   std::cout << "PPR: " << ( (float) Phtp / ( (float) Phtp + (float) Phfp ) * 100 ) << endl; 
   std::cout << "ACC: " << ( ( (float) Phtp + (float) Phtn ) / ( (float) Phtp + (float) Phtn + (float) Phfp + (float) Phfn) * 100 ) << endl; 
   std::cout << "F1: " << ( ( 2 * (float) Phtp) / ( 2 * (float) Phtp + (float) Phfp + (float) Phfn) * 100 ) << endl; 
*/
   catnum++;
   }
   if(Ltp+Lfp+Lfn > 0) {
   std::cout << in_filename << ",";
   std::cout << in_tfilename << ",";
   std::cout << "Lidocaine";
   std::cout << "," << to_string(Ltp) << "," << to_string(Lfp) << "," << to_string(Ltn) << "," << to_string(Lfn) << "," << to_string(Ltp+Lfp+Ltn+Lfn);
   std::cout << "," << ( (float) Ltp / ( (float) Ltp + (float) Lfn ) * 100 ); 
   std::cout << "," << ( (float) Lfp / ( (float) Lfp + (float) Ltn ) * 100 ); 
   std::cout << "," << ( (float) Ltp / ( (float) Ltp + (float) Lfp ) * 100 ); 
   std::cout << "," << ( ( (float) Ltp + (float) Ltn ) / ( (float) Ltp + (float) Ltn + (float) Lfp + (float) Lfn) * 100 ); 
   std::cout << "," << ( ( 2 * (float) Ltp) / ( 2 * (float) Ltp + (float) Lfp + (float) Lfn) * 100 ) << endl; /*
   std::cout << "TP: " << to_string(Ltp) << " FP: " << to_string(Lfp) << " TN: " << to_string(Ltn) << " FN: " << to_string(Lfn) << " Total: " << to_string(Ltp+Lfp+Ltn+Lfn) << endl;
   std::cout << "TPR: " << ( (float) Ltp / ( (float) Ltp + (float) Lfn ) * 100 ) << endl; 
   std::cout << "FPR: " << ( (float) Lfp / ( (float) Lfp + (float) Ltn ) * 100 ) << endl; 
   std::cout << "PPR: " << ( (float) Ltp / ( (float) Ltp + (float) Lfp ) * 100 ) << endl; 
   std::cout << "ACC: " << ( ( (float) Ltp + (float) Ltn ) / ( (float) Ltp + (float) Ltn + (float) Lfp + (float) Lfn) * 100 ) << endl;
   std::cout << "F1: " << ( ( 2 * (float) Ltp) / ( 2 * (float) Ltp + (float) Lfp + (float) Lfn) * 100 ) << endl; 
*/
   catnum++;
   }
   if(Otp+Ofp+Ofn > 0) {
   std::cout << in_filename << ",";
   std::cout << in_tfilename << ",";
   std::cout << "Odanestron";
   std::cout << "," << to_string(Otp) << "," << to_string(Ofp) << "," << to_string(Otn) << "," << to_string(Ofn) << "," << to_string(Otp+Ofp+Otn+Ofn);
   std::cout << "," << ( (float) Otp / ( (float) Otp + (float) Ofn ) * 100 ); 
   std::cout << "," << ( (float) Ofp / ( (float) Ofp + (float) Otn ) * 100 ); 
   std::cout << "," << ( (float) Otp / ( (float) Otp + (float) Ofp ) * 100 ); 
   std::cout << "," << ( ( (float) Otp + (float) Otn ) / ( (float) Otp + (float) Otn + (float) Ofp + (float) Ofn) * 100 ); 
   std::cout << "," << ( ( 2 * (float) Otp) / ( 2 * (float) Otp + (float) Ofp + (float) Ofn) * 100 ) << endl; /*
   std::cout << "TP: " << to_string(Otp) << " FP: " << to_string(Ofp) << " TN: " << to_string(Otn) << " FN: " << to_string(Ofn) << " Total: " << to_string(Otp+Ofp+Otn+Ofn) << endl;
   std::cout << "TPR: " << ( (float) Otp / ( (float) Otp + (float) Ofn ) * 100 ) << endl; 
   std::cout << "FPR: " << ( (float) Ofp / ( (float) Ofp + (float) Otn ) * 100 ) << endl; 
   std::cout << "PPR: " << ( (float) Otp / ( (float) Otp + (float) Ofp ) * 100 ) << endl; 
   std::cout << "ACC: " << ( ( (float) Otp + (float) Otn ) / ( (float) Otp + (float) Otn + (float) Ofp + (float) Ofn) * 100 ) << endl; 
   std::cout << "F1: " << ( ( 2 * (float) Otp) / ( 2 * (float) Otp + (float) Ofp + (float) Ofn) * 100 ) << endl; 
*/
   catnum++;
   }
   if(Dtp+Dfp+Dfn > 0) {
   std::cout << in_filename << ",";
   std::cout << in_tfilename << ",";
   std::cout << "Dexamethasone";
   std::cout << "," << to_string(Dtp) << "," << to_string(Dfp) << "," << to_string(Dtn) << "," << to_string(Dfn) << "," << to_string(Dtp+Dfp+Dtn+Dfn);
   std::cout << "," << ( (float) Dtp / ( (float) Dtp + (float) Dfn ) * 100 ); 
   std::cout << "," << ( (float) Dfp / ( (float) Dfp + (float) Dtn ) * 100 ); 
   std::cout << "," << ( (float) Dtp / ( (float) Dtp + (float) Dfp ) * 100 ); 
   std::cout << "," << ( ( (float) Dtp + (float) Dtn ) / ( (float) Dtp + (float) Dtn + (float) Dfp + (float) Dfn) * 100 ); 
   std::cout << "," << ( ( 2 * (float) Dtp) / ( 2 * (float) Dtp + (float) Dfp + (float) Dfn) * 100 ) << endl; /*
   std::cout << "TP: " << to_string(Dtp) << " FP: " << to_string(Dfp) << " TN: " << to_string(Dtn) << " FN: " << to_string(Dfn) << " Total: " << to_string(Dtp+Dfp+Dtn+Dfn) << endl;
   std::cout << "TPR: " << ( (float) Dtp / ( (float) Dtp + (float) Dfn ) * 100 ) << endl; 
   std::cout << "FPR: " << ( (float) Dfp / ( (float) Dfp + (float) Dtn ) * 100 ) << endl; 
   std::cout << "PPR: " << ( (float) Dtp / ( (float) Dtp + (float) Dfp ) * 100 ) << endl; 
   std::cout << "ACC: " << ( ( (float) Dtp + (float) Dtn ) / ( (float) Dtp + (float) Dtn + (float) Dfp + (float) Dfn) * 100 ) << endl; 
   std::cout << "F1: " << ( ( 2 * (float) Dtp) / ( 2 * (float) Dtp + (float) Dfp + (float) Dfn) * 100 ) << endl; 
*/
   catnum++;
   }
   std::cout << in_filename << ",";
   std::cout << in_tfilename << ",";
   std::cout << "All Video Meds (" << to_string(catnum) << ")";
   std::cout << "," << to_string(tp) << "," << to_string(fp) << "," << to_string(tn) << "," << to_string(fn) << "," << to_string(tp+fp+tn+fn);
   std::cout << "," << ( (float) tp / ( (float) tp + (float) fn ) * 100 ); 
   std::cout << "," << ( (float) fp / ( (float) fp + (float) tn ) * 100 ); 
   std::cout << "," << ( (float) tp / ( (float) tp + (float) fp ) * 100 ); 
   std::cout << "," << ( ( (float) tp + (float) tn ) / ( (float) tp + (float) tn + (float) fp + (float) fn) * 100 ); 
   std::cout << "," << ( ( 2 * (float) tp) / ( 2 * (float) tp + (float) fp + (float) fn) * 100 ) << endl; 
/*
   std::cout << "Total Time: " << msec << endl;
   std::cout << "Frames:" << msec/100 << endl;
   std::cout << "Conditions (Frames x Medications): " << (msec/100) * catnum << endl;
   std::cout << "All Medications" << endl;
   std::cout << "TP: " << to_string(tp) << " FP: " << to_string(fp) << " TN: " << to_string(tn) << " FN: " << to_string(fn) << " Total: " << to_string(tp+fp+tn+fn) << endl;
   std::cout << "TPR: " << ( (float) tp / ( (float) tp + (float) fn ) * 100 ) << endl; 
   std::cout << "FPR: " << ( (float) fp / ( (float) fp + (float) tn ) * 100 ) << endl; 
   std::cout << "PPR: " << ( (float) tp / ( (float) tp + (float) fp ) * 100 ) << endl; 
   std::cout << "ACC: " << ( ( (float) tp + (float) tn ) / ( (float) tp + (float) tn + (float) fp + (float) fn) * 100 ) << endl; 
   std::cout << "F1: " << ( ( 2 * (float) tp) / ( 2 * (float) tp + (float) fp + (float) fn) * 100 ) << endl; 
*/ 


};
