#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

static bool threasholdTest(Mat imgOri);
static bool OTSUTest(Mat imgOri);
static bool TRIANGLETest(Mat imgOri);
static bool histogramTest(Mat &img, string winName);

int main()
{
	Mat imgOri = imread("apple.jpg");
	//threasholdTest(imgOri);
	OTSUTest(imgOri);
	//TRIANGLETest(imgOri);

	waitKey(0);
	return true;
}

static bool threasholdTest(Mat imgOri)
{
	Mat imgTemp;
	if (imgOri.empty())
	{
		cout << "Cannot load picture!" << endl;
		return false;
	}

	if (1 != imgOri.channels())
	{
		cvtColor(imgOri, imgOri, COLOR_BGR2GRAY);
	}

	for (int i = 0; i < 5; i++)
	{
		threshold(imgOri, imgTemp, 127, 255, i);
		imshow("imgBin_" + to_string(i), imgTemp);
	}

	return true;
}

static bool histogramTest(Mat &img, string winName)
{
	Mat hist;
	const int histSize = 256;
	float range[] = { 0, 255 };
	const float* ranges = { range };
	int dims = img.channels();
	if (3 == dims)
	{
		vector<Mat> bgrChannel;
		split(img, bgrChannel);

		Mat bHist;
		Mat gHist;
		Mat rHist;

		calcHist(&bgrChannel[0], 1, 0, Mat(), bHist, 1, &histSize, &ranges);
		calcHist(&bgrChannel[1], 1, 0, Mat(), gHist, 1, &histSize, &ranges);
		calcHist(&bgrChannel[2], 1, 0, Mat(), rHist, 1, &histSize, &ranges);

		/* Draw Histogram */
		int histW = 512;
		int histH = 512;
		int binW = cvRound((float)histW / histSize);

		normalize(bHist, bHist, 0, histH / 2, NORM_MINMAX, -1, Mat());
		normalize(gHist, gHist, 0, histH / 2, NORM_MINMAX, -1, Mat());
		normalize(rHist, rHist, 0, histH / 2, NORM_MINMAX, -1, Mat());

		Mat imHist(Size(histH, histW), CV_8UC3, Scalar(255, 255, 255));
		for (int i = 0; i < bHist.rows - 1; i++)
		{
			line(imHist, Point(i*binW, histH - cvRound(bHist.at<float>(i))), \
				Point((i + 1)*binW, histH - cvRound(bHist.at<float>(i + 1))), Scalar(255, 0, 0), 2, 8, 0);
			line(imHist, Point(i*binW, histH - cvRound(gHist.at<float>(i))), \
				Point((i + 1)*binW, histH - cvRound(gHist.at<float>(i + 1))), Scalar(0, 255, 0), 2, 8, 0);
			line(imHist, Point(i*binW, histH - cvRound(rHist.at<float>(i))), \
				Point((i + 1)*binW, histH - cvRound(rHist.at<float>(i + 1))), Scalar(0, 0, 255), 2, 8, 0);
		}


		namedWindow(winName);
		imshow(winName, imHist);
	}
	else if (1 == dims)
	{
		calcHist(&img, 1, 0, Mat(), hist, 1, &histSize, &ranges);

		/* Draw histogram */
		int histH = 512;
		int histW = 512;
		int binW = cvRound((float)histW / histSize);
		normalize(hist, hist, 0, hist.rows * 2 - 10, NORM_MINMAX, -1, Mat());

		Mat imHist = Mat::ones(Size(histH, histW), CV_8UC1) * 255;
		for (int i = 0; i < hist.rows; i++)
		{
			rectangle(imHist, Point(i*binW, histH), Point((i + 1)*binW, histH - cvRound(hist.at<float>(i))), Scalar(0), -1);
		}

		namedWindow(winName);
		imshow(winName, imHist);
	}
	else
	{
		cout << "Not Support This Type Picture!" << endl;
	}


	return true;
}

static bool OTSUTest(Mat imgOri)
{
	Mat imgOTSU;
	if (imgOri.empty())
	{
		cout << "Cannot load picture!" << endl;
		return false;
	}

	if (1 != imgOri.channels())
	{
		cvtColor(imgOri, imgOri, COLOR_BGR2GRAY);
	}

	imshow("imgOri", imgOri);
	histogramTest(imgOri, "OTSU_HIST");
	double th = threshold(imgOri, imgOTSU, 0, 255, THRESH_BINARY | THRESH_OTSU);
	putText(imgOTSU, "Threshold is " + to_string(th), Point(0, 200), FONT_HERSHEY_PLAIN, 2.0, (0, 0, 0), 1);
	imshow("imgOTSU", imgOTSU);
	cout << "Threshold is: " << th << endl;

	return true;
}
static bool TRIANGLETest(Mat imgOri)
{
	Mat imgTRIANGLE;
	if (imgOri.empty())
	{
		cout << "Cannot load picture!" << endl;
		return false;
	}

	if (1 != imgOri.channels())
	{
		cvtColor(imgOri, imgOri, COLOR_BGR2GRAY);
	}

	imshow("imgOri", imgOri);
	histogramTest(imgOri, "TRIANGLE_HIST");
	double th = threshold(imgOri, imgTRIANGLE, 0, 255, THRESH_BINARY | THRESH_TRIANGLE);
	putText(imgTRIANGLE, "Threshold is " + to_string(th), Point(0, 200), FONT_HERSHEY_PLAIN, 2.0, (0, 0, 0), 1);
	imshow("imgOTSU", imgTRIANGLE);
	cout << "Threshold is: " << th << endl;

	return true;
}