# -*- coding: utf-8 -*-
import Files
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import pyplot as plt
import numpy as np
plt.rc('text', usetex=True)
plt.rc('font', family='serif')
plt.rc('text.latex', preamble=r'\usepackage{amsmath}')


names = Files.getNames()
for name in names:
    #if name.find("bc")!=-1 or name.find("ts")!=-1:
    #if name.find("mes")!=-1:
    try:
        [t, T, points]=Files.readFile_N(name)
        points = np.array(points)
        z = points[:, 2]
        fig = plt.figure()
        plt.rcParams["figure.figsize"] = plt.rcParamsDefault["figure.figsize"]
        ax = plt.axes(projection='3d')
        X,Y=np.meshgrid(z,t)
        Z=np.array(T)
        ax.xaxis.set_rotate_label(False)
        ax.yaxis.set_rotate_label(False)
        ax.zaxis.set_rotate_label(False)
        ax.set_xlabel(r"$z$[m]",rotation='horizontal')
        ax.set_ylabel(r"$t$[s]",rotation='horizontal')
        ax.set_zlabel(r"$T$[K]",rotation='horizontal')
        
        MAX = np.amax(Z)
        MIN = np.amin(Z)
        ax.set_zlim(MIN, MAX)
        ax.plot_surface(X,Y,Z)
        plt.savefig(Files.p_ts_plots_3d + name + '_plt_3d.pdf',bbox_inches='tight')
    except:
        pass