#include "traitementimage.h"
#include <percepunit.h>
#include <imagealgorithm.h>

TraitementImage::TraitementImage(Mat *image)
{
    src = *image;

    /// Initialisation des variables
    Mat ImageBaseBatiment, findVegetation,vegeta;


    /// Création de copies de l'image en entrée
    src.copyTo(ImageBaseBatiment); src.copyTo(findVegetation);


    /// 1. Detection de la Vegetation
    vegeta = DetectionVegetation(findVegetation);
    findVegetation.copyTo(src);
    findVegetation = Erosion(findVegetation, 2);
    vegeta = Erosion(vegeta, 7);
    vegeta = Dilatation(vegeta, 6);
    vegeta = Erosion(vegeta, 4);
    vegetContours = ObjectfindContour(vegeta);          // On obtient les contours de la végétation


    /// 2. Detection de la route
    pixelOfRoad();                                      // On trouve les pixels correspondant à la teinte de la route
    thinning();                                         // On lance la squeletisation
    srcRoad = Dilatation(srcRoad, 1);


    /// 3. Detection des contours de la route
    Mat RoadContour;
    srcRoad.copyTo(RoadContour);
    roadContours = ObjectfindContour(RoadContour);      // On obtient les contours de la route


    /// 4. Detection des contours des bâtiments
    percepunit Classification(&ImageBaseBatiment);      // Classification supervisée sur l'image afin d'améliorer le traitement
    BatimentTraitement(ImageBaseBatiment);              // On obtient les contours de la végétation


    /// 5. Affichage des resutats
    affiche(src, "src");
    affiche(ImageBaseBatiment, "Detection Batiment");

    waitKey(0);
}

Mat TraitementImage::DetectionVegetation(Mat& image){

    Mat pixelOfVegetation, mask;

    cvtColor(src, pixelOfVegetation, CV_BGR2GRAY);

    threshold(pixelOfVegetation, pixelOfVegetation, 110, 180,1);

    pixelOfVegetation.copyTo(mask);

    srcVeg = Mat::ones(mask.rows,mask.cols,mask.type());   // Creation d'une matrice vide de 1

    srcVeg = srcVeg*255; // On remplace la matrice par la value 255

    for(int j=0;j<srcVeg.rows;j++)
    {
        for (int i = 0; i < srcVeg.cols; i++)
        {
            if(pixelOfVegetation.at<uchar>(j,i) == 0) // Si pixel de l'image sont noir
                srcVeg.at<uchar>(j,i) = 0; // alors on met le pixel de la matrice vide en noir
            else
                srcVeg.at<uchar>(j,i) = 255; // sinon blanc
        }
    }

    srcVeg.convertTo(srcVeg, CV_8U);
    src.convertTo(src, CV_8U);
    Mat Vegetation = srcVeg ;

    return srcVeg;
}


vector <vector<Point> > TraitementImage::ObjectfindContour(Mat& imageFindContour){

    Mat canny_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    RNG rng(12345);

    /// Detect edges using canny
    Canny( imageFindContour, canny_output, 100, 100*2, 3 );

    /// Find contours
    findContours( canny_output, contours, hierarchy, CV_RETR_TREE,
                  CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    /// Draw contours
    Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );

    for( int i = 0; i < contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 255),
                               rng.uniform(0,255), rng.uniform(0,255) );
        drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0,
                      Point() );
    }

    imageFindContour = drawing;

    return contours;

}

Mat TraitementImage::Erosion(Mat image, int valueOfErosion){
    Mat element = getStructuringElement(MORPH_ELLIPSE, Size( 2 * valueOfErosion + 1, 2 * valueOfErosion + 1), Point(valueOfErosion, valueOfErosion));

    erode(image, image, element);
    return image;
}

Mat TraitementImage::Dilatation(Mat image, int valueOfDilatation){

    Mat element = getStructuringElement(MORPH_ELLIPSE, Size( 2 * valueOfDilatation + 1, 2 * valueOfDilatation + 1), Point(valueOfDilatation, valueOfDilatation));

    dilate(image, image, element);
    return image;
}

