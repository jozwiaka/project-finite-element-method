#!/usr/bin/env python
# -*- coding: utf8 -*-
from __future__ import unicode_literals
import codecs
from matplotlib import pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import Files
import Analitical
from itertools import product, combinations

#yzad_u=r"$u[K]$"
#yzad_u=r"$\dot{q}_s\Big[{\dfrac{W}{m^2}}\Big]$"
yzad_u=r"$u\Big[{\dfrac{W}{m^2}}\Big]$"
plt.rc('text', usetex=True)
plt.rc('font', family='serif')
plt.rc('text.latex', preamble=r'\usepackage{amsmath}')

def plot_cuboid():
    # step=0.01
    # x = np.arange(step,0.4,step)
    # y = np.arange(step,0.5,step)
    # z = np.arange(step,0.6,step)
    # points = []
    # for zi in z:
    #     for yi in y:
    #         for xi in x:
    #               points.append([xi,yi,zi]) 
    # points = np.array(points)
    # x = points[:, 0]
    # y = points[:, 1]
    # z = points[:, 2]
    
    fig = plt.figure()
    ax = plt.axes(projection='3d')
    # ax.scatter(x, y, z, c="red")
    ax.set_xlabel(r"$x$[m]",rotation='horizontal')
    ax.set_ylabel(r"$y$[m]",rotation='horizontal')
    ax.set_zlabel(r"$z$[m]",rotation='horizontal')
    
    rx = [0, 0.4]
    ry = [0, 0.5]
    rz = [0, 0.6]
    
    for s, e in combinations(np.array(list(product(rx, ry, rz))), 2):
        if ((np.sum(np.abs(s-e)) == rx[1]-rx[0]) or (np.sum(np.abs(s-e)) == ry[1]-ry[0]) or (np.sum(np.abs(s-e)) == rz[1]-rz[0])):
            ax.plot3D(*zip(s, e), color="black")
    plt.savefig(Files.p_ts_bc1 + "cuboid" + '.pdf',bbox_inches='tight')  

def plot_grid(name):
    name='ts2_bc2'
    [dt, T, points] = Files.readFile_N(name)
    points = np.array(points)
    x = points[:, 0]
    y = points[:, 1]
    z = points[:, 2]
    colors = [0]*len(points)

    fig = plt.figure()
    ax = plt.axes(projection='3d')
    ax.scatter(x, y, z, c="red")
    ax.set_xlabel(r"$x$",rotation='horizontal')
    ax.set_ylabel(r"$y$",rotation='horizontal')
    ax.set_zlabel(r"$z$",rotation='horizontal')
    plt.savefig(Files.p_ts_bc1 + "grid_j1" + '.pdf',bbox_inches='tight')

def plot_local_element():
    x = [-1, 1,-1,-1,-1, 1, 1, 1]
    y = [-1,-1, 1,-1, 1,-1, 1, 1]
    z = [-1,-1,-1, 1, 1, 1,-1, 1]
    fig = plt.figure()
    ax = plt.axes(projection='3d')
    
    color="black"
    for i in range(len(x)):
        xi = np.linspace(x[i],-x[i],10)
        yi = np.linspace(y[i],y[i],10)
        zi =  np.linspace(z[i],z[i],10)
        ax.plot(xi,yi,zi,c=color)
        
        xi =  np.linspace(x[i],x[i],10)
        yi = np.linspace(y[i],-y[i],10)
        zi =  np.linspace(z[i],z[i],10)
        ax.plot(xi,yi,zi,c=color)
        
        xi = np.linspace(x[i],x[i],10)
        yi = np.linspace(y[i],y[i],10)
        zi =  np.linspace(z[i],-z[i],10)
        ax.plot(xi,yi,zi,c=color)
    
    ax.scatter(x, y, z,c="red",s=50)
    
    ax.xaxis.set_rotate_label(False)
    ax.yaxis.set_rotate_label(False)
    ax.zaxis.set_rotate_label(False)
    ax.set_xlabel(r"$\xi$",rotation='horizontal')
    ax.set_ylabel(r"$\eta$",rotation='horizontal')
    ax.set_zlabel(r"$\delta$",rotation='horizontal')
    ax.set_xticks([-1,0,1])
    ax.set_yticks([-1,0,1])
    ax.set_zticks([-1,0,1])
 
    plt.savefig(Files.p_acs + 'local' + '.pdf',bbox_inches='tight')
   
    
