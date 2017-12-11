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
//    Mat src = loadimg("01_Monedas.jpg", CV_LOAD_IMAGE_COLOR, 1);
//        Mat src = loadimg("02_Monedas.jpg", CV_LOAD_IMAGE_COLOR, 1);
        Mat src = loadimg("03_Monedas.jpg", CV_LOAD_IMAGE_COLOR, 1);
    Mat src1;
    cvtColor(src, src1, CV_BGR2GRAY);


    threshold(src1, src1, 250, 255, THRESH_BINARY_INV);
    imshow("Binarizada", src1);

    Mat element = getEE(2);
    Mat imgMorf = Morfologia(src1, element, 'c');
    imshow("Dilatada", imgMorf);

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    findContours(imgMorf, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

    vector<float> area(contours.size());
    for( size_t i = 0; i< contours.size(); i++ ){
        area[i] = contourArea(contours[i]);
        cout<<area[i]<<endl;
    }


    Scalar color = Scalar(0, 0, 0);

    /// Get the moments
    vector<Moments> mu(contours.size() );
    for( size_t i = 0; i < contours.size(); i++ )
    { mu[i] = moments( contours[i], false ); }

    ///  Get the mass centers:
    vector<Point2f> mc( contours.size() );
    for( size_t i = 0; i < contours.size(); i++ ){
        mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
        putText(src, to_string(i), mc[i], FONT_HERSHEY_COMPLEX_SMALL, 0.8, color, 1, CV_AA);
    }

    imshow("Centroides", src);
    int uno = 0, dos = 0, cinco = 0, diez = 0, veinte = 0, cincuenta = 0;
    int un_e = 0, dos_e = 0;
    float Area;
    for (int i = 0; i < area.size(); i++){
        Area = area[i];

        if (area[i] > 7500){
            dos_e++;
        }else if(area[i] > 6800 && area[i] < 7400 ){
            cincuenta++;
        }else if(area[i] > 6300 && area[i] < 6700){
            un_e++;
        }else if(area[i] > 5700 && area[i] < 6200){
            veinte++;
        }else if(area[i] > 5100 && area[i] < 5500){
            cinco++;
        }else if(area[i] > 4400 && area[i] < 5000){
            diez++;
        }else if(area[i] > 3900 && area[i] < 4300){
            dos++;
        }else if(area[i] < 3900){
            uno++;
        }
    }

    cout << uno << " Monedas de 1 centavo" << endl;
    cout << dos << " Monedas de 2 centavos" << endl;
    cout << cinco << " Monedas de 5 centavos" << endl;
    cout << diez << " Monedas de 10 centavos" << endl;
    cout << veinte << " Monedas de 20 centavos" << endl;
    cout << cincuenta << " Monedas de 50 centavos" << endl;


    cout << un_e << " Monedas de 1 euro" << endl;
    cout << dos_e << " Monedas de 2 euro" << endl;

    cout << "MONTO TOTAL" << endl;
    float monto = uno + dos * 2 + cinco * 5 + diez * 10 + veinte * 20 + cincuenta * 50;
    cout << monto << endl;

    float monto2 = un_e * 100 + dos_e * 2 * 100;
    cout << (monto + monto2)/100 << endl;





    waitKey(0);

    return 0;
}
