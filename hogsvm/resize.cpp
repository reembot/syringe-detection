#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/ml.hpp"
#include "opencv2/objdetect.hpp"
#include <iostream>
#include <time.h>
using namespace cv;
using namespace cv::ml;
using namespace std;
int main( int argc, char** argv )
{
    cout << "Loading files to resize" << endl;
    vector< String > files;
    glob( "toresize", files );
    namedWindow("resized");
    for( size_t i=0;; i++ )
    {
        cout << "Loading " << files[i] << endl;
        Mat img, rimg;
        img = imread( files[i] );
        if ( img.empty() )
        {
            return 0;
        }
        resize(img, rimg, Size(128,128), INTER_LINEAR);
//        imshow("resized", rimg);
//        if( waitKey(0) == 27
//        {
//            return 0;
//        }
        String rimgname = to_string(i) + ".png";
        cout << "Writing " << rimgname << endl;
        imwrite(rimgname, rimg);
    }
}

