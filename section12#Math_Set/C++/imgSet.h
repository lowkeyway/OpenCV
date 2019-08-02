#ifndef __IMGSET_H__
#define __IMGSET_H__

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class mathSet {
public:
	mathSet() {};
	~mathSet() {};

	bool complementSet(Mat imgOri, Mat &imgOut);
};


bool complementSet(Mat imgOri, Mat &imgOut)
{
	if (CV_8UC3 != imgOri.type())
	{
		cout << "Can not match this picture type!" << endl;
		return false;
	}
		
	imgOut = 255 - imgOri;

	return true;
}

#endif // !__IMGSET_H__
