#!/usr/bin/env python
# -*- coding: utf-8 -*-
# author：lowkeyway time:8/31/2019

import sys
import cv2 as cv
import numpy as np
import matplotlib.pyplot as plt

def showImgPara(img):
    print("Sizeof img is: " + str(img.size))
    print("rows x cols is: " + str(np.size(img, 0)) + " x " + str(np.size(img, 1)))
    print("dimension is: ", img.ndim)
    print("dtype is: ", img.dtype)
    print("itemsize is: ", img.itemsize)
    print("shape is(rows, cols, depth): ", img.shape)
    print("\n")

def myPyramidLa(imgOri):
    if 3 == len(imgOri.shape):
        imgOri = cv.cvtColor(imgOri, cv.COLOR_BGR2GRAY)

    temp = myPyramidDown(imgOri)
    temp = myPyramidUp(temp)
    pyLa = cv.subtract(imgOri, temp)+127

    plt.figure("myPyramidLa")
    plt.subplot(131), plt.imshow(imgOri, "gray"), plt.title("imgOri")
    plt.subplot(132), plt.imshow(temp, "gray"), plt.title("temp")
    plt.subplot(133), plt.imshow(pyLa, "gray"), plt.title("pyLa")

    print("\n----myPyramidLa----")
    showImgPara(imgOri)
    showImgPara(temp)
    showImgPara(pyLa)

    x, y = imgOri.shape
    imgOut = np.zeros([y, x*3], dtype=imgOri.dtype)
    imgOut[:y, :x] = imgOri
    imgOut[:y, x:2*x] = temp
    imgOut[:y, 2*x:3*x] = pyLa
    cv.imshow("pyLa", imgOut)

def myPyramidUp(imgOri):
    kernel = (4/256 * np.array(
        [
            [1,  4,  6,  4, 1],
            [4, 16, 24, 16, 4],
            [6, 24, 36, 24, 6],
            [4, 16, 24, 16, 4],
            [1,  4,  6,  4, 1]
        ]
    ))

    if 3 == len(imgOri.shape):
        imgOri = cv.cvtColor(imgOri, cv.COLOR_BGR2GRAY)
    x, y = imgOri.shape
    temp = np.copy(imgOri)

    # Resize
    for index in range(x):
        temp = np.insert(temp, 2*index+1, 0, axis=1)
    for index in range(y):
        temp = np.insert(temp, 2*index+1, 0, axis=0)

    myDst = cv.filter2D(temp, -1, kernel=kernel)

    pyDst = cv.pyrUp(imgOri)
    diff = cv.subtract(pyDst, myDst)

    plt.figure("myPyramidUp")
    plt.subplot(223), plt.imshow(imgOri, "gray"), plt.title("imgOri")
    plt.subplot(221), plt.imshow(myDst, "gray"), plt.title("myDst")
    plt.subplot(222), plt.imshow(myDst, "gray"), plt.title("pyDst")
    plt.subplot(224), plt.imshow(diff, "gray"), plt.title("Diff")



    # plt.show()
    return myDst


def myPyramidDown(imgOri):
    kernel = (1/256 * np.array(
        [
            [1,  4,  6,  4, 1],
            [4, 16, 24, 16, 4],
            [6, 24, 36, 24, 6],
            [4, 16, 24, 16, 4],
            [1,  4,  6,  4, 1]
        ]
    ))
    if 3 == len(imgOri.shape):
        imgOri = cv.cvtColor(imgOri, cv.COLOR_BGR2GRAY)

    x, y = imgOri.shape
    temp = cv.filter2D(imgOri, -1, kernel=kernel)
    myDst = temp[::2, ::2]

    pyDst = cv.pyrDown(imgOri)
    diff = cv.subtract(pyDst, myDst)

    plt.figure("myPyramidDown")
    plt.subplot(223), plt.imshow(imgOri, "gray"), plt.title("imgOri")
    plt.subplot(221), plt.imshow(myDst, "gray"), plt.title("myDst")
    plt.subplot(222), plt.imshow(pyDst, "gray"), plt.title("pyDst")
    plt.subplot(224), plt.imshow(diff, "gray"), plt.title("Diff")
    # plt.show()

    return myDst

def main_func(argv):
    imgOri = cv.imread("Fig0701.tif")
    # myPyramidDown(imgOri)
    imgOri = cv.imread("lowkeyway.jpg")
    # myPyramidUp(imgOri)
    myPyramidLa(imgOri)

    # plt.show()
    cv.waitKey(0)

if __name__ == '__main__':
    main_func(sys.argv)