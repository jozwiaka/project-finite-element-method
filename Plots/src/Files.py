#!/usr/bin/env python
# -*- coding: utf8 -*-
from __future__ import unicode_literals
import codecs
import glob
# paths:
from Analitical import ss_bc1, ts_bc1

p_ts_plots_3d="../plt/ts/plots_3d/"
p_ts_errors="../plt/ts/errors/"
p_ss_errors="../plt/ss/errors/"
p_ts_data = "../plt/ts/data/"
p_ts_bc1 = "../plt/ts/bc1/"
p_ts_bc2 = "../plt/ts/bc2/"
p_ts_bc3 = "../plt/ts/bc3/"
p_ts_bcx = "../plt/ts/bcx/"
p_ss_data = "../plt/ss/data/"
p_ss_bc1 = "../plt/ss/bc1/"
p_ss_bc2 = "../plt/ss/bc2/"
p_ss_bc3 = "../plt/ss/bc3/"
p_ss_bcx = "../plt/ss/bcx/"
p_acs = "../plt/acs/"
p_acs_bc1 = "../plt/acs/bc1/"
p_acs_bc2 = "../plt/acs/bc2"
p_acs_pm = "../plt/acs/pm/"
p_acs_data = "../plt/acs/data/"


def data(path,d):
    f = codecs.open(path,"w","utf-8")
    f.write("\\begin{comment}\n źł\n\\end{comment}")
    f.write("\n")
    f.write(r"\begin{table}[H]")
    f.write("\n")
    f.write(r"\centering")
    f.write("\n")
    # f.write(r"\begin{footnotesize}")
    # f.write("\n")
    f.write(r"\label{tab:table}")
    f.write("\n")
    f.write(r"\begin{tabular}{c|ccc|ccc|c}")
    f.write(r"\multicolumn{2}{c}{Parametry geometryczne} & \quad & \multicolumn{2}{c}{Parametry materiałowe} & \quad & \multicolumn{2}{c}{Parametry czasowe}")
    f.write("\n")
    f.write(r"\\")
    f.write("\n")
    f.write(r"$a\left[\unit{m}\right]$ & $"+d[0]+r"$ & \quad & $\lambda \left[\unit{ \dfrac{W}{Km}}\right]$ & $"+d[1]+r"$ & \quad & $t_c \left[\unit{s}\right]$ & $"+d[2]+r"$")
    f.write("\n")
    f.write(r"\\")
    f.write("\n")
    f.write(r"$b\left[\unit{m}\right]$ & $"+d[3]+r"$ & \quad & $\rho \left[\unit{\dfrac{kg}{m^3}}\right]$ & $"+d[4]+r"$ & \quad & $\Delta t \left[\unit{s}\right]$ & $"+d[5]+r"$")
    f.write("\n")
    f.write(r"\\")
    f.write("\n")
    f.write(r"$c\left[\unit{m}\right]$ & $"+d[6]+r"$ & \quad & $c_w \left[\unit{\dfrac{J}{kgK}}\right]$ & $"+d[7]+r"$ & \quad & &")
    f.write("\n")
    f.write(r"\\")
    f.write("\n")
    f.write(r"\multicolumn{2}{c}{Źródła ciepła} & \quad & \multicolumn{2}{c}{Warunek początkowy} & \quad & \multicolumn{2}{c}{Ilość węzłów}")
    f.write("\n")
    f.write(r"\\")
    f.write("\n")
    f.write(r"$\dot{q}_v\left[\unit{\dfrac{W}{m^3}}\right]$ & $"+d[8]+r"$ & \quad & $T_0 \left[\unit{K}\right]$ & $"+d[9]+r"$ & \quad & $n \left[\unit{-}\right]$ & $"+d[10]+r"$")
    f.write("\n")
    f.write(r"\\")
    f.write("\n")
    f.write(r"\multicolumn{2}{c}{Warunek brzegowy I} & \quad & \multicolumn{2}{c}{Warunek brzegowy II} & \quad & \multicolumn{2}{c}{Warunek brzegowy III}")
    f.write("\n")
    f.write(r"\\")
    f.write("\n")
    f.write(r"$T_s\left[\unit{K}\right]$ & $"+d[11]+r"$ & \quad & $\dot{q_s} \left[\unit{\dfrac{W}{m^2}}\right]$ & $"+d[12]+r"$ & \quad & $T_{\infty} \left[\unit{K}\right]$ & $"+d[13]+r"$")
    f.write("\n")
    f.write(r"\\")
    f.write("\n")
    f.write(r"&  & \quad & & & \quad &$h \unit{\left[\dfrac{W}{m^2K}\right]}$ & $"+d[14]+r"$")
    f.write("\n")
    f.write(r"\\")
    f.write("\n")
    f.write(r"\multicolumn{2}{c}{Sygnał wejściowy} & \quad & \multicolumn{2}{c}{Regulator PID}")
    f.write("\n")
    f.write(r"\\")
    f.write("\n")
    f.write(r" $T_{zad}\left[\unit{K}\right]$ & $"+d[15]+r"$ & \quad & $k_p \left[\unit{-}\right]$ & $"+d[16]+r"$")
    f.write("\n")
    f.write(r"\\")
    f.write("\n")
    f.write(r"$A_m\left[\unit{*}\right]$ & $"+d[18]+r"$ & \quad & $T_i \left[\unit{s}\right]$ & $"+d[19]+r"$")
    f.write("\n")
    f.write(r"\\")
    f.write("\n")
    f.write(r"$f_0\left[\unit{Hz}\right]$ & $"+d[21]+r"$ & \quad & $T_d \left[\unit{s}\right]$ & $"+d[22]+r"$")
    f.write("\n")
    f.write(r"\\")
    f.write("\n")
    # f.write(r"\multicolumn{2}{c}{Wartość zadana} & \quad & \multicolumn{2}{c}{Regulator PID} & \quad & \multicolumn{2}{c}{Moduł Peltiera}")
    # f.write("\n")
    # f.write(r"\\")
    # f.write("\n")
    # f.write(r" $T_{zad}\left[\unit{K}\right]$ & $"+d[15]+r"$ & \quad & $k_p \left[\unit{-}\right]$ & $"+d[16]+r"$ & \quad & $\alpha \left[\unit{\dfrac{V}{K}}\right]$ & $"+d[17]+r"$")
    # f.write("\n")
    # f.write(r"\\")
    # f.write("\n")
    # f.write(r"$A_m\left[\unit{*}\right]$ & $"+d[18]+r"$ & \quad & $T_i \left[\unit{s}\right]$ & $"+d[19]+r"$ & \quad & $K_P \left[\unit{\dfrac{W}{K}}\right]$ & $"+d[20]+r"$")
    # f.write("\n")
    # f.write(r"\\")
    # f.write("\n")
    # f.write(r"$f_0\left[\unit{Hz}\right]$ & $"+d[21]+r"$ & \quad & $T_d \left[\unit{s}\right]$ & $"+d[22]+r"$ & \quad & $R \left[\unit{\Omega}\right]$ & $"+d[23]+r"$")
    # f.write("\n")
    # f.write(r"\\")
    # f.write("\n")
    f.write(r"\end{tabular}")
    f.write("\n")
    # f.write(r"\end{footnotesize}")
    # f.write("\n")
    f.write(r"\caption{Dane wejściowe.}")
    f.write("\n")
    f.write(r"\end{table}")
    f.close()

