// Reema Shadid - CSCI612 Final Project
// Code adapted from Kris Selvidge centroid detection for Lab

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include "colors.h"
#include "bounding_box.h"

using namespace cv;
using namespace std;

// See www.asciitable.com
#define ESCAPE_KEY (27)
#define SYSTEM_ERROR (-1)

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

// bool colorScan( Mat& frame, bounding_box& ROI, bounding_box* label_box, string& label ) {
    
//     Mat range;
//     bool found = false;

//         if ( label == "Rocuronium" ) {
//             inRange(frame, Scalar(0,64,192), Scalar(64,128,255), range);
//             if (countNonZero(range) > 5000) {
//                 found = true;
//             }
//         }
//         else if ( label == "Propofol" ) {
//             inRange(frame, Scalar(0,128,128),Scalar(64,192,255), range);
//             if (countNonZero(range) > 5000) {
//             found = true;
//             }
//         }
//         else if ( label == "Phenylephrine" ) {
//             //inRange(frame, Scalar(128,90,180), Scalar(240,162,208), range);
//             inRange(frame, Scalar(130,120,170), Scalar(180,140,200), range);
//             if (countNonZero(range) > 5000) {
//             found = true;
//             }
//         }
//         else if ( label == "Odanestron" ) {
//             inRange(frame, Scalar(180,180,180), Scalar(255,255,255), range);
//             if (countNonZero(range) > 5000) {
//             found = true;
//             }
//         }
//         else if ( label == "Lidocaine" ) {
//             inRange(frame, Scalar(200,200,200), Scalar(120,120,120), range);
//             if (countNonZero(range) > 5000) {
//             found = true;
//             }
//         // }
//         // else if ( label == "Dexamethasone" ) {
//         //     // inRange(frame, Scalar(60,60,60), Scalar(70,70,70), range);
//         //     if (countNonZero(range) > 5000) {
//         //     found = true;
//         //     }
//         }

//     // scan region of interest for label boundaries
//     if ( found ) {

//         for ( int j= 0; j< ROI.height(); j++ ) {
//             for ( int i= 0; i< ROI.width(); i++ ) {

//                 if ( range.at<Vec3b>(j,i)[0] > 0 ) {
//                     if (label_box->x_min > i) label_box->x_min = i;
//                     if (label_box->x_max < i) label_box->x_max = i;
//                     if (label_box->y_min > j) label_box->y_min = j;
//                     if (label_box->y_max < j) label_box->y_max = j;
//                 }
//             }  
//         }
//     }
//     return found;
// }

