#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

#define PIC_SIZE	256*2
#define UCHAR_MAX	256

using namespace std;
using namespace cv;

class BinPic
{
public:
	bool myBinPicTest(uchar th);
public:
	vector<Mat> m_vMyPic;
};

bool BinPic::myBinPicTest(uchar th)
{
	int row, col, count;
	Mat imgOri = Mat::zeros(Size(PIC_SIZE, PIC_SIZE), CV_8UC1);

	// Clone imgOri, use pointer to vist data.
	Mat img1 = imgOri.clone();
	for (row = 0; row < img1.rows; row++)
	{
		uchar* p = img1.ptr<uchar>(row);
		for (col = 0; col < img1.cols; col++)
		{
			p[col] = saturate_cast<uchar>(col / 2);
		}
	}
	m_vMyPic.push_back(img1);

	// copyTo imgOri, use at dynamic pointer to vist data.
	Mat img2;
	imgOri.copyTo(img2);
	for (row = 0; row < img2.rows; row++)
	{
		for (col = 0; col < img2.cols; col++)
		{
			img2.at<uchar>(row, col) = saturate_cast<uchar>(col % UCHAR_MAX);
		}
	}
	m_vMyPic.push_back(img2);

	imshow("imgOri", imgOri);
	imshow("img1", img1);
	imshow("img2", img2);

	// Threadhold, use vector to vist data;
	for (vector<Mat>::iterator it = m_vMyPic.begin(); it != m_vMyPic.end(); it++)
	{
		for (Mat_<uchar>::iterator pix = it->begin<uchar>(); pix != it->end<uchar>(); pix++)
		{
			*pix = *pix > th ? UCHAR_MAX - 1 : 0;
		}
	}

	imshow("imgOri", imgOri);
	imshow("img1_out", img1);
	imshow("img2_out", img2);

	waitKey(0);
	return true;
}


int main()
{
	BinPic B;
	B.myBinPicTest(150);

	return true;
}