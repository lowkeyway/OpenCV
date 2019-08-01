#!/usr/bin/env python
# -*- coding: utf-8 -*-
# author：lowkeyway time:7/19/2019

import  sys
import numpy as np
import  cv2  as cv


def divideTest():
    imgTungWire = cv.imread("Fig0229(a)(tungsten_filament_shaded).tif")
    imgShadow = cv.imread("Fig0229(b)(tungsten_sensor_shading).tif")
    imgOut = np.zeros(imgTungWire.shape)

    imgTungWireTemp = imgTungWire.astype(imgOut.dtype)
    imgShadowTemp = imgShadow.astype(imgOut.dtype)

    cv.divide(imgTungWireTemp, imgShadowTemp, imgOut)
    # cv.divide(imgTungWire, imgShadow, imgOut)

    cv.imshow("Tungsten", imgTungWire)
    cv.imshow("Shadow", imgShadow)
    cv.imshow("imgOut", imgOut)


def mutiplyTest():
    imgTooth = cv.imread("Fig0230(a)(dental_xray).tif")
    imgMask = cv.imread("Fig0230(b)(dental_xray_mask).tif")
    imgMaskOneBit = np.copy(imgMask)
    imgOut = np.zeros(imgTooth.shape, imgTooth.dtype)

    imgMaskOneBit[imgMaskOneBit>0] = 1

    cv.multiply(imgTooth, imgMaskOneBit, imgOut)

    cv.imshow("Tooth", imgTooth)
    cv.imshow("Mask", imgMask)
    cv.imshow("Out", imgOut)

def main_func(argv):
    # mutiplyTest()
    divideTest()

    cv.waitKey(0)

if __name__ == '__main__':
    main_func(sys.argv)