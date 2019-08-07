#!/usr/bin/env python
# -*- coding: utf-8 -*-
# author：lowkeyway time:8/2/2019

import sys, os, math
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

def highPass(rows, cols):
    mask = np.ones((rows, cols), np.uint8)
    # mask[int(rows/4):int(rows*3/4), int(cols/4):int(cols*3/4)] = 0
    cv.circle(mask, (int(cols/2), int(rows/2)), int(min(rows, cols)/8), 0, -1)
    return mask

def lowPass(rows, cols):
    mask = np.zeros((rows, cols), np.uint8)
    # mask[int(rows/4):int(rows*3/4), int(cols/4):int(cols*3/4)] = 1
    cv.circle(mask, (int(cols/2), int(rows/2)), int(min(rows, cols)/8), 1, -1)
    return mask


def npFFTshow(array):
    oriFFT = np.fft.fft2(array)
    cenFFT = np.fft.fftshift(oriFFT)

    cenFFTALog = np.log(np.abs(cenFFT))
    plt.figure("Array Show")
    plt.subplot(111)
    plt.imshow(cenFFTALog, "gray")

    plt.show()

def FFT(fileName):
    imgOri = cv.imread(fileName, 0)
    original = np.fft.fft2(imgOri)
    center = np.fft.fftshift(original)

    # Amplitude
    oriALog = np.log(np.abs(original))
    cenALog = np.log(np.abs(center))

    # Phase
    oriPh = np.angle(original)
    cenPh = np.angle(center)

    # iFFT
    iCenter = np.fft.ifftshift(center)
    iOriginal = np.fft.ifft2(iCenter)
    imgBack = np.abs(iOriginal)

    # Smoothing High Pass
    rows, cols = imgOri.shape
    HP = highPass(rows, cols)
    highPassCenter = center * HP

    ihighPassCenter = np.fft.ifftshift(highPassCenter)
    iHighPassOriginal = np.fft.ifft2(ihighPassCenter)
    imgHighPass = np.abs(iHighPassOriginal)

    # Smoothing Low Pass
    rows, cols = imgOri.shape
    LP = lowPass(rows, cols)
    lowPassCenter = center * LP

    iLowPassCenter = np.fft.ifftshift(lowPassCenter)
    iLowPassOriginal = np.fft.ifft2(iLowPassCenter)
    imgLowhPass = np.abs(iLowPassOriginal)

    # fingure 1
    plt.figure(fileName.split(".")[0] + "_FFT")

    plt.subplot(131), plt.imshow(imgOri, "gray"), plt.title("ImgOri")
    plt.subplot(132), plt.imshow(cenALog, "gray"), plt.title("FFT_Amplitude_Center")
    plt.subplot(133), plt.imshow(cenPh, "gray"), plt.title("FFT_Phase_Center")

    # finger 2
    plt.figure(fileName.split(".")[0] + "IFFT")
    plt.subplot(231), plt.imshow(cenALog, "gray"), plt.title("FFT_Amplitude_Center")
    plt.subplot(232), plt.imshow(HP, "gray"), plt.title("High Pass")
    plt.subplot(233), plt.imshow(LP, "gray"), plt.title("Low Pass")

    plt.subplot(234), plt.imshow(imgBack, "gray"), plt.title("IFFT_BACK")
    plt.subplot(235), plt.imshow(imgHighPass, "gray"), plt.title("IFFT_HighPass")
    plt.subplot(236), plt.imshow(imgLowhPass, "gray"), plt.title("IFFT_LowPass")

    plt.show()

def magnitude(x, y):
    x_m = x * x
    y_m = y * y
    z_m = x_m + y_m
    return np.sqrt(z_m)

def DFT(fileName):
    imgOri = cv.imread(fileName, 0)
    showImgPara(imgOri)
    dftOri = cv.dft(np.float32(imgOri), flags=cv.DFT_COMPLEX_OUTPUT)
    dftShift = np.fft.fftshift(dftOri)

    cenALog = 20 * np.log10(magnitude(dftShift[:, :, 0], dftShift[:, :, 1]))
    # fingure 1
    plt.figure(fileName.split(".")[0] + "_DFT")

    plt.subplot(121), plt.imshow(imgOri, "gray"), plt.title("ImgOri")
    plt.subplot(122), plt.imshow(cenALog, cmap="gray"), plt.title("FFT_Amplitude_Center")

    plt.show()



def main_func(argv):
    FFT("Fig0240(a)(apollo 17_boulder_noisy).tif")
    # FFT("Fig0222(a)(face).tif")
    # DFT("Fig0240(a)(apollo 17_boulder_noisy).tif")

    # average = np.ones([3, 3], np.uint8)*1/9
    # npFFTshow(average)

if __name__ == '__main__':
    main_func(sys.argv)