def ts_data(name):
    d = []
    with open(p_ts_data+name[:3]+"_data.txt") as file:
        for line in file:
            d.append(line)
    data(p_ts_data+name[:3]+"_table.txt",d)
    
    
def ss_data(name):
    d = []
    with open(p_ss_data+name[:3]+"_data.txt") as file:
        for line in file:
            d.append(line)
    data(p_ss_data+name[:3]+"_table.txt",d)
    
def acs_data(name):
    d = []
    with open(p_acs_data+name[:4]+"_data.txt") as file:
        for line in file:
            d.append(line)
    data(p_acs_data+name[:4]+"_table.txt",d)


def getNames():
    files = glob.glob('../plt/**/*.txt', recursive=True)
    temp = []
    names = []

    for f in files:
        temp.append(f.split("\\")[-1])
        
    for i in range(len(temp)):
        temp[i] = temp[i][:-4]
        if temp[i][-1] != "N":
            names.append(temp[i])
    return names

def readFile(name):
    if name.find("ts") != -1:
        if name.find("bc1") != -1:
            return read_ts_bc1(name)
        if name.find("bc2") != -1:
            return read_ts_bc2(name)
        if name.find("bc3") != -1:
            return read_ts_bc3(name)
        if name.find("data")!=-1:
            return ts_data(name)

    if name.find("ss") != -1:
        if name.find("bc1") != -1:
            return read_ss_bc1(name)
        if name.find("bc2") != -1:
            print("bc2")
        if name.find("bc3") != -1:
            print("bc3")
        if name.find("data")!=-1:
            return ss_data(name)

    if name.find("acs") != -1:
        if name.find("u_matlab") != -1:
            return read_acs_mes(name)
        if name.find("obj_matlab") != -1:
            return read_acs_obj_matlab(name)
        if name.find("obj") != -1:
            return read_acs_obj(name)
        if name.find("mes") != -1:
            return read_acs_mes(name)
        if name.find("matlab") != -1:
            return read_acs_matlab(name)
        if name.find("data")!=-1:
            return acs_data(name)


def readFile_N(name):
    if name.find("ts") != -1:
        if name.find("bc1") != -1:
            return read_TN(p_ts_bc1+name)
        if name.find("bc2") != -1:
            return read_TN(p_ts_bc2+name)
        if name.find("bc3") != -1:
            return read_TN(p_ts_bc3+name)
    if name.find("acs") != -1 and name.find("mes") != -1:
        return read_TN(p_acs+name)


