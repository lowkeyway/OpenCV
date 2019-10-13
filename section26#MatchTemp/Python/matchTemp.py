#!/usr/bin/env python
# -*- coding: utf-8 -*-
# author：lowkeyway time:9/2/2019

import sys
import cv2 as cv
import numpy as np

def npWhereTest():
    temp = np.arange(9).reshape(3, 3)
    print(type(temp), temp)

    index = np.where(temp > 5)
    print(type(index), index)
    print(temp[index])

    print(list(zip(*index)))
    print(list(zip(*index[::-1])))

def zipTest():
    a = np.array([1, 1, 2, 2, 2, 2, 3, 3, 3, 3])
    b = np.array([2, 3, 0, 1, 2, 3, 0, 1, 2, 3])
    print(a)
    print(b)
    c = zip(a, b)
    print(list(c))

    d = [1, 2], [3, 4], [5, 6]
    print(type(d), d)

    e = zip(*d)
    print(type(e), list(e))

def matchTest(imgOri, imgTemp):
    cv.imshow("imgOri", imgOri)
    cv.imshow("imgTemp", imgTemp)

    th, tw = imgTemp.shape[:2]
    methods = [cv.TM_SQDIFF_NORMED, cv.TM_CCORR_NORMED, cv.TM_CCOEFF_NORMED]

    for md in methods:
        imgTarget = cv.matchTemplate(imgOri, imgTemp, md)

        if cv.TM_SQDIFF_NORMED == md:
            t = 1 - 0.98
            loc = np.where(imgTarget < t)
        else:
            t = 0.98
            loc = np.where(imgTarget > t)

        for pt in zip(*loc[::-1]):
            cv.rectangle(imgOri, pt, (pt[0]+tw, pt[1]+th), (0, 0, 255))

        cv.imshow("imgTarget_" + str(md), imgTarget)
        cv.imshow("imgOriEye_" + str(md), imgOri)



def main_func(argv):
    imgOri = cv.imread("Gril.jpg")
    imgTemp = cv.imread("eye.png")
    # matchTest(imgOri, imgTemp)
    # npWhereTest()
    zipTest()

    cv.waitKey(0)


if __name__ == '__main__':
    main_func(sys.argv)