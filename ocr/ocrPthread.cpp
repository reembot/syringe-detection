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

#include <pthread.h>
#include <sched.h>

#define NUM_THREADS 4

typedef struct {
    int threadIdx;
    Mat roisrc;
    Mat prevroi;
    int countPr;
    int countR;
    int countPh;
    int countL;
    int countW;
    int w1x;
    int w1y;
}  threadParams_t;

// POSIX thread declarations and scheduling attributes
//
pthread_t threads[NUM_THREADS];
threadParams_t threadParams[NUM_THREADS];


// See www.asciitable.com
#define ESCAPE_KEY (27)
#define SYSTEM_ERROR (-1)

void *findLabelThread(void *threadp)
{
    threadParams_t *threadParams = (threadParams_t *)threadp;

    for(int j=threadParams->threadIdx;j<threadParams->roisrc.rows; j+=NUM_THREADS)
      {
        for(int i=0;i<threadParams->roisrc.cols; i++)
        {
//get values for current and previous frames
           float bval = (float)threadParams->roisrc.at<Vec3b>(j,i)[0];           
           float gval = (float)threadParams->roisrc.at<Vec3b>(j,i)[1];           
           float rval = (float)threadParams->roisrc.at<Vec3b>(j,i)[2];
           float bval2 = (float)threadParams->prevroi.at<Vec3b>(j,i)[0];           
           float gval2 = (float)threadParams->prevroi.at<Vec3b>(j,i)[1];           
           float rval2 = (float)threadParams->prevroi.at<Vec3b>(j,i)[2];

//check if there is difference between current and previous frame
           if(abs(bval-bval2) < 30 && abs(gval-gval2) < 30 && abs(gval-gval2) < 30) 
           {  

//check for yellow colors where red is highest followed closely by green with almost no blue
             if ( (rval > 96) && (rval-bval > rval/3) && (bval < 32)  && (gval < rval * 9 / 10) && (gval > 3 * rval / 5) ) 
             {
               threadParams->countPr++;   
             } 
//check for mostly red color
             else if ( (rval > 224) && (bval < 64) && (gval < 96) ) 
             {   
               threadParams->countR++;
             } 
//check for light purple where most values are high and green is 1/2-3/4 of red
             else if ( (rval > 128) && (bval > 128) && (gval < 3 * rval / 4) && (gval > rval / 2) ) 
             {
               threadParams->countPh++;
             } 
//check for non-color values (gray and white)
             else if ( (abs(rval-gval) < 20) && (abs(rval-bval) < 20) && (abs(gval-bval) < 20) ) 
             {
//check for gray
               if ( (rval > 48) && (gval > 48) && (bval > 48) && (rval < 80) && (gval < 80) && (bval < 80) ) 
               {
                 threadParams->countL++;
               } 
//check for white
               else if ( (rval > 175) && (gval > 175) && (bval > 175) ) 
               {
//only count if first label threshold is not met or if haven't skipped down to next reasonable label row (rowbuff)
                 if( threadParams->w1x==0 || j > threadParams->w1y + 75) 
                 { 
                   threadParams->countW++; 
                 }
                 if(threadParams->w1x==0 && threadParams->countW > 100)
                 {
                   threadParams->w1x=i; 
                   threadParams->w1y=j;
                 } 
               }
             }
           } 
        }
      }
}

