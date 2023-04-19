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

      Mat dst,gray;
      Mat gray1,gray2,gray3;

       gray = src.clone();
       int gval = 0,bval=0,rval=0;
       int myhist[256] = {0};


//to add:  change roix and roiy values to adapt to new video set
//most likely need to use something like this
      roisrc = src(Rect(0,0,780,959));

      //roisrc=src(Rect(ROIX,ROIY,SRCX-ROIX,SRCY-ROIY));

      Canny(roisrc, dst, 1, 225, 3);
     
      vector<Vec4i> linesP; // will hold the results of the detection
      
      vector<Vec4i> parallels;
      
      HoughLinesP(dst, linesP, 1, CV_PI/180, 20, 75, 10 ); // runs the actual detection
      
/*

Options:

Instead of parallel lines, look for a specific slope to determine if line is horizontal as new videos will have syringes oriented that way

In addition to minimum line length retrieved from houghlinesp parameter, look for a specific maximum line by adding a condition where x2-x1 is in range
i.e. if min length from parameter is 75 but we do not want lines longer than 100 then add a if(l[2]-l[0] < 100) to only pick up lines of length 75-100


to differentiate labels, we need to look for lines that are a given height between each other

first identify top most line

then as looping through lines find the next line that has l[3] - l[1] (height, y value differences) greater than threshold
for 1280x960 typical syringe height is probably 100-120 or so height

once the set of top label lines are identified, draw bounding box


*/

      cout << "Lines from new Frame:" << endl;
      //go through the linesP vector 
      for( size_t i = 0; i < linesP.size(); i++ )
      {

//to add: first job is to ignore lines too wide l[2]-l[0] > 100 or something like that to identify label lines
		//slope is equal to 0
        float slope=0;
        //flag for parallel lines
        bool hasparallel=0;
        //store the x1, x2, y1, y2 coordinate points
        Vec4i l = linesP[i];
        //output the coordinates
        cout << l[0] << " " << l[1] << " " << l[2] << " " << l[3] << endl;
        //calculate the slope for (y2-y1)/(x2-x1)
        slope = (float)(l[3]-l[1])/(float)(l[2]-l[0]);
        //output the slope
// to add:  check if slope is approximately horiztonal, only continue if it is otherwise skip line
// also to add:  keep track of minimum y (top most line)
//the code below can be removed as we do not need to look for parallel lines if we are thresholding the y gap between lines
//keep track with ymin variable
        cout << slope <<endl;
        //for loop through the linesP vector for the second line
        for( size_t j = 0; j < linesP.size(); j++ )
        {
		    //store the coordinas for the second line
           Vec4i l2 = linesP[j];
           //calculating the slope
           float slope2 = (float)(l2[3]-l2[1])/(float)(l2[2]-l2[0]);
           //checking if the slope is similar to slope1
           cout << "checking against slope "<<slope2<<endl;
           //calculating the slope difference
           float slopediff = abs(slope-slope2);
           //outputting the difference
           cout<< "Difference #" << j << " " << slopediff << endl;
           //if condition is met, and slope is less than 0.3, then we change the flag to parallel
           if(slopediff <0.3){cout<<"Parallel line found " << endl;hasparallel=1;}
        }
        //if parallel, then we display the line, takng into account the area we want it to detcect
        if(hasparallel){
        line( src, Point(l[0]+ROIX, l[1]+ROIY), Point(l[2]+ROIX, l[3]+ROIY), Scalar(0,0,255), 3, LINE_AA);
        //add to the parrallel vector
        parallels.push_back(linesP[i]);
        }

//to add:  after identifying the top most line of length label
//  
      }
      //if parallel vector size is greater than 0
      if(parallels.size()>0){
		  //add to the linehistory vector
      linehistory.push_back(parallels);
      }

cout << "test test test " << endl;

/*      
      cout << "Line History " << endl;
      
      for(int x=linehistory.size();x>((linehistory.size()>10)?linehistory.size()-10:0);x--){
		 //outputtng frame #
        cout << "frame #" << x << endl;
        vector<Vec4i> linesP = linehistory[x-1];
        cout << linesP.size() << endl;
        for( size_t i = 0; i < linesP.size(); i++ ) {
          Vec4i l = linesP[i];
          cout << l[0] << " " << l[1] << " " << l[2] << " " << l[3] << endl;
        }
      }
*/
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
