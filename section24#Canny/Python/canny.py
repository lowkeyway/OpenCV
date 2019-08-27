#!/usr/bin/env python
# -*- coding: utf-8 -*-
# author：lowkeyway time:8/27/2019

import sys
import cv2 as cv
import numpy as np

def cannyTest(imgOri, needResize = False, needBlur = False):
    cv.imshow("cannyTest imgOri", imgOri)
    if needResize:
        imgOri = cv.resize(imgOri, (int(imgOri.shape[1]/2), int(imgOri.shape[0]/2)))
    if needBlur:
        imgOri = cv.GaussianBlur(imgOri, (11, 11), 1)


    imgEdge = cv.Canny(imgOri, 50, 100)
    cv.imshow("cannyTest imgEdge", imgEdge)

    imgEdgeSrc = cv.bitwise_and(imgOri, imgOri, mask=imgEdge)
    cv.imshow("cannyTest imgEdgeSrc", imgEdgeSrc)

    h, w = imgOri.shape[:2]
    result = np.zeros([h, w*2, 3], dtype=imgOri.dtype)
    result[0:h, 0:w, :] = imgOri
    result[0:h, w:2*w, :] = imgEdgeSrc
    cv.putText(result, "imgOri", (10, 30), cv.FONT_ITALIC, 1.0, (0, 0, 255), 2)
    cv.putText(result, "imgUnit", (10+w, 30), cv.FONT_ITALIC, 1.0, (0, 0, 255), 2)
    cv.imshow("cannyTest Unite", result)

    cv.waitKey(0)

def main_func(argv):
    imgGirl = cv.imread("Gril.jpg")
    cannyTest(imgGirl, needResize=True, needBlur=False)


if __name__ == '__main__':
    main_func(sys.argv)