int main(int argc, char** argv)
{
   const char* default_file = "0";
   const char* output_file = "vout.csv";
   const char* filename = argc >=2 ? argv[1] : default_file;
   const char* out_filename = argc >=3 ? argv[2] : output_file;
   ofstream outfile;
   outfile.open(out_filename);
   VideoCapture cap;
   int waitval = 1;
   if (*filename == '0') {
     cap.open(0);
   } else {
     cap.open(filename);
   }
   bool hadRoc=0,hadProp=0,hadPhen=0,hadLid=0,hadOd=0,hadDex=0;

   Mat src, roisrc, srcSegSum, prevroi, srcclone, dst, gray,threshed,canned;

   cap.read(src);
   if(src.empty())
   {
     return 0;
   }
   dst = src.clone();
   cvtColor(dst, gray, COLOR_BGR2GRAY);
   Canny(gray, canned, 35, 70);
   int cnt=0,circnt=0;

   while (1)
   {
      cnt++;
      cap.read(src);
      if(src.empty())
      {
        break;
      }
      
      srcclone = src.clone();
      roisrc = srcclone(Rect(150,75,750,885));

      std::string msecs = to_string(int(cap.get(CAP_PROP_POS_MSEC)));

      float bval=0,gval=0,rval=0,bval2=0,gval2=0,rval2=0;

      int rowbuff = 75;
      int W1x=0,W1y=960,W2x=0,W2y=0;

      int countR=0,countPr=0,countPh=0,countL=0,countW=0;
      bool hasRoc=0,hasProp=0,hasPhen=0,hasLid=0,hasOd=0,hasDex=0;

//skip first frame to setup previous for frame differencing
      if(cnt>1) 
      {
        for(int i=0; i<NUM_THREADS; i++)
        {
          threadParams[i].threadIdx=i;
          threadParams[i].roisrc=roisrc;
          threadParams[i].prevroi=prevroi;
          threadParams[i].countPr=0;
          threadParams[i].countR=0;
          threadParams[i].countPh=0;
          threadParams[i].countL=0;
          threadParams[i].countW=0;
          threadParams[i].w1x=0;
          threadParams[i].w1y=0;
          pthread_create(&threads[i],   // pointer to thread descriptor
            NULL,
            findLabelThread, // thread function entry point
            (void *)&(threadParams[i]) // parameters to pass in
          );
        }
        for(int i=0; i<NUM_THREADS; i++)
        {
          pthread_join(threads[i], NULL);
          countPr+=threadParams[i].countPr;
          countR+=threadParams[i].countR;
          countPh+=threadParams[i].countPh;
          countL+=threadParams[i].countL;
          countW+=threadParams[i].countW;
          if(threadParams[i].w1y < W1y)
          {
            W1x = threadParams[i].w1x;
            W1y = threadParams[i].w1y;
          }
        }
        
        if(countPr > 1000)
        {
          hasProp=1;
          if(!hadProp)
          {
            outfile << "Propofol" << "," << msecs << endl;
          }
          hadProp=1;
        }
        if(countR > 1000)
        {
          hasRoc=1;
          if(!hadRoc)
          {
            outfile << "Rocuronium" << "," << msecs << endl;
          }
          hadRoc=1;
        }
        if(countPh > 1000)
        {
          hasPhen=1;
          if(!hadPhen)
          {
            outfile << "Phenylephrine" << "," << msecs << endl;
          }
          hadPhen=1;
        }
        if(countL > (hadProp*3000+hadRoc*3000+hadPhen*3000+hadDex*3000+hadOd*3000+3000))
        {
          hasLid=1;
          if(!hadLid)
          {
            outfile << "Lidocaine" << "," << msecs << endl;
          }
          hadLid=1;
        }
        if(countW > 1000)
        {
          hasOd=1;
          if(!hadOd)
          {
            outfile << "Odanestron" << "," << msecs << endl;
          }
          hadOd=1;
          hasDex=1;
          if(!hadDex)
          {
            outfile << "Dexamethasone" << "," << msecs << endl;
          }
          hadDex=1;
        }

      }


//if no new medications were detected then check counts to see if one was removed
      if (!hasProp && !hasRoc && !hasPhen && !hasLid && !hasOd && !hasDex)
      {
        if(hadProp && countPr <= 1000) 
        { 
          hadProp=0; 
          outfile << "Propofol" << "," << cap.get(CAP_PROP_POS_MSEC) << endl;
        }
        else if(hadRoc && countR <= 1000) 
        { 
          hadRoc=0; 
          outfile << "Rocuronium" << "," << cap.get(CAP_PROP_POS_MSEC) << endl;
        }
        else if(hadPhen && countPh <= 1000) 
        { 
          hadPhen=0; 
          outfile << "Phenylephrine" << "," << cap.get(CAP_PROP_POS_MSEC) << endl;
        }
        else if(hadLid && countL < (hadProp*3000+hadRoc*3000+hadPhen*3000+hadDex*3000+hadOd*3000+3000)) 
        { 
          hadLid=0; 
          outfile << "Lidocaine" << "," << cap.get(CAP_PROP_POS_MSEC) << endl;
        }
        else if(countW <= 500) 
        { 
          if(hadDex)
          {
            hadDex=0; 
            outfile << "Dexamethasone" << "," << cap.get(CAP_PROP_POS_MSEC) << endl;
          }
          if(hadOd)
          {
            hadOd=0;
            outfile << "Odanestron" << "," << cap.get(CAP_PROP_POS_MSEC) << endl;
          }
        }
        else if(countW <= 1000 && ( (!hadOd && !hadDex) || (hadOd && hadDex) || (hadDex && !hadOd) ) ) 
        { 
          int ybound = (srcclone.rows-W1y > 150) ? 150 : srcclone.rows-W1y;
          dst = srcclone(Rect(W1x,W1y,300,ybound));
          pyrUp(dst,dst,Size(dst.cols*2,dst.rows*2));
          pyrUp(dst,dst,Size(dst.cols*2,dst.rows*2));
          cvtColor(dst, gray, COLOR_BGR2GRAY);
          GaussianBlur(gray, gray, Size(7, 7), 0);
          vector<Vec3f> circles;
          Canny(gray, canned, 35, 70);
          HoughCircles(gray, circles, HOUGH_GRADIENT, 1, gray.rows/32, 70, 20, 20, 35);
          circnt+=circles.size();
          for( size_t i = 0; i < circles.size(); i++ )
          {
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);
            circle( dst, center, radius, Scalar(0,0,255), 3, 8, 0 );
            if(i < 3) 
            {
              putText(dst, to_string(radius), Point(100*(i+1),100), FONT_HERSHEY_COMPLEX_SMALL, 3, Scalar(0,0,255), 3, LINE_AA);
            }
          }
          if(circles.size() > 0)
          {
            if(hadDex)
            {
            hadDex=0; 
            outfile << "Dexamethasone" << "," << cap.get(CAP_PROP_POS_MSEC) << endl;
            }
            if(!hadOd)
            {
            hadOd=1;
            outfile << "Odanestron" << "," << cap.get(CAP_PROP_POS_MSEC) << endl;              
            }
          }
          else
          {
            if(!hadDex)
            {
            hadDex=1; 
            outfile << "Dexamethasone" << "," << cap.get(CAP_PROP_POS_MSEC) << endl;
            }
            if(hadOd)
            {
            hadOd=0;
            outfile << "Odanestron" << "," << cap.get(CAP_PROP_POS_MSEC) << endl;              
            }
          }
        }
      }
      prevroi = roisrc.clone();
   }
   outfile.close();
}
