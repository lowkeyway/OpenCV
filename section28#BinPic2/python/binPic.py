#!/usr/bin/env python
# -*- coding: utf-8 -*-
# author：lowkeyway time:9/8/2019

import sys
import cv2 as cv
import numpy as np
import matplotlib.pyplot as plt

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

def OTSUTest(imgOri):
    if 2 != len(imgOri.shape):
        imgOri = cv.cvtColor(imgOri, cv.COLOR_BGR2GRAY)
        print("Cover to Gray")

    histCover(imgOri, "Hist_OTSU_Ori")
    th, imgTemp = cv.threshold(imgOri, 0, 255, cv.THRESH_BINARY | cv.THRESH_OTSU)
    histCover(imgTemp, "Hist_OTSU_Out")

def TRIANGLETest(imgOri):
    if 2 != len(imgOri.shape):
        imgOri = cv.cvtColor(imgOri, cv.COLOR_BGR2GRAY)
        print("Cover to Gray")

    histCover(imgOri, "Hist_TRIANGLET_Ori")
    th, imgTemp = cv.threshold(imgOri, 0, 255, cv.THRESH_BINARY | cv.THRESH_OTSU)
    histCover(imgTemp, "Hist_TRIANGLET_Out")

def threasholdTest(imgOri):
    if 2 != len(imgOri.shape):
        imgOri = cv.cvtColor(imgOri, cv.COLOR_BGR2GRAY)
        print("Cover to Gray")

    cv.imshow("imgOri", imgOri)
    # imgTemp = np.copy(imgOri)
    binMode = [cv.THRESH_BINARY, cv.THRESH_BINARY_INV, cv.THRESH_TRUNC, cv.THRESH_TOZERO, cv.THRESH_TOZERO_INV]
    for mode in binMode:
        th, imgTemp = cv.threshold(imgOri, 127, 255, mode)
        cv.imshow("threashold_"+str(mode), imgTemp)


def main_func(argv):
    imgOri = cv.imread("Gril.jpg")
    # threasholdTest(imgOri)
    # OTSUTest(imgOri)
    TRIANGLETest(imgOri)

    cv.waitKey(0)

if __name__ == '__main__':
    main_func(sys.argv)