def plot_global_element():
    a=1
    b=1
    c=1
    x = np.array([-1, 1,-1,-1,-1, 1, 1, 1])*a
    y = np.array([-1,-1, 1,-1, 1,-1, 1, 1])*b
    z = np.array([-1,-1,-1, 1, 1, 1,-1, 1])*c
    fig = plt.figure()
    ax = plt.axes(projection='3d')
    
    color="black"
    for i in range(len(x)):
        xi = np.linspace(x[i],-x[i],10)
        yi = np.linspace(y[i],y[i],10)
        zi =  np.linspace(z[i],z[i],10)
        ax.plot(xi,yi,zi,c=color)
        
        xi =  np.linspace(x[i],x[i],10)
        yi = np.linspace(y[i],-y[i],10)
        zi =  np.linspace(z[i],z[i],10)
        ax.plot(xi,yi,zi,c=color)
        
        xi = np.linspace(x[i],x[i],10)
        yi = np.linspace(y[i],y[i],10)
        zi =  np.linspace(z[i],-z[i],10)
        ax.plot(xi,yi,zi,c=color)
    
    ax.scatter(x, y, z,c="red",s=50)
    
    ax.set_xlabel(r"$x$")
    ax.set_ylabel(r"$y$")
    ax.set_zlabel(r"$z$")

    def format_func_x(x,ticks):
        if x == -a:
            return r"$x_1$"
        if x == 0:
            return r"$x_c$"
        if x == a:
            return r"$x_2$"
    def format_func_y(x,ticks):
        if x == -b:
            return r"$y_1$"
        if x == 0:
            return r"$y_c$"
        if x == b:
            return r"$y_2$"
    def format_func_z(x,ticks):
        if x == -c:
            return r"$z_1$"
        if x == 0:
            return r"$z_c$"
        if x == c:
            return r"$z_2$"
        
    ax.set_xticks([-1,0,1])
    ax.set_yticks([-1,0,1])
    ax.set_zticks([-1,0,1])
    ax.xaxis.set_major_formatter(plt.FuncFormatter(format_func_x))
    ax.yaxis.set_major_formatter(plt.FuncFormatter(format_func_y))
    ax.zaxis.set_major_formatter(plt.FuncFormatter(format_func_z))
    
    plt.savefig(Files.p_acs + 'global' + '.pdf',bbox_inches='tight')


def plot_element():
    plot_local_element()
    plot_global_element()
    
def plot(name):
    if name.find("data") !=-1:
        return Files.readFile(name)
    if name.find("ts") != -1:
        if name.find("bc1") != -1:
            plot_ts_bc1(name)
            plot_ts_bc1_TN(name)
            return
        if name.find("bc2") != -1:
            plot_ts_bc2(name)
            plot_ts_bc2_TN(name)
            return
        if name.find("bc3") != -1:
            plot_ts_bc3(name)
            plot_ts_bc3_TN(name)
            return

    if name.find("ss") != -1:
        if name.find("bc1") != -1:
            return plot_ss_bc1(name)
        if name.find("bc2") != -1:
            print("bc1")
        if name.find("bc3") != -1:
            print("bc1")

    if name.find("acs") != -1:
        if name.find("u_matlab") != -1:
            return plot_acs_u_matlab(name)
        if name.find("obj") != -1:
            if name.find("matlab") != -1:
                return -1
            return plot_acs_obj(name)
        if name.find("matlab") != -1:
            return plot_acs_mes(name)

