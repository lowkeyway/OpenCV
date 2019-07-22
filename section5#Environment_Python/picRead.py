#!/usr/bin/env python
# -*- coding: utf-8 -*-
# author：lowkeyway time:7/1/2019
import cv2 as cv
import sys

def main_func(argc):
    pic = cv.imread("python.jpg")
    cv.namedWindow("python")
    cv.imshow("python", pic)

    cv.waitKey(0)
    cv.destroyAllWindows()

if __name__ == '__main__':
    main_func(sys.argv)