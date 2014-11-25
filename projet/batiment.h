#ifndef BATIMENT_H
#define BATIMENT_H

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <initializer_list>

using namespace cv;
using namespace std;

class Batiment
{
public:
    Batiment(Mat *src);
    Mat RemplaceValuePixel(Mat& image, int minRouge, int maxRouge, int minBleu, int maxBleu, int minVert, int maxVert);

private:
    Mat image;
};

#endif // BATIMENT_H