Mat TraitementImage::affiche(Mat& image, char* const title){
    namedWindow(title, CV_WINDOW_NORMAL );
    imshow(title, image);

    return src;
}

Mat TraitementImage::thinningIteration(Mat& img, int iter){
    CV_Assert(img.channels() == 1);
    CV_Assert(img.depth() != sizeof(uchar));
    CV_Assert(img.rows > 3 && img.cols > 3);

    Mat marker = Mat::zeros(img.size(), CV_8UC1);

    int nRows = img.rows;
    int nCols = img.cols;

    if (img.isContinuous()) {
        nCols *= nRows;
        nRows = 1;
    }

    int x, y;
    uchar *pAbove;
    uchar *pCurr;
    uchar *pBelow;
    uchar *nw, *no, *ne;    // north (pAbove)
    uchar *we, *me, *ea;
    uchar *sw, *so, *se;    // south (pBelow)

    uchar *pDst;

    // initialize row pointers
    pAbove = NULL;
    pCurr  = img.ptr<uchar>(0);
    pBelow = img.ptr<uchar>(1);

    for (y = 1; y < img.rows-1; ++y) {
        // shift the rows up by one
        pAbove = pCurr;
        pCurr  = pBelow;
        pBelow = img.ptr<uchar>(y+1);

        pDst = marker.ptr<uchar>(y);

        // initialize col pointers
        no = &(pAbove[0]);
        ne = &(pAbove[1]);
        me = &(pCurr[0]);
        ea = &(pCurr[1]);
        so = &(pBelow[0]);
        se = &(pBelow[1]);

        for (x = 1; x < img.cols-1; ++x) {
            // shift col pointers left by one (scan left to right)
            nw = no;
            no = ne;
            ne = &(pAbove[x+1]);
            we = me;
            me = ea;
            ea = &(pCurr[x+1]);
            sw = so;
            so = se;
            se = &(pBelow[x+1]);

            int A  = (*no == 0 && *ne == 1) + (*ne == 0 && *ea == 1) +
                    (*ea == 0 && *se == 1) + (*se == 0 && *so == 1) +
                    (*so == 0 && *sw == 1) + (*sw == 0 && *we == 1) +
                    (*we == 0 && *nw == 1) + (*nw == 0 && *no == 1);
            int B  = *no + *ne + *ea + *se + *so + *sw + *we + *nw;
            int m1 = iter == 0 ? (*no * *ea * *so) : (*no * *ea * *we);
            int m2 = iter == 0 ? (*ea * *so * *we) : (*no * *so * *we);

            if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
                pDst[x] = 1;
        }
    }

    img &= ~marker;
    return img;

}

Mat TraitementImage::thinning(){
    Mat bw;
    threshold(srcRoad, bw, 10, 255, CV_THRESH_BINARY);

    bw = bw.clone();
    bw /= 255;         // convert to binary image

    Mat prev = Mat::zeros(bw.size(), CV_8UC1);
    Mat diff;

    do {
        thinningIteration(bw, 0);
        thinningIteration(bw, 1);
        absdiff(bw, prev, diff);
        bw.copyTo(prev);
    }
    while (countNonZero(diff) > 0);

    bw *= 255;
    srcRoad = bw;
    return srcRoad;

}

Mat TraitementImage::pixelOfRoad()
{
    Mat PixelRoad,image,channel[3], RoadMask;
    src.copyTo(PixelRoad);
    PixelRoad = Erosion(PixelRoad, 2);
    PixelRoad = Dilatation(PixelRoad, 2);
    PixelRoad = Erosion(PixelRoad, 4);
    PixelRoad = Dilatation(PixelRoad, 5);

    split(PixelRoad, channel);
    RoadMask = channel[2]; // On récupère la bande rouge

    int minRouge = 105;
    int maxRouge = 150;

    int minBleu = 110;
    int maxBleu = 150;

    int minVert = 110;
    int maxVert = 150;

    int ChanBleu = 0;
    int ChanRouge = 0;
    int ChanVert = 0;


    RoadMask.convertTo(RoadMask,CV_8U);
    for (int row = 0; row < RoadMask.rows; row++) {

        for (int col = 0; col < RoadMask.cols; col++) {

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
                RoadMask.at<uchar>(row,col) = 255;

            }
            else RoadMask.at<uchar>(row,col) = 0;

        }

    }
    RoadMask = Erosion(RoadMask, 7);
    RoadMask = Dilatation(RoadMask, 30);

    srcRoad = RoadMask;

    return RoadMask;

}

