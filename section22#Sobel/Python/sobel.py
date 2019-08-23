#!/usr/bin/env python
# -*- coding: utf-8 -*-
# author：lowkeyway time:8/21/2019

import sys
import cv2 as cv
import numpy as np

def sobelTest(imgOri):
    cv.imshow("Sobel imgOri", imgOri)
    # imgOri = cv.cvtColor(imgOri, cv.COLOR_BGR2GRAY)
    imgSobel = cv.Sobel(imgOri, cv.CV_16S, 1, 1)
    imgSobel = cv.convertScaleAbs(imgSobel)

    cv.imshow("Sobel imgSobel", imgSobel)


def scharrTest(imgOri):
    imgOri = cv.resize(imgOri, (int(imgOri.shape[1]*0.5), int(imgOri.shape[0]*0.5)))
    cv.imshow("Scharr Imgori", imgOri)

    if 3 == len(imgOri.shape):
        imgOri = cv.cvtColor(imgOri, cv.COLOR_BGR2GRAY)

    imgScharrX = cv.Scharr(imgOri, cv.CV_32F, 1, 0)
    imgScharrY = cv.Scharr(imgOri, cv.CV_32F, 0, 1)

    imgScharrX = cv.convertScaleAbs(imgScharrX)
    imgScharrY = cv.convertScaleAbs(imgScharrY)

    cv.imshow("Scharr imgScharrX", imgScharrX)
    cv.imshow("Scharr imgScharrY", imgScharrY)

    imgScharr = cv.addWeighted(imgScharrX, 0.5, imgScharrY, 0.5, 1, dtype=cv.CV_32F)
    imgScharr = cv.convertScaleAbs(imgScharr)
    cv.imshow("Scharr imgScharr", imgScharr)


def main_func(argv):
    imgOri = cv.imread("Fig0342(a)(contact_lens_original).tif")
    # sobelTest(imgOri)
    
    imgGril = cv.imread("Gril.jpg")
    scharrTest(imgGril)

    cv.waitKey(0)

if __name__ == '__main__':
    main_func(sys.argv)