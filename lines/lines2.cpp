#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

bool intersection(Vec4i a, Vec4i b, Point2f& intersection_point) {
    float x1 = a[0];
    float y1 = a[1];
    float x2 = a[2];
    float y2 = a[3];
    float x3 = b[0];
    float y3 = b[1];
    float x4 = b[2];
    float y4 = b[3];

    float denominator = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);
    if (denominator == 0) {
        return false;
    }

    float ua = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / denominator;
    float ub = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / denominator;

    if (ua < 0 || ua > 1 || ub < 0 || ub > 1) {
        return false;
    }

    intersection_point.x = x1 + ua * (x2 - x1);
    intersection_point.y = y1 + ua * (y2 - y1);
    return true;
}

int main() {
    // Open the video capture device
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "Failed to open video capture device" << endl;
        return 1;
    }

    // Create a window to display the output
    namedWindow("Video", WINDOW_NORMAL);

    while (true) {
        // Read a frame from the video capture device
        Mat frame;
        cap >> frame;
        if (frame.empty()) {
            break;
        }

        // Apply pre-processing to the frame
        Mat gray, blurred, edges;
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        GaussianBlur(gray, blurred, Size(3, 3), 0);
        Canny(blurred, edges, 50, 150);

        // Detect lines in the image using HoughLinesP
        vector<Vec4i> lines;
        HoughLinesP(edges, lines, 1, CV_PI / 180, 50, 50, 10);

        // Filter the lines to keep only those that are roughly parallel to each other
        vector<Vec4i> filtered_lines;
        double angle_threshold = 15; // degrees
        for (size_t i = 0; i < lines.size(); i++) {
            Vec4i line = lines[i];
            double dx = line[2] - line[0];
            double dy = line[3] - line[1];
            double angle = abs(atan2(dy, dx) * 180 / CV_PI);
            if (angle < angle_threshold || angle > 180 - angle_threshold) {
                filtered_lines.push_back(line);
            }
        }

        // Find the intersection points of the remaining lines
        vector<Point2f> corners;
        for (size_t i = 0; i < filtered_lines.size(); i++) {
            Vec4i line1 = filtered_lines[i];
            for (size_t j = i + 1; j < filtered_lines.size(); j++) {
                Vec4i line2 = filtered_lines[j];
                Point2f pt;
                bool found = intersection(line1, line2, pt);
                if (found) {
                    corners.push_back(pt);
                }
            }
        }

        // Draw the rectangle around the corners, if enough corners are found
        if (corners.size() >= 4) {
            // Find the bounding box around the corners
            Rect box = boundingRect(corners);

            // Draw the bounding box
            rectangle(frame, box, Scalar(0, 0, 255), 2);

            // Display the resulting image
            imshow("Video", frame);
        } else {
            // Display the original image
            imshow("Video", edges);
        }

        // Wait for a key press and check if it's the ESC key
        if (waitKey(1) == 27) {
            break;
        }
    }

    // Release the video capture device and destroy the window
    cap.release();
    destroyAllWindows();

    return 0;
}
