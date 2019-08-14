#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

#define GAUSS_NOISE			"GaussNoise_127"
#define FILE_TYPE			".png"
#define GAUSS_NOISE_FILE	string(GAUSS_NOISE)+string(FILE_TYPE)

#define MEANS				127
#define STANDARD_DAV		10

#define MAX_KERNEL_SIZE		500
#define MAX_SIGMA			255

using namespace std;
using namespace cv;

static void showImgPara(Mat &img);
static void showImgMinMaxMeansStdev(Mat &img);
static void kernelSizeCallBack(int, void*);
static void xSigmaCallBack(int, void*);
static void ySigmaCallBack(int, void*);

Mat gImgOri, gImgGauss, gImgGaussFilter;
int gKernelSize = 11;
int gXSigma = 10;
int gYSigma = 10;


int main()
{
	/* Original Image */
	gImgOri = imread("original.png", IMREAD_GRAYSCALE);
	if (gImgOri.empty())
	{
		cout << "Cannot find original picture!!" << endl;
		return false;
	}
	imshow("imgOri", gImgOri);
	//showImgPara(gImgOri);
	showImgMinMaxMeansStdev(gImgOri);


	/* Gauss Noise Picture Create */
	gImgGauss = imread(GAUSS_NOISE_FILE, IMREAD_GRAYSCALE);
	if (gImgGauss.empty())
	{
		cout << "Cannot find noise picture!!" << endl;
		return false;
	}
	imshow("imgGauss", gImgGauss);
	//showImgPara(gImgGauss);
	showImgMinMaxMeansStdev(gImgGauss);

	/* Gauss filter */
	//GaussianBlur(gImgGauss, gImgGaussFilter, Size(7, 7), 10, 10);
	GaussianBlur(gImgGauss, gImgGaussFilter, Size(gKernelSize, gKernelSize), gXSigma, gYSigma);
	imshow("Gauss Filter Out", gImgGaussFilter);
	showImgMinMaxMeansStdev(gImgGaussFilter);

	/* Add Bar */
	cv::createTrackbar("Kernel Size", "Gauss Filter Out", &gKernelSize, MAX_KERNEL_SIZE, kernelSizeCallBack);
	cv::createTrackbar("X Sigma", "Gauss Filter Out", &gXSigma, MAX_KERNEL_SIZE, xSigmaCallBack);
	cv::createTrackbar("Y Sigma", "Gauss Filter Out", &gYSigma, MAX_KERNEL_SIZE, ySigmaCallBack);

	waitKey(0);
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
	cout << "----showImgPara End----" << endl;
}


static void showImgMinMaxMeansStdev(Mat &img)
{
	/* Max and Min value and location */
	double minValue, maxValue;
	Point minLdx, maxLdx;
	minMaxLoc(img, &minValue, &maxValue, &minLdx, &maxLdx, Mat());
	cout << "Min: " << minValue << "[" << minLdx << "]" << endl;
	cout << "Max: " << maxValue << "[" << maxLdx << "]" << endl;

	/* means and stdev*/
	Mat means, stdev;
	meanStdDev(img, means, stdev);
	cout << "Means: " << means.at<double>(0, 0) << endl;
	cout << "Standard Deviationst: " << stdev.at<double>(0, 0) << endl;
	cout << "----showImgMinMaxMeansStdev End----\n" << endl;
}

static void kernelSizeCallBack(int, void*)
{
	if (gKernelSize % 2 == 0)
	{
		gKernelSize = gKernelSize + 1;
	}
	GaussianBlur(gImgGauss, gImgGaussFilter, Size(gKernelSize, gKernelSize), gXSigma, gYSigma);
	imshow("Gauss Filter Out", gImgGaussFilter);
	showImgMinMaxMeansStdev(gImgGaussFilter);
}


static void xSigmaCallBack(int, void*)
{
	GaussianBlur(gImgGauss, gImgGaussFilter, Size(gKernelSize, gKernelSize), gXSigma, gYSigma);
	imshow("Gauss Filter Out", gImgGaussFilter);
	showImgMinMaxMeansStdev(gImgGaussFilter);
}


static void ySigmaCallBack(int, void*)
{
	GaussianBlur(gImgGauss, gImgGaussFilter, Size(gKernelSize, gKernelSize), gXSigma, gYSigma);
	imshow("Gauss Filter Out", gImgGaussFilter);
	showImgMinMaxMeansStdev(gImgGaussFilter);
}