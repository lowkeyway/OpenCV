#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

static bool drawLines(Mat &image, vector<Point2f> pt1, vector<Point2f> pt2);

vector<Scalar> colorVec;
RNG rng(12345);

int main(int *arg, int **argv)
{
	VideoCapture capture;

	Mat img, imgPre;
	Mat	imgGary, imgGaryPre;
	/*
	cornerPoints: corner point from shi-tomash, should copy to linePoint and initPoint
	initPoints: used only detect corner point 1st time
	linePoints[0]: corner point previous frame
	linePoints[1]: corner point current frame
	*/
	vector<Point2f> cornerPoints;
	vector<Point2f> initPoints;
	vector<Point2f> linePoints[2];
	int maxCorners = 500;
	double quality_level = 0.01;
	double minDistance = 0.04;
	int blockSize = 3;
	double k = 0.04;

	vector<uchar> status;
	vector<float> err;
	int flags = 0;

	capture.open("vtest.avi");
	capture.read(imgPre);
	cvtColor(imgPre, imgGaryPre, COLOR_BGR2GRAY);

	goodFeaturesToTrack(imgGaryPre, cornerPoints, maxCorners, quality_level, minDistance, Mat(), blockSize, false, k);
	initPoints.insert(initPoints.end(), cornerPoints.begin(), cornerPoints.end());
	linePoints[0].insert(linePoints[0].end(), cornerPoints.begin(), cornerPoints.end());

	TermCriteria criteria = TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 30, 0.01);

	int width = capture.get(CAP_PROP_FRAME_WIDTH);
	int height = capture.get(CAP_PROP_FRAME_HEIGHT);
	Mat KLTFrame = Mat::zeros(Size(width * 2, height), imgPre.type());
	Rect rio (0, 0, width, height);

	while (true)
	{
		if (!capture.read(img))
		{
			cout << "Video Finish!!!" << endl;
			break;
		}

		//imshow("img", img);
		rio.x = 0;
		img.copyTo(KLTFrame(rio));

		cvtColor(img, imgGary, COLOR_BGR2GRAY);
		calcOpticalFlowPyrLK(imgGaryPre, imgGary, linePoints[0], linePoints[1], status, err, Size(31, 31), 3, criteria, flags);
		size_t i, k;
		for (i = k = 0; i < linePoints[1].size(); i++)
		{
			double dist = abs(linePoints[0][i].x - linePoints[1][i].x) + abs(linePoints[0][i].y - linePoints[1][i].y);
			if (status[i] && dist > 2)
			{
				linePoints[0][k] = linePoints[0][i];
				linePoints[1][k] = linePoints[1][i];
				initPoints[k] = initPoints[i];
				circle(img, linePoints[1][k], 3, Scalar(0, 255, 0), -1, 8);
				k++;
			}
		}

		linePoints[0].resize(k);
		linePoints[1].resize(k);
		initPoints.resize(k);

		drawLines(img, initPoints, linePoints[1]);

		rio.x = width;
		img.copyTo(KLTFrame(rio));
		imshow("imgDst", KLTFrame);

		char c = waitKey(50);
		if (27 == c)
		{
			break;
		}

		std::swap(linePoints[0], linePoints[1]);
		cv::swap(imgGaryPre, imgGary);

		if (initPoints.size() < 40)
		{
			goodFeaturesToTrack(imgGaryPre, cornerPoints, maxCorners, quality_level, minDistance, Mat(), blockSize, false, k);
			initPoints.insert(initPoints.end(), cornerPoints.begin(), cornerPoints.end());
			linePoints[0].insert(linePoints[0].end(), cornerPoints.begin(), cornerPoints.end());
			cout << "total feature points : " << linePoints[0].size() << endl;
		}
	}

	return true;
}


static bool drawLines(Mat &image, vector<Point2f> pt1, vector<Point2f> pt2)
{
	if (colorVec.size() < pt1.size())
	{
		for (size_t col = colorVec.size(); col < pt1.size(); col++)
		{
			colorVec.push_back(Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)));
		}
	}

	for (size_t t = 0; t < pt1.size(); t++)
	{
		line(image, pt1[t], pt2[t], colorVec[t], 2, 8, 0);
	}

	return true;
}
