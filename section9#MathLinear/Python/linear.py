#!/usr/bin/env python
# -*- coding: utf-8 -*-
# author：lowkeyway time:7/16/2019

import sys
import numpy as np
import cv2 as cv

def showImgPara(img):
    print("Sizeof img is: " + str(img.size))
    print("rows x cols is: " + str(np.size(img, 0)) + " x " + str(np.size(img, 1)))
    print("dimension is: ", img.ndim)
    print("dtype is: ", img.dtype)
    print("itemsize is: ", img.itemsize)
    print("shape is(rows, cols, depth): ", img.shape)

def linear(picName, a, b):
    #####################   Original Picture    #####################
    imgOri = cv.imread(picName)
    cv.namedWindow("imgOri")
    cv.imshow("imgOri", imgOri)
    showImgPara(imgOri)

    #####################   Output Picture    #####################
    imgOut = np.zeros(imgOri.shape, imgOri.dtype)

    if(a >= 0):
        imgOut = a * imgOri + b
    else:
        imgOut = 255 - (-a)*imgOri + b

    cv.namedWindow("imgOut")
    cv.imshow("imgOut", imgOut)


def main_func(argv):
    linear("left06.jpg", -1, 0)
    cv.waitKey(0)

if __name__ == '__main__':
    main_func(sys.argv)