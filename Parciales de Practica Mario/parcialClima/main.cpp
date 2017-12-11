#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>
#include <../include/pdi_functions.h>
#include <../include/my_functions.h>

using namespace cv;
using namespace std;
using namespace pdi;

/// Multiplica una máscara binaria (con valores 0-negro y 255-blanco) a una imagen a color
Mat mult_mask(Mat src, Mat mask){
    mask = mask / 255;
    cvtColor(mask, mask, CV_GRAY2BGR);

    Mat dst;
    multiply(src, mask, dst);

    return dst;
}

int main(){
//    Mat src = loadimg("clima1.jpg", CV_LOAD_IMAGE_COLOR, 1);
//    segmentacionBGR_trackbar("../imgs/clima1.jpg");

    Mat src = imread("../imgs/clima1.jpg", CV_LOAD_IMAGE_COLOR);
    Point3i color = Point3i(255,255,255);
    int radio = 76;
    Mat seg = colorDistance(src, color, radio);
    cvtColor(seg, seg, CV_BGR2GRAY);
    threshold(seg, seg, 1, 255, THRESH_BINARY_INV);

    Mat EE = getEE(1);
    Mat aux = seg.clone();
    aux = Morfologia(aux, EE, 'e');

    imshow("src", src);
    imshow("aux", aux);
    Mat salida = mult_mask(src, aux);

    imshow("salida", salida);




    waitKey();

    return 0;
}
