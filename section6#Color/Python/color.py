#!/usr/bin/env python
# -*- coding: utf-8 -*-
# author：lowkeyway time:7/1/2019

import sys
import cv2 as cv

def main_func(argv):
    img = cv.imread("girl.jpg")
    cv.namedWindow("Girl")
    cv.imshow("Girl", img)

    gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
    cv.namedWindow("GirlGray")
    cv.imshow("GirlGray", gray)
    cv.imwrite("GirlGray.jpg", gray)

    cv.waitKey(6000)
    cv.destroyAllWindows()

if __name__ == '__main__':
    main_func(sys.argv)