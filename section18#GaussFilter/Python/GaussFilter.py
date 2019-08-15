#!/usr/bin/env python
# -*- coding: utf-8 -*-
# author：lowkeyway time:8/10/2019

import sys
import numpy as np
import cv2 as cv

GAUSS_NOISE = "GaussNoise"
FILE_TYPE = ".png"
GAUSS_NOISE_FILE = GAUSS_NOISE + FILE_TYPE

MEANS = 127
STANDARD_DAV = 10


'''
Min/Max and Location
'''
def meanStdevShow(img):
    means, stdev = cv.meanStdDev(img)
    print("means: {}" .format(means))
    print("stdev: {}" .format(stdev))
    print("----meanStdevShow End----\n")

'''
Min/Max and Location
'''
def minMaxShow(img):
    min, max, minLdx, maxLdx = cv.minMaxLoc(img)
    print("Min: %.2f[%d, %d]" %(min, minLdx[0], minLdx[1]))
    print("Max: %.2f[%d, %d]" %(max, maxLdx[0], maxLdx[1]))
    print("----minMaxShow End----\n")
'''
Gauss Noise Picture Creat
'''
def gaussKernelTest(row, col, depth, mu, sigma):
    kernelX = cv.getGaussianKernel(row, sigma)
    kernelY = cv.getGaussianKernel(col, sigma)
    kernelXY = kernelX*kernelY.T
    print(kernelXY)

def main_func(argv):
    # Original Image
    imgOri = cv.imread("origimal.png", cv.IMREAD_GRAYSCALE)
    cv.imshow("Original", imgOri)
    minMaxShow(imgOri)
    meanStdevShow(imgOri)

    # Gauss Kernel Test
    gaussKernelTest(3, 3, 1, 0, 1)

    # Noise Image
    imgNoise = cv.imread(GAUSS_NOISE_FILE, cv.IMREAD_GRAYSCALE)
    cv.imshow("Noise Image", imgNoise)
    minMaxShow(imgNoise)
    meanStdevShow(imgNoise)

    # Gauss Noise Filter
    imgGaussFilter = cv.GaussianBlur(imgNoise, (101, 101), STANDARD_DAV)
    cv.imshow("Gauss Filter Out", imgGaussFilter)
    minMaxShow(imgGaussFilter)
    meanStdevShow(imgGaussFilter)
    cv.waitKey(0)

if __name__ == '__main__':
    main_func(sys.argv)