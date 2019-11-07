#!/usr/bin/env python
# -*- coding: utf-8 -*-
# author：lowkeyway time:10/22/2019

import sys
import numpy as np
import cv2 as cv
import time

def shiTomasTest(imgOri):
    maxCorners = 500;
    quality_level = 0.01;
    minDistance = 0.04;
    imgGary = cv.cvtColor(imgOri, cv.COLOR_BGR2GRAY)
    corners = cv.goodFeaturesToTrack(imgGary, maxCorners, quality_level, minDistance)

    for corner in corners:
        b = np.random.randint(0, 256)
        g = np.random.randint(0, 256)
        r = np.random.randint(0, 256)

        cv.circle(imgOri, (int(corner[0][0]), int(corner[0][1])), 2, (int(b), int(g), int(r)), 2)

    cv.imshow("imgShiTomas", imgOri)

def harrisTest(imgOri):
    blockSize = 2
    apertureSize = 3
    k = 0.04

    imgGary = cv.cvtColor(imgOri, cv.COLOR_BGR2GRAY)

    imgHarris = cv.cornerHarris(imgGary, blockSize, apertureSize, k)

    imgDst = np.zeros(imgHarris.shape, dtype=np.float32)
    cv.normalize(imgHarris, imgDst, 0, 255, cv.NORM_MINMAX)
    imgDstScale = cv.convertScaleAbs(imgDst)

    for row in range(imgDstScale.shape[0]):
        for col in range(imgDstScale.shape[1]):
            if int(imgDstScale[row, col]) > 85:
                b = np.random.randint(0, 256)
                g = np.random.randint(0, 256)
                r = np.random.randint(0, 256)
                cv.circle(imgOri, (col, row), 2, (int(b), int(g), int(r)), 2)

    cv.imshow("imgHarrisDst", imgOri)

def main_func(argv):
    imgOri = cv.imread("right13.jpg")
    if imgOri is None:
        print("Cannot load this picture!")
        return False

    imgHarris = np.copy(imgOri)
    imgShiTomas = np.copy(imgOri)


    start = time.process_time()
    harrisTest(imgHarris)
    harrisTime = time.process_time()
    shiTomasTest(imgShiTomas)
    shiTomasTime = time.process_time()

    print("Harris takes {:.2f} s" .format(harrisTime - start))
    print("Harris takes {:.2f} s" .format(shiTomasTime - harrisTime))
    cv.imshow("imgOri", imgOri)

    cv.waitKey(0)

if __name__ == '__main__':
    main_func(sys.argv)