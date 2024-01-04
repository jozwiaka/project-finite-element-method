#!/usr/bin/env python
# -*- coding: utf8 -*-
from __future__ import unicode_literals
import codecs
import glob
import Files
import Plots
import numpy as np
from matplotlib import pyplot as plt
import Analitical

# Plot of relative errors for different z coordinates.
# get names


class SS:
    def getNames():
        files = glob.glob('../plt/ss/bc*/*.txt', recursive=True)
        temp = []
        names = []

        for f in files:
            temp.append(f.split("\\")[-1])

        for i in range(len(temp)):
            temp[i] = temp[i][:-4]
            if temp[i][-1] != "N":
                names.append(temp[i])
        names_sorted = list(np.zeros(shape=(len(names))))
        for i in range(len(names)):
            wyraz = int(names[i].split('_')[0].split('ss')[1])
            names_sorted[wyraz - 1] = names[i]
        return names_sorted

    def error_bc1(names):
        if names == []:
            return

        plt.figure()
        i=0
        for name in names:
            [x, T, lambdax, qv, T1, T2] = Files.readFile(name)
            x = np.array(x)
            Tmes = np.array(T)
            Ta = Analitical.ss_bc1(x, qv, lambdax,
                                   x[-1], T1, T2)
            dT = np.abs(Tmes-Ta)/Ta*100
            plt.plot(x, dT, label="$l_z="+str(i)+"$")
            i=i+1
        plt.xlabel("$z[m]$")
        plt.ylabel(
            "$\\dfrac{|T_{\\text{MES}}-T_\\text{{analit.}}|}{T_\\text{{analit.}}}\\cdot100\\%$")
        plt.legend()
        plt.grid()
        plt.savefig(Files.p_ss_errors + "error_ss_bc1" +
                    '.pdf', bbox_inches='tight')


class TS:
    def getNames():
        files = glob.glob('../plt/ts/bc*/*.txt', recursive=True)
        temp = []
        names = []

        for f in files:
            temp.append(f.split("\\")[-1])

        for i in range(len(temp)):
            temp[i] = temp[i][:-4]
            if temp[i][-1] != "N":
                names.append(temp[i])
        names_sorted = list(np.zeros(shape=(len(names))))
        for i in range(len(names)):
            wyraz = int(names[i].split('_')[0].split('ts')[1])
            names_sorted[wyraz - 1] = names[i]
        return names_sorted

    def error_bc1(names):
        if names == []:
            return

        plt.figure()
        for name in names:
            [dt, x, T0, lambdax, rho, cw, T1, T] = Files.readFile(name)
            [dt, T, points] = Files.readFile_N(name)
            points = np.array(points)
            z = points[:, 2]
            t = np.arange(0, len(T)*dt, dt)
            Tmes = np.array(T[-1][:])
            Ta = Analitical.ts_bc1(z, t[-1], T0, lambdax, rho, cw, T1)
            dT = np.abs(Tmes-Ta)/Ta*100
            plt.plot(z, dT, label="$\\Delta t="+f'{dt:.0f}'+"$ s")
        plt.xlabel("$z[m]$")
        plt.ylabel(
            "$\\dfrac{|T_{\\text{MES}}-T_\\text{{analit.}}|}{T_\\text{{analit.}}}\\cdot100\\%$")
        plt.legend()
        plt.grid()
        plt.savefig(Files.p_ts_errors + "error_ts_bc1" +
                    '.pdf', bbox_inches='tight')

    def error_bc2(names):
        if names == []:
            return

        plt.figure()
        for name in names:
            [dt, x, T0, lambdax, rho, cw, qs, T] = Files.readFile(name)
            [dt, T, points] = Files.readFile_N(name)
            points = np.array(points)
            z = points[:, 2]
            t = np.arange(0, len(T)*dt, dt)
            Tmes = np.array(T[-1][:])
            Ta = Analitical.ts_bc2(z, t[-1], T0, lambdax, rho, cw, qs)
            dT = np.abs(Tmes-Ta)/Ta*100
            plt.plot(z, dT, label="$\\Delta t="+f'{dt:.0f}'+"$ s")
        plt.xlabel("$z[m]$")
        plt.ylabel(
            "$\\dfrac{|T_{\\text{MES}}-T_\\text{{analit.}}|}{T_\\text{{analit.}}}\\cdot100\\%$")
        plt.legend()
        plt.grid()
        plt.savefig(Files.p_ts_errors + "error_ts_bc2" +
                    '.pdf', bbox_inches='tight')

    def error_bc3(names):
        if names == []:
            return

        plt.figure()
        for name in names:
            [dt, x, T0, lambdax, rho, cw, T1, alpha, T] = Files.readFile(name)
            t = np.arange(0, len(T)*dt, dt)
            [dt, T, points] = Files.readFile_N(name)
            points = np.array(points)
            z = points[:, 2]
            t = np.arange(0, len(T)*dt, dt)
            Tmes = np.array(T[-1][:])
            Ta = Analitical.ts_bc3(z, t[-1], T0, lambdax, rho, cw, T1, alpha)
            dT = np.abs(Tmes-Ta)/Ta*100
            plt.plot(z, dT, label="$\\Delta t="+f'{dt:.0f}'+"$ s")
        plt.xlabel("$z[m]$")
        plt.ylabel(
            "$\\dfrac{|T_{\\text{MES}}-T_\\text{{analit.}}|}{T_\\text{{analit.}}}\\cdot100\\%$")
        plt.legend()
        plt.grid()
        plt.savefig(Files.p_ts_errors + "error_ts_bc3" +
                    '.pdf', bbox_inches='tight')


bc = ["bc1", "bc2", "bc3"]

ss = SS
names = [[], [], []]
for name in ss.getNames():
    for i in range(len(bc)):
        if bc[i] in name:
            names[i].append(name)
ss.error_bc1(names[0])
# ss.error_bc2(names[1])
# ss.error_bc3(names[2])

ts = TS
names = [[], [], []]
for name in ts.getNames():
    for i in range(len(bc)):
        if bc[i] in name:
            names[i].append(name)
ts.error_bc1(names[0])
ts.error_bc2(names[1])
ts.error_bc3(names[2])
