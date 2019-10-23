#!/usr/bin/env python
# -*- coding: utf-8 -*-
# author：lowkeyway time:9/10/2019

import sys
import cv2 as cv
import numpy as np

def connectionTest(imgOri):
    if 2 != len(imgOri.shape):
        imgOri = cv.cvtColor(imgOri, cv.COLOR_BGR2GRAY)

    cv.imshow("imgGray", imgOri)

    imgOri = cv.adaptiveThreshold(imgOri, 255, cv.ADAPTIVE_THRESH_GAUSSIAN_C, cv.THRESH_BINARY_INV, 111, 10)
    cv.imshow("imgBin", imgOri)

    # labels = np.zeros(imgOri.shape, cv.CV_32S)
    labNum, labels, stats, centroids = cv.connectedComponentsWithStats(imgOri)

    colors = []
    for index in range(labNum):
        b = np.random.randint(0, 256)
        g = np.random.randint(0, 256)
        r = np.random.randint(0, 256)
        colors.append([b, g, r])

    rows, cols = imgOri.shape
    imgCon = np.zeros((rows, cols, 3), dtype=np.uint8)
    print(rows, cols)
    for row in range(rows):
        for col in range(cols):
            raw = labels[row][col]
            if raw == 0:
                continue
            imgCon[row][col] = colors[raw]

    for index in range(1, labNum, 1):
        x, y, w, h, area = stats[index]
        cx, cy = centroids[index]
        cv.circle(imgCon, (np.int32(cx), np.int32(cy)), 2, (0, 0, 255), 2, 8, 0)
        cv.rectangle(imgCon, (np.int32(x), np.int32(y)), (np.int32(x + w), np.int32(y + h)), colors[index], 1, 8, 0)

    cv.imshow("imgCon", imgCon)


def main_func(argv):
    imgOri = cv.imread("red10.jfif")
    connectionTest(imgOri)

    cv.waitKey(0)

if __name__ == '__main__':
    main_func(sys.argv)