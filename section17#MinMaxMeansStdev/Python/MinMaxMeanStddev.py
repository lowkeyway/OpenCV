#!/usr/bin/env python
# -*- coding: utf-8 -*-
# author：lowkeyway time:8/10/2019

import sys
import numpy as np
import cv2 as cv

GAUSS_NOISE = "GaussNoise_127"
FILE_TYPE = ".png"
GAUSS_NOISE_FILE = GAUSS_NOISE + FILE_TYPE

MEANS = 127
STANDARD_DAV = 10


'''
Normal Show
'''
def normalShow(img):
    img = img.astype(np.float)
    imgNormal = np.copy(img)
    # imgNormal = imgNormal.astype(np.float)
    cv.normalize(img, imgNormal, 0, 255, cv.NORM_MINMAX)


    minMaxShow(imgNormal)
    meanStdevShow(imgNormal)

    imgNormal = imgNormal.astype(np.uint8)
    cv.imshow("imgNormal", imgNormal)

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
    # print("Min: %.2f[{}, {}]" %(min) .format(minLdx[0], minLdx[1]))
    # print("Max: %.2f[{}, {}]" %(max) .format(maxLdx[0], maxLdx[1]))
    print("Min: %.2f[%d, %d]" %(min, minLdx[0], minLdx[1]))
    print("Max: %.2f[%d, %d]" %(max, maxLdx[0], maxLdx[1]))
    print("----minMaxShow End----\n")
'''
Gauss Noise Picture Creat
'''
def noisePicCreat(imgOri, fileName, mu, sigma):
    normalNoise = np.random.normal(mu,sigma,imgOri.shape)
    imgNoise = np.zeros(imgOri.shape, normalNoise.dtype)
    imgTemp = imgOri.astype(normalNoise.dtype)

    cv.add(normalNoise, imgTemp, imgNoise)
    imgNoise[imgNoise > 255] = 255.0
    imgNoise[imgNoise < 0] = 0
    imgNoise = imgNoise.astype(imgOri.dtype)

    cv.imwrite(fileName + FILE_TYPE, imgNoise)

    cv.namedWindow("imgNoise")
    cv.imshow("imgNoise", imgNoise)

def main_func(argv):
    imgOri = np.zeros([512, 512, 1], np.uint8)
    noisePicCreat(imgOri, GAUSS_NOISE, MEANS, STANDARD_DAV)

    imgNoise = cv.imread(GAUSS_NOISE_FILE, cv.IMREAD_GRAYSCALE)
    minMaxShow(imgNoise)
    meanStdevShow(imgNoise)

    normalShow(imgNoise)
    cv.waitKey(0)

if __name__ == '__main__':
    main_func(sys.argv)