Mat TraitementImage::BatimentTraitement(Mat& image){

    // Initialisation
    int z = 61;
    Mat bilateral, equalize,channel[3], rouge, canny_output, imageSoustraction, image_thresh, mask;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    RNG rng(12345);


    /// Apply Bilateral Filter
    bilateralFilter(image, bilateral, z, z, z);

    /// Extract Channel Red
    split(bilateral, channel);
    rouge = channel[2];

    /// Apply Histogram Equalization
    equalizeHist( rouge, image );                       // prend en entrée l'image en niveau de gris, crée une image equalize

    /// On passe toutes les pixels inférieurs à un seuil en noir
    for(int j=0;j<image.rows;j++)
    {
        for (int i=0;i<image.cols;i++)
        {
            if( image.at<uchar>(j,i)<180)
                image.at<uchar>(j,i) = 0; //white
            else
                image.at<uchar>(j,i) = 255; //white

        }
    }
    image = Erosion(image, 1);
    image = Dilatation(image, 1);
    image = Erosion(image, 2);
    image = Dilatation(image, 1);


    /// Obtenir une zone uniforme
    Mat imageDilatation = image;

    Mat A= Mat::ones(imageDilatation.rows,imageDilatation.cols,CV_8U);

    imageSoustraction = image-(A*255 - imageDilatation);


    threshold(imageSoustraction, image_thresh, 125, 255, THRESH_BINARY);


    image_thresh.copyTo(mask);                                               // Loop through the border pixels and if they're black, floodFill from there
    for (int i = 0; i < mask.cols; i++) {
        if (mask.at<char>(0, i) == 0) {
            floodFill(mask, Point(i, 0), 255, 0, 10, 10);
        }
        if (mask.at<char>(mask.rows-1, i) == 0) {
            floodFill(mask, Point(i, mask.rows-1), 255, 0, 10, 10);
        }
    }
    for (int i = 0; i < mask.rows; i++) {
        if (mask.at<char>(i, 0) == 0) {
            floodFill(mask, Point(0, i), 255, 0, 10, 10);
        }
        if (mask.at<char>(i, mask.cols-1) == 0) {
            floodFill(mask, Point(mask.cols-1, i), 255, 0, 10, 10);
        }
    }



    for (int row = 0; row < mask.rows; ++row) {                              // Compare mask with original.

        for (int col = 0; col < mask.cols; ++col) {
            if (mask.at<char>(row, col) == 0) {
                image.at<char>(row, col) = 255;
            }
        }

    }

    /// Detect edges using canny
    Canny( image, canny_output, 100, 100*2, 3 );

    /// Find contours
    findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );


    vector<RotatedRect> minRect(contours.size());
    for (int i = 0; i < contours.size(); i++ ){
        minRect[i] = minAreaRect(Mat(contours[i]));
    }

    batiContours = minRect;

    Mat drawingBatiment = Mat::zeros( canny_output.size(), CV_8UC3 );

    for (int i = 0; i < contours.size(); i++)
    {
        if (contours.at(i).size() > 40){
            Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );

            Point2f rect_points[4]; minRect[i].points(rect_points);             //rotated rectangle

            for (int j = 0; j < 4; j++)
                line(drawingBatiment, rect_points[j], rect_points[(j+1)%4], color, 1, 8);
        }

    }

    image = drawingBatiment;
    return image;

}

vector<vector<Point>> TraitementImage::exportcontour(int i){

    if (i == 1)
       return roadContours ;

    else if (i == 2)
        return vegetContours;

}

vector<RotatedRect> TraitementImage::exportcontourBati(){

       return batiContours;

}

TraitementImage::~TraitementImage(){
    delete this;
}
