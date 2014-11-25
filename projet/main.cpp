#include <QCoreApplication>
#include <traitementimage.h>
#include <percepunit.h>
#include <batiment.h>
#include <imagealgorithm.h>


int main(int argc, char *argv[1]){

    Mat image = imread(argv[1]);

    TraitementImage Traitement(&image);

    waitKey(0);
    return 0;
}
