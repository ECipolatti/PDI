#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <bitset>

using namespace cv;
using namespace std;

vector<Mat> bitplan(Mat img){
	vector<Mat> plano;
	for (int p=0;p<8;p++){
		Mat BP(img.cols,img.cols,CV_8U);
		
		for (int i=0; i<img.rows; i++){
			for (int j=0; j<img.cols; j++){
				std::bitset<8> intensidad((int) img.at<uchar>(i,j));
				BP.at<uchar>(i,j) = intensidad[p]*255; 				
			}
		}
		plano.push_back(BP);
	}
	return plano;
}

Mat join_image(vector<Mat> v){
	Mat imagen(v[0].cols,v[0].rows,CV_8U);
	for (int i=0; i<v[0].rows; i++){
		for (int j=0; j<v[0].cols; j++){
			int pixel=0;
			for (int p=0; p<8; p++){
				pixel = pixel +((v[p].at<uchar>(i,j))*pow(2,p)); 				
			}
			imagen.at<uchar>(i,j) = pixel*255;
		}
	}
	
	return imagen;
}

Mat codificar(Mat dest,Mat src){
	Mat aux(dest.cols,dest.rows,CV_8U);
	vector<Mat> plano =	bitplan(dest);
	for (int i=0; i<plano[0].rows; i++){
		for (int j=0; j<plano[0].cols; j++){
			plano[0].at<uchar>(i,j) = src.at<uchar>(i,j);
		}
	}
	aux = join_image(plano);
	return aux;
}

Mat decodificar(Mat img){
	vector<Mat> plano =	bitplan(img);
	return plano[0];
}


int main(int argc, char** argv) {
	//create a gui window:
	
	
	Mat img = imread("lion.png",CV_8U);
	imshow("Output", img);
	
	vector<Mat> bitp = bitplan(img);
	imshow("plano",bitp[2]);
	
	
	imshow("UNION",join_image(bitp));
		//display the image:
		imshow("Output", img);
	
//IMAGEN BINARIA
	Mat img2 = Mat::zeros(img.cols,img.rows,CV_8U);
	for (int i=50;i<img.cols-50;i++){
		for (int j=50;j<img.rows-50;j++){
			img2.at<uchar>(i,j)=255;
		}
	}
	
	Mat img3 = codificar(img,img2);
	imshow("codificacion",img3);
	imshow("decodificacion",decodificar(img3));
	
	waitKey(0);
	return 0;
} 


