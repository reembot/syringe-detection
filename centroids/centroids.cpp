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

int main(int argc, char** argv)
{
//open camera or video file
   const char* default_file = "0";
   const char* filename = argc >=2 ? argv[1] : default_file;
   VideoCapture cap;
   if (*filename == '0') {
     cap.open(0);
   } else {
     cap.open(filename);
   }
   bool origView = 1;
   namedWindow(filename);
   char winInput;

//mats for original capture, grayscale version, previous frame and frame difference
   Mat src, srcGray, srcPrev, srcDiff;

   cap.read(src);
   if(src.empty())
   {
     return 0;
   }

   VideoWriter vout;
    Size S = Size((int) cap.get(CAP_PROP_FRAME_WIDTH),    // Acquire input size
                  (int) cap.get(CAP_PROP_FRAME_HEIGHT));

   vout.open("centroid.mp4", VideoWriter::fourcc('H','2','6','4'), cap.get(CAP_PROP_FPS), S, true);


// default method will simply compare change in intensity on single channel image (grayscale)
   cv::cvtColor(src, srcGray, COLOR_BGR2GRAY);
   srcPrev = srcGray.clone();
   srcDiff = srcGray.clone();

// alternatively we could do a difference on each color channel with code below
//   srcGray = src.clone();  
//   vector<Mat> spl, splprev, spldiff;
//   split(src, spl);
//   srcPrev = src.clone();
//   srcDiff = src.clone();


   printf("Press 's' to write screen capture, 'c' to switch views or 'esc' to exit\n");
   int cnt = 0;


   while (1)
   {
      cnt++;
      
      cap.read(src);
      if(src.empty())
      {
        break;
      }

      cv::cvtColor(src, srcGray, COLOR_BGR2GRAY);

// commented code for trying difference on BGR channels instead of grayscale
//      split(src,spl);
//      split(src,spldiff);
//      split(srcPrev,splprev);

      absdiff(srcPrev, srcGray, srcDiff);

//      absdiff(splprev[0], spl[0], spldiff[0]);
//      absdiff(splprev[1], spl[1], spldiff[1]);
//      absdiff(splprev[2], spl[2], spldiff[2]);

//      merge(spldiff, srcDiff);

//      cv::cvtColor(srcDiff, srcDiff, COLOR_BGR2GRAY);
      
       srcPrev = srcGray.clone();
//      srcPrev = src.clone();

       int x_min = src.cols;
       int x_max = 0;
       int y_min = src.rows;
       int y_max = 0;
       int y_hat = 0;
       int x_hat = 0;

// troubleshooting 
//       std::cout << x_max << " " << x_min << " " << y_min << " " << y_max << " hats " << x_hat << " " << y_hat << endl;


       for(int j=0;j<src.rows-2;j++) //due to intrinsic camera issue skipping last few rows as 718 and 719 always vary in intensity
       {
         for(int i=0;i<src.cols;i++)
         {
             if(srcDiff.at<Vec3b>(j,i)[0] > 175)
             {
                 if(x_min > i)x_min = i;
                 if(x_max < i)x_max = i;
                 if(y_min > j)y_min = j;
                 if(y_max < j)y_max = j;
             }
         }
       }

// y hat and x hat from old crosshairs code,  useful if getting exact center
//       x_hat = int((x_max - x_min)/2) + x_min;
//       y_hat = int((y_max - y_min)/2) + y_min;
//       std::cout << x_max << " " << x_min << " " << y_min << " " << y_max << " hats " << x_hat << " " << y_hat << endl;

       if(x_max > x_min)
       {
// for marking center of movement
//         line(src,Point(x_hat-10,y_hat),Point(x_hat+10,y_hat),Scalar(0,0,255),5); // from left to right
//         line(src,Point(x_hat,y_hat-10),Point(x_hat,y_hat+10),Scalar(0,0,255),5); // from top to bottom

           int h = y_max - y_min;
           int w = x_max - x_min; 

           if(h > 50  && h < 300){                      
             Mat dst = src(Rect(x_min+w/2,y_min,w/2,h));
//commented code for using standard deviation to reduce number of roi's to search
//             Scalar meanRGB,stddev;
//             meanStdDev(dst,meanRGB,stddev);
//             int totaldev;
//             totaldev = int(stddev[0] + stddev[1] + stddev[2]);
//             if(totaldev > 135) {
             //search for reddish orange for Rocuronium
             Mat searchRoc;
             inRange(dst,Scalar(0,0,200),Scalar(120,120,255),searchRoc);
//             std::cout << "Roc = " << to_string(sum(searchRoc)[0]) << endl;
             bool hasRoc = (sum(searchRoc)[0] > 5000);
             //search for light purple Phenylephrine
             Mat searchPhen;
             inRange(dst,Scalar(200,0,200),Scalar(255,200,255),searchPhen);
//             std::cout << "Phen = " << to_string(sum(searchPhen)[0]) << endl;
             bool hasPhen = (sum(searchPhen)[0] > 40000);
             //search for yellow for Propofol
             Mat searchProp;
             inRange(dst,Scalar(0,225,225),Scalar(200,255,255),searchProp);
             bool hasProp = (sum(searchProp)[0] > 500000);
             //search gray for Lidocaine, too difficult to differentiate currently
//             Mat searchLid;
//             inRange(dst,Scalar(50,50,50),Scalar(100,100,100),searchLid);
//             bool hasLid = (sum(searchLid)[0] > 20000);
//             std::cout << "Lid = " << to_string(sum(searchLid)[0]) << endl;
             if(hasRoc || hasPhen || hasProp) {
               if(hasRoc &&  sum(searchPhen)[0] < sum(searchRoc)[0]) { putText(src, "Rocuronium", Point(x_min+w/2+30,y_min+30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(200,200,250), 1, LINE_AA);
               }  else if(hasPhen && sum(searchProp)[0] < sum(searchPhen)[0]) { putText(src, "Phenylephrine", Point(x_min+w/2+30,y_min+30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(200,200,250), 1, LINE_AA); 
               }  else if(hasProp) { putText(src, "Propofol", Point(x_min+w/2+30,y_min+30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(200,200,250), 1, LINE_AA); 
//               }  else if(hasLid) { putText(src, "Lidocaine", Point(x_min+w/2+30,y_min+30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(200,200,250), 1, LINE_AA); 
               }
             std::cout << to_string(cnt) << ".png" << endl;
             std::cout << "Rocuronium = " << to_string(sum(searchRoc)[0]) << endl;
             std::cout << "Phenylephrine = " << to_string(sum(searchPhen)[0]) << endl;
             std::cout << "Propofol = " << to_string(sum(searchProp)[0]) << endl;
//             std::cout << "Lid = " << to_string(sum(searchLid)[0]) << endl;
             std::cout << endl;
               rectangle(src,Point(x_min+w/2,y_min),Point(x_max,y_max),Scalar(0,0,255),5,2);
//               String imgname =  to_string(cnt) + "B" + to_string(int(meanRGB[0])) + "G" + to_string(int(meanRGB[1])) + "R" + to_string(int(meanRGB[2])) + "D" + to_string(totaldev) + ".png";
//               String imgname =  to_string(cnt) + "B" + to_string(int(meanRGB[0])) + "G" + to_string(int(meanRGB[1])) + "R" + to_string(int(meanRGB[2])) 
//                   + "BD" + to_string(int(stddev[0])) + "GD" + to_string(int(stddev[1])) +  + "RD" + to_string(int(stddev[2])) + ".png";
               String imgname = to_string(cnt) + ".png";
               imwrite(imgname, dst);
             }
           }
       }

      if (origView)
      {
        imshow(filename,src);
      } else {
        imshow(filename,srcDiff);
      }

      vout.write(src);

      if ((winInput = waitKey(1)) == ESCAPE_KEY)
      {
          break;
      }
      else if(winInput == 's') 
      {
          String imgname = to_string(cnt) + ".png";
          imwrite(imgname, src);
      }
      else if(winInput == 'c') 
      {
          origView = origView ? 0 : 1; //flip flag to show original or difference
      }

   }
   destroyWindow(filename); 
}
