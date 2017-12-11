#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>
#include <../include/pdi_functions.h>
#include <../include/my_functions.h>

using namespace cv;
using namespace std;
using namespace pdi;


Mat mascaras(Mat src){
    Mat hsv;
    cvtColor(src, hsv, CV_BGR2HSV);
//    imshow("HSV", hsv);
//    segmentacionBGR_trackbar(hsv);

    Point3i color = Point3i(94,77,234);
    int radio = 72;
    Mat aux = colorDistance(hsv, color, radio);

//    imshow("seg", seg);

    cvtColor(aux, aux, CV_BGR2GRAY);
    threshold(aux, aux, 1, 255, THRESH_BINARY);
    aux = filtro_mediana(aux, 3);
//    imshow("Nubes", aux);
    Mat element = getEE(1);
    Mat aux2 = aux.clone();

    aux2 = Morfologia(aux2, element, 'e');
    aux2 = Morfologia(aux2, element, 'd');
    aux2 = Morfologia(aux2, element, 'd');
    aux2 = Morfologia(aux2, element, 'd');
    aux2 = Morfologia(aux2, element, 'd');
    aux2 = Morfologia(aux2, element, 'd');
    aux2 = Morfologia(aux2, element, 'd');
    aux2 = Morfologia(aux2, element, 'd');
    aux2 = Morfologia(aux2, element, 'd');
    aux2 = Morfologia(aux2, element, 'd');
    aux2 = Morfologia(aux2, element, 'd');
    aux2 = Morfologia(aux2, element, 'd');
    aux2 = Morfologia(aux2, element, 'd');
    aux2 = Morfologia(aux2, element, 'd');
    aux2 = Morfologia(aux2, element, 'd');
    aux2 = Morfologia(aux2, element, 'd');
    aux2 = Morfologia(aux2, element, 'd');
    aux2 = Morfologia(aux2, element, 'd');

    aux2 = Morfologia(aux2, element, 'c');

//    imshow("Nubes Bin", aux2);

    return aux2;
}

int main() {

    Vector<Mat> imgOriginal;
    Vector<Mat> Mascaras;

    for (int i = 1; i < 22; i ++){
        string pepe = "f" + to_string(i) + ".jpg";
        Mat a = loadimg(pepe, CV_LOAD_IMAGE_COLOR, 0);
//        imshow("PEPE", a);
        Mat b = mascaras(a);

        imgOriginal.push_back(a);
        Mascaras.push_back(b);
    }

    Mat Salida = Mat::zeros(imgOriginal[0].size(), imgOriginal[0].type());

    for(int m = 0; m < imgOriginal.size(); m++){
        for (int i = 0; i < imgOriginal[m].rows; i ++){
            for (int j = 0; j < imgOriginal[m].cols; j ++){
                if (Mascaras[m].at<uchar>(i,j) == 0 ){
                    Salida.at<Vec3b>(i,j) = imgOriginal[m].at<Vec3b>(i,j);
                }
            }
        }
    }
    imshow("SALIDA", Salida);

//    imwrite("M.jpg", Salida);

    Mat hsv;
    cvtColor(Salida, hsv, CV_BGR2HSV);
//    segmentacionBGR_trackbar(hsv);

    Point3i color = Point3i(39,147,66);
    int radio = 8;
    Mat seg = colorDistance(hsv, color, radio);
    cvtColor(seg, seg, CV_BGR2GRAY);
    threshold(seg, seg, 1, 255, THRESH_BINARY);
    seg = filtro_mediana(seg, 3);
    imshow("seg", seg);

//    detect_linesP_trackbar(seg);
    cvtColor(seg, seg, CV_GRAY2BGR);
    vector<Vec4i> lines;
    Mat dst = seg.clone();
    Mat con_lineas = seg.clone();
    lines = detect_linesP(seg, dst, 77, 0, 360, 68, 67, 0);
    for(size_t i = 0; i < lines.size(); ++i) {
        Vec4i l = lines[i];
        line(con_lineas, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,0,0), 3, CV_AA);
    }
    imshow("Original con lineas", con_lineas);
    Vec4i l = lines[0];
    double angle = atan2(l[3] - l[1], l[2] - l[0]) * 180.0 / CV_PI - 90;

    Salida = rotateMat(Salida, angle);
    imshow("sin nubes", Salida);


    waitKey();
    return 0;
}
