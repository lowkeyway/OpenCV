#!/usr/bin/env python
# -*- coding: utf-8 -*-
# author：lowkeyway time:9/25/2019

import sys
import cv2 as cv
import numpy as np
import matplotlib.pyplot as plt
import random

def contoursTest(imgOri):
    cv.imshow("imgOri", imgOri)
    imgGary = cv.cvtColor(imgOri, cv.COLOR_BGR2GRAY)
    cv.imshow("imgGary", imgGary)
    imgBin = cv.Canny(imgOri, 80, 160)
    cv.imshow("imgBin", imgBin)

    out, contours, hierarchy = cv.findContours(imgBin, cv.RETR_LIST, cv.CHAIN_APPROX_SIMPLE)
    result = np.zeros(imgOri.shape, dtype=imgOri.dtype)
    for index, con in enumerate(contours):
        color = (random.randint(0, 255), random.randint(0, 255), random.randint(0, 255))
        cv.drawContours(imgOri, contours, index, color, 1, 8)
        cv.drawContours(result, contours, index, color, 1, 8)

    cv.imshow("imgMix", imgOri)
    cv.imshow("result", result)

def main_func(argv):
    imgOri = cv.imread("synaptics.jfif")
    if imgOri is None:
        print("Cannot load this picture!")
        return

    contoursTest(imgOri)

    cv.waitKey(0)

if __name__ == '__main__':
    main_func(sys.argv)