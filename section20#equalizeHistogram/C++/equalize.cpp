#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

static bool histShow(vector<Mat> vecOri, string fileName);
static bool hsvHistShow(vector<Mat> vecOri, string fileName);
static bool imgShow(vector<Mat> vecOri, string fileName);
static bool equlizeHist(vector<Mat> vecOri, vector<Mat> &vecEqu);

int main()
{
	vector<Mat> vecOri;
	vector<Mat> vecEqu;
	Mat imgOri1 = imread("Fig0316(1)(top_left).tif", 0); vecOri.push_back(imgOri1);
	//Mat imgOri2 = imread("Fig0316(2)(2nd_from_top).tif", 0); vecOri.push_back(imgOri2);
	//Mat imgOri3 = imread("Fig0316(3)(third_from_top).tif", 0); vecOri.push_back(imgOri3);
	//Mat imgOri4 = imread("Fig0316(4)(bottom_left).tif", 0); vecOri.push_back(imgOri4);
	Mat imgOri5 = imread("lowkeyway.jpg"); vecOri.push_back(imgOri5);

	imgShow(vecOri, "Ori");
	histShow(vecOri, "Ori");
	equlizeHist(vecOri, vecEqu);
	imgShow(vecEqu, "Equ");
	histShow(vecEqu, "Equ");
	hsvHistShow(vecOri, "HSV");

	waitKey(0);
	return true;
}

static bool histShow(vector<Mat> vecOri, string fileName)
{
	int matNum = (int)vecOri.size();
	const int channel[] = { 0 };
	int histSize = 256;
	float range[] = { 0, 256 };
	const float* ranges = { range };
	Mat hist;

	int w = 512, h = 512;
	int binW = 0;

	vector<Mat> bgrChannel;

	Mat bHist;
	Mat gHist;
	Mat rHist;

	printf("vecOri size is %d\n", matNum);

	for (int i = 0; i < matNum; i++)
	{
		if (3 == vecOri[i].channels())
		{
			split(vecOri[i], bgrChannel);

			calcHist(&bgrChannel[0], 1, 0, Mat(), bHist, 1, &histSize, &ranges);
			calcHist(&bgrChannel[1], 1, 0, Mat(), gHist, 1, &histSize, &ranges);
			calcHist(&bgrChannel[2], 1, 0, Mat(), rHist, 1, &histSize, &ranges);

			/* Draw Histogram */
			int binW = cvRound((float)w / histSize);

			normalize(bHist, bHist, 0, h / 2, NORM_MINMAX, -1, Mat());
			normalize(gHist, gHist, 0, h / 2, NORM_MINMAX, -1, Mat());
			normalize(rHist, rHist, 0, h / 2, NORM_MINMAX, -1, Mat());

			Mat imHist(Size(h, w), CV_8UC3, Scalar(255, 255, 255));
			for (int i = 0; i < bHist.rows - 1; i++)
			{
				line(imHist, Point(i*binW, h - cvRound(bHist.at<float>(i))), \
					Point((i + 1)*binW, h - cvRound(bHist.at<float>(i + 1))), Scalar(255, 0, 0), 2, 8, 0);
				line(imHist, Point(i*binW, h - cvRound(gHist.at<float>(i))), \
					Point((i + 1)*binW, h - cvRound(gHist.at<float>(i + 1))), Scalar(0, 255, 0), 2, 8, 0);
				line(imHist, Point(i*binW, h - cvRound(rHist.at<float>(i))), \
					Point((i + 1)*binW, h - cvRound(rHist.at<float>(i + 1))), Scalar(0, 0, 255), 2, 8, 0);
			}

			imshow("Histogram_" + fileName + to_string(i), imHist);
		}
		else if (1 == vecOri[i].channels())
		{
			calcHist(&vecOri[i], 1, channel, Mat(), hist, 1, &histSize, &ranges);

			binW = cvRound((float)w / hist.rows);
			normalize(hist, hist, 0, h, NORM_MINMAX, -1, Mat());

			Mat imgHist(Size(w, h), CV_8UC1, Scalar(255));
			for (int row = 0; row < hist.rows - 1; row++)
			{
				rectangle(imgHist, Point(row*binW, h - cvRound(hist.at<float>(row))), \
					Point((row + 1)*binW, h), Scalar(0), -1);
			}

			imshow("Histogram_" + fileName + to_string(i), imgHist);
		}
		else
		{
			cout << "Cannot support this type picture!" << endl;
			continue;
		}
	}

	return true;
}


static bool equlizeHist(vector<Mat> vecOri, vector<Mat> &vecEqu)
{
	Mat temp;
	vector<Mat> vecHsv;

	for (int i = 0; i < (int)vecOri.size(); i++)
	{
		if (1 == vecOri[i].channels())
		{
			equalizeHist(vecOri[i], temp);
			vecEqu.push_back(temp);
		}
		else if (3 == vecOri[i].channels())
		{
			cout << "-----" << i << "-----" << endl;
			cvtColor(vecOri[i], temp, COLOR_BGR2HSV);
			split(temp, vecHsv);
			equalizeHist(vecHsv[2], vecHsv[2]);
			merge(vecHsv, temp);
			cvtColor(temp, temp, COLOR_HSV2BGR);
			vecEqu.push_back(temp);
		}
		else
		{
			cout << "equlizeHist: Cannot support this type picture!" << endl;
			continue;
		}
	}

	return true;
}


static bool imgShow(vector<Mat> vecOri, string fileName)
{
	for (int i = 0; i < (int)vecOri.size(); i++)
	{
		imshow("Img_" + fileName + to_string(i), vecOri[i]);
	}

	return true;
}

static bool hsvHistShow(vector<Mat> vecOri, string fileName)
{
	int num = (int)vecOri.size();
	Mat temp;
	vector<Mat> vecHsv;

	for (int i = 0; i < num; i++)
	{
		if (3 != vecOri[i].channels())
			continue;
		cvtColor(vecOri[i], temp, COLOR_BGR2HSV);
		vecHsv.push_back(temp);
	}

	float hRange[] = { 0, 180 };
	float sRange[] = { 0, 256 };
	const float* ranges[] = { hRange, sRange };
	int histSize[] = { 30, 32 };
	int ch[] = { 0, 1 };
	Mat hist;
	int scale = 10; //Can i modify this value???

	for (int i = 0; i < (int)vecHsv.size(); i++)
	{
		calcHist(&vecHsv[i], 1, ch, noArray(), hist, 2, histSize, ranges, true);
		normalize(hist, hist, 0, 255, NORM_MINMAX);
		Mat histImg(histSize[0] * scale, histSize[1] * scale, CV_8UC3);

		for (int h = 0; h < histSize[0]; h++)
		{
			for (int s = 0; s < histSize[1]; s++)
			{
				float hval = hist.at<float>(h, s);
				rectangle(histImg, Rect(h*scale, s*scale, scale, scale), Scalar((int)hval), -1);
			}
		}

		imshow("Histogram_" + fileName + to_string(i), histImg);
	}

	return 0;
}