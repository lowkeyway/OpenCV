#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define VEDIO_WIN_NAME	"VedioOri"
#define VEDIO_WIN_NAME1	"VedioResize"
#define VEDIO_WIN_NAME2	"VedioOut"
#define DELAY_TIME		30


static bool waitEscKey(int delayT);

int main()
{
	double fx = 0.0, fy = 0.0;
	VideoCapture capture(0);
	//VideoCapture capture("Hospital.mp4");

	if (!capture.isOpened()) 
	{
		printf("could not read this video file...\n");
		return false;
	}

	Size size = Size((int)capture.get(CAP_PROP_FRAME_WIDTH),\
		(int)capture.get(CAP_PROP_FRAME_HEIGHT));
	double fps = capture.get(CAP_PROP_FPS);
	int fourcc = static_cast<int>(capture.get(CAP_PROP_FOURCC));

	//VideoWriter writer("outPut.mp4", VideoWriter::fourcc('D', 'I', 'V', 'X'), fps, Size(size.x * 2.0, size.y * 2.0), true);
	VideoWriter writer("outPut.mp4", fourcc, fps, Size(size.height * 2.0, size.width * 2.0), true);

	Mat frameOri, frameResize, frameOut;
	//namedWindow(VEDIO_WIN_NAME);
	namedWindow(VEDIO_WIN_NAME1);
	namedWindow(VEDIO_WIN_NAME2);
	while (capture.read(frameOri))
	{
		resize(frameOri, frameResize, Size(frameOri.cols * 2.0, frameOri.rows * 2.0), fx = 0, fy = 0, INTER_LINEAR);
		transpose(frameResize, frameResize);
		flip(frameResize, frameResize, 1);

		medianBlur(frameResize, frameOut, 11);
		//imshow(VEDIO_WIN_NAME, frameOri);
		imshow(VEDIO_WIN_NAME1, frameResize);
		imshow(VEDIO_WIN_NAME2, frameOut);
		writer.write(frameOut);

		if (true == waitEscKey(DELAY_TIME))
		{
			break;
		}
	}

	capture.release();
	writer.release();

	return true;
}

static bool waitEscKey(int delayT)
{
	char key = waitKey(delayT);
	//cout << "----" << (int)key << "----" << endl;
	if (key == 27)
	{
		return true;
	}
	if (32 == key)
	{
		while (true)
		{
			char keyRep = waitKey(0);
			if (32 == keyRep)
			{
				break;
			}

			if (27 == keyRep)
			{
				return true;
			}
		}
	}

	return false;
}