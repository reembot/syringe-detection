#include<cuda_runtime.h>
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
    int framenum;
}  threadParams_t;

// POSIX thread declarations and scheduling attributes
//
pthread_t threads[NUM_THREADS];
threadParams_t threadParams[NUM_THREADS];


// See www.asciitable.com
#define ESCAPE_KEY (27)
#define SYSTEM_ERROR (-1)

__global__ void mykernel(unsigned char *roirow){
    unsigned int tid = threadIdx.x;
    float bval,gval,rval;
    unsigned int i = blockIdx.x*blockDim.x+tid;
    if(i<2027520 && i%3==0) 
    {
      bval=(float)roirow[i];
      gval=(float)roirow[i+1];
      rval=(float)roirow[i+2];

//    if(abs(bval-bval2) < 30 && abs(gval-gval2) < 30 && abs(gval-gval2) < 30)
    if(1)
    {
      if ( (rval > 96) && (rval-bval > rval/3) && (bval < 32)  && (gval < rval * 9 / 10) && (gval > 3 * rval / 5) ) 
      {
        roirow[i]=1;
      }   
      else if ( (rval > 224) && (bval < 64) && (gval < 96) ) 
      {   
        roirow[i]=2; 
      } 
      else if ( (rval > 128) && (bval > 128) && (gval < 3 * rval / 4) && (gval > rval / 2) ) 
      {
        roirow[i]=3;
      } 
      else if ( (abs(rval-gval) < 20) && (abs(rval-bval) < 20) && (abs(gval-bval) < 20) ) 
      {
        if ( (rval > 48) && (gval > 48) && (bval > 48) && (rval < 80) && (gval < 80) && (bval < 80) ) 
        {
          roirow[i]=4;
        } 
        else if ( (rval > 175) && (gval > 175) && (bval > 175) ) 
        {
          roirow[i]=5;
        }
        else
        {
          roirow[i]=0;
        }
      }
      else
      {
        roirow[i]=0;
      }
    }
    else
    {
      roirow[i]=0;
    }
    }
    
}

