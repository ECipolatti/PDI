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

    //************* MANZANAS VERDES *****************
    Mat src = imread("../imgs/EXAMEN09.jpg", CV_LOAD_IMAGE_COLOR);

    Point3i color = Point3i(15,160,105);
    int radio = 70;
    Mat verdes_seg = colorDistance(src, color, radio);
    Mat verdes_segGris;
    cvtColor(verdes_seg, verdes_segGris, CV_BGR2GRAY);
//    imshow("segGris", verdes_segGris);
//    Mat segGrisBinarizada;
    threshold(verdes_segGris, verdes_segGris, 1, 255, THRESH_BINARY);
//    imshow("segGrisBinarizada", verdes_segGris);
    Mat verdes_segGris2 = verdes_segGris.clone();

    vector<vector<Point>> contours = count_element(verdes_segGris);
    vector<float> areas = get_areas(contours);
    vector<int> indices = get_idx_areas(contours);

    cout << "Cantidad de Manzanas VERDES: " << contours.size() << endl;

    // *************** MANZANAS ROJAS ************************
//    String file = "../imgs/EXAMEN09.jpg";
//    segmentacionBGR_trackbar(file);
    Mat src2 = imread("../imgs/EXAMEN09.jpg", CV_LOAD_IMAGE_COLOR);
    Point3i color2 = Point3i(45,32,194);
    int radio2 = 120;
    Mat seg2 = colorDistance(src2, color2, radio2);
    imshow("seg2", seg2);
    Mat rojasGris;
    cvtColor(seg2, rojasGris, CV_BGR2GRAY);
    Mat rojasBinarizadas;
    threshold(rojasGris, rojasBinarizadas, 1, 255, THRESH_BINARY);
    imshow("Rojas Binarizadas", rojasBinarizadas);
    Mat rojasBinarizadas2 = rojasBinarizadas.clone();

    vector<vector<Point>> contours2 = count_element(rojasBinarizadas);
    vector<float> areas2 = get_areas(contours2);
    vector<int> indices2 = get_idx_areas(contours2);
    cout << "Cantidad de Manzanas ROJAS: " << contours2.size() << endl;


    Scalar color3 = Scalar(255, 0, 0);
    Mat final = src.clone();
    for (int i = 0; i < areas.size(); i++){

        if (areas[i] < 400){
            cout << areas[i] << endl;
            final = paint_contour_area(final, contours, indices[i], color3);
        }
    }

    for (int i = 0; i < areas2.size(); i++){
        if (areas2[i] < 400){
            final = paint_contour_area(final, contours2, indices2[i], color3);
        }
    }


    imshow("MANZANAS AZULES", final);

    waitKey();
    return 0;
}
