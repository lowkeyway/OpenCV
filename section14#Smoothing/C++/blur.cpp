#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;

#define KERNEL_THRESHOLD	200

void blurCallBack(int, void *);


int gKernelSize = 20;
Mat gImgOri, gImgOut;

int main()
{
	gImgOri = imread("Fig0235(c)(kidney_original).tif");
	namedWindow("imgOri");
	imshow("imgOri", gImgOri);

	blur(gImgOri, gImgOut, Size(gKernelSize, gKernelSize));
	namedWindow("imgOut");
	imshow("imgOut", gImgOut);

	cv::createTrackbar("Kernel Size", "imgOut", &gKernelSize, KERNEL_THRESHOLD, blurCallBack);

	waitKey(0);
	return true;
}


void blurCallBack(int ,void *)
{
	if (gKernelSize <= 0)
		return;
	blur(gImgOri, gImgOut, Size(gKernelSize, gKernelSize));
	imshow("imgOut", gImgOut);
	cout << gKernelSize << endl;
}