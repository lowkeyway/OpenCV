#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

static void drawOptFlowMap(const Mat& flow, Mat& cflowmap, int step,
	double, const Scalar& color)
{
	for (int y = 0; y < cflowmap.rows; y += step)
		for (int x = 0; x < cflowmap.cols; x += step)
		{
			const Point2f& fxy = flow.at<Point2f>(y, x);
			line(cflowmap, Point(x, y), Point(cvRound(x + fxy.x), cvRound(y + fxy.y)),
				color);
			circle(cflowmap, Point(x, y), 2, color, -1);
		}
}

int main(int argc, char** argv)
{
	//VideoCapture cap(0);
	VideoCapture cap;
	cap.open("vtest.avi");
	if (!cap.isOpened())
		return -1;

	Mat flow, cflow, frame, preframe;
	//Mat gray, prevgray, uflow;
	UMat gray, prevgray, uflow;
	namedWindow("flow", 1);

	for (;;)
	{
		//cap >> frame;
		bool ret = cap.read(frame);
		cvtColor(frame, gray, COLOR_BGR2GRAY);

		if (!prevgray.empty())
		{
			calcOpticalFlowFarneback(prevgray, gray, uflow, 0.5, 3, 15, 3, 5, 1.2, 0);
			//cvtColor(prevgray, cflow, COLOR_GRAY2BGR);
			uflow.copyTo(flow);
			drawOptFlowMap(flow, preframe, 16, 1.5, Scalar(0, 255, 0));
			imshow("flow", preframe);
		}
		if (waitKey(30) >= 0)
			break;
		std::swap(prevgray, gray);
		std::swap(preframe, frame);
	}
	return 0;
}
