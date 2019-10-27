#!/usr/bin/env python
# -*- coding: utf-8 -*-
# author：lowkeyway time:9/19/2019

import sys
import cv2 as cv
import numpy as np
import matplotlib.pyplot as plt

def dilateErodeTest(imgOri):
    if 3 == len(imgOri.shape):
        imgOri = cv.cvtColor(imgOri, cv.COLOR_BGR2GRAY)

    cv.imshow("imgOri", imgOri)

    th, imgBin = cv.threshold(imgOri, 0, 255, cv.THRESH_BINARY | cv.THRESH_OTSU)
    cv.imshow("imgBin", imgBin)

    kernel = np.ones((3, 3), dtype=np.uint8)
    imgDilate = cv.dilate(imgBin, kernel)
    cv.imshow("imgDilate", imgDilate)

    imgErode = cv.erode(imgBin, kernel)
    cv.imshow("imgErode", imgErode)

    plt.figure("DilateErode")
    plt.subplot(221); plt.imshow(imgOri, "gray"); plt.title("imgOri")
    plt.subplot(222); plt.imshow(imgBin, "gray"); plt.title("imgBin")
    plt.subplot(223); plt.imshow(imgDilate, "gray"); plt.title("imgDilate")
    plt.subplot(224); plt.imshow(imgErode, "gray"); plt.title("imgErode")

    plt.show()


def main_func(argv):
    imgOri = cv.imread("fingerprint.png")
    if imgOri is None:
        print("Cannot load this picture!")
        return

    dilateErodeTest(imgOri)

    cv.waitKey(0)

if __name__ == '__main__':
    main_func(sys.argv)