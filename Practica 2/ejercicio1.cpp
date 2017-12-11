#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>


using namespace cv;
using namespace std;

// Example main copied from https://gist.github.com/EyalAr/3940636
int main(int argc, char** argv) {
	//create a gui window:
	int a = 0.5;
	int c = 0.2;
	vector<int> lut;
	
//	cv::Mat img =cv::imread("FAMILIA.jpg", CV_16S);
//	cv::namedWindow("Ventana");
//	cv::imshow("Ventana", img);
//	
	for (int i=0; i<256; i++){
		int s = (a*i)+c;
		if (s>255)
			s = 255;
		if (s<0)
			s = 0;
		lut.push_back(s);
	};
	
	for (int i=0; i<256; i++){
		cout<<lut[i]<<endl;
	};
	
	

//	
//	namedWindow("Output",1);
//	//initialize a 120X350 matrix of black pixels:
//	Mat output = Mat::zeros( 120, 350, CV_8UC3 );
//	//write text on the matrix:
//	putText(output,
//		"Hello World :)",
//		cvPoint(15,70),
//		FONT_HERSHEY_PLAIN,
//		3,
//		cvScalar(0,255,0),
//		4);
//		//display the image:
//		imshow("Output", output);
//		
//		//wait for the user to press any key:
//		waitKey(0);
		return 0;
} 

/// warning: This template will *dynamicaly* link against opencv, so yo'll need to copy some 
///          dll files from zinjai\mingw-gcc5\opencv3\bin in order to run this example

/// aviso: esta plantilla est� configurada para enlazar *din�micamente* con opencv, por lo
///        que necesitar� copiar algunos archivos dll desde zinjai\mingw-gcc5\opencv3\bin
///        para ejecutar correctamente este ejemplo
