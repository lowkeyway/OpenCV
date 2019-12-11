#if 0
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;
void draw_goodFeatures(Mat &image, vector<Point2f> goodFeatures);
void draw_lines(Mat &image, vector<Point2f> pt1, vector<Point2f> pt2);
vector<Point2f> featurePoints;
vector<Scalar> color_lut;
RNG rng(5000);
int main(int argc, char** argv) {
	VideoCapture capture;
	capture.open("vtest.avi");

	double qualityLevel = 0.01;
	int minDistance = 10;
	int blockSize = 3;
	bool useHarrisDetector = false;
	double k = 0.04;
	int maxCorners = 5000;
	Mat frame, gray;
	vector<Point2f> pts[2];
	vector<Point2f> initPoints;
	vector<uchar> status;
	vector<float> err;
	TermCriteria criteria = TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 30, 0.01);
	double derivlambda = 0.5;
	int flags = 0;

	// detect first frame and find corners in it
	Mat old_frame, old_gray;
	capture.read(old_frame);
	cvtColor(old_frame, old_gray, COLOR_BGR2GRAY);
	goodFeaturesToTrack(old_gray, featurePoints, maxCorners, qualityLevel, minDistance, Mat(), blockSize, useHarrisDetector, k);
	initPoints.insert(initPoints.end(), featurePoints.begin(), featurePoints.end());
	pts[0].insert(pts[0].end(), featurePoints.begin(), featurePoints.end());
	int width = capture.get(CAP_PROP_FRAME_WIDTH);
	int height = capture.get(CAP_PROP_FRAME_HEIGHT);
	VideoWriter writer("D:/test.mp4", VideoWriter::fourcc('D', 'I', 'V', 'X'), 10, Size(width * 2, height), true);
	Mat result = Mat::zeros(Size(width * 2, height), CV_8UC3);
	Rect roi(0, 0, width, height);
	while (true) {
		bool ret = capture.read(frame);
		if (!ret) break;
		imshow("frame", frame);
		roi.x = 0;
		frame.copyTo(result(roi));
		cvtColor(frame, gray, COLOR_BGR2GRAY);

		// calculate optical flow
		//calcOpticalFlowPyrLK(old_gray, gray, pts[0], pts[1], status, err, Size(31, 31), 3, criteria, flags);
		calcOpticalFlowPyrLK(old_gray, gray, pts[0], pts[1], status, err, Size(31, 31), 3, criteria, derivlambda, flags);
		size_t i, k;
		for (i = k = 0; i < pts[1].size(); i++)
		{
			
			double dist = abs(pts[0][i].x - pts[1][i].x) + abs(pts[0][i].y - pts[1][i].y);
			if (status[i] && dist > 2) {
				pts[0][k] = pts[0][i];
				initPoints[k] = initPoints[i];
				pts[1][k++] = pts[1][i];
				circle(frame, pts[1][i], 3, Scalar(0, 255, 0), -1, 8);
			}
		}
		
		pts[1].resize(k);
		pts[0].resize(k);
		initPoints.resize(k);
		
		draw_lines(frame, initPoints, pts[1]);
		imshow("result", frame);
		roi.x = width;
		frame.copyTo(result(roi));
		char c = waitKey(50);
		if (c == 27) {
			break;
		}

		// update old
		std::swap(pts[1], pts[0]);
		cv::swap(old_gray, gray);

		// need to re-init
		if (initPoints.size() < 40) {
			goodFeaturesToTrack(old_gray, featurePoints, maxCorners, qualityLevel, minDistance, Mat(), blockSize, useHarrisDetector, k);
			initPoints.insert(initPoints.end(), featurePoints.begin(), featurePoints.end());
			pts[0].insert(pts[0].end(), featurePoints.begin(), featurePoints.end());
			printf("total feature points : %d\n", pts[0].size());
		}
		writer.write(result);
	}
	return 0;
}

void draw_goodFeatures(Mat &image, vector<Point2f> goodFeatures) {
	for (size_t t = 0; t < goodFeatures.size(); t++) {
		circle(image, goodFeatures[t], 2, Scalar(0, 255, 0), 2, 8, 0);
	}
}

void draw_lines(Mat &image, vector<Point2f> pt1, vector<Point2f> pt2) {
	if (color_lut.size() < pt1.size()) {
		for (size_t t = 0; t < pt1.size(); t++) {
			color_lut.push_back(Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)));
		}
	}
	for (size_t t = 0; t < pt1.size(); t++) {
		line(image, pt1[t], pt2[t], color_lut[t], 2, 8, 0);
	}
}
#endif