////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {

    // open camera or video file with CLI arg, if entered
    const char* default_file = "0"; // for camera
    const char* filename = argc >= 2 ? argv[1] : default_file;
    const char* output_file = "centroids_vout.csv";
    const char* out_filename = argc >=3 ? argv[2] : output_file;

    const char* show_window = "no_show";
    const char* show = argc >=4 ? argv[3] : show_window;

    ofstream outfile;
    outfile.open(out_filename);
    VideoCapture cap;
    char winInput;

    // mats for original capture, grayscale version, previous frame and frame difference
    Mat src, srcGray, srcPrev, srcDiff;

    if  ( *filename == '0' ) {
        cap.open(0);
    } else {
        cap.open(filename);
    }

    // check source exists
    cap.read(src);
    if ( src.empty() ) {
        return 0;
    }

       VideoWriter vout;
       Size S = Size((int) cap.get(CAP_PROP_FRAME_WIDTH),    // Acquire input size
                  (int) cap.get(CAP_PROP_FRAME_HEIGHT));

       vout.open(argv[4], VideoWriter::fourcc('H','2','6','4'), 10, S, true);

    // compare change in intensity on single channel image (grayscale)
    cv::cvtColor(src, srcGray, COLOR_BGR2GRAY);
    srcPrev = srcGray.clone();
    srcDiff = srcGray.clone();

    // track frame count and timestamp
    int count = 0;
    int maxChange = 0;
    int minChange = 0;
    bounding_box prev_bbox(src.rows, src.cols);
    bounding_box bbox(src.rows, src.cols);

    //unordered_map<string,bounding_box*> label_boxes;
    unordered_map<string,bool> had_label;
    vector<string> labels = {"Rocuronium", "Propofol", "Phenylephrine", "Odanestron", "Lidocaine", "Dexamethasone"};
    //vector<string> labels = {"Rocuronium", "Propofol", "Phenylephrine", "Odanestron", "Lidocaine"};

    for ( string label : labels ) {
        //label_boxes.insert( pair<string,bounding_box*>(label, new bounding_box(src.rows, src.cols)) );
        had_label.insert( pair<string,bool>(label,false) );
    }

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

        // set bounding box size
        //bounding_box bbox(src.rows, src.cols);

        // search entire frame for threshold met
        for ( int j= 0; j< src.rows; j++ ) {
            for ( int i= 0; i< src.cols; i++ ) {

                if ( srcDiff.at<Vec3b>(j,i)[0] > 90 ) { // threshold found using GIMP with a black background
                    if (bbox.x_min > i) bbox.x_min = i;
                    if (bbox.x_max < i) bbox.x_max = i;
                    if (bbox.y_min > j) bbox.y_min = j;
                    if (bbox.y_max < j) bbox.y_max = j;
                }
            }
        }
        // calculate centroid
        int x_hat = int((bbox.x_max - bbox.x_min)/2) + bbox.x_min;
        int y_hat = int((bbox.y_max - bbox.y_min)/2) + bbox.y_min;


        // if threshold met (movement detected)
        if ( bbox.x_max > 0 && bbox.y_max > 0 ) {
            
            // mark center of movement with a crosshair
            line( src, Point(x_hat-10,y_hat), Point(x_hat+10,y_hat), Scalar(0,0,255),5 ); // from left to right
            line( src, Point(x_hat,y_hat-10), Point(x_hat,y_hat+10), Scalar(0,0,255),5 ); // from top to bottom

            // draw rectangle around box
            rectangle( src, Point(bbox.x_min, bbox.y_min), Point(bbox.x_max, bbox.y_max), Scalar(0,255,0),5,0 );
		}
		
		// calculate change in area between current and last movement box
        int areaChange = int( ((bbox.area() - prev_bbox.area()) / prev_bbox.area()) *100);
		
		// if shrank by more than 70% (hand drops a syringe)
        if ( areaChange < 0 && areaChange < -70 ) { 
			
            //search for each color
            for ( string label : labels ) {
				
				// reset bounding box
                //label_boxes[label]->clear();
				
				// check for label color existence 
                //bool found = colorScan(src, prev_bbox, label_boxes[label], label);
                bool found = colorScan(src, prev_bbox, label);

                if ( had_label[label] && found ) { // if label is still there (both true)
						
                } else {
                    // take timestamp
                    std:string msecs = to_string(int(cap.get(CAP_PROP_POS_MSEC)));
                    
                    if ( found ) {     // did not have before (single true)
                        had_label[label] = true;
                        outfile << label << "," << msecs << endl;

                    } else if ( had_label[label] ) { // had before, but no longer
                        had_label[label] = false;
                        outfile << label << "," << msecs << endl;
                            
                    } else { // both false, was not there previously nor entered
						continue;
					}
                }
            }
        }
        
        
        if ( strcmp(show, "show") == 0 ) {

            int position = 50;
            for ( string label : labels ) {
                if ( had_label[label] ) {

                    Scalar color;
                    if ( label == "Rocuronium" )
                        color = Scalar(64,128,255);
                    else if ( label == "Propofol" )
                        color = Scalar(64,192,255);
                    else if ( label == "Phenylephrine" )
                        color = Scalar(240,162,208);
                    else if ( label == "Odanestron" )
                        color = Scalar(255,255,255);
                    else if ( label == "Lidocaine" )
                        color = Scalar(70,70,70);

                    putText(src, label, Point(30,position), FONT_HERSHEY_COMPLEX_SMALL, 0.8, color, 1, LINE_AA);
                    //bounding_box* color_box = label_boxes[label];
                    //rectangle( src, Point(color_box->x_min, color_box->y_min), Point(color_box->x_max, color_box->y_max), color, 5, 0 );
                    position += 50;
                }
            }
            vout.write(src);
        }

        // save bbox history
        prev_bbox = bbox;
        bbox.clear();
    }

    //for ( string label : labels )
	//	delete label_boxes[label];
    vout.release();
    cap.release();
    return 0;
}
