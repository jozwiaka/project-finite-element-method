#!/usr/bin/env python
# -*- coding: utf8 -*-
from __future__ import unicode_literals
import codecs
import Animations
import Plots
import Analitical
import Remove
import Files
from matplotlib import pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
from multiprocessing import Process

# Plots.plot_element()
# Animations.createGif("ts1_bc1")
mode = 1
if mode:
    names = Files.getNames()
    for n in names:
        Plots.plot(n)
    #Remove.rm()
else:
    Remove.rm()




# if __name__ == "__main__":
#     names = Files.getNames()
#     processes=[]
#     for n in names:
#         processes.append(Process(target=Plots.plot,args=(n,)))

#     [p.start() for p in processes]
#     [p.join() for p in processes]
#     # Remove.rm()