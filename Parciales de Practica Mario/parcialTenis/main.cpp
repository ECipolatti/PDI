#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>
#include <../include/pdi_functions.h>
#include <../include/my_functions.h>

using namespace cv;
using namespace std;
using namespace pdi;

int main(){
    Mat src = loadimg("tenis01.jpg", CV_LOAD_IMAGE_COLOR, 1);
    Mat A = edge_detection(src, 3, 1);
//    cvtColor(src, src, CV_BGR2GRAY);
//    threshold(src, src, 180, 255, THRESH_BINARY);
////    imshow("lalala", src);
//    Mat A = edge_detection(src, 3, 1);
////    detect_linesP_trackbar(src);


    waitKey();
    return 0;
}