def plot_acs_obj(name):
    plt.figure()
    [dt,yIn, T] = Files.readFile(name)
    #T = np.array(T)
    #T = T - T[0]
    t = np.arange(0, len(T)*dt, dt)
    plt.plot(t, T, color='r', label="MES")
    # #plt.title("T(t) dla x = "+str(x))
    try: 
        [G,t2,T]=Files.readFile(name[:4]+'_obj_matlab')
        plt.plot(t2, T, color='b', label=G)
    except:
        print('MATLAB file not found for \'' + name +'\'')
    plt.xlabel("$t[s]$")
    plt.ylabel("$T[K]$")
    plt.legend()
    name_T = name+"_T"
    #plt.title(name_T)
    plt.grid()
    plt.savefig(Files.p_acs + name_T + '.pdf',bbox_inches='tight')
    
    plt.figure()
    plt.plot(t, yIn, color='r', label="Sygnał wejściowy")
    # #plt.title("T(t) dla x = "+str(x))
    plt.xlabel("$t[s]$")
    plt.ylabel(yzad_u)
    plt.legend()
    name_yIn = name+"_yIn"
    #plt.title(name_yIn)
    plt.grid()
    plt.savefig(Files.p_acs + name_yIn + '.pdf',bbox_inches='tight')

def plot_acs_mes(name):
    plt.figure()
    try:
        [t, T] = Files.readFile(name[:4]+'_mes')
        #T = np.array(T)
        #T = T - T[0]
        plt.plot(t, T, color='r', label="MES")
    # #plt.title("T(t) dla x = "+str(x))
    except:
        pass
    try:    
        [G,t,T]=Files.readFile(name)
        plt.plot(t, T, color='b', label="$G_Z(s)$")
    except:
        return;
    plt.xlabel("$t[s]$")
    plt.ylabel("$T[K]$")
    plt.legend()
    #plt.title(name)
    plt.grid()
    plt.savefig(Files.p_acs + name + '.pdf',bbox_inches='tight')

def plot_acs_u_matlab(name):
    plt.figure()
    try:
        [t, T] = Files.readFile(name)
        #T=np.array(T)
        #T=T-293
        plt.plot(t, T, color='r', label="u(t)")
    # #plt.title("T(t) dla x = "+str(x))
    except:
        return
    plt.xlabel("$t[s]$")
    plt.ylabel(yzad_u)
    plt.legend()
    #plt.title(name)
    plt.grid()
    plt.savefig(Files.p_acs + name + '.pdf',bbox_inches='tight')

def plot_ts_bc1(name):
    plt.figure()
    [dt, x, T0, lambdax, rho, cw, T1, T] = Files.readFile(name)
    t = np.arange(0, len(T)*dt, dt)
    plt.plot(t, T, color='r', label="MES")
    plt.plot(t, Analitical.ts_bc1(x, t, T0,
             lambdax, rho, cw, T1), '--', color='black', label=Analitical.f_ts_bc1)
    # #plt.title("T(t) dla x = "+str(x))
    plt.xlabel("$t[s]$")
    plt.ylabel("$T[K]$")
    plt.legend()
    # #plt.title(name)
    plt.grid()
    plt.legend(bbox_to_anchor=(-0.02,-0.4), loc=3)
    plt.savefig(Files.p_ts_bc1 + name + '.pdf', bbox_inches='tight')


def plot_ts_bc1_TN(name):
    plt.figure()
    [dt, x, T0, lambdax, rho, cw, T1, T] = Files.readFile(name)
    [t, T, points] = Files.readFile_N(name)
    points = np.array(points)
    z = points[:, 2]
    for i in range(0, len(t)):
        if(i%100!=0):
            continue
        plt.plot(z, T[i][:], label=r"MES, $t="+str(int(i*dt))+"$ s")
        # #plt.title("T(t) dla x = "+str(x))
    plt.plot(z, Analitical.ts_bc1(z, t[len(t)-1], T0,
                                  lambdax, rho, cw, T1), '--', color='black', label=r"Analityczne, $t="+str(int(t[len(t)-1]))+"$ s")
    plt.xlabel("$z[m]$")
    plt.ylabel("$T[K]$")
    plt.legend()
    # #plt.title(name)
    plt.grid()
    plt.legend(bbox_to_anchor=(-0.02,-0.55), loc=3)
    plt.savefig(Files.p_ts_bc1 + name + '_TN.pdf',bbox_inches='tight')


def plot_ts_bc2(name):
    plt.figure()
    [dt, x, T0, lambdax, rho, cw, qs, T] = Files.readFile(name)
    t = np.arange(0, len(T)*dt, dt)
    plt.plot(t, T, color='r', label="MES")
    plt.plot(t, Analitical.ts_bc2(x, t, T0,
             lambdax, rho, cw, qs), '--', color='black', label=Analitical.f_ts_bc2)
    # #plt.title("T(t) dla x = "+str(x))
    plt.xlabel("$t[s]$")
    plt.ylabel("$T[K]$")
    plt.legend()
    # #plt.title(name)
    plt.grid()
    plt.legend(bbox_to_anchor=(-0.02,-0.4), loc=3)
    plt.savefig(Files.p_ts_bc2 + name + '.pdf',bbox_inches='tight')


