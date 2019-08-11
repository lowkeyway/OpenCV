#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

#define GAUSS_NOISE			"GaussNoise_127"
#define FILE_TYPE			".png"
#define GAUSS_NOISE_FILE	string(GAUSS_NOISE)+string(FILE_TYPE)

#define MEANS				127
#define STANDARD_DAV		10

using namespace std;
using namespace cv;

static void showImgPara(Mat &img);
static void showImgMinMaxMeansStdev(Mat &img);
static double BoxMuller();
static void noisePicCreat(Mat &imgOri, string fileName, uchar sigma);



int main()
{
	/* Original Image */
	Mat imgOri(512, 512, CV_8UC1, Scalar(MEANS));
	imshow("imgOri", imgOri);
	//showImgPara(imgOri);
	showImgMinMaxMeansStdev(imgOri);


	/* Gauss Noise Picture Create */
	noisePicCreat(imgOri, GAUSS_NOISE, STANDARD_DAV);
	Mat imgGauss = imread(GAUSS_NOISE_FILE, IMREAD_GRAYSCALE);
	if (imgGauss.empty())
	{
		cout << "Cannot find this picture!!" << endl;
		return false;
	}
	//showImgPara(imgGauss);

	showImgMinMaxMeansStdev(imgGauss);

	/* normalize */
	Mat imgNormal = Mat::zeros(imgOri.size(), CV_32FC1);
	normalize(imgGauss, imgNormal, 0, 255, NORM_MINMAX);
	imshow("imgNormal", imgNormal);

	showImgMinMaxMeansStdev(imgNormal);

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

static double BoxMuller()
{
	const double epsilon = numeric_limits<double>::min();
	double U0, U1;
	double Z0, Z1;

	do {
		U0 = rand()*(1.0 / RAND_MAX);
		U1 = rand()*(1.0 / RAND_MAX);
	} while (U0 < epsilon || U1 < epsilon);

	Z0 = sqrt(-2.0*log(U0))*cos(2 * CV_PI*U1);
	//Z1 = sqrt(-2.0*log(U0))*sin(2 * CV_PI*U1);

	//printf("[rand]U0 = %lf, U1 = %lf, Z0 = %lf, Z1 = %lf\n\n", U0, U1, Z0, Z1);


	return Z0;//We use one of them is OK.
}

static void noisePicCreat(Mat &imgOri, string fileName, uchar sigma)
{
	int row, col, channel, rows, cols, channels;
	int noisePicCount;

	rows = imgOri.rows;
	cols = imgOri.cols;
	channels = imgOri.channels();
	Mat imgNoise = imgOri.clone();


	for (row = 0; row < rows; row++)
	{
		uchar *pOri = imgOri.ptr<uchar>(row);
		uchar *pNoise = imgNoise.ptr<uchar>(row);
		for (col = 0; col < cols * channels; col++)
		{
			for (channel = 0; channel < channels; channel++)
			{
				pNoise[col + channel] = saturate_cast<uchar>(BoxMuller() * sigma + (double)pOri[col + channel]);
			}
		}
	}

	cout << "col = " << col << " , cols = " << cols << endl;

	namedWindow("noisePic");
	imshow("noisePic", imgNoise);
	imwrite(fileName+ FILE_TYPE, imgNoise);

	cout << "-----------noisePicCreat Done----------\n" << endl;
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