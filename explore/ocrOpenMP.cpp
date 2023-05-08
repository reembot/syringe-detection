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

// See www.asciitable.com
#define ESCAPE_KEY (27)
#define SYSTEM_ERROR (-1)

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
   int thread_count=4;

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
      roisrc = srcclone(Rect(150,75,750,885));
      matPr = roisrc.clone();
      matR = matPr.clone();
      matPh = matPr.clone();
      matL = matPr.clone();
      matD = matPr.clone();

      float bval=0,gval=0,rval=0,bval2=0,gval2=0,rval2=0;

      int rowbuff = 75;
      int W1x=0,W1y=0,W2x=0,W2y=0;

      int countR=0,countPr=0,countPh=0,countL=0,countW=0;
      bool hasRoc=0,hasProp=0,hasPhen=0,hasLid=0,hasOd=0,hasDex=0;

//skip first frame to setup previous for frame differencing
      if(cnt>1) 
      {
     
//loop through every pixel or until a new medication label is detected
/*
      for(int j=0;j<roisrc.rows && !hasProp && !hasRoc && !hasPhen && !hasLid && !hasOd && !hasDex;j++)
      {
        for(int i=0;i<roisrc.cols && !hasProp && !hasRoc && !hasPhen && !hasLid && !hasOd && !hasDex;i++)
        {
*/

      for(int j=0;j<roisrc.rows; j++)
      {
#pragma omp parallel for num_threads(thread_count)
        for(int i=0;i<roisrc.cols; i++)
        {

//get values for current and previous frames
           bval = (float)roisrc.at<Vec3b>(j,i)[0];           
           gval = (float)roisrc.at<Vec3b>(j,i)[1];           
           rval = (float)roisrc.at<Vec3b>(j,i)[2];
           bval2 = (float)prevroi.at<Vec3b>(j,i)[0];           
           gval2 = (float)prevroi.at<Vec3b>(j,i)[1];           
           rval2 = (float)prevroi.at<Vec3b>(j,i)[2];

//check if there is difference between current and previous frame
           if(abs(bval-bval2) < 30 && abs(gval-gval2) < 30 && abs(gval-gval2) < 30) 
           {  

//check for yellow colors where red is highest followed closely by green with almost no blue
             if ( (rval > 96) && (rval-bval > rval/3) && (bval < 32)  && (gval < rval * 9 / 10) && (gval > 3 * rval / 5) ) 
             {
               countPr++;   
//               matPr.at<Vec3b>(j,i)[0]=0; matPr.at<Vec3b>(j,i)[1]=155; matPr.at<Vec3b>(j,i)[2]=255;          
               if(!hasProp && !hadProp && countPr > 1000) 
               { 
                 hasProp=1;
                 hadProp=1;
                 outfile << "Propofol" << "," << cap.get(CAP_PROP_POS_MSEC) << endl;
               }
             } 
//check for mostly red color
             else if ( (rval > 224) && (bval < 64) && (gval < 96) ) 
             {   
               countR++;
//               matPr.at<Vec3b>(j,i)[0]=0; matPr.at<Vec3b>(j,i)[1]=0; matPr.at<Vec3b>(j,i)[2]=255;          
               if(!hasRoc && !hadRoc && countR > 1000) 
               { 
                 hasRoc=1;
                 hadRoc=1;
                 outfile << "Rocuronium" << "," << cap.get(CAP_PROP_POS_MSEC) << endl;
               }  
             } 
//check for light purple where most values are high and green is 1/2-3/4 of red
             else if ( (rval > 128) && (bval > 128) && (gval < 3 * rval / 4) && (gval > rval / 2) ) 
             {
               countPh++;
//               matPr.at<Vec3b>(j,i)[0]=255; matPr.at<Vec3b>(j,i)[1]=155; matPr.at<Vec3b>(j,i)[2]=224;          
               if(!hasPhen && !hadPhen && countPh > 1000) 
               { 
                 hasPhen=1;
                 hadPhen=1;
                 outfile << "Phenylephrine" << "," << cap.get(CAP_PROP_POS_MSEC) << endl;
               }
             } 
//check for non-color values (gray and white)
             else if ( (abs(rval-gval) < 20) && (abs(rval-bval) < 20) && (abs(gval-bval) < 20) ) 
             {
//check for gray
               if ( (rval > 48) && (gval > 48) && (bval > 48) && (rval < 80) && (gval < 80) && (bval < 80) ) 
               {
                 countL++;
//                 matPr.at<Vec3b>(j,i)[0]=0; matPr.at<Vec3b>(j,i)[1]=255; matPr.at<Vec3b>(j,i)[2]=0;          
                 if(!hasLid && !hadLid && countL > (hadProp*3000+hadRoc*3000+hadPhen*3000+hadDex*3000+hadOd*3000+3000)) 
                 { 
                   hasLid=1;
                   hadLid=1;
                   outfile << "Lidocaine" << "," << cap.get(CAP_PROP_POS_MSEC) << endl;
                 }
               } 
//check for white
               else if ( (rval > 175) && (gval > 175) && (bval > 175) ) 
               {
//only count if first label threshold is not met or if haven't skipped down to next reasonable label row (rowbuff)
                 if( W1x==0 || j > W1y + rowbuff) 
                 { 
                   countW++; 
                 }
//                 matPr.at<Vec3b>(j,i)[0]=255; matPr.at<Vec3b>(j,i)[1]=0; matPr.at<Vec3b>(j,i)[2]=0;
//first text white label threshold
                 if(W1x==0 && countW > 500)
                 {
                   W1x=i; 
                   W1y=j;
                 } 
//second text white label threshold does not need circle check as both types are now present
                 else if(W2x==0 && countW > 1000) 
                 {
                   if(!hasOd && !hadOd) 
                   { 
                     hasOd=1;
                     hadOd=1;
                     outfile << "Odanestron" << "," << cap.get(CAP_PROP_POS_MSEC) << endl;                   
                   } 
                   if(!hasDex && !hadDex)
                   {
                     hasDex=1;
                     hadDex=1;
                     outfile << "Dexamethasone" << "," << cap.get(CAP_PROP_POS_MSEC) << endl;                   
                   } 
                 }                         
               }
             }
           } 
           else 
           {
//               matPr.at<Vec3b>(j,i)[0]=0; matPr.at<Vec3b>(j,i)[1]=0; matPr.at<Vec3b>(j,i)[2]=0;                       
           }
        }
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
//          cout << " Circles " << circles.size() << endl;

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


//      cout << countPr << " " << countR << " " << countPh << " " << countL << " " << countW << endl;


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
//   cout << "Total circles: " << circnt << endl;
//   destroyWindow(filename); 
   outfile.close();
}