def plot_ts_bc2_TN(name):
    plt.figure()
    [dt, x, T0, lambdax, rho, cw, qs, T] = Files.readFile(name)
    [t, T, points] = Files.readFile_N(name)
    points = np.array(points)
    z = points[:, 2]
    
    for i in range(0, len(t)):
        if(i%100!=0):
            continue
        plt.plot(z, T[i][:], label=r"MES, $t="+str(int(i*dt))+"$ s")
        # #plt.title("T(t) dla x = "+str(x))
    plt.plot(z, Analitical.ts_bc2(z, t[len(t)-1], T0,
                                  lambdax, rho, cw, qs), '--', color='black', label=r"Analityczne, $t="+str(int(t[len(t)-1]))+"$ s")
   
    plt.xlabel("$z[m]$")
    plt.ylabel("$T[K]$")
    plt.legend()
    # #plt.title(name)
    plt.grid()
    plt.legend(bbox_to_anchor=(-0.02,-0.55), loc=3)
    plt.savefig(Files.p_ts_bc2 + name + '_TN.pdf',bbox_inches='tight')


def plot_ts_bc3(name):
    plt.figure()
    [dt, x, T0, lambdax, rho, cw, T1, alpha, T] = Files.readFile(name)
    t = np.arange(0, len(T)*dt, dt)
    plt.plot(t, T, color='r', label="MES")
    plt.plot(t, Analitical.ts_bc3(x, t, T0,
             lambdax, rho, cw, T1, alpha), '--', color='black', label=Analitical.f_ts_bc3)
    # #plt.title("T(t) dla x = "+str(x))
    plt.xlabel("$t[s]$")
    plt.ylabel("$T[K]$")
    plt.legend()
    # #plt.title(name)
    plt.grid()
    plt.legend(bbox_to_anchor=(-0.02,-0.4), loc=3)
    plt.savefig(Files.p_ts_bc3 + name + '.pdf',bbox_inches='tight')


def plot_ts_bc3_TN(name):
    plt.figure()
    [dt, x, T0, lambdax, rho, cw, T1, alpha, T] = Files.readFile(name)
    [t, T, points] = Files.readFile_N(name)
    points = np.array(points)
    z = points[:, 2]
    for i in range(0, len(t)):
        if(i%100!=0):
            continue
        plt.plot(z, T[i][:], label=r"MES, $t="+str(int(i*dt))+"$ s")
        # #plt.title("T(t) dla x = "+str(x))
    plt.plot(z, Analitical.ts_bc3(z, t[len(t)-1], T0,
                                   lambdax, rho, cw, T1, alpha), '--', color='black', label=r"Analityczne, $t="+str(int(t[len(t)-1]))+"$ s")
   
    plt.xlabel("$z[m]$")
    plt.ylabel("$T[K]$")
    plt.legend()
    # #plt.title(name)
    plt.grid()
    plt.legend(bbox_to_anchor=(-0.02,-0.55), loc=3)
    plt.savefig(Files.p_ts_bc3 + name + '_TN.pdf',bbox_inches='tight')


def plot_ss_bc1(name):
    plt.figure()
    [x, T, lambdax, qv, T1, T2] = Files.readFile(name)
    x = np.array(x)
    plt.plot(x, T, color='r', label="MES")
    plt.plot(x, Analitical.ss_bc1(x, qv, lambdax,
                                  x[-1], T1, T2), '--', color='black', label=Analitical.f_ss_bc1)
    # #plt.title("T(x) dla t$->\infty$")
    plt.xlabel("$z[m]$")
    plt.ylabel("$T[K]$")
    plt.grid()
    plt.legend()
    # #plt.title(name)
    plt.legend(bbox_to_anchor=(-0.02,-0.4), loc=3)
    plt.savefig(Files.p_ss_bc1 + name + '.pdf',bbox_inches='tight')
