#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>
#include <../include/pdi_functions.h>
#include <../include/my_functions.h>

using namespace cv;
using namespace std;
using namespace pdi;

float estaTorcida(Mat src){
    Mat Roi = src(Rect(0, 0, src.cols, 50));
//    imshow("ROI", Roi);
    float f = porcentaje(Roi, 255, 0);
//    cout << "PORCENTAJE DE BLANCOS: " << f*100 << endl;

    return f*100;
}


int main(){
    Mat src = loadimg("../imgs/test2.png", CV_LOAD_IMAGE_COLOR, 1);
//    segmentacionBGR_trackbar("../imgs/1.png");

    Point3i color = Point3i(0,0,254);
    int radio = 141;
    Mat seg = colorDistance(src, color, radio);
    imshow("seg", seg);
    cvtColor(seg, seg, CV_BGR2GRAY);
    threshold(seg, seg, 1, 255, THRESH_BINARY);
    imshow("Binarizada", seg);

    Mat element = getEE(1);
    Mat morph = seg.clone();
    morph = Morfologia(morph, element, 'd');
    morph = Morfologia(morph, element, 'd');
    morph = Morfologia(morph, element, 'd');
    morph = Morfologia(morph, element, 'e');

    imshow("MORFOLOGIA", morph);

    Point a; a.x = src.rows/2; a.y = src.cols/2;
    int W = src.cols;
    int H = src.rows;
    float P = estaTorcida(morph);
    vector<vector<Point>> contours = count_element(morph);
    vector<float> areas = get_areas(contours);

    cout << contours.size() << endl;

    Scalar color2 = Scalar(255, 0, 0);
    /// Get the moments
    vector<Moments> mu(contours.size() );
    for( size_t i = 0; i < contours.size(); i++ )
    { mu[i] = moments( contours[i], false ); }

    ///  Get the mass centers:
    vector<Point2f> mc( contours.size() );
    for( size_t i = 0; i < contours.size(); i++ ){
        mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
        putText(src, to_string(i), mc[i], FONT_HERSHEY_COMPLEX_SMALL, 0.8, color2, 1, CV_AA);
    }
    imshow("src con Centros", src);

    vector<int> indices = get_idx_areas(contours);

    Mat aux = src.clone();
    if (P > 0){
        cout << "La imagen viene derecha" << endl;

        line(src, a, mc[indices[0]], Scalar(255, 255, 0), 1, 8);
        line(src, a, mc[indices[2]], Scalar(255, 255, 0), 1, 8);
        namedWindow("src con Lineas", 0);
        imshow("src con Lineas", src);

        Point p1 = Point(W/2, H/2);
        Point p2 = Point(mc[indices[0]].x, mc[indices[0]].y);
        Point p3 = Point(mc[indices[2]].x, mc[indices[2]].y);

        float dx21 = p2.x - p1.x; float dy21 = p2.y - p1.y;
        float dx31 = p3.x - p1.x; float dy31 = p3.y - p1.y;

        float m12 = sqrt(dx21*dx21 + dy21*dy21);
        float m13 = sqrt(dx31*dx31 + dy31*dy31);
        float angle = acos( (dx21*dx31 + dy21*dy31) / (m12*m13) ) * 180/M_PI;

        if (mc[indices[0]].x < W/2){
            angle = 360 - angle;
        }
        cout << "La aguja marca " << angle << "°" << endl;
    }else{
        cout << "La imagen viene torcida" << endl;

        Point b; b.x = W/2; b.y = 0;
//        line(aux, a, b, Scalar(255, 255, 0), 1, 8);
//        line(aux, a, mc[indices[2]], Scalar(255, 255, 0), 1, 8);
//        namedWindow("src con Lineas", 0);
//        imshow("src con Lineas", aux);
        Point p1 = Point(W/2, H/2);
        Point p2 = Point(W/2, 0);
        Point p3 = Point(mc[indices[2]].x, mc[indices[2]].y);
        float dx21 = p2.x - p1.x; float dy21 = p2.y - p1.y;
        float dx31 = p3.x - p1.x; float dy31 = p3.y - p1.y;

        float m12 = sqrt(dx21*dx21 + dy21*dy21);
        float m13 = sqrt(dx31*dx31 + dy31*dy31);
        float angle = acos( (dx21*dx31 + dy21*dy31) / (m12*m13) ) * 180/M_PI;

        cout << "Angulo para enderezar la imagen: " << angle << endl;
        if (mc[indices[2]].x < W/2){
            angle = 360 - angle;
        }
        cout << "Angulo para enderezar la imagen: " << angle << endl;
        Mat aux2 = rotateMat(aux, angle);
        imshow("ORIGINAL ROTADA", aux2);

        //---------------------------------
        Mat seg2 = colorDistance(aux2, color, radio);
        imshow("seg2", seg2);
        cvtColor(seg2, seg2, CV_BGR2GRAY);
        threshold(seg2, seg2, 1, 255, THRESH_BINARY);
        imshow("Binarizada 2", seg2);

        Mat element2 = getEE(1);
        Mat morph2 = seg2.clone();
        morph2 = Morfologia(morph2, element2, 'd');
        morph2 = Morfologia(morph2, element2, 'd');
        morph2 = Morfologia(morph2, element2, 'd');
        morph2 = Morfologia(morph2, element2, 'e');

        imshow("MORFOLOGIA 2", morph2);

        vector<vector<Point>> contours2 = count_element(morph2);
        vector<float> areas2 = get_areas(contours2);

        cout << contours2.size() << endl;

        Scalar color3 = Scalar(255, 0, 0);
        /// Get the moments
        vector<Moments> mu2(contours2.size() );
        for( size_t i = 0; i < contours2.size(); i++ )
        { mu2[i] = moments( contours2[i], false ); }

        ///  Get the mass centers:
        vector<Point2f> mc2( contours2.size() );
        for( size_t i = 0; i < contours2.size(); i++ ){
            mc2[i] = Point2f( mu2[i].m10/mu2[i].m00 , mu2[i].m01/mu2[i].m00 );
            putText(aux, to_string(i), mc[i], FONT_HERSHEY_COMPLEX_SMALL, 0.8, color3, 1, CV_AA);
        }
        imshow("src con Centros", src);

        vector<int> indices2 = get_idx_areas(contours2);

        Point p11 = Point(W/2, H/2);
        Point p22 = Point(mc2[indices2[0]].x, mc2[indices2[0]].y);
        Point p33 = Point(mc2[indices2[2]].x, mc2[indices2[2]].y);

        float dx211 = p22.x - p11.x; float dy211 = p22.y - p11.y;
        float dx311 = p33.x - p11.x; float dy311 = p33.y - p11.y;

        float m122 = sqrt(dx211*dx211 + dy211*dy211);
        float m133 = sqrt(dx311*dx311 + dy311*dy311);
        float angle2 = acos( (dx211*dx311 + dy211*dy311) / (m122*m133) ) * 180/M_PI;

        if (mc2[indices2[0]].x < W/2){
            angle2 = 360 - angle2;
        }
        cout << "La aguja marca " << angle2 << "°" << endl;
    }




    waitKey();
    return 0;
}
