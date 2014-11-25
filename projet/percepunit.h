#ifndef PERCEPUNIT_H
#define PERCEPUNIT_H

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace cv;
using namespace std;

class percepunit
{
    // externally accessible class members?
public:
    // constructor method
    percepunit(Mat ROI, Mat alpha, int ix, int iy, int iw, int ih) {
        ROI.copyTo(image); // make copies since args are refs!!
        alpha.copyTo(mask);
        x1 = ix;
        y1 = iy;
        w = iw;
        h = ih;
        x2 = x1 + iw;
        y2 = y1 + ih;
    }

    percepunit(Mat *image);
    int copyPercept(percepunit &unit, Mat &dest);

    void dump() {
        printf("members: x:%d, y:%d, w:%d, h:%d\n", x1, y1, w, h, x2, y2);
    }

    Mat image; // percept itself
    Mat mask; // alpha channel
    int x1; // x location of percept in frame
    int y1; // y location
    int w; // width of patch (not sure these will be needed)
    int h; // height of patch
    int x2; // coord of far X edge
    int y2; // coord of far Y edge

    // Destructor
    ~percepunit() {
        // Free image and mask
        image.release();
        mask.release();
    }
};

#endif // PERCEPUNIT_H


