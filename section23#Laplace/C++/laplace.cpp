#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

static bool laplaceTest(Mat imgOri, bool needResize = false, bool needBlur = false);
static void showImgPara(Mat &img);

int main()
{
	Mat imgOri = imread("Fig0338(a)(blurry_moon).tif");
	laplaceTest(imgOri);

	Mat imgGril = imread("Gril.jpg");
	//laplaceTest(imgGril, true, false);

	waitKey(0);
	return true;
}


static bool laplaceTest(Mat imgOri, bool needResize, bool needBlur)
{
	Mat imgLaplace, imgLaplaceDelta, imgLaplaceAdd1;
	float fx = 0, fy = 0;

	showImgPara(imgOri);
	imshow("laplaceTest imgOri", imgOri);

	if (needBlur)
	{
		GaussianBlur(imgOri, imgOri, Size(7, 7), 0, 0);
	}

	if (needResize)
	{
		resize(imgOri, imgOri, Size(imgOri.cols * 0.5, imgOri.rows * 0.5), fx = 0, fy = 0, INTER_LINEAR);
	}

	if (1 != imgOri.channels())
	{
		cvtColor(imgOri, imgOri, COLOR_BGR2GRAY);
	}


	// Laplace without delta
	Laplacian(imgOri, imgLaplace, CV_16S, 3, 1, 0);
	convertScaleAbs(imgLaplace, imgLaplace);
	imshow("laplaceTest imgLaplace", imgLaplace);

	// Laplace with delta
	Laplacian(imgOri, imgLaplaceDelta, CV_16S, 3, 1, 100);
	convertScaleAbs(imgLaplaceDelta, imgLaplaceDelta);
	imshow("laplaceTest imgLaplaceDelta", imgLaplaceDelta);

	// Laplace add original
	addWeighted(imgOri, 1.0, imgLaplace, -1.0, 0.0, imgLaplaceAdd1, CV_32F);
	convertScaleAbs(imgLaplaceAdd1, imgLaplaceAdd1);
	imshow("laplaceTest imgLaplaceAdd1", imgLaplaceAdd1);


	return true;
}

static void showImgPara(Mat &img)
{
	cout << "sizeof(img) is: " << sizeof(img) << ", img size is: " << img.size << endl;
	cout << "rows x cols: (" << img.rows << " x " << img.cols << ")" << endl;
	cout << "dims: " << img.dims << endl;
	cout << "channels: " << img.channels() << endl;
	cout << "type: " << img.type() << endl;
	cout << "depth:" << img.depth() << endl;
	cout << "elemSize:" << img.elemSize() << " (Bytes per element)" << endl;
	cout << "elemSize1:" << img.elemSize1() << "(Bytes per channel)" << endl;
	cout << "step[0]: " << img.step[0] << " (Bytes per cows only when 2 dims)" << endl;
	cout << "step[1]: " << img.step[1] << " (Bytes per element only when 2 dims)" << endl;
	cout << "step1(0): " << img.step1(0) << ", step1(1): " << img.step1(1) << " (step / elemSize1)" << endl;
	cout << "----- showImgPara End ----\n" << endl;
}