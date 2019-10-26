#!/usr/bin/env python
# -*- coding: utf-8 -*-
# author：lowkeyway time:9/17/2019

import sys
import cv2 as cv
import numpy as np
import matplotlib.pyplot as plt

def houghTest(imgOri):
    cv.imshow("imgOri", imgOri)
    imgCan = cv.Canny(imgOri, 100, 200)
    cv.imshow("imgCan", imgCan)

    imgHough = imgOri.copy()
    imgHoughP = imgOri.copy()
    Hlines = cv.HoughLines(imgCan, 1, np.pi/180, 150)

    for line in Hlines:
        rho = line[0][0]
        theta = line[0][1]
        x0 = rho*np.cos(theta)
        y0 = rho*np.sin(theta)

        p1 = (int(x0 + 1000*np.sin(theta)), int(y0 - 1000*np.cos(theta)))
        p2 = (int(x0 - 1000*np.sin(theta)), int(y0 + 1000*np.cos(theta)))

        cv.line(imgHough, p1, p2, (0, 0, 255), 3, cv.LINE_AA)

    HPlines = cv.HoughLinesP(imgCan, 1, np.pi/180, 50, None, 50, 10)
    for line in HPlines:
        x0, y0, x1, y1 = line[0]
        cv.line(imgHoughP, (x0, y0), (x1, y1), (0, 255, 0), 1, cv.LINE_AA)

    cv.imshow("imgHough", imgHough)
    cv.imshow("imgHoughP", imgHoughP)


    plt.figure("HoughPic")
    plt.subplot(221); plt.imshow(imgOri); plt.title("ImgOri")
    plt.subplot(222); plt.imshow(imgCan, "gray"); plt.title("imgCan")
    plt.subplot(223); plt.imshow(imgHough, "gray"); plt.title("imgHough")
    plt.subplot(224); plt.imshow(imgHoughP, "gray"); plt.title("imgHoughP")

    plt.show()


def main_func(argv):
    imgOri = cv.imread("sudoku.png")
    if imgOri is None:
        print("Cannot load this picture!")
        return

    houghTest(imgOri)
    cv.waitKey(0)

if __name__ == '__main__':
    main_func(sys.argv)