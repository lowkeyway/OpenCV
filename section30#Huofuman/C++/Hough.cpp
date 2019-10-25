#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

static bool houghTest(Mat imgOri);

int main()
{
	Mat imgOri = imread("building.jpg");
	if (imgOri.empty())
	{
		cout << "Cannot read image" << endl;
	}

	houghTest(imgOri);

	waitKey(0);
	return true;
}

static bool houghTest(Mat imgOri)
{
	Mat imgCan;
	imshow("imgOri", imgOri);

	//GaussianBlur(imgOri, imgOri, Size(7, 7), 0);

	Canny(imgOri, imgCan, 100, 160, 3, false);
	imshow("Canny", imgCan);

	vector<Vec2f> lines;
	Mat imgHough = imgOri.clone();
	Mat imgHoughP = imgOri.clone();

	HoughLines(imgCan, lines, 1, CV_PI / 180, 200, 0, 0);

	Point pt1, pt2;
	for (size_t i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0];
		float theta = lines[i][1];
		double a = cos(theta), b = sin(theta);
		double x0 = a * rho, y0 = b * rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		line(imgHough, pt1, pt2, Scalar(0, 0, 255), 3, LINE_AA);
	}

	imshow("imgHough", imgHough);


	vector<Vec4i> Plines;
	HoughLinesP(imgCan, Plines, 1, CV_PI / 180, 80, 30, 10);
	for (size_t i = 0; i < Plines.size(); i++)
	{
		line(imgHoughP, Point(Plines[i][0], Plines[i][1]),
			Point(Plines[i][2], Plines[i][3]), Scalar(0, 255, 0), 1, 8);
	}

	imshow("imgHoughP", imgHoughP);


	return true;
}