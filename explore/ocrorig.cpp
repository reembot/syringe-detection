/*
 *
 *  Example by Sam Siewert 
 *
 *  Updated for OpenCV 3.1
 *
 *  Updated by CSCI612 group for 4.1.1
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;


int main( int argc, char** argv )
{
   const char* input_vfile = "../data/vout1.mp4";
   const char* in_vfilename = argc >=3 ? argv[2] : input_vfile;

    namedWindow(in_vfilename);

    Mat src,gray;

    vector<Vec3f> circles;

    VideoCapture cap(in_vfilename);

    while(1)
    {
        cap.read(src);

        cvtColor(src, gray, COLOR_BGR2GRAY);

        GaussianBlur(gray, gray, Size(9,9), 2, 2);

        HoughCircles(gray, circles, HOUGH_GRADIENT, 1, gray.rows/8, 100, 50, 0, 0);

        for( size_t i = 0; i < circles.size(); i++ )
        {
          Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
          int radius = cvRound(circles[i][2]);
          // circle center
          circle( src, center, 3, Scalar(0,255,0), -1, 8, 0 );
          // circle outline
          circle( src, center, radius, Scalar(0,0,255), 3, 8, 0 );
        }

        imshow(in_vfilename, src);

        char c = waitKey(10);
        if( c == 'q' ) break;
    }

    destroyWindow(in_vfilename);
    
};
