#!/usr/bin/env python
# -*- coding: utf-8 -*-
# author：lowkeyway time:7/19/2019

import sys
import cv2 as cv
import numpy as np

def main_func(argv):
    imgBone = cv.imread("Fig0232(a)(partial_body_scan).tif")
    cv.imshow("boneOri", imgBone)

    imgOut = np.zeros(imgBone.shape, imgBone.dtype)
    cv.bitwise_not(imgBone, imgOut)
    cv.imshow("boneOut", imgOut)
    cv.imwrite("bonBitwiseNot.png", imgOut)


    cv.waitKey(0)

if __name__ == '__main__':
    main_func(sys.argv)