#!/usr/bin/env python
# -*- coding: utf-8 -*-
# author：lowkeyway time:8/24/2019

import sys
import cv2 as cv
import numpy as np

def laplaceTest(imgOri, needResize = False, needBlur = False):
    cv.imshow("imgOri", imgOri)
    if len(imgOri.shape) is not 2:
        imgOri = cv.cvtColor(imgOri, cv.COLOR_BGR2GRAY)

    if needResize:
        imgOri = cv.resize(imgOri, (int(imgOri.shape[1]*0.5), int(imgOri.shape[0]*0.5)))

    if needBlur:
        imgOri = cv.GaussianBlur(imgOri, (3, 3), 1)

    # Laplace only
    imgLap = cv.Laplacian(imgOri, cv.CV_16S)
    imgLap = cv.convertScaleAbs(imgLap)
    cv.imshow("imglap", imgLap)

    # Laplace Delta
    imgLapDelta = cv.Laplacian(imgOri, cv.CV_16S, delta=100)
    imgLapDelta = cv.convertScaleAbs(imgLapDelta)
    cv.imshow("imgLapDelta", imgLapDelta)

    # Laplace Add
    imgLapAdd = cv.addWeighted(imgOri, 1.0, imgLap, -1.0, 0, dtype=cv.CV_32F)
    imgLapAdd = cv.convertScaleAbs(imgLapAdd)
    cv.imshow("imgLapAdd", imgLapAdd)


def main_func(argv):
    imgMoon = cv.imread("Fig0338(a)(blurry_moon).tif")
    # laplaceTest(imgMoon)

    imgGril = cv.imread("Gril.jpg")
    laplaceTest(imgGril, True, True)

    cv.waitKey(0)

if __name__ == '__main__':
    main_func(sys.argv)