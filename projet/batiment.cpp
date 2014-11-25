#include "batiment.h"

Batiment::Batiment(Mat *src)
{
    image = *src;
Mat res;
//    blur(image, image, Size(3,3));

    pyrMeanShiftFiltering( image, res, 10, 40, 3);

      RemplaceValuePixel(image,179,255,179,255,179,255);

    *src = res;


}



Mat Batiment::RemplaceValuePixel(Mat& image, int minRouge, int maxRouge, int minBleu, int maxBleu, int minVert, int maxVert)
{
    Mat StructureMask;

    blur(image, image, Size(3,3) );
    Mat channel[3],rouge;
    split(image, channel);
    rouge = channel[2];

    int ChanBleu = 0;
    int ChanRouge = 0;
    int ChanVert = 0;

    rouge.copyTo(StructureMask);

    StructureMask.convertTo(StructureMask,CV_8U);
    for (int row = 0; row < StructureMask.rows; row++) {

        for (int col = 0; col < StructureMask.cols; col++) {

            ChanBleu=(int)channel[0].at<uchar>(row,col) ;
            ChanRouge=(int)channel[2].at<uchar>(row,col);
            ChanVert=(int)channel[1].at<uchar>(row,col);

            if (minBleu < ChanBleu
                    && maxBleu > ChanBleu
                    && minRouge < ChanRouge
                    && maxRouge > ChanRouge
                    && minVert < ChanVert
                    && maxVert > ChanVert )
            {
                StructureMask.at<uchar>(row,col) = 255;

            }
            else StructureMask.at<uchar>(row,col) = 0;

        }

    }

    StructureMask.convertTo(StructureMask, CV_8U);
    cvtColor(StructureMask, StructureMask, CV_GRAY2RGB);

    image.convertTo(image, CV_8U);

    image = StructureMask - image;

    return image;

}
