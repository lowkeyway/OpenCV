#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

static bool pyraminTest(Mat imgOri, int level);

int main()
{
	Mat imgOri = imread("Fig0701.tif");
	imshow("imgOri", imgOri);

	pyraminTest(imgOri, 3);

	waitKey(0);
	return true;
}

static bool pyraminTest(Mat imgOri, int level)
{
	vector<Mat> vecPyUp;
	vector<Mat> vecPyDown;
	vector<Mat> vecLapImg;
	Mat temp;
	int i = 0, count = 0;
	// Down
	count = 0;
	for (i = 0; i < level; i++)
	{
		vecPyUp.push_back(imgOri);
		pyrDown(imgOri, imgOri);
	}
	for (vector<Mat>::iterator it = vecPyUp.begin(); it != vecPyUp.end(); it++, count++)
	{
		imshow("imgPyram Zoom In " + to_string(count), *it);
	}

	// Up
	count = 0;
	Mat imgMin = vecPyUp[vecPyUp.size() - 1].clone();
	vecPyDown.push_back(imgMin);
	for (int i = 0; i < level-1; i++)
	{
		pyrUp(imgMin, imgMin, vecPyUp[level - i - 1 - 1].size());
		vecPyDown.push_back(imgMin);
	}
	for (vector<Mat>::iterator it = vecPyDown.begin(); it != vecPyDown.end(); it++, count++)
	{
		imshow("imgPyram Zoom Out " + to_string(count), *it);
	}

	// Laplace
	count = 0;
	for (i = 0; i < vecPyUp.size() - 1; i++)
	{
		subtract(vecPyUp[i], vecPyDown[level - i - 1], temp);
		temp = temp + Scalar(127, 127, 127);
		vecLapImg.push_back(temp);
	}
	for (vector<Mat>::iterator it = vecLapImg.begin(); it != vecLapImg.end(); it++, count++)
	{
		imshow("imgPyram laplace " + to_string(count), *it);
	}

	return true;
}