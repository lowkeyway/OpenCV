#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <random>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <ctime>
#include <stdio.h>


using namespace std;
using namespace cv;

static void showImgPara(Mat &img);
static bool addTest(Mat imgOri, string fileName, int noiseNumber);
static void noisePicCreat(Mat &imgOri, string fileName, int noiseNum);
static double BoxMuller();


int main()
{
	Mat imgOri = imread("Fig0226(galaxy_pair_original).tif");
	if (imgOri.empty())
	{
		cout << "Picture load failed!";
		return -1;
	}
	namedWindow("original");
	imshow("original", imgOri);

	noisePicCreat(imgOri, "Noise", 10);
	addTest(imgOri, "Noise", 10);
	//for (int i = 0; i < 10; i++)
	//	cout << BoxMuller() << endl;;

	waitKey(0);
	//system("pause");
	return 0;
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
}

static bool addTest(Mat imgOri, string fileName, int noiseNumber)
{
	Mat *pNoisePicList = new Mat[noiseNumber];
	Mat imgAdd = Mat::zeros(imgOri.size(), imgOri.type());

	for (int i = 0; i < noiseNumber; i++)
	{
		pNoisePicList[i] = imread(fileName + to_string(i) + ".png");
		if (pNoisePicList[i].empty())
		{
			cout << "Cannot find Noise" << noiseNumber << ".png" << endl;
			return false;
		}
	}

	for(int j = 0; j < noiseNumber; j++)
	{
		addWeighted(imgAdd, 1.0, pNoisePicList[j], 1.0 / noiseNumber, 0.0, imgAdd);
	}

	namedWindow("imgAdd");
	imshow("imgAdd", imgAdd);

	delete[]pNoisePicList;
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

static void noisePicCreat(Mat &imgOri, string fileName, int noiseNum)
{
	int row, col, rows, cols, channals;
	int noisePicCount;
	Mat* pNoisePicList = new Mat[noiseNum];
	
	rows = imgOri.rows;
	cols = imgOri.cols;
	channals = imgOri.channels();

	for (noisePicCount = 0; noisePicCount < noiseNum; noisePicCount++)
	{
		pNoisePicList[noisePicCount] = imgOri.clone();
		for (row = 0; row < rows; row++)
		{
			uchar *pOri = imgOri.ptr<uchar>(row);
			uchar *pNoise = pNoisePicList[noisePicCount].ptr<uchar>(row);
			for (col = 0; col < cols * channals; col++)
			{
				pNoise[col] = saturate_cast<uchar>(BoxMuller() * 32 + (double)pOri[col]);
				pNoise[col + 1] = saturate_cast<uchar>(BoxMuller() * 32 + (double)pOri[col + 1]);
				pNoise[col + 2] = saturate_cast<uchar>(BoxMuller() * 32 + (double)pOri[col + 2]);
			}
		}

		cout << "col = " << col << " , cols = " << cols << endl;

		namedWindow("noisePic" + to_string(noisePicCount));
		imshow("noisePic" + to_string(noisePicCount), pNoisePicList[noisePicCount]);
		imwrite(fileName + to_string(noisePicCount) + ".png", pNoisePicList[noisePicCount]);
	}
	
	delete[]pNoisePicList;
	cout << "-----------Done----------" << endl;
}