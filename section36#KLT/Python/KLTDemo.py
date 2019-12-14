#!/usr/bin/env python
# -*- coding: utf-8 -*-
# author：lowkeyway time:10/29/2019


'''
==================================================================
This file copy from OpenCV official deamo.
Lucas-Kanade sparse optical flow demo. Uses goodFeaturesToTrack
for track initialization and back-tracking for match verification
between frames.
==================================================================
'''
import sys
import cv2 as cv
import numpy as np


lk_params = dict(winSize=(15, 15),
                 maxLevel=2,
                 criteria=(cv.TERM_CRITERIA_EPS | cv.TERM_CRITERIA_COUNT, 10, 0.03))

feature_params = dict(maxCorners=500,
                      qualityLevel=0.3,
                      minDistance=7,
                      blockSize=7)

class KLT:
    def __init__(self, src):
        self.video = cv.VideoCapture(src)
        # tracks is a list which storage a list of float point (x, y)
        self.tracks = []
        self.frameIndex = 0
        self.track_len = 5

    def run(self):
        ret, imgPre = self.video.read()
        if ret is not True:
            return ret

        imgPreGary = cv.cvtColor(imgPre, cv.COLOR_BGR2GRAY)
        corPre = cv.goodFeaturesToTrack(imgPreGary, **feature_params)
        if corPre is None:
            return

        for x, y in (np.float32(corPre).reshape(-1, 2)):
            self.tracks.append([(x, y)])

        while True:
            ret, imgCur = self.video.read()
            if ret is not True:
                return ret

            imgCurGary = cv.cvtColor(imgCur, cv.COLOR_BGR2GRAY)

            # mask = np.zeros_like(imgCurGary)
            # mask[:] = 255
            corPre = np.float32([tr[-1] for tr in self.tracks]).reshape(-1, 1, 2)
            corCur, st, err = cv.calcOpticalFlowPyrLK(imgPreGary, imgCurGary, corPre, None, **lk_params)

            goodNew = corCur[st == 1]
            goodOld = corPre[st == 1]
            # disp = abs(goodNew - goodOld).reshape(-1, 2).max(-1)
            # good = disp > 2
            newTrack = []
            for tr, new, old in zip(self.tracks, goodNew, goodOld):
                newX, newY = new.ravel()
                oldX, oldY = old.ravel()
                if (abs(newX - oldX) + abs(newY - oldY)) > 2:
                    # cv.circle(imgCur, (newX, newY), 2, (0, 255, 0), -1)
                    tr.append((newX, newY))
                    if len(tr) > self.track_len:
                        del tr[0]
                    newTrack.append(tr)
            self.tracks = newTrack
            cv.polylines(imgCur, [np.int32(tr) for tr in self.tracks], False, (0, 255, 0))

            cv.imshow("img", imgCur)

            imgPreGary = imgCurGary.copy()
            self.frameIndex += 1

            ch = 0xff & cv.waitKey(30)
            if ch == 27:
                return True

            if self.frameIndex % 5 == 0:
                corPre = cv.goodFeaturesToTrack(imgPreGary, **feature_params)
                if corPre is None:
                    return

                for x, y in np.float32(corPre).reshape(-1, 2):
                    self.tracks.append([(x, y)])


def main_func(argv):
    try:
        src = sys.argv[1]
    except:
        src = "vtest.avi"
    print(__doc__)

    KLT(src).run()
    cv.destroyAllWindows()


if __name__ == '__main__':
    main_func(sys.argv)