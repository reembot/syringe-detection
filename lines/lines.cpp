#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

// See www.asciitable.com
#define ESCAPE_KEY (27)
#define SYSTEM_ERROR (-1)
#define ROIX (300)
#define ROIY (50)
#define SRCX (1280)
#define SRCY (720)

int main(int argc, char** argv)
{
   const char* default_file = "0";
   const char* filename = argc >=2 ? argv[1] : default_file;
   VideoCapture cap;
   if (*filename == '0') {
     cap.open(0);
   } else {
     cap.open(filename);
   }
   bool canView = 0;
   namedWindow(filename);
   char winInput;
   int waitval=100;
   bool origView=1,tview=1;

/*
   VideoWriter vout;
    Size S = Size((int) cap.get(CAP_PROP_FRAME_WIDTH),    // Acquire input size
                  (int) cap.get(CAP_PROP_FRAME_HEIGHT));

   vout.open("lines.mp4", VideoWriter::fourcc('H','2','6','4'), cap.get(CAP_PROP_FPS), S, true);
*/

   printf("Press space to pause/unpause, 's' to step frame by frame, 'c' to switch views or 'esc' to exit\n");
   int cnt = 0;
   vector<vector<Vec4i>> linehistory;
   while (1)
   {
      Mat src,roisrc;
      cnt++;
      
      cap.read(src);
      if(src.empty()){
        break;
      }
      //store mat variables
      Mat dst,gray;
      Mat gray1,gray2,gray3;

       gray = src.clone();
       int gval = 0,bval=0,rval=0;
       int myhist[256] = {0};

       //to focus on just the mousepad area
       roisrc=src(Rect(ROIX,ROIY,SRCX-ROIX,SRCY-ROIY));
//      GaussianBlur(src,gray, Size(81,81), 3);
//       bilateralFilter(src,gray,4,8,2);

       for(int j=0;j<src.rows-2;j++)
       {
//         int scandown = 0;
         for(int i=0;i<src.cols;i++)
         {
             bval = gray.at<Vec3b>(j,i)[0];
             gval = gray.at<Vec3b>(j,i)[1];
             rval = gray.at<Vec3b>(j,i)[2];
//             bval=bval+(8-bval%8);gval=gval+(8-gval%8);rval=rval+(8-rval%8);
             if(abs(bval-gval)>10 || abs(rval-gval)>10 || abs(rval-bval)>10) {gval=0;}
             else 
             {
//                     scandown--;
//                    if(gval>240){scandown=50;}
//                    if(scandown<0) {gval=0;}
//                    gval=(gval>150)?gval:255;


//                  gval=gval+(64-gval%64);


//                  gval=int((float)gval*((float)gval/255));

//                  if(gval>255){gval=255;}           
//                  if(gval>64&&gval<192){gval=0;}       


//                  gval=(gval<50)?0:(gval<200)?128:(gval<225)?196:255;
//                  gval=(gval<100)?0:((gval>240)?255:gval/2);                 
             }
             myhist[gval]++;
//             gray.at<Vec3b>(j,i)[0]=gval;
//             gray.at<Vec3b>(j,i)[1]=gval;
//             gray.at<Vec3b>(j,i)[2]=gval;
         }
       }

//       bilateralFilter(gray,dst,16,32,8);

//      GaussianBlur(gray,src, Size(9,9), 3);
//      addWeighted(gray,1.5,src,-0.5,0,src);
//      gray.convertTo(gray,-1,1,-200);
//      gray.convertTo(gray,-1,2,0);
//      gray.convertTo(gray,-1,1,-50);
//      gray.convertTo(gray,-1,2,0);
//      gray.convertTo(gray,-1,1,-50);
//      gray.convertTo(gray,-1,2,0);
//      gray.convertTo(gray,-1,1,-100);
//      gray.convertTo(gray,-1,1.5,0);

//        dst = gray.clone();

//      gray = src.clone();
//      threshold(gray,dst,150,255,THRESH_BINARY);
//dst = gray.clone();
//      GaussianBlur(gray, gray, Size(9,9), 2, 2);


      // Edge detection with thresholds of param3 to param4 intensity for hysteresis with param5xparam5 sobel and default l1 norm 


//      Canny(gray, dst, 100, 100, 3);
//      cv::cvtColor(src, gray, COLOR_BGR2GRAY);
      Canny(roisrc, dst, 1, 225, 3);



      vector<Vec4i> linesP; // will hold the results of the detection
      vector<Vec4i> parallels; //will store the parallel lines
      HoughLinesP(dst, linesP, 1, CV_PI/180, 20, 75, 10 ); // runs the actual detection

      cout << "Lines from new Frame:" << endl;
      
      //for loop to go through the linesP vector
      for( size_t i = 0; i < linesP.size(); i++ )
      {
        //calculating the slope
        float slope=0;
        //flag to detect if parallel or not
        bool hasparallel=0;
        //Store each vector element 
        Vec4i l = linesP[i];
        //prints the x1, x2, y1, y2 coordinate 
        cout << l[0] << " " << l[1] << " " << l[2] << " " << l[3] << endl;
        //calculate the slop y2-y1, x2 -x1
        slope = (float)(l[3]-l[1])/(float)(l[2]-l[0]);
        //print the slope
        cout << slope <<endl;
        //go through the second line detected
        for( size_t j = 0; j < linesP.size(); j++ )
        {
          //store the vector element for the second line
           Vec4i l2 = linesP[j];
           //calcylate the slope (y2-y1)/(x2-x1)
           float slope2 = (float)(l2[3]-l2[1])/(float)(l2[2]-l2[0]);
           //checking againest the first slope
           cout << "checking against slope "<<slope2<<endl;
           //finding the difference of the slopes of both lines
           float slopediff = abs(slope-slope2);
           //if the difference is less than 0.3 than it is parallel
           cout<< "Difference #" << j << " " << slopediff << endl;
           //update the hasparrallel flag to 1
           if(slopediff <0.3){cout<<"Parallel line found " << endl;hasparallel=1;}
        }
        //if it is parallel, then we display the lines, and add it to the parallel vector
        if(hasparallel){
        line( src, Point(l[0]+ROIX, l[1]+ROIY), Point(l[2]+ROIX, l[3]+ROIY), Scalar(0,0,255), 3, LINE_AA);
        parallels.push_back(linesP[i]);
        }
      }
      //otherwise we push it to the line history vector
      if(parallels.size()>0){
      linehistory.push_back(parallels);
      }
      //will print the line history elements
      cout << "Line History " << endl;
      for(int x=linehistory.size();x>((linehistory.size()>10)?linehistory.size()-10:0);x--){
        //for each frame number
        cout << "frame #" << x << endl;

        vector<Vec4i> linesP = linehistory[x-1];

        cout << linesP.size() << endl;

        for( size_t i = 0; i < linesP.size(); i++ ) {

          Vec4i l = linesP[i];

          cout << l[0] << " " << l[1] << " " << l[2] << " " << l[3] << endl;
        }
      }

      if (origView)
      {
        imshow(filename, src);
      } else if(tview) {
        imshow(filename,gray);
//      } else { 
//        imshow(filename,dst);
      }
//      vout.write(src);

      if ((winInput = waitKey(waitval)) == ESCAPE_KEY)
      {
          break;
      }
      else if(winInput == 's') 
      {
          waitval = 0;
      }
      else if(winInput == 'h') 
      {
          int total=0;
          for(int i=0;i<256;i++){if(myhist[i]>0){cout<<i<<" "<<myhist[i]<<endl;total+=myhist[i];}}
      }
      else if(winInput == ' ') 
      {
          waitval = (waitval == 100) ? 0 : 100;
      }
      else if(winInput == 'c') 
      {
          origView = origView ? 0 : 1; //flip flag to show original or difference
      }
      else if(winInput == 't') 
      {
          tview = tview ? 0 : 1; //flip flag to show original or difference
      }

   }
   destroyWindow(filename); 
}
