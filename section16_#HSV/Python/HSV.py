#!/usr/bin/env python
# -*- coding: utf-8 -*-
# author：lowkeyway time:8/8/2019

import sys
import cv2 as cv
import numpy as np

hPos = 100
sPos = 100
vPos = 100
HSV_MAX = 300

def showImgPara(img):
    print("Sizeof img is: " + str(img.size))
    print("rows x cols is: " + str(np.size(img, 0)) + " x " + str(np.size(img, 1)))
    print("dimension is: ", img.ndim)
    print("dtype is: ", img.dtype)
    print("itemsize is: ", img.itemsize)
    print("shape is(rows, cols, depth): ", img.shape)
    print("\n")

def splitRGBTest(imgOri):
    cv.imshow("imgOri", imgOri)
    B,G,R = cv.split(imgOri)

    cv.imshow("B", B)
    cv.imshow("G", G)
    cv.imshow("R", R)

def hCallBack(pos):
    hChannel = np.copy(HSV_MV[0])
    print("---------Begain", pos, "---------")
    # showImgPara(hChannel)
    hChannel = hChannel * (pos / 100.0)
    # showImgPara(hChannel)
    hChannel = hChannel.astype(HSV_TempMV[0].dtype)
    # showImgPara(hChannel)
    HSV_TempMV[0] = hChannel
    imgHSVOut = np.zeros(imgOri.shape, imgOri.dtype)
    imgHSVOut = cv.merge(HSV_TempMV)
    imgOut = cv.cvtColor(imgHSVOut, cv.COLOR_HSV2BGR)

    cv.imshow("imgOri", imgOut)
    print(pos)

def sCallBack(pos):
    sChannel = np.copy(HSV_MV[1])
    print("---------Begain", pos, "---------")
    # showImgPara(sChannel)
    sChannel = sChannel * (pos / 100.0)
    # showImgPara(hChannel)
    sChannel = sChannel.astype(HSV_TempMV[1].dtype)
    # showImgPara(hChannel)
    HSV_TempMV[1] = sChannel
    imgHSVOut = np.zeros(imgOri.shape, imgOri.dtype)
    imgHSVOut = cv.merge(HSV_TempMV)
    imgOut = cv.cvtColor(imgHSVOut, cv.COLOR_HSV2BGR)

    cv.imshow("imgOri", imgOut)
    print(pos)

def vCallBack(pos):
    vChannel = np.copy(HSV_MV[2])
    print("---------Begain", pos, "---------")
    # showImgPara(hChannel)
    vChannel = vChannel * (pos / 100.0)
    # showImgPara(hChannel)
    vChannel = vChannel.astype(HSV_TempMV[0].dtype)
    # showImgPara(hChannel)
    HSV_TempMV[2] = vChannel
    imgHSVOut = np.zeros(imgOri.shape, imgOri.dtype)
    imgHSVOut = cv.merge(HSV_TempMV)
    imgOut = cv.cvtColor(imgHSVOut, cv.COLOR_HSV2BGR)

    cv.imshow("imgOri", imgOut)
    print(pos)

def cover2HSVTest(imgOri):
    global HSV_MV, HSV_TempMV
    cv.imshow("imgOri", imgOri)
    HSV = cv.cvtColor(imgOri, cv.COLOR_BGR2HSV)
    cv.imshow("HSV", HSV)

    HSV_MV = cv.split(HSV)
    HSV_TempMV = cv.split(HSV)


    cv.imshow("H", HSV_MV[0])
    cv.imshow("S", HSV_MV[1])
    cv.imshow("V", HSV_MV[2])

    cv.createTrackbar("H", "imgOri", hPos, HSV_MAX, hCallBack)
    cv.createTrackbar("S", "imgOri", sPos, HSV_MAX, sCallBack)
    cv.createTrackbar("V", "imgOri", vPos, HSV_MAX, vCallBack)



def main_func(argv):
    global  imgOri
    imgOri = cv.imread("Danxia.jpg")
    # splitRGBTest(imgOri)
    cover2HSVTest(imgOri)

    cv.waitKey(0)


if __name__ == '__main__':
    main_func(sys.argv)