# Import libraries
from matplotlib import pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import animation
import numpy as np
import Files
import Colors

p_gif = "../gif/"


def createGif(name):
    [dt, T, points] = Files.readFile_N(name)
    points = np.array(points)
    x = points[:, 0]
    y = points[:, 1]
    z = points[:, 2]
    colors = [0]*len(points)

    fig = plt.figure()
    ax = plt.axes(projection='3d')

    for i in range(0, len(T[0])):
        colors[i] = Colors.TtoColor(T[0][i])
    ax.scatter(x, y, z, c=colors)
    ti = -1
    global upOnce

    def update(i, fig, ax):
        nonlocal ti
        t = round(ti*dt, 2)
        plt.title("t = "+str(t)+" s")
        print("t = ", t)
        for i in range(0, len(T[0])):
            colors[i] = Colors.TtoColor(T[ti][i])
        ax.scatter(x, y, z, c=colors)
        ti = ti+1
        return fig, ax

    anim = animation.FuncAnimation(
        fig, update, frames=len(T), repeat=False, fargs=(fig, ax))
    anim.save(p_gif+name+'.gif', dpi=80,
              writer='imagemagick', fps=1)
