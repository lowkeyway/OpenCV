#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include<time.h>

using namespace std;
using namespace cv;

static bool shiTomas(Mat imgOri);
static bool harrisTest(Mat imgOri);

RNG rng(12345);

int main()
{
	Mat imgOri = imread("right13.jpg");
	if (imgOri.empty())
	{
		cout << "Cannot load this picture!" << endl;
		return false;
	}
	clock_t start, harrisTime, shiTomasTime;
	double totaltime;
	Mat imgHarris = imgOri.clone();
	Mat imgShiTomas = imgOri.clone();

	start = clock();
	harrisTest(imgHarris);
	harrisTime = clock();

	shiTomas(imgShiTomas);
	shiTomasTime = clock();

	imshow("imgOri", imgOri);

	cout << "Harris takes [" << (double)(harrisTime - start) / CLOCKS_PER_SEC << "] s" << endl;
	cout << "shiTomas takes [" << (double)(shiTomasTime - harrisTime) / CLOCKS_PER_SEC << "] s" << endl;
	
	waitKey(0);
	return true;
}

static bool harrisTest(Mat imgOri)
{
	int blockSize = 2;
	int apertureSize = 3;
	double k = 0.04;


	Mat imgGray, imgHarris;
	//showImgPara(imgGray);
	cvtColor(imgOri, imgGray, COLOR_BGR2GRAY);

	imgHarris = Mat::zeros(imgOri.size(), imgOri.type());

	cornerHarris(imgGray, imgHarris, blockSize, apertureSize, k);

	Mat imgDst, imgDstScale;
	imgDst = Mat::zeros(imgHarris.size(), imgHarris.type());
	normalize(imgHarris, imgDst, 0, 255, NORM_MINMAX);

	convertScaleAbs(imgDst, imgDstScale);

	for (int row = 0; row < imgDstScale.rows; row++)
	{
		for (int col = 0; col < imgDstScale.cols; col++)
		{
			int rsp = (int)imgDstScale.at<uchar>(row, col);
			if (rsp > 85)
			{
				int b = rng.uniform(0, 256);
				int g = rng.uniform(0, 256);
				int r = rng.uniform(0, 256);
				circle(imgOri, Point(col, row), 5, Scalar(b, g, r), 2);
			}
		}
	}

	imshow("imgHarris", imgOri);

	return true;
}

static bool shiTomas(Mat imgOri)
{
	int maxCorners = 500;
	double quality_level = 0.01;
	double minDistance = 0.04;

	Mat imgGray, imgShiTomas;
	cvtColor(imgOri, imgGray, COLOR_BGR2GRAY);

	vector<Point2f> corners;

	goodFeaturesToTrack(imgGray, corners, maxCorners, quality_level, minDistance, Mat(), 3, false);

	for (int i = 0; i < corners.size(); i++) {
		int b = rng.uniform(0, 256);
		int g = rng.uniform(0, 256);
		int r = rng.uniform(0, 256);
		circle(imgOri, corners[i], 5, Scalar(b, g, r), 3, 8, 0);
	}

	imshow("imgShiTomas", imgOri);

	return true;
}