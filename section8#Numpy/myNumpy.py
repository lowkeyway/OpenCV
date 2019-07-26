#!/usr/bin/env python
# -*- coding: utf-8 -*-
# author：lowkeyway time:7/9/2019

import sys
import numpy as np
import  cv2 as cv

def showImgPara(img):
    print("Sizeof img is: " + str(img.size))
    print("rows x cols is: " + str(np.size(img, 0)) + " x " + str(np.size(img, 1)))
    print("dimension is: ", img.ndim)
    print("dtype is: ", img.dtype)
    print("itemsize is: ", img.itemsize)
    print("shape is(rows, cols, depth): ", img.shape)

def cover2signalChannel(img):
    b, g, r = cv.split(img)
    # cv.imshow("Blue", b)
    # cv.imshow("Green", g)
    # cv.imshow("Red", r)

    return r

def picTest(path, name):
    print("\n*****************picTest*****************")

    img = cv.imread(path)
    cv.namedWindow(name)
    cv.imshow(name, img)
    showImgPara(img)
    # cover2signalChannel(img)

def numpyTest():
    print("\n*****************numpyTest*****************")
    data1 = [1, 2, 3]
    data2 = [[1, 2, 3], [4, 5, 6]]
    data3 = [[[1, 2, 3], [4, 5, 6]],
             [[7, 8 ,9], [10, 11, 12]]]

    arr1 = np.array(data1)
    arr2 = np.array(data2)
    arr3 = np.array(data3)

    print(arr1.ndim, arr2.ndim, arr3.ndim)
    print(arr1.shape, arr2.shape, arr3.shape)

    e1 = arr2[[0, 1], [1, 2]]
    e2 = arr3[np.array([[0, 0], [1, 1]]), np.array([[0, 1], [0, 1]])]
    print(e1)
    print(e2)


def creatPic(m, n, name):
    print("\n*****************creatPic*****************")
    img = np.zeros([m, n, 3], np.uint8)
    # print(img)
    ################## piexel by pixel ##################
    # for i in range(0, m):
    #     for j in range(0, n):
    #         if(i > m/4 and i < m*3/4 and j > n/4 and j < n*3/4):
    #             img[i, j, 0] = 255
    #             img[i, j, 1] = 0
    #             img[i, j, 2] = 0

    ################## slice ##################
    img[int(m/4):int(m*3/4), int(n/4):int(n*3/4), 0] = 255
    img[int(m/4):int(m*3/4), int(n/4):int(n*3/4), 1] = 0
    img[int(m/4):int(m*3/4), int(n/4):int(n*3/4), 2] = 0

    cv.imshow(name, img)
    showImgPara(img)

def pic2digit(picName, fileName):
    print("\n*****************pic2digit*****************")
    img = cv.imread(picName)
    print("**** original picture ****")
    showImgPara(img)

    imgSignal = cover2signalChannel(img)
    # cv.imshow("signalPic", imgSignal)
    print("**** signal channel picture ****")
    showImgPara(imgSignal)
    np.savetxt(fileName, imgSignal, fmt="%02d")



def main_func(argv):
    picTest("girl.jpg", "girl")
    numpyTest()
    creatPic(200, 400, "test")
    pic2digit("check-circle.png", "check-circle.txt")


    cv.waitKey(0)

if __name__ == '__main__':
    main_func(sys.argv)