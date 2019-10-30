#!/usr/bin/env python
# -*- coding: utf-8 -*-
# author：lowkeyway time:9/20/2019

import sys
import cv2 as cv
import numpy as np
import matplotlib.pyplot as plt

ORIGINAL_WIN = "Original"
ERO_DIL_WIN = "ErodeDilate"
OPEN_CLOSE_WIN = "OpenClose"
TOP_BLACK_WIN = "TopBlack"

ERO_DIL_BAR = "ErodeDilateBar"
OPEN_CLOSE_BAR = "OpenCloseBar"
TOP_BLACK_BAR = "TopBlackBar"
TH_MAX = 10
TH_INIT = 5

class Morphology:

    imgOri = None
    g_iStructuringElement = cv.MORPH_RECT

    def __init__(self, fileName):
        self.imgOri = cv.imread(fileName)
        if self.imgOri is None:
            print("Cannot load this picture")
            return

    def ErodeDilateCallBack(self, th):
        diff = th - TH_MAX/2
        size = int(abs(diff)) * 2 + 1
        kernel = cv.getStructuringElement(self.g_iStructuringElement, (size, size))
        if diff < 0:
            imgErodeDilate = cv.morphologyEx(self.imgOri, cv.MORPH_ERODE, kernel)
        else:
            imgErodeDilate = cv.morphologyEx(self.imgOri, cv.MORPH_DILATE, kernel)

        cv.imshow(ERO_DIL_WIN, imgErodeDilate)


    def OpenCloseCallBack(self, th):
        diff = th - TH_MAX / 2
        size = int(abs(diff)) * 2 + 1
        kernel = cv.getStructuringElement(self.g_iStructuringElement, (size, size))
        if diff < 0:
            imgOpenClose = cv.morphologyEx(self.imgOri, cv.MORPH_OPEN, kernel)
        else:
            imgOpenClose = cv.morphologyEx(self.imgOri, cv.MORPH_CLOSE, kernel)

        cv.imshow(OPEN_CLOSE_WIN, imgOpenClose)

    def TopBlackCallBack(self, th):
        diff = th - TH_MAX / 2
        size = int(abs(diff)) * 2 + 1
        kernel = cv.getStructuringElement(self.g_iStructuringElement, (size, size))
        if diff < 0:
            imgTopBlack = cv.morphologyEx(self.imgOri, cv.MORPH_TOPHAT, kernel)
        else:
            imgTopBlack = cv.morphologyEx(self.imgOri, cv.MORPH_BLACKHAT, kernel)

        cv.imshow(TOP_BLACK_WIN, imgTopBlack)


    def changeEleShape(self):
        while True:
            ch = cv.waitKey(0)
            # Escape
            if ch == 27:
                return False

            # Space
            if ch == 32:
                self.g_iStructuringElement = (self.g_iStructuringElement+1)%3
                if self.g_iStructuringElement == cv.MORPH_RECT:
                    print("MORPH_RECT")
                elif self.g_iStructuringElement == cv.MORPH_CROSS:
                    print("MORPH_CROSS")
                elif self.g_iStructuringElement == cv.MORPH_ELLIPSE:
                    print("MORPH_ELLIPSE")
                else:
                    print("Some thing Wrong!")
                return True

    def morphologyThread(self):

        cv.namedWindow(ORIGINAL_WIN)
        cv.namedWindow(ERO_DIL_WIN)
        cv.namedWindow(OPEN_CLOSE_WIN)
        cv.namedWindow(TOP_BLACK_WIN)

        cv.imshow(ORIGINAL_WIN, self.imgOri)

        cv.createTrackbar(ERO_DIL_BAR, ERO_DIL_WIN, TH_INIT, TH_MAX, self.ErodeDilateCallBack)
        cv.createTrackbar(OPEN_CLOSE_BAR, OPEN_CLOSE_WIN, TH_INIT, TH_MAX, self.OpenCloseCallBack)
        cv.createTrackbar(TOP_BLACK_BAR, TOP_BLACK_WIN, TH_INIT, TH_MAX, self.TopBlackCallBack)

        while True:
            self.ErodeDilateCallBack(TH_INIT)
            self.OpenCloseCallBack(TH_INIT)
            self.TopBlackCallBack(TH_INIT)
            if False == self.changeEleShape():
                break

def main_func(argv):
    M = Morphology("HappyFish.jpg")
    M.morphologyThread()

    # cv.waitKey(0)

if __name__ == '__main__':
    main_func(sys.argv)