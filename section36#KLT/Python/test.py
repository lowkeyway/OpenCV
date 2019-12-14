#!/usr/bin/env python
# -*- coding: utf-8 -*-
# author：lowkeyway time:10/31/2019

import sys


def main_func(argv):
    A = [1, 2, 3]
    B = ['a', 'b']

    for a, b in zip(A, B):
        print(a, b)


if __name__ == '__main__':
    main_func(sys.argv)