void *findLabelThread(void *threadp)
{
    cudaError_t cuda_ret;
    threadParams_t *threadParams = (threadParams_t *)threadp;
    int size = 16*sizeof(int);
    unsigned char rowvals[2027520];
//    for(int i=0;i<506880;i++) rowvals[i]=0;
    unsigned char *gpuroirow;
    cuda_ret=cudaMalloc((void **)&gpuroirow,2027520*sizeof(unsigned char));
    cudaMemcpy(gpuroirow,threadParams->roisrc.ptr<uchar>(0),2027520*sizeof(unsigned char),cudaMemcpyHostToDevice);
//         cudaDeviceSynchronize();
//    cudaStreamSynchronize(0);
    mykernel<<<1980,1024>>>(gpuroirow);
//    cudaDeviceSynchronize();
//    cudaStreamSynchronize(0);
//         cuda_ret = cudaGetLastError();
    cudaMemcpy(rowvals,gpuroirow,2027520*sizeof(unsigned char),cudaMemcpyDeviceToHost);
//    cudaDeviceSynchronize();
//    cudaStreamSynchronize(0);
    for(int i=0;i<2027520;i+=3) 
    {
      switch(rowvals[i])
      {
        case 1:
          threadParams->countPr++;
          break;
        case 2:
          threadParams->countR++;
          break;
        case 3:
          threadParams->countPh++;
          break;
        case 4:
          threadParams->countL++;
          break;
        case 5:
          threadParams->countW++;
          break;
      }
    }

    cudaFree(gpuroirow);
//    cudaStreamSynchronize(0);

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
   bool origView=1;
   bool hadRoc=0,hadProp=0,hadPhen=0,hadLid=0,hadOd=0,hadDex=0;
   int pW1x=0,pW1y=0,pW2x=0,pW2y=0;
   int prevStamp=0;

/*
   namedWindow(filename);
   namedWindow("zoomed");
   namedWindow("canned");
*/

   char winInput;

   Mat src, roisrc, srcSeg, srcSeg2, srcSegSum, prevroi, srcclone, dst, gray,threshed,canned,subshow;
   Mat matPr,matR,matPh,matL,matD;
   int showMat = 0;

   cap.read(src);
   if(src.empty())
   {
     return 0;
   }
   srcSeg = src.clone();
   srcSegSum = src.clone();
   dst = src.clone();
   cvtColor(dst, gray, COLOR_BGR2GRAY);
   Canny(gray, canned, 35, 70);
   int cnt=0,circnt=0;

//   VideoWriter vout;
//   Size S = Size((int) cap.get(CAP_PROP_FRAME_WIDTH),    // Acquire input size
//                  (int) cap.get(CAP_PROP_FRAME_HEIGHT));

//   vout.open("colors.mp4", VideoWriter::fourcc('H','2','6','4'), cap.get(CAP_PROP_FPS), S, true);
//   vout.open(argv[3], VideoWriter::fourcc('H','2','6','4'), 10, Size(1280,800), true);

//   printf("Press space to pause and unpause, 's' to step through each frame, 'c' to switch views or 'esc' to exit\n");

   while (1)
   {
      if(cnt>594){return(0);}
      float bval=0,gval=0,rval=0,bval2=0,gval2=0,rval2=0;
      
      int rowbuff = 75;
      int W1x=0,W1y=960,W2x=0,W2y=0;

      int countR=0,countPr=0,countPh=0,countL=0,countW=0;
      bool hasRoc=0,hasProp=0,hasPhen=0,hasLid=0,hasOd=0,hasDex=0;

      for(int i=0; i<NUM_THREADS; i++)
      {
        cnt++;
        cap.read(src);
        if(src.empty())
        {
          return(0);
        }
      
        roisrc = src(Rect(150,75,768,880)).clone();





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
          threadParams[i].framenum=cnt;
          pthread_create(&threads[i],   // pointer to thread descriptor
//            (void *)0,     // use default attributes
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

      std::string msecs = to_string(int(cap.get(CAP_PROP_POS_MSEC)));



        if(countPr > 4000)
        {
          hasProp=1;
          if(!hadProp)
          {
            outfile << "Propofol" << "," << msecs << endl;
          }
          hadProp=1;
        }
        if(countR > 4000)
        {
          hasRoc=1;
          if(!hadRoc)
          {
            outfile << "Rocuronium" << "," << msecs << endl;
          }
          hadRoc=1;
        }
        if(countPh > 8000)
        {
          hasPhen=1;
          if(!hadPhen)
          {
            outfile << "Phenylephrine" << "," << msecs << endl;
          }
          hadPhen=1;
        }
        if(countL > (hadProp*12000+hadRoc*12000+hadPhen*12000+hadDex*12000+hadOd*12000+12000))
        {
          hasLid=1;
          if(!hadLid)
          {
            outfile << "Lidocaine" << "," << msecs << endl;
          }
          hadLid=1;
        }
        if(countW > 4000)
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


      if(1)
      {
        if(hadProp && countPr <= 4000) 
        { 
          hadProp=0; 
          outfile << "Propofol" << "," << cap.get(CAP_PROP_POS_MSEC) << endl;
        }

        if(hadRoc && countR <= 4000) 
        { 
          hadRoc=0; 
          outfile << "Rocuronium" << "," << cap.get(CAP_PROP_POS_MSEC) << endl;
        }

        if(hadPhen && countPh <= 8000) 
        { 
          hadPhen=0; 
          outfile << "Phenylephrine" << "," << cap.get(CAP_PROP_POS_MSEC) << endl;
        }

        if(hadLid && countL < (hadProp*12000+hadRoc*12000+hadPhen*12000+hadDex*12000+hadOd*12000+12000)) 
        { 
          hadLid=0; 
          outfile << "Lidocaine" << "," << cap.get(CAP_PROP_POS_MSEC) << endl;
        }
     }
/*
        if(countW <= 2000) 
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
        else if(countW <= 4000 && ( (!hadOd && !hadDex) || (hadOd && hadDex) || (hadDex && !hadOd) ) ) 
        { 
          int ybound = (srcclone.rows-W1y > 150) ? 150 : srcclone.rows-W1y;
          dst = srcclone(Rect(W1x,W1y,300,ybound));
          pyrUp(dst,dst,Size(dst.cols*2,dst.rows*2));
          pyrUp(dst,dst,Size(dst.cols*2,dst.rows*2));
          cvtColor(dst, gray, COLOR_BGR2GRAY);
          GaussianBlur(gray, gray, Size(7, 7), 0);
          vector<Vec3f> circles;
          
          int maxthresh = threshold(gray,threshed,0,255,THRESH_BINARY + THRESH_OTSU);
          int minthresh = maxthresh / 2;
          maxthresh = maxthresh-10;
          minthresh = minthresh-5;
          maxthresh = (maxthresh < 10) ? 255: maxthresh;
          minthresh = (maxthresh == 255) ? 255: minthresh;

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
            if(hadOd) //unnecessary should not reach
            {
            hadOd=0;
            outfile << "Odanestron" << "," << cap.get(CAP_PROP_POS_MSEC) << endl;              
            }
          }
        }


*/
   }
 
   outfile.close();
}
