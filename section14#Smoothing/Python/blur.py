#!/usr/bin/env python
# -*- coding: utf-8 -*-
# author：lowkeyway time:8/1/2019

import sys
import cv2 as cv


def main_func(argv):
    imgOri = cv.imread("Fig0235(c)(kidney_original).tif")
    cv.namedWindow("imgOri")
    cv.imshow("imgOri", imgOri)


    imgOut = cv.blur(imgOri, (20, 20))
    cv.namedWindow("imgOut")
    cv.imshow("imgOut", imgOut)

    cv.waitKey(0)
    cv.destroyAllWindows()


if __name__ == '__main__':
    main_func(sys.argv)