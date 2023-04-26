#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

#define ESCAPE_KEY (27)
#define SYSTEM_ERROR (-1)
#define ROIX (300)
#define ROIY (50)
#define SRCX (1280)
#define SRCY (720)

int main(int argc, char** argv)
{
    const char* default_file = "0";
    const char* filename = argc >= 2 ? argv[1] : default_file;
    VideoCapture cap;
    if (*filename == '0') {
        cap.open(0);
    } else {
        cap.open(filename);
    }
    bool canView = 0;
    namedWindow(filename);
    char winInput;
    int waitval = 100;
    bool origView = 1, tview = 1;

    printf("Press space to pause/unpause, 's' to step frame by frame, 'c' to switch views or 'esc' to exit\n");
    int cnt = 0;
    vector<vector<Vec4i>> linehistory;
    while (1)
    {
        Mat src, roisrc;
        cnt++;

        cap.read(src);
        if (src.empty()) {
            break;
        }

        Mat dst;
       
        int myhist[256] = {0};

        roisrc = src(Rect(0, 0, 780, 959));

        Canny(roisrc, dst, 1, 225, 3);

        vector<Vec4i> linesP;

        HoughLinesP(dst, linesP, 1, CV_PI / 180, 20, 75, 10);

        cout << "Lines from new Frame:" << endl;

        int ymin = INT_MAX;
        Vec4i top_line;

        for (size_t i = 0; i < linesP.size(); i++)
        {
            Vec4i l = linesP[i];
            float slope = (float)(l[3] - l[1]) / (float)(l[2] - l[0]);

            // Check if the line is approximately horizontal and has a length between 75 and 100
            if (abs(slope) < 0.3 && (l[2] - l[0] >= 75) && (l[2] - l[0] <= 100))
            {
                // Check if the line is the topmost line
                if (l[1] < ymin)
                {
                    ymin = l[1];
                    top_line = l;
                }
            }
        }

        // Draw the topmost line
        line(src, Point(top_line[0] + ROIX, top_line[1] + ROIY), Point(top_line[2] + ROIX, top_line[3] + ROIY), Scalar(0, 0, 255), 3, LINE_AA);

        // Calculate the bounding box coordinates based on the topmost line
        int box_width = top_line[2] - top_line[0];
        int box_height = box_width * 2; // Assuming the height is twice the width
        Rect bounding_box(top_line[0] + ROIX, top_line[1] + ROIY, box_width, box_height);

        // Draw the bounding box
        rectangle(src, bounding_box, Scalar(0, 255, 0), 2);

        if (origView)
        {
            imshow(filename, src);
        }
        else if (tview)
        {
            imshow(filename, dst);
        }

        if ((winInput = waitKey(waitval)) == ESCAPE_KEY)
        {
            break;
        }
        else if (winInput == 's')
        {
            waitval = 0;
        }
        else if (winInput == 'h')
        {
            int total = 0;
            for (int i = 0; i < 256; i++)
            {
                if (myhist[i] > 0)
                {
                    cout << i << " " << myhist[i]<<endl;
                    
                    total+=myhist[i];}}
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
