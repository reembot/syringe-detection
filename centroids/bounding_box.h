#ifndef BBOX
#define BBOX

#include <string>
using namespace std;

class bounding_box {
    public:
        bounding_box() {}
        bounding_box(int rows, int cols)
            : cols(cols), rows(rows), x_min(cols), x_max(0), y_min(rows), y_max(0) {}
        bounding_box(const bounding_box& copy)
            : x_min(copy.x_min), x_max(copy.x_max), y_min(copy.y_min), y_max(copy.y_max) {}

        int x_min, x_max, y_min, y_max;
        int cols, rows;
        
        void  clear() { x_min = cols; x_max = 0; y_min = rows; y_max = 0; }
        int   height() { return x_max > x_min ? y_max - y_min : 0; }
        int   width()  { return x_max > x_min ? x_max - x_min : 0; }
        float area() { return x_max > x_min ? height() * width() : 0; }
};

#endif
