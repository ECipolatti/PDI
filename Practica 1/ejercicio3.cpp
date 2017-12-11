#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace cv;
using namespace std;

void CallBackFunc(int event, int x, int y, int flags, void* userdata);


int main(int argc, char** argv) {
	//create a gui window:
	namedWindow("Output"); //by default is in autosize
	
	Mat img = imread("botellas.tif",CV_LOAD_IMAGE_GRAYSCALE);
	
	int p1=0; int p2=0; int p3=0;
	float porcentaje_total = 0;
	float porcentaje_lleno = 0;
	int botella=1;
	for (int i=0; i<img.cols;i++){
		float aux1 =(float) img.at<uchar>(35,i);
		float aux2 =(float) img.at<uchar>(35,i+1);
//		cout<<aux1<<" "<<aux2<<endl;
		if ((aux1-aux2)<(-150)){
			p1 = i;
		}
		if (((aux1-aux2)>150) or (i+1==img.cols)){
			p2 = i;
			p3 = floor((p1+p2)/2);
//			cout<< "punto intermedio es: "<<p3<<endl;
			for (int j=0;j<img.rows;j++){
				float color = (float) img.at<uchar>(j,p3);
				color = color*256;
				if (color > 10){
					porcentaje_total = porcentaje_total +1;
				}
				if ((color > 10) && (color < 200)){
					porcentaje_lleno = porcentaje_lleno +1;
				}
			}
			float porcentaje = (porcentaje_lleno/porcentaje_total)*100;
			cout<<"porcentaje que hay en la botella "<<botella<<" es: "<<porcentaje<<"%"<<endl;
			p1=0;p2=0;
			int porcentaje_total=0;
			int porcentaje_lleno=0;
			botella++;
		}
	
	}
	cout<<"termine";
	
	
	//set the callback function for any mouse event
	setMouseCallback("Output", CallBackFunc, NULL);
	
	//	
	//display the image:
	imshow("Output", img);
	
	
	waitKey(0);
	return 0;
} 


void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if  ( event == EVENT_LBUTTONDOWN )
	{
		cout << "Left click - position (" << x << ", " << y << ")" << endl;
	}
	else if  ( event == EVENT_RBUTTONDOWN )
	{
		cout << "Right click - position (" << x << ", " << y << ")" << endl;
	}
	else if  ( event == EVENT_MBUTTONDOWN )
	{
		cout << "Middle click - position (" << x << ", " << y << ")" << endl;
	}
//	else if ( event == EVENT_MOUSEMOVE )
//	{
//		cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;
//		
//	}
}


