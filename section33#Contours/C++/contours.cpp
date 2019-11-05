#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

static bool contoursTest(Mat imgOri);
RNG rng(12345);

int main()
{
	Mat imgOri = imread("gril.jfif");
	if (imgOri.empty())
	{
		cout << "Cannot load this picture!" << endl;
		return false;
	}

	contoursTest(imgOri);

	waitKey(0);
	return true;
}

static bool contoursTest(Mat imgOri)
{
	imshow("imgOri", imgOri);

	// binary
	Mat imgGray, imgBin;
	cvtColor(imgOri, imgGray, COLOR_BGR2GRAY);
	GaussianBlur(imgGray, imgGray, Size(5, 5), 0, 0);
	//Canny(imgGray, imgBin, 80, 160);
	threshold(imgGray, imgBin, 0, 255, THRESH_BINARY | THRESH_OTSU);

	//Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5), Point(-1, -1));
	//morphologyEx(imgBin, imgBin, MORPH_CLOSE, kernel);

	imshow("binary", imgBin);


	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(imgBin, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

	Mat result = Mat::zeros(imgOri.size(), imgOri.type());
	Mat pts;
	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(1, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(imgOri, contours, i, color, 1, 8);
		drawContours(result, contours, i, color, 1, 8);
	}

	imshow("imgOri", imgOri);
	imshow("result", result);

	return true;
}