#!/usr/bin/env python
# -*- coding: utf-8 -*-
# author：lowkeyway time:7/17/2019

import sys
import cv2 as cv
import numpy as np
import random


def addTest(imgOri, fileName, noiseNumber):
    noisePic = np.zeros(imgOri.shape, imgOri.dtype)

    for index in range(noiseNumber):
        temp = cv.imread(fileName + str(index) + ".png")
        if temp is None:
            print(fileName + str(index) + ".png is not exist!")
            return
        cv.addWeighted(noisePic, 1, temp, 1.0/noiseNumber, 0.0, noisePic)

    cv.namedWindow("imgAdd")
    cv.imshow("imgAdd", noisePic)

    cv.imwrite("imgAdd.png", noisePic)


def noisePicCreat(imgOri, fileName, noiseNumber):
    for index in range(noiseNumber):
        fileExist = cv.imread(fileName + str(index) + ".png")
        if fileExist is not None:
            print(fileName + str(index) + ".png is exist!")
            cv.namedWindow("imgNoise" + str(index))
            cv.imshow("imgNoise" + str(index), fileExist)
            continue
        normalNoise = np.random.normal(0,1,imgOri.shape)*16
        imgNoise = np.zeros(imgOri.shape, normalNoise.dtype)
        imgTemp = imgOri.astype(normalNoise.dtype)

        cv.add(normalNoise, imgTemp, imgNoise)
        imgNoise[imgNoise > 255] = 255.0
        imgNoise[imgNoise < 0] = 0
        imgNoise = imgNoise.astype(imgOri.dtype)

        cv.imwrite(fileName + str(index) + ".png", imgNoise)

        cv.namedWindow("imgNoise" + str(index))
        cv.imshow("imgNoise" + str(index), imgNoise)

def main_func(argv):
    imgOri = cv.imread("Fig0226(galaxy_pair_original).tif")
    cv.namedWindow("imgOri")
    cv.imshow("imgOri", imgOri)

    noisePicCreat(imgOri, "Noise", 10)
    addTest(imgOri, "Noise", 10)

    cv.waitKey(0)
    cv.destroyAllWindows()

if __name__ == '__main__':
    main_func(sys.argv)