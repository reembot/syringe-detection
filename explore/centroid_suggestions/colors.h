#ifndef LABELCOLORS
#define LABELCOLORS

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "bounding_box.h"
#include <string>
using namespace cv;
using namespace std;

//inline bool colorScan( Mat& frame, bounding_box& ROI, bounding_box* label_box, string& label ) {
inline bool colorScan( Mat& frame, bounding_box& ROI, string& label ) {
    
    bool found = false;
    int pixel_count = 0;
    float bval, gval, rval;


    // for ( int j= 0; j< ROI.height() && !found; j++ ) {
    //     for ( int i= 0; i< ROI.width() && !found; i++ ) {
    //for ( int j= ROI.y_min; j< ROI.height() && !found; j++ ) {
    //    for ( int i= ROI.x_min; i< ROI.width() && !found; i++ ) {
	for ( int j= 0; j< frame.rows && !found; j++ ) {
        for ( int i= 0; i< frame.cols && !found; i++ ) {

                bval = (float)frame.at<Vec3b>(j,i)[0];          
                gval = (float)frame.at<Vec3b>(j,i)[1];           
                rval = (float)frame.at<Vec3b>(j,i)[2];

                
                if ( label == "Propofol" ) { // yellow
                    if ( (rval > 96) && (rval-bval > rval/3) && (bval < 32) && (gval < rval * 9 / 10) && (gval > 3 * rval / 5) ) 
                        pixel_count++;
                }
                else if ( label == "Rocuronium" ) { // red
                    if ( (rval > 224) && (bval < 64) && (gval < 96) ) 
                        pixel_count++; 
                }
                else if ( label == "Phenylephrine" ) { // lavender
                    if ( (rval > 128) && (bval > 128) && (gval < 3 * rval / 4) && (gval > rval / 2) )
                        pixel_count++;
                }
                // else if ( label == "Lidocaine" ) { // gray
                //     if ( (rval > 48) && (gval > 48) && (bval > 48) && (rval < 80) && (gval < 80) && (bval < 80) ) 
                //         pixel_count++;
                // }
                // else if ( label == "Odanestron" || label == "Dexamethasone" ) { // white
                //     if ( (rval > 175) && (gval > 175) && (bval > 175) )
                //         pixel_count++;
                // }

                else if ( label == "Lidocaine" || label == "Odanestron" || label == "Dexamethasone" ) {
					// gray & white
					if ( (abs(rval-gval) < 20) && (abs(rval-bval) < 20) && (abs(gval-bval) < 20) ) { 
                    
						if ( label == "Lidocaine" ) { // gray
							if ( (rval > 48) && (gval > 48) && (bval > 48) && (rval < 80) && (gval < 80) && (bval < 80) )
								pixel_count++;
						}
						else if ( label == "Odanestron" || label == "Dexamethasone" ) { // white
							if ( (rval > 175) && (gval > 175) && (bval > 175) )
								pixel_count++;
						}
					}
                } 
                else {
                    cout << "Label unsupported for color detection." << endl;
                    return false;
                }
                if ( pixel_count > 1000 )
					found = true;
        } 
    }
    
    if ( found ) {

        // check for letter "O" with HoughCircles()
        if ( label == "Odanestron" ) {
            pyrUp(frame,frame,Size(frame.cols*2,frame.rows*2));
            pyrUp(frame,frame,Size(frame.cols*2,frame.rows*2));
	        Mat gray;
            cvtColor(frame, gray, COLOR_BGR2GRAY);
            GaussianBlur(gray, gray, Size(7, 7), 0);
            vector<Vec3f> circles;
            HoughCircles(gray, circles, HOUGH_GRADIENT, 1, gray.rows/32, 70, 20, 20, 35);
            
            if (circles.size() > 0)
                return true;
            else
                return false;
        }
        return true;
    }
}

#endif
