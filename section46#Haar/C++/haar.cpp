#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;
CascadeClassifier faceDetector;
String haar_data_file = "haarcascade_frontalface_alt2.xml";
int main(int artc, char** argv) {
	Mat frame, gray;
	vector<Rect> faces;
	VideoCapture capture(0);
	faceDetector.load(haar_data_file);
	namedWindow("HaarDetect", WINDOW_AUTOSIZE);
	while (true) {
		capture.read(frame);
		flip(frame, frame, 1);
		cvtColor(frame, gray, COLOR_BGR2GRAY);
		equalizeHist(gray, gray);
		faceDetector.detectMultiScale(gray, faces, 1.2, 1, 0, Size(100, 100), Size(400, 400));
		for (size_t t = 0; t < faces.size(); t++) {
			rectangle(frame, faces[t], Scalar(0, 0, 255), 2, 8, 0);
		}
		char c = waitKey(10);
		if (c == 27) {
			return 0;
		}
		imshow("HaarDetect", frame);
	}

	waitKey(0);
	return 0;
}
