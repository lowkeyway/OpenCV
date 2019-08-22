#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

static bool scharrTest(Mat imgOri);
static bool sobelTest(Mat imgOri);
static void showImgPara(Mat &img);

int main()
{
	Mat imgOri = imread("Fig0342(a)(contact_lens_original).tif");
	sobelTest(imgOri);

	Mat imgGril = imread("Gril.jpg");
	//scharrTest(imgGril);

	waitKey(0);
	return true;
}

static bool sobelTest(Mat imgOri)
{
	imshow("imgOri", imgOri);
	Mat imgSobel;
	Sobel(imgOri, imgSobel, imgOri.depth(), 1, 1, 3, 1, 0, BORDER_DEFAULT);
	imshow("imgSobel", imgSobel);

	return true;
}

static bool scharrTest(Mat imgOri)
{
	Mat imgScharr, imgScharrX, imgScharrY;
	Mat imgScharrAbsX, imgScharrAbsY;
	double fx = 0.0, fy = 0.0;
	//GaussianBlur(imgOri, imgOri, Size(11, 11), 0, 0);
	resize(imgOri, imgOri, Size(imgOri.cols * 0.5, imgOri.rows * 0.5), fx = 0, fy = 0, INTER_LINEAR);

	showImgPara(imgOri);
	imshow("scharrTest imgOri", imgOri);
	if (1 != imgOri.channels())
	{
		cvtColor(imgOri, imgOri, COLOR_BGR2GRAY);
	}

	showImgPara(imgOri);

	showImgPara(imgOri);

	Scharr(imgOri, imgScharrX, CV_32F, 1, 0);
	Scharr(imgOri, imgScharrY, CV_32F, 0, 1);

	convertScaleAbs(imgScharrX, imgScharrAbsX);
	convertScaleAbs(imgScharrY, imgScharrAbsY);

	showImgPara(imgScharrAbsX);
	showImgPara(imgScharrAbsY);

	imshow("imgScharrX", imgScharrAbsX);
	imshow("imgScharrY", imgScharrAbsY);

	addWeighted(imgScharrAbsX, 0.5, imgScharrAbsY, 0.5, 1, imgScharr);
	imshow("imgScharr", imgScharr);


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