def read_TN(path):
    t = []
    T = []
    with open(path + "_TN.txt", "r") as file:
        for line in file:
            line = line.split(', ')
            line = line[:len(line)-1]
            t.append(float(line[0]))
            line = [float(line[i]) for i in range(1,len(line))]
            T.append(line)
    points = []
    with open(path + "_N.txt", "r") as file:
        for line in file:
            line = line.split(', ')
            line = [float(line[i]) for i in range(len(line))]
            points.append(line)
        return [t, T, points]
   
def read_acs_obj_matlab(name):
    T = []
    t=[]
    G=""
    firstLine = True
    with open(p_acs+name+".txt") as file:
        for line in file:
            if firstLine == True:
                G=line
                firstLine = False
                continue
            line = line.split(', ')
            line = [float(line[i]) for i in range(len(line))]
            t.append(line[0])
            T.append(line[1])
        return [G,t,T] 
   
def read_acs_matlab(name):
    T = []
    t=[]
    G=""
    firstLine = True
    with open(p_acs+name+".txt") as file:
        for line in file:
            if firstLine == True:
                G=line
                firstLine = False
                continue
            line = line.split(', ')
            line = [float(line[i]) for i in range(len(line))]
            t.append(line[0])
            T.append(line[1])
        return [G,t,T] 

def read_acs_mes(name):
    T = []
    t=[]
    with open(p_acs+name+".txt") as file:
        for line in file:
            line = line.split(', ')
            line = [float(line[i]) for i in range(len(line))]
            t.append(line[0])
            T.append(line[1])
        return [t,T] 

def read_acs_monitor(name):
    Tref = []
    Tj = []
    t=[]
    firstLine=True
    with open(p_acs+name+".txt") as file:
        for line in file:                
            line = line.split(', ')
            line = [float(line[i]) for i in range(len(line)-1)]
            if firstLine:
                z=line
                len_Tj=len(z)-1
                for j in range(len_Tj):
                    Tj.append([])
                firstLine=False
                continue
            t.append(line[0])
            Tref.append(line[1])
            for j in range(len(Tj)):    
                Tj[j].append(line[2+j])
        return [z,t,Tref,Tj] 

def read_acs_obj(name):
    firstLine = True
    dt = 0
    yIn = []
    T = []
    with open(p_acs+name+".txt") as file:
        for line in file:
            if firstLine == True:
                dt = float(line)
                firstLine = False
                continue
            line = line.split(', ')
            line = [float(line[i]) for i in range(len(line))]
            yIn.append(line[0])
            T.append(line[1])
        return [dt, yIn, T]

def read_ts_bc1(name):
    firstLine = True
    dt = 0
    x = 0
    T1 = 0
    T0 = 0
    lambdax = 0
    rho = 0
    cw = 0
    T = []
    with open(p_ts_bc1+name+".txt") as file:
        for line in file:
            if firstLine == True:
                line = line.split(', ')
                line = [float(line[i]) for i in range(len(line))]
                [dt, x, T0, lambdax, rho, cw, T1] = line
                firstLine = False
                continue
            T.append(float(line))
        return [dt, x, T0, lambdax, rho, cw, T1, T]


def read_ts_bc2(name):
    firstLine = True
    dt = 0
    x = 0
    qs = 0
    T0 = 0
    lambdax = 0
    rho = 0
    cw = 0
    T = []
    with open(p_ts_bc2+name+".txt") as file:
        for line in file:
            if firstLine == True:
                line = line.split(', ')
                line = [float(line[i]) for i in range(len(line))]
                [dt, x, T0, lambdax, rho, cw, qs] = line
                firstLine = False
                continue
            T.append(float(line))
        return [dt, x, T0, lambdax, rho, cw, qs, T]


def read_ts_bc3(name):
    firstLine = True
    dt = 0
    x = 0
    T1 = 0
    alpha = 0
    T0 = 0
    lambdax = 0
    rho = 0
    cw = 0
    T = []
    with open(p_ts_bc3+name+".txt") as file:
        for line in file:
            if firstLine == True:
                line = line.split(', ')
                line = [float(line[i]) for i in range(len(line))]
                [dt, x, T0, lambdax, rho, cw, T1, alpha] = line
                firstLine = False
                continue
            T.append(float(line))
        return [dt, x, T0, lambdax, rho, cw, T1, alpha, T]


def read_ss_bc1(name):
    firstLine = True
    T1 = 0
    T2 = 0
    lambdax = 0
    qv = 0
    x = []
    T = []
    i = 0
    with open(p_ss_bc1+name+".txt", "r") as file:
        for line in file:
            i = i+1
            line = line.split(', ')
            line = [float(line[i]) for i in range(len(line))]
            if firstLine == True:
                [lambdax, qv, T1, T2] = line
                firstLine = False
                continue
            x.append(line[0])
            T.append(line[1])
        return [x, T, lambdax, qv, T1, T2]
