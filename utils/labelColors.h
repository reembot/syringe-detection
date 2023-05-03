#ifndef LABELCOLORS
#define LABELCOLORS
int labelColors(Mat frame, bool checkR, bool checkPr, bool checkPh, bool checkL, bool checkO, bool checkD)
{
      int countR=0,countPr=0,countPh=0,countL=0,countW=0;
     
//loop through every pixel or until a new medication label is detected
      for(int j=0;j<frame.rows;j++)
      {
        for(int i=0;i<frame.cols;i++)
        {
//get bgr values
           float bval = (float)frame.at<Vec3b>(j,i)[0];           
           float gval = (float)frame.at<Vec3b>(j,i)[1];           
           float rval = (float)frame.at<Vec3b>(j,i)[2];
//check for yellow colors where red is highest followed closely by green with almost no blue
           if ( (rval > 96) && (rval-bval > rval/3) && (bval < 32)  && (gval < rval * 9 / 10) && (gval > 3 * rval / 5) ) 
           {
             countPr++;   
             if(checkPr && countPr > 1000) 
             { 
               return 1;
             }
           } 
//check for mostly red color
           else if ( (rval > 224) && (bval < 64) && (gval < 96) ) 
           {   
             countR++;
             if(checkR && countR > 1000) 
             { 
               return 2;
             }  
           } 
//check for light purple where most values are high and green is 1/2-3/4 of red
           else if ( (rval > 128) && (bval > 128) && (gval < 3 * rval / 4) && (gval > rval / 2) ) 
           {
             countPh++;
             if(checkPh && countPh > 1000) 
             { 
               return 3;
             }
           } 
//check for non-color values (gray and white)
           else if ( (abs(rval-gval) < 20) && (abs(rval-bval) < 20) && (abs(gval-bval) < 20) ) 
           {
//check for gray
             if ( (rval > 48) && (gval > 48) && (bval > 48) && (rval < 80) && (gval < 80) && (bval < 80) ) 
             {
               countL++;
               if(checkL && countL > 3000) 
               { 
                 return 4;
               }
             } 
//check for white
             else if ( (rval > 175) && (gval > 175) && (bval > 175) ) 
             {
               countW++; 
//first text white label threshold
               if((checkO || checkD) && countW > 500)
               {
                 pyrUp(frame,frame,Size(frame.cols*2,frame.rows*2));
                 pyrUp(frame,frame,Size(frame.cols*2,frame.rows*2));
                 Mat gray;
                 cvtColor(frame, gray, COLOR_BGR2GRAY);
                 GaussianBlur(gray, gray, Size(7, 7), 0);
                 vector<Vec3f> circles;
                 HoughCircles(gray, circles, HOUGH_GRADIENT, 1, gray.rows/32, 70, 20, 20, 35);
                 if(circles.size() > 0)
                 {
                   return 5;
                 }
                 else
                 {
                   return 6;
                 }
               } 
             }
           }
        }
      }
  return 0;
}

#endif
