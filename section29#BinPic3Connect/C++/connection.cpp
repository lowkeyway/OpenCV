#if 1
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <fstream>

using namespace std;
using namespace cv;

RNG rng(12345);
static bool connectionTest(Mat imgOri);
static void showImgPara(Mat &img);
static bool pic2digit(Mat &img, string picName, string fileName);
static bool tran2singleChannel(Mat &src, Mat &tar);

int main(int argc, char** argv)
{
	Mat imgOri = imread("red10.jfif");
	if (imgOri.empty())
	{
		cout << "Cannot load picture!" << endl;
		return false;
	}

	connectionTest(imgOri);

	waitKey(0);

	return true;
}

static bool connectionTest(Mat imgOri)
{
	int labelsNum;

	imshow("imgOri", imgOri);

	//pyrMeanShiftFiltering(imgOri, imgOri, 10, 100);

	if (1 != imgOri.channels())
	{
		cvtColor(imgOri, imgOri, COLOR_BGR2GRAY);
	}
	imshow("imgGary", imgOri);
	adaptiveThreshold(imgOri, imgOri, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 111, 10);
	//threshold(imgOri, imgOri, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
	imshow("imgBin", imgOri);

	Mat labels = Mat::zeros(imgOri.size(), CV_32S);
	Mat stats, centroids;
	labelsNum = connectedComponentsWithStats(imgOri, labels, stats, centroids);
	cout << "labelsNum: " << labelsNum << endl;
	pic2digit(labels, "labels", "labels.txt");
	cout << centroids << endl;

	vector<Vec3b> colors(labelsNum);

	// background color
	colors[0] = Vec3b(0, 0, 0);

	// object color
	for (int i = 1; i < labelsNum; i++) {
		colors[i] = Vec3b(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
	}

	// render result
	Mat dst = Mat::zeros(imgOri.size(), CV_8UC3);
	imshow("dst", dst);

	int w = imgOri.cols;
	int h = imgOri.rows;
	for (int row = 0; row < h; row++) {
		for (int col = 0; col < w; col++) {
			int label = labels.at<int>(row, col);
			if (label == 0) continue;
			dst.at<Vec3b>(row, col) = colors[label];
		}
	}
	for (int i = 1; i < labelsNum; i++) {
		// Center
		int cx = centroids.at<double>(i, 0);
		int cy = centroids.at<double>(i, 1);

		int x = stats.at<int>(i, CC_STAT_LEFT);
		int y = stats.at<int>(i, CC_STAT_TOP);
		int w = stats.at<int>(i, CC_STAT_WIDTH);
		int h = stats.at<int>(i, CC_STAT_HEIGHT);
		int area = stats.at<int>(i, CC_STAT_AREA);

		// Center Draw circle
		circle(dst, Point(cx, cy), 2, Scalar(0, 255, 0), 2, 8, 0);
		// Rect
		Rect rect(x, y, w, h);
		rectangle(dst, rect, colors[i], 1, 8, 0);
		//putText(dst, format("num:%d", i), Point(x, y), FONT_HERSHEY_SIMPLEX, .5, Scalar(0, 0, 255), 1);
		printf(" num: %d,  rice area %d\n", i, area);
	}

	imshow("ccla-demo", dst);

	return true;
}

static bool tran2singleChannel(Mat &src, Mat &tar)
{
	vector<Mat> channels;

	split(src, channels);
	tar = channels.at(0);
	convertScaleAbs(tar, tar);

	return true;
}

static bool pic2digit(Mat &img, string picName, string fileName)
{
	uchar*		pTemp = NULL;
	Mat			signalChannel;
	ofstream	output;

	if (img.empty())
	{
		cout << "Load picture fail!" << endl;
		return false;
	}

	output.open(fileName, ios::in);
	if (output)
	{
		cout << "File Exist!" << endl;
		return false;
	}
	else
	{
		output.open(fileName);
	}

	cout << endl << "/******************pic2digit******************/" << endl;
	//showImgPara(img);

	//namedWindow(picName);
	//imshow(picName, img);


	tran2singleChannel(img, signalChannel);

	//namedWindow(picName + "_signalChannel");
	//imshow(picName + "_signalChannel", signalChannel);

	showImgPara(signalChannel);


	for (int i = 0; i < signalChannel.rows; i++)
	{
		for (int j = 0; j < signalChannel.cols; j++)
		{
			for (int k = 0; k < signalChannel.step[1]; k++)
			{
				pTemp = signalChannel.data + signalChannel.step[0] * i + signalChannel.step[1] * j + k;
				output << setw(2) << setfill('0') << (int)*pTemp << " ";
			}
		}
		output << endl;
	}

	output.close();

	imwrite(picName + "_signalChannel.jpg", signalChannel);

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
}
#endif
