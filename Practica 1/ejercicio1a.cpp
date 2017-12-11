#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// Example main copied from https://gist.github.com/EyalAr/3940636
int main(int argc, char** argv) {
	//create a gui window:
	namedWindow("Output",1);
	//initialize a 120X350 matrix of black pixels:
	Mat output = Mat::zeros( 120, 350, CV_8UC3 );
	//write text on the matrix:
	putText(output,
		"Hello World :)",
		cvPoint(15,70),
		FONT_HERSHEY_PLAIN,
		3,
		cvScalar(0,255,0),
		4);
		//display the image:
//		imshow("Output", output);
		//wait for the user to press any key:
		
		cv::Mat img =cv::imread("lion.png", CV_16S);
		cv::namedWindow("Ventana");
		cv::imshow("Ventana", img);
		cv::imwrite("leoncito.png",img);
		cout<<"Cantidad de canales: "<<img.channels()<<endl;
		cout<<"Profundidad: "<<img.depth()<<endl;
		cout<<img.at(20,20);
		
		waitKey(0);
		return 0;
} 

/// warning: This template will *dynamicaly* link against opencv, so yo'll need to copy some 
///          dll files from zinjai\mingw-gcc5\opencv3\bin in order to run this example

/// aviso: esta plantilla está configurada para enlazar *dinámicamente* con opencv, por lo
///        que necesitará copiar algunos archivos dll desde zinjai\mingw-gcc5\opencv3\bin
///        para ejecutar correctamente este ejemplo

