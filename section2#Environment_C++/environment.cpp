#include "pch.h"
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  

using namespace cv;

int main()
{
	Mat img = imread("Girl.jpg");	//Put Girl.jpg into the same path with source code.
	namedWindow("Girl");		//Windowns named Girl
	imshow("Girl", img);		//Show img in Girl window
	waitKey(6000);			//Waiting for a key input or 6S timeout
	return 0;
}