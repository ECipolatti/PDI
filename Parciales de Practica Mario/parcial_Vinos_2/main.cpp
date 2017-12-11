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

    Mat src = loadimg("../imgs/tinto_7.jpg", CV_LOAD_IMAGE_COLOR, 0);

    vector<Mat> channels = canales("../imgs/tinto_3.jpg", 0, 0);

    imshow("CANAL S", channels[4]);
    Mat aux = channels[4].clone();
    threshold(aux, aux, 25, 255, THRESH_BINARY);
    imshow("AUX", aux);
    int cantBlancos = 0;
    int ultimo = 0;
    for (int i = 0; i < aux.rows; i++){
        for (int j = 0; j < aux.cols; j++){
            if (aux.at<uchar>(i,j) != 0){
                cantBlancos++;
                if (ultimo < i)
                    ultimo = i;
            }
        }
    }

    cout << cantBlancos << endl;

    Mat reducida = src(Rect(0, 0, src.cols, ultimo));
    imshow("Original Reducida", reducida);

    cvtColor(reducida, reducida, CV_BGR2GRAY);
    threshold(reducida, reducida, 252, 255, THRESH_BINARY_INV);
    imshow("Reducida", reducida);
    Mat element = getEE(2);
    Mat morph = reducida.clone();
//    morph = filtro_mediana(morph, 3);
    morph = Morfologia(morph, element, 'e');
    morph = Morfologia(morph, element, 'd');
    morph = Morfologia(morph, element, 'e');
    morph = Morfologia(morph, element, 'd');
    morph = Morfologia(morph, element, 'e');
    morph = Morfologia(morph, element, 'd');
    morph = Morfologia(morph, element, 'e');
    morph = Morfologia(morph, element, 'd');
    morph = Morfologia(morph, element, 'd');
    morph = Morfologia(morph, element, 'd');
    morph = Morfologia(morph, element, 'd');
    morph = Morfologia(morph, element, 'd');
    morph = Morfologia(morph, element, 'd');
    morph = Morfologia(morph, element, 'd');
    morph = Morfologia(morph, element, 'e');
    morph = Morfologia(morph, element, 'e');
    morph = Morfologia(morph, element, 'e');
    morph = Morfologia(morph, element, 'e');


    imshow("Morph", morph);

    int cantBlancos2 = 0;
    for (int i = 0; i < morph.rows; i++){
        for (int j = 0; j < morph.cols; j++){
            if (morph.at<uchar>(i,j) != 0){
                cantBlancos2++;
            }
        }
    }
    cout<<"porcentaje: " << cantBlancos2/cantBlancos << endl;

//    segmentacionBGR_trackbar("../imgs/tinto_3.jpg");


    ///-----------------   ESTO ANDA JOSHA ----------------------------------------------
//    Point3i color = Point3i(66,80,253);
//    int radio = 83;
//    Mat seg = colorDistance(src, color, radio);

//    imshow("seg", seg);

//    cvtColor(seg, seg,CV_BGR2GRAY);
//    threshold(seg, seg, 1, 255, THRESH_BINARY);
//    imshow("LALALA", seg);

//    float f = porcentaje(seg, 255, 0);
//    cout << f*100 << endl;
//    if (f*100 > 2){
//        cout << "EL VINO ES TINTO" << endl;
//    }else{
//        cout << "EL VINO ES BLANCO" << endl;
//    }




    waitKey();
    return 0;
}
