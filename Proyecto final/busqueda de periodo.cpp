#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <windows.h>
#include <conio.h>
using namespace cv;
using namespace std;

class image
{
public:
	Mat im,im1,im2,im3,cs,im_enc,frame;
	int getim();
	
};

int image::getim()
{
	im=imread("parrot.tif");
//	imshow("ORIGINAL",im);
//	Mat img=im.clone();
	if (im.empty()) 
	{
		cout << "Error : Image cannot be loaded..!!" << endl;
		return -1;
	}
	resize(im,im,Size(240,240),0.0,0.0,1);
	Mat img=im.clone();
	namedWindow("Input Image", CV_WINDOW_AUTOSIZE);
	imshow("Input Image",im);
	for (int per=50;per<80;per++){
//		int per= 60;
	
		Mat temp=Mat::zeros(im.size(),im.type());
		double m=im.rows,x,x1,y,y1; 
		int enciter=30;
		int deciter=per-enciter;
		for(int iter=0;iter<enciter;iter++)
		{
			for(double i=0;i<m;i++)
			{
				for(double j=0;j<m;j++)
				{
					x=fmod((i+j),m);   
					y=fmod((i+2*j),m);            
					temp.at<Vec3b>(x,y) = im.at<Vec3b>(i,j);
				}
			}
			temp.copyTo(im);
			temp=Mat::zeros(im.size(),im.type());
		}
//		imshow("Scrambled Image",im);
		
		Mat img2;
		for(int iter=0;iter<deciter;iter++)
		{
			for(double i=0;i<m;i++)
			{
				for(double j=0;j<m;j++)
				{
					x=fmod((i+j),m);   
					y=fmod((i+2*j),m);            
					temp.at<Vec3b>(x,y)=im.at<Vec3b>(i,j);
				}
			}
			temp.copyTo(im);
			img2 = im.clone();
			temp=Mat::zeros(im.size(),im.type());
		}
		namedWindow("Inverse Scrambled Image", CV_WINDOW_AUTOSIZE);
		imshow("Inverse Scrambled Image",im);
//		Sleep(1);
//		getch();
		
		
//		bool isEqual = (sum(img != img2) == Scalar(0,0,0,0));
//		if(isEqual){
//			cout<<"son iguales"<<endl;
//			cout<<"El periodo es: "<<per;
//		}
//		else{
//			cout<<"son distintas"<<endl;
//		}
		
		
		bool igual=true;
		for (int i=0;im.rows;i++){
			for(int j=0;im.cols;j++){
				if (img.at<Vec3b>(i,j) != im.at<Vec3b>(i,j))
					igual = false;
					break;	
			}
			if (!igual){
				break;
			}
//			cout<<img.at<Vec3b>(i,8)<<" "<<img2.at<Vec3b>(i,8)<<endl;
		}
		if(igual){
			cout<<"SON iguales y el periodo es: "<<per<<endl;}
		else{
			cout<<"son distintos "<<per<<endl;
//			Sleep(5000);
		}
//		getch();
	}
	
	
	waitKey(0);
	return 0;
	
}

// Example main copied from https://gist.github.com/EyalAr/3940636
int main(int argc, char** argv) {
	//create a gui window:

	image my;
	my.getim();
	
	
	waitKey(0);
	return 0;
} 

/// warning: This template will *dynamicaly* link against opencv, so yo'll need to copy some 
///          dll files from zinjai\mingw-gcc5\opencv3\bin in order to run this example

/// aviso: esta plantilla está configurada para enlazar *dinámicamente* con opencv, por lo
///        que necesitará copiar algunos archivos dll desde zinjai\mingw-gcc5\opencv3\bin
///        para ejecutar correctamente este ejemplo

