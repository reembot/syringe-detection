#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace cv;


// See www.asciitable.com
#define ESCAPE_KEY (27)
#define SYSTEM_ERROR (-1)


Mat canny_frame, timg_gray, timg_grad;
Mat frame;
Mat image,src, src_gray;
Mat grad;

int lowThreshold = 0;
const int max_lowThreshold = 100;
const int ratio = 3;
const int kernel_size = 3;
const char* window_name = "video_display";
int ksize = 1;
int scale = 1;
int delta = 0;
int ddepth = CV_16S;


void CannyThreshold(int, void*)
{
    cvtColor(frame, timg_gray, COLOR_BGR2GRAY);

    /// Reduce noise with a kernel 3x3
    blur( timg_gray, canny_frame, Size(3,3) );

    /// Canny detector
    Canny( canny_frame, canny_frame, lowThreshold, lowThreshold*ratio, kernel_size );

    /// Using Canny's output as a mask, we display our result
    timg_grad = Scalar::all(0);

    frame.copyTo( timg_grad, canny_frame);

    imshow( window_name, timg_grad );

}



int main( int argc, char** argv )
{
   VideoCapture cam0(0);
   char winInput;
   namedWindow("video_display");


   if (!cam0.isOpened())
   {
       exit(SYSTEM_ERROR);
   }

   cam0.set(CAP_PROP_FRAME_WIDTH, 640);
   cam0.set(CAP_PROP_FRAME_HEIGHT, 480);
   
   char key = 'n';
   int method = 1; // method 1 = normal, 2 = canny, 3 = sobel
   int frame_count = 0;
   struct timespec start, stop;
   double exectime;
   double fps;
   clock_gettime(CLOCK_REALTIME, &start);


   while (1)
   {
    cam0.read(frame);
    switch(method) {
      case 1:
        imshow("video_display", frame);
        frame_count++;
        break;
      case 2:
        namedWindow( window_name, WINDOW_AUTOSIZE );
        createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );
        CannyThreshold(0, 0);
        frame_count++;
        break;
      case 3:
        GaussianBlur(frame, src, Size(3, 3), 0, 0, BORDER_DEFAULT);
        // Convert the image to grayscale
        cvtColor(src, src_gray, COLOR_BGR2GRAY);
        Mat grad_x, grad_y;
        Mat abs_grad_x, abs_grad_y;
        Sobel(src_gray, grad_x, ddepth, 1, 0, ksize, scale, delta, BORDER_DEFAULT);
        Sobel(src_gray, grad_y, ddepth, 0, 1, ksize, scale, delta, BORDER_DEFAULT);
        // converting back to CV_8U
        convertScaleAbs(grad_x, abs_grad_x);
        convertScaleAbs(grad_y, abs_grad_y);
        addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
        imshow(window_name, grad);
        frame_count++;
        break;
    }
    int oldmethod = method;
    key = (char)waitKey(10);
    if(key == 27)
    {
      break;
    }
    cam0.read(frame);
    if (key == 'n' || key == 'N')
    {
      method = 1;
    }
    if (key == 'c' || key == 'C')
    {
      method = 2;
    }
    if (key == 's' || key == 'S')
    {
      method = 3;
    }
    if(oldmethod!=method){
      clock_gettime(CLOCK_REALTIME, &stop);
      exectime = (stop.tv_sec - start.tv_sec) + (double)(stop.tv_nsec - start.tv_nsec) / (double) 1000000000L;
      fps = frame_count / exectime;
      printf("Method %d complete\n",oldmethod);
      printf("Total Time: %f secs\n",exectime);
      printf("Frame captured rate: %f fps \n", fps);
      frame_count = 0;
      clock_gettime(CLOCK_REALTIME, &start);
    }
   }

   destroyWindow("video_display");

}
