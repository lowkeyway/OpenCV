#!/usr/bin/env python
# -*- coding: utf-8 -*-
# author：lowkeyway time:8/15/2019

import sys
import cv2 as cv
import matplotlib.pyplot as plt
import numpy as np

def histCover(img, fileName):
    plt.figure(fileName, figsize=(16, 8))

    dims = len(img.shape)
    if 3 == dims:
        print("This is a BGR picture")
        color = ["r", "g", "b"]

        # Show original picture
        img = img[:,:, [2, 1, 0]]
        plt.subplot(121)
        plt.imshow(img)

        plt.subplot(122)
        # Show histogram
        for index, c in enumerate(color):
            hist = cv.calcHist([img], [index], None, [256], [0, 255])
            plt.plot(hist, color=c)
            plt.xlim([0, 255])

    elif 2 == dims:
        print("This is a gray picture")

        # Show original picture
        plt.subplot(121)
        plt.imshow(img, "gray")

        # Show histogram
        plt.subplot(122)
        hist = cv.calcHist([img], [0], None, [256], [0, 255])
        plt.plot(hist)
        plt.xlim([0, 255])

    else:
        print("Cannot support this type picture")

    plt.show()

def main_func(argv):
    imgOri1 = cv.imread("Fig0316(1)(top_left).tif", 0)
    imgOri2 = cv.imread("Fig0316(2)(2nd_from_top).tif", 0)
    imgOri3 = cv.imread("Fig0316(3)(third_from_top).tif", 0)
    imgOri4 = cv.imread("Fig0316(4)(bottom_left).tif", 0)
    imgOri5 = cv.imread("lowkeyway.jpg")

    histCover(imgOri1, "Fig0316(1)(top_left).tif")
    histCover(imgOri2, "Fig0316(2)(2nd_from_top).tif")
    histCover(imgOri3, "Fig0316(3)(third_from_top).tif")
    histCover(imgOri4, "Fig0316(4)(bottom_left).tif")
    histCover(imgOri5, "lowkeyway.jpg")


if __name__ == '__main__':
    main_func(sys.argv)