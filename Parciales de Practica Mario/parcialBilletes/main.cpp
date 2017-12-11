#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>
#include <../include/pdi_functions.h>
#include <../include/my_functions.h>

using namespace cv;
using namespace std;
using namespace pdi;

Mat rotateMat(Mat src, float deg){
    Point2f pc(src.cols/2.f, src.rows/2.f);

    Mat r = getRotationMatrix2D(pc, deg, 1.0);
    Mat dst;
    warpAffine(src, dst, r, src.size());

    return dst;
}

int contarRombos(Mat src, int column1, int column2){
    bool adentro = false;
    int contador = 0;

    for (int i = 0; i < src.rows; i++){
        if (src.at<uchar>(i, column1) == 0){
            if (adentro == false){
                adentro = true;
                contador = contador + 1;
            }
        }
        else{
            adentro = false;
        }
    }

    for (int i = 0; i < src.rows; i++){
        if (src.at<uchar>(i, column2) == 0){
            if (adentro == false){
                adentro = true;
                contador = contador + 1;
            }
        }
        else{
            adentro = false;
        }
    }

    return contador;
}

int main(){
//    string billete("B2C1_01.jpg");
//    string billete = "B2C1_02a.jpg";
//    string billete = "B5C1_01.jpg";
//    string billete = "B5C1_02a.jpg";
//    string billete = "B10C1_01.jpg";
//    string billete = "B10C1_02a.jpg";
//    string billete = "B20C1_01a.jpg";
//    string billete = "B20C1_02.jpg";
//    string billete = "B50C1_01a.jpg";
//    string billete = "B50C1_02.jpg";
//    string billete = "B100C1_01.jpg";
    string billete = "B100C1_02a.jpg";

    Mat src = loadimg(billete, CV_LOAD_IMAGE_GRAYSCALE , 1);
    threshold(src, src, 150, 255, THRESH_BINARY);
    imshow("Binarizado", src);
    Mat rotada = src.clone();

    Mat aux = src(Rect(130, 0, 60, 100));
    int rombos = contarRombos(aux, 25, 35);
    if (rombos > 6)
        rotada = rotateMat(rotada, 180);

    imshow("Imagen al Derecho", rotada);
    Mat aux2 = rotada(Rect(130, 0, 60, 100));
    Mat EE1 = getEE(9);
    aux2 = Morfologia(aux2, EE1, 'c');

    namedWindow("Rect", 0);
    imshow("Rect", aux2);
//    Mat LOLO;

//    Canny(aux2, LOLO, 100, 150, 3, 0);
//    namedWindow("LOLO", 0);
//    imshow("LOLO", LOLO);

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    findContours(aux2, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

//    int rombos2 = contarRombos(aux2, 25, 35);

    int rombos2 = contours.size()-1;
    switch (rombos2) {
    case 6:
        cout << rombos2 << " - Billete de $ 2 " << endl;
        break;
    case 5:
        cout << rombos2 << " - Billete de $ 5 " << endl;
        break;
    case 4:
        cout << rombos2 << " - Billete de $ 10 " << endl;
        break;
    case 3:
        cout << rombos2 << " - Billete de $ 20 " << endl;
        break;
    case 2:
        cout << rombos2 << " - Billete de $ 50 " << endl;
        break;
    case 1:
        cout << rombos2 << " - Billete de $ 100 " << endl;
        break;
    default:
        cout << "NI IDEA QUE BILLETE METISTE AMEO" << endl;
        break;
    }

    waitKey(0);

    return 0;
}
