#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

static bool cannyTest(Mat imgOri, bool needResize = false);

int main()
{
	Mat imgOri = imread("Gril.jpg");
	if (imgOri.empty())
	{
		cout << "Cannot find this picture!" << endl;
		return false;
	}
	cannyTest(imgOri);

	return true;
}

static bool cannyTest(Mat imgOri, bool needResize)
{
	Mat imgEdge;

	imshow("Canny Original", imgOri);

	if (1 != imgOri.channels())
	{
		cvtColor(imgOri, imgOri, COLOR_BGR2GRAY);
	}

	if (true == needResize)
	{
		resize(imgOri, imgOri, Size(imgOri.cols/2, imgOri.rows/2), 0, 0, INTER_LINEAR);
		//convertScaleAbs(imgOri, imgOri);
	}

	GaussianBlur(imgOri, imgOri, Size(5, 5), 1);

	Canny(imgOri, imgEdge, 20, 50, 3, false);
	imshow("Canny Edge", imgEdge);

	waitKey(0);
	return true;
}