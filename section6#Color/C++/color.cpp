#include "pch.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	Mat img = imread("check-circle.png");	//Put Girl.jpg into the same path with source code.
	if (img.empty())
	{
		cout << "The picture is not exist!" << endl;
		return 1;
	}
	namedWindow("Girl");		//Windowns named Girl
	imshow("Girl", img);		//Show img in Girl window

	Mat gray;
	cvtColor(img, gray, COLOR_BGR2GRAY);
	namedWindow("GirlGray");
	imshow("GirlGray", gray);
	imwrite("check-circle_gray.jpg", gray);

	waitKey(6000);			//Waiting for a key input or 6S timeout
	return 0;
}