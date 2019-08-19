#!/usr/bin/env python
# -*- coding: utf-8 -*-
# author：lowkeyway time:8/16/2019

import sys
import cv2 as cv
import matplotlib.pyplot as plt
import logging

def equCvt(imgList):
    equList = []
    picNum = len(imgList)
    for index, img in enumerate(imgList, start=1):
        dim = len(img.shape)
        if 2 == dim:
            img = cv.equalizeHist(img)
            equList.append(img)
        elif 3 == dim:
            img = cv.cvtColor(img, cv.COLOR_BGR2HSV)
            HSV = cv.split(img)
            HSV[2] = cv.equalizeHist(HSV[2])
            img = cv.merge(HSV)
            img = cv.cvtColor(img, cv.COLOR_HSV2BGR)
            equList.append(img)

        else:
            print("equShow: cannot support thsi type")
    return  equList

def histShow(imgList, fileName):
    picNum = len(imgList)
    histSize = 256
    ranges = [0, 256]
    plt.figure("Histograme " + str(fileName))
    for index, img in enumerate(imgList, start=1):
        dim = len(img.shape)
        if 2 == dim:
            hist = cv.calcHist([img], [0], None, [histSize], ranges)

            plt.subplot(picNum, 2, index*2-1)
            plt.imshow(img, "gray")

            plt.subplot(picNum, 2, index*2)
            plt.plot(hist)
            plt.xlim([0, 255])
        elif 3 == dim:
            color = ["r", "g", "b"]
            img = cv.cvtColor(img, cv.COLOR_BGR2RGB)

            plt.subplot(picNum, 2, index*2-1)
            plt.imshow(img)

            plt.subplot(picNum, 2, index*2)
            for i, c in enumerate(color):
                hist = cv.calcHist([img], [i], None, [histSize], ranges)
                plt.plot(hist, color=c)
                plt.xlim([0, 256])
        else:
            print("Cannot support this type now")


def imgShow(imgList, fileName):
    for index, img in enumerate(imgList):
        cv.imshow("Image" + fileName + str(index), img)
    cv.waitKey(0)

def main_func(argv):
    imgList = []
    equList = []

    imOri1 = cv.imread("Fig0316(1)(top_left).tif", 0)
    imOri2 = cv.imread("Fig0316(2)(2nd_from_top).tif", 0)
    imOri3 = cv.imread("Fig0316(3)(third_from_top).tif", 0)
    imOri4 = cv.imread("Fig0316(4)(bottom_left).tif", 0)
    imOri5 = cv.imread("lowkeyway.jpg")

    imgList.append(imOri1)
    imgList.append(imOri2)
    imgList.append(imOri3)
    imgList.append(imOri4)
    imgList.append(imOri5)

    # imgShow(imgList, "Ori")
    histShow(imgList, "Ori")
    equList = equCvt(imgList)
    histShow(equList, "Equ")

    plt.show()

if __name__ == '__main__':
    main_func(sys.argv)