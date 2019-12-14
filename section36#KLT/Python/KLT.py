#!/usr/bin/env python
# -*- coding: utf-8 -*-
# author：lowkeyway time:10/25/2019

import numpy as np
import cv2
from time import clock
import sys

lk_params = dict(winSize=(15, 15),
                 maxLevel=2,
                 criteria=(cv2.TERM_CRITERIA_EPS | cv2.TERM_CRITERIA_COUNT, 10, 0.03))

feature_params = dict(maxCorners=500,
                      qualityLevel=0.3,
                      minDistance=7,
                      blockSize=7)


class App:
    def __init__(self, video_src):  # 构造方法，初始化一些参数和视频路径
        self.track_len = 10
        self.detect_interval = 5
        self.tracks = []
        self.cam = cv2.VideoCapture(video_src)
        self.prev_gray = None
        self.frame_idx = 0

    def run(self):  # 光流运行方法
        ret, frame = self.cam.read()  # 读取视频帧
        if ret == True:
            self.prev_gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)  # 转化为灰度虚图像
            vis = self.prev_gray.copy()

            p = cv2.goodFeaturesToTrack(self.prev_gray, **feature_params)  # 像素级别角点检测
            if p is not None:
                # for x, y in np.float32(p).reshape(-1, 2):
                #     self.tracks.append([(x, y)])  # 将检测到的角点放在待跟踪序列中
                self.tracks = p

        while True:
            ret, frame = self.cam.read()  # 读取视频帧
            if ret == True:
                frame_gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)  # 转化为灰度虚图像
                vis = frame.copy()

                if len(self.tracks) > 0:  # 检测到角点后进行光流跟踪
                    img0, img1 = self.prev_gray, frame_gray
                    # p0 = np.float32([tr[-1] for tr in self.tracks]).reshape(-1, 1, 2)
                    p0 = self.tracks
                    p1, st, err = cv2.calcOpticalFlowPyrLK(img0, img1, p0, None,
                                                           **lk_params)  # 前一帧的角点和当前帧的图像作为输入来得到角点在当前帧的位置
                    # p0r, st, err = cv2.calcOpticalFlowPyrLK(img1, img0, p1, None,
                    #                                         **lk_params)  # 当前帧跟踪到的角点及图像和前一帧的图像作为输入来找到前一帧的角点位置
                    # d = abs(p0 - p0r).reshape(-1, 2).max(-1)  # 得到角点回溯与前一帧实际角点的位置变化关系
                    d = abs(p1 - p0).reshape(-1, 2).max(-1)  # 得到角点回溯与前一帧实际角点的位置变化关系
                    good = d > 2  # 判断d内的值是否小于1，大于1跟踪被认为是错误的跟踪点
                    # new_tracks = []
                    # for tr, (x, y), good_flag in zip(self.tracks.tolist(), p1.reshape(-1, 2), good):  # 将跟踪正确的点列入成功跟踪点
                    #     if not good_flag:
                    #         continue
                    #     tr.append((x, y))
                    #     if len(tr) > self.track_len:
                    #         del tr[0]
                    #     new_tracks.append(tr)
                    #     cv2.circle(vis, (x, y), 2, (0, 255, 0), -1)

                    goodPre = p0[st == 1]
                    goodNew = p1[st == 1]

                    for new, pre in zip(goodNew, goodPre):
                        newX, newY = new.ravel()
                        preX, preY = pre.ravel()

                        if abs(newX - preX) + abs(newY - preY) > 2:
                            cv2.circle(vis, (newX, newY), 2, (0, 255, 0), -1)


                    self.tracks = p1
                    cv2.polylines(vis, [np.int32(tr) for tr in self.tracks], False,
                                  (0, 255, 0))  # 以上一振角点为初始点，当前帧跟踪到的点为终点划线
                    # draw_str(vis, (20, 20), 'track count: %d' % len(self.tracks))

                if self.frame_idx % self.detect_interval == 0:  # 每5帧检测一次特征点
                    # mask = np.zeros_like(frame_gray)  # 初始化和视频大小相同的图像
                    # mask[:] = 255  # 将mask赋值255也就是算全部图像的角点
                    # for x, y in [np.int32(tr[-1]) for tr in self.tracks]:  # 跟踪的角点画圆
                    #     cv2.circle(mask, (x, y), 5, 0, -1)
                    # p = cv2.goodFeaturesToTrack(frame_gray, mask=mask, **feature_params)  # 像素级别角点检测
                    p = cv2.goodFeaturesToTrack(frame_gray, **feature_params)  # 像素级别角点检测
                    # if p is not None:
                    #     for x, y in np.float32(p).reshape(-1, 2):
                    #         self.tracks.append([(x, y)])  # 将检测到的角点放在待跟踪序列中
                    # self.tracks.extend(p)
                    self.tracks = p
                self.frame_idx += 1
                self.prev_gray = frame_gray
                cv2.imshow('lk_track', vis)

            ch = 0xFF & cv2.waitKey(60)
            if ch == 27:
                break


def main_func(argv):
    try:
        video_src = sys.argv[1]
    except:
        video_src = "vtest.avi"

    print(__doc__)
    App(video_src).run()
    cv2.destroyAllWindows()


if __name__ == '__main__':
    main_func(sys.argv)