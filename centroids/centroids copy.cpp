// Reema Shadid - CSCI612 Final Project
// Code adapted from Kris Selvidge centroid detection for Lab

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

/////////////////////////////////////////////////////////////////////////////////////
///////// BOUNDING BOX CLASS ////////////////////////////////////////////////////////

class bounding_box {
    public:
        bbox(int rows, int cols) : x_min(cols), x_max(0), y_min(rows), y_max(0) {}

        int x_min, x_max, y_min, y_max;
        int height();
        int width();
        float area();
};

int bounding_box::height() { return x_max > x_min ? y_max - y_min : 0; }
int bounding_box::width() { return x_max > x_min ? x_max - x_min : 0; }
float bounding_box::area() { return x_max > x_min ? height() * width() : 0; }

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

bool colorScan( Mat& frame, bounding_box& bbox ) { // can specify region of interest
    
    Mat range;
    bool Roc, Prop, Phen;
    int min_x, min_y, max_x, max_y;

    // Rocuronium
    inRange(frame, Scalar(0,0,225), Scalar(150,150,255), range);
    Roc = (sum(range)[0] > 1000);
    
    // Propofol
    inRange(frame, Scalar(0,240,240),Scalar(200,255,255), range);
    Prop = (sum(range)[0] > 1000);

    // Phenylephrine
    inRange(frame, Scalar(200,0,200), Scalar(255,200,255), range);
    Phen = (sum(range)[0] > 75000);

    if (Roc)
        putText(frame, "Rocuronium", Point(30,100), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0,0,255), 1, LINE_AA);
    if (Prop)
        putText(frame, "Propofol", Point(30,50), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0,0,255), 1, LINE_AA);
    if (Phen)
        putText(frame, "Phenylephrine", Point(30,150), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0,0,255), 1, LINE_AA);
    
    if ( Roc || Prop || Phen ) {

        for ( int j= 0; j< bbox.rows; j++ ) {
            for ( int i= 0; i< bbox.cols; i++ ) {

                if ( range.at<Vec3b>(j,i)[0] > 0 ) {
                    if (min_x > i) min_x = i;
                    if (max_x < i) max_x = i;
                    if (min_y > j) min_y = j;
                    if (max_y < j) max_y = j;
                }
            }  
        }
        // draw rectangle around color detection
        rectangle( frame, Point(min_x, min_y), Point(max_x, max_y), Scalar(0,255,0), 5, 0 );
        return true;

    } else {
        return false;
    }
}

////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {

    // open camera or video file with CLI arg, if entered
    const char* default_file = "0";
    const char* filename = argc >=2 ? argv[1] : default_file;
    VideoCapture cap;
    char winInput;

    // mats for original capture, grayscale version, previous frame and frame difference
    Mat src, srcGray, srcPrev, srcDiff;

    if  ( *filename == '0' ) {
        cap.open(0);
    } else {
        cap.open(filename);
    }
    namedWindow(filename);

    // check source exists
    cap.read(src);
    if ( src.empty() ) {
        return 0;
    }

    // compare change in intensity on single channel image (grayscale)
    cv::cvtColor(src, srcGray, COLOR_BGR2GRAY);
    srcPrev = srcGray.clone();
    srcDiff = srcGray.clone();

    // track frame count and timestamp
    int count = 0;
    bounding_box prev_bbox;
    printf("Press 's' to write screen capture, 'c' to switch views, or 'esc' to exit\n");

    while (1) {
        count++;
      
        cap.read(src);
        if ( src.empty() )
            break;

        cv::cvtColor(src, srcGray, COLOR_BGR2GRAY);
        // difference between last frame and current
        absdiff(srcPrev, srcGray, srcDiff);
        // prep new "previous" frame with current one
        srcPrev = srcGray.clone();

        // set bounding box coords
        bounding_box bbox(src.rows, src.cols);

        // search entire frame for threshold met
        for ( int j= 0; j< src.rows; j++ ) {
            for ( int i= 0; i< src.cols; i++ ) {

                if ( srcDiff.at<Vec3b>(j,i)[0] > 175 ) { // threshold for.... using GIMP
                    if (bbox.x_min > i) bbox.x_min = i;
                    if (bbox.x_max < i) bbox.x_max = i;
                    if (bbox.y_min > j) bbox.y_min = j;
                    if (bbox.y_max < j) bbox.y_max = j;
                }
            }
        }
        // calculate centroid
        x_hat = int((bbox.x_max - bbox.x_min)/2) + bbox.x_min;
        y_hat = int((bbox.y_max - bbox.y_min)/2) + bbox.y_min;

        // if threshold met (movement detected)
        if ( bbox.x_max > bbox.x_min ) {
            
            // mark center of movement with a crosshair
            line( src, Point(x_hat-10,y_hat), Point(x_hat+10,y_hat), Scalar(0,0,255),5 ); // from left to right
            line( src, Point(x_hat,y_hat-10), Point(x_hat,y_hat+10), Scalar(0,0,255),5 ); // from top to bottom

            // draw rectangle around box
            rectangle( src, Point(bbox.x_min, bbox.y_min), Point(bbox.x_max, bbox.y_max), Scalar(0,255,0),5,0 );
            //rectangle( srcDiff, Point(bbox.x_min, bbox.y_min), Point(bbox.x_max, bbox.y_max), Scalar(255),5,0 );

            int areaChange = int( prev_bbox.area() / (bbox.area()*100) );
            cout << "Area change: " << areaChange << endl;

            if ( areaChange > 50 ) { // if change is greater than 50% 

                //Mat dst = src(Rect(x_min + w/2, y_min, w/2, h));
                
                // pass source frame and area of interest
                bool hasColor = colorScan(src, prev_bbox);
                if (hasColor)
                    int time = int(cap.get(CAP_PROP_POS_MSEC));
            }

            // save bbox history
            prev_bbox = bbox;
        }
    }
}

