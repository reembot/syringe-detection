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

__global__ void mykernel(unsigned char *roirow, unsigned char *prevroi){
    unsigned int tid = threadIdx.x;
    float bval,gval,rval,bval2,gval2,rval2;
    unsigned int i = blockIdx.x*blockDim.x+tid;
    if(i<506880 && i%3==0) 
    {
      bval=(float)roirow[i];
      gval=(float)roirow[i+1];
      rval=(float)roirow[i+2];
      bval=(float)prevroi[i];
      gval=(float)prevroi[i+1];
      rval=(float)prevroi[i+2];

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
    unsigned char rowvals[506880];
//    for(int i=0;i<506880;i++) rowvals[i]=0;
    unsigned char *gpuprevroi;
    unsigned char *gpuroirow;
    cuda_ret=cudaMalloc((void **)&gpuroirow,506880*sizeof(unsigned char));
    cuda_ret=cudaMalloc((void **)&gpuprevroi,506880*sizeof(unsigned char));
    cudaMemcpy(gpuroirow,threadParams->roisrc.ptr<uchar>(threadParams->threadIdx*220),506880*sizeof(unsigned char),cudaMemcpyHostToDevice);
    cudaMemcpy(gpuprevroi,threadParams->prevroi.ptr<uchar>(threadParams->threadIdx*220),506880*sizeof(unsigned char),cudaMemcpyHostToDevice);
//         cudaDeviceSynchronize();
//    cudaStreamSynchronize(0);
    mykernel<<<495,1024>>>(gpuroirow,gpuprevroi);
//    cudaDeviceSynchronize();
//    cudaStreamSynchronize(0);
//         cuda_ret = cudaGetLastError();
    cudaMemcpy(rowvals,gpuroirow,506880*sizeof(unsigned char),cudaMemcpyDeviceToHost);
//    cudaDeviceSynchronize();
//    cudaStreamSynchronize(0);
    for(int i=0;i<506880;i+=3) 
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
    cudaFree(gpuprevroi);
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
      cnt++;
      cap.read(src);
      if(src.empty())
      {
        break;
      }
      
      srcclone = src.clone();
      roisrc = srcclone(Rect(150,75,768,880)).clone();
      matPr = roisrc.clone();
      matR = matPr.clone();
      matPh = matPr.clone();
      matL = matPr.clone();
      matD = matPr.clone();

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
        if(countPh > 2000)
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
//      if (!hasProp && !hasRoc && !hasPhen && !hasLid && !hasOd && !hasDex)
      if(1)
      {
        if(hadProp && countPr <= 1000) 
        { 
          hadProp=0; 
          outfile << "Propofol" << "," << cap.get(CAP_PROP_POS_MSEC) << endl;
        }
        if(hadRoc && countR <= 1000) 
        { 
          hadRoc=0; 
          outfile << "Rocuronium" << "," << cap.get(CAP_PROP_POS_MSEC) << endl;
        }
        if(hadPhen && countPh <= 2000) 
        { 
          hadPhen=0; 
          outfile << "Phenylephrine" << "," << cap.get(CAP_PROP_POS_MSEC) << endl;
        }
        if(hadLid && countL < (hadProp*3000+hadRoc*3000+hadPhen*3000+hadDex*3000+hadOd*3000+3000)) 
        { 
          hadLid=0; 
          outfile << "Lidocaine" << "," << cap.get(CAP_PROP_POS_MSEC) << endl;
        }
        if(countW <= 500) 
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
          
          int maxthresh = threshold(gray,threshed,0,255,THRESH_BINARY + THRESH_OTSU);
          int minthresh = maxthresh / 2;
          maxthresh = maxthresh-10;
          minthresh = minthresh-5;
          maxthresh = (maxthresh < 10) ? 255: maxthresh;
          minthresh = (maxthresh == 255) ? 255: minthresh;
  //      Canny(gray, canned, minthresh, maxthresh+20);

          Canny(gray, canned, 35, 70);

          HoughCircles(gray, circles, HOUGH_GRADIENT, 1, gray.rows/32, 70, 20, 20, 35);

//          HoughCircles(gray, circles, HOUGH_GRADIENT, 1, gray.rows/32, 60, 20, 20, 30); //60D,80O
//          HoughCircles(gray, circles, HOUGH_GRADIENT, 1, gray.rows/32, 50, 20, 20, 30);
//          HoughCircles(gray, circles, HOUGH_GRADIENT, 1, gray.rows/32, maxthresh, 20, 20, 30);

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

//       resize(dst,subshow,Size(600,300));
//       imshow("zoomed",subshow);
//       resize(canned,subshow,Size(600,300));
//       imshow("canned",subshow);

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
      }


      if(hadRoc) {
        putText(src, "Rocuronium", Point(30,100), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0,0,255), 1, LINE_AA);
      }
      if(hadProp) {
        putText(src, "Propofol", Point(30,50), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0,0,255), 1, LINE_AA);
      }
      if(hadPhen) {
        putText(src, "Phenylephrine", Point(30,150), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0,0,255), 1, LINE_AA);
      }

      if(hadLid) { 
        putText(src, "Lidocaine", Point(30,200), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0,0,255), 1, LINE_AA); 
      }

      if(hadOd) { 
        putText(src, "Odanestron", Point(30,250), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0,0,255), 1, LINE_AA); 
      }

      if(hadDex) { 
        putText(src, "Dexamethasone", Point(30,300), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0,0,255), 1, LINE_AA); 
      }

//      showMat = 0;
//      if(showMat == 0) { srcSegSum = matPr.clone(); }
/*
      if(showMat == 1) { srcSegSum = matR.clone(); }
      if(showMat == 2) { srcSegSum = matPh.clone(); }
      if(showMat == 3) { srcSegSum = matL.clone(); }
      if(showMat == 4) { srcSegSum = matD.clone(); }
*/
//      srcSegSum = dst.clone();

      
/*
      if (origView)
      {
        imshow(filename,src);
      } else {
        imshow(filename,srcSegSum);
      }
*/
      
/*
      Mat colorcanned,resizedsrc, resizedseg, expanddst, expandcanned; 
      Mat combinedh1,combinedh2,combinedv;
      cvtColor(canned, colorcanned, COLOR_GRAY2BGR);
      resize(dst,expanddst,Size(640,320));
      resize(colorcanned,expandcanned,Size(640,320));
      resize(src,resizedsrc,Size(640,480));
      resize(srcSegSum,resizedseg,Size(640,480));
      hconcat(resizedsrc,resizedseg,combinedh1);
      hconcat(expanddst,expandcanned,combinedh2);
      vconcat(combinedh1,combinedh2,combinedv);

//      imshow(filename,combinedv);
//      imshow(filename,src);
      vout.write(combinedv);
*/
      prevroi = roisrc.clone();

//      vout.write(src);

/*
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
      else if(winInput == 'm') 
      {
          showMat = (showMat > 3) ? 0 : showMat + 1; //flip flag to show original or difference
      }
      else if(winInput == 'p') 
      {
          String imgname = filename + to_string(cnt) + ".png";
          imwrite(imgname, src);
      }
*/

   }
//   destroyWindow(filename); 
   outfile.close();
}
