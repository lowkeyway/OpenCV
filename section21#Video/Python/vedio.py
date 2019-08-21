#!/usr/bin/env python
# -*- coding: utf-8 -*-
# author：lowkeyway time:8/19/2019

import sys
import cv2 as cv
import numpy as np

def waitEscKey(delayTime):
    keyValue = cv.waitKey(delayTime)
    if 27 == keyValue:
        return True
    if 30 == keyValue:
        while True:
            keyValue = cv.waitKey(0)
            if 27 == keyValue:
                return True
            if 30 == keyValue:
                break

    return False


def main_func(argv):
    capture = cv.VideoCapture(0)
    fps = capture.get(cv.CAP_PROP_FPS)
    w = capture.get(cv.CAP_PROP_FRAME_WIDTH)
    h = capture.get(cv.CAP_PROP_FRAME_HEIGHT)

    out = cv.VideoWriter("outPut.mp4", cv.VideoWriter_fourcc('D', 'I', 'V', 'X'), fps,\
                         (int(w), int(h)), True)


    while True:
        ret, frameOri = capture.read()
        if ret is not True:
            break
        frameOri = cv.flip(frameOri, 1)
        frameOut = cv.GaussianBlur(frameOri, (5, 5), 10)
        cv.imshow("videoOri", frameOri)
        cv.imshow("videoOut", frameOut)

        out.write(frameOut)
        if True == waitEscKey(30):
            break

    capture.release()
    out.release()

if __name__ == '__main__':
    main_func(sys.argv)