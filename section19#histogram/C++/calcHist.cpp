#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define ORI_IMG	"original image"
#define HIS_IMG "histogram image"

static void showImgPara(Mat &img);
static bool histogramTest(Mat &img, string winName);

int main()
{
	Mat imgOri1 = imread("Fig0316(1)(top_left).tif", 0);
	Mat imgOri2 = imread("Fig0316(2)(2nd_from_top).tif", 0);
	Mat imgOri3 = imread("Fig0316(3)(third_from_top).tif", 0);
	Mat imgOri4 = imread("Fig0316(4)(bottom_left).tif", 0);
	Mat imgOri5 = imread("lowkeyway.jpg");
	//namedWindow(ORI_IMG);
	//imshow(ORI_IMG, imgOri1);

	showImgPara(imgOri5);

	histogramTest(imgOri1, "Fig0316(4)(bottom_left)");
	histogramTest(imgOri2, "Fig0316(2)(2nd_from_top)");
	histogramTest(imgOri3, "Fig0316(3)(third_from_top)");
	histogramTest(imgOri4, "Fig0316(4)(bottom_left)");
	histogramTest(imgOri5, "lowkeyway");

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
	cout << "---------    showImgPara End    ----------" << endl;
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
				Point((i + 1)*binW, histH - cvRound(bHist.at<float>(i+1))), Scalar(255, 0, 0), 2, 8, 0);
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