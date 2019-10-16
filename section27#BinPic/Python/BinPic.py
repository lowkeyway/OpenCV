#!/usr/bin/env python
# -*- coding: utf-8 -*-
# author：lowkeyway time:9/4/2019

import sys
import cv2 as cv
import numpy as np

PIC_SIZE = 256*2
UCHAR_MAX = 256


def myBinPicTest(th):
    imgOri = np.zeros([PIC_SIZE, PIC_SIZE], dtype=np.uint8)

    # copy & array
    img1 = np.copy(imgOri)
    for row in range(img1.shape[0]):
        for col in range(img1.shape[1]):
            img1[row][col] = col / 2


    # Slice and zeros(shape, dtype)
    img2 = np.zeros(imgOri.shape, imgOri.dtype)
    for col in range(img2.shape[1]):
        img2[:, col] = col % UCHAR_MAX

    cv.imshow("imgOri", imgOri)
    cv.imshow("img1", img1)
    cv.imshow("img2", img2)

    # Img1 throshold
    img1[img1>th] = 255
    img1[img1<=th] = 0

    # Img2 throshold
    max = np.where(img2 > th)
    img2[max] = 255
    min = np.where(img2 <= th)
    img2[min] = 0

    cv.imshow("img1_Out", img1)
    cv.imshow("img2_Out", img2)
    cv.waitKey(0)

def main_func(argv):
    myBinPicTest(127)


if __name__ == '__main__':
    main_func(sys.argv)