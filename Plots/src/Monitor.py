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

p_j = [             "$p_1=(0.1\,\\text{m},\,  0.25\,\\text{m}, \, 0.3\,\\text{m})$",
                    "$p_2=(0.2\,\\text{m},\,  0.25\,\\text{m}, \,0.15\,\\text{m})$",
                    "$p_3=(0.2\,\\text{m},\,  0.25\,\\text{m}, \,0.45\,\\text{m})$",
                    "$p_4=(0.2\,\\text{m},\, 0.375\,\\text{m}, \,0.15\,\\text{m})$",
                    "$p_5=(0.3\,\\text{m},\,  0.25\,\\text{m}, \,0.15\,\\text{m})$",
                    "$p_6=(0.3\,\\text{m},\,  0.25\,\\text{m}, \, 0.3\,\\text{m})$",
                    "$p_7=(0.3\,\\text{m},\, 0.375\,\\text{m}, \,0.15\,\\text{m})$"]
p_ref = "$p_{\\text{ref}}=(0.2\,\\text{m},\,  0.25\,\\text{m},  \,0.3\,\\text{m})$"


class Monitor:
    def getNames():
        files = glob.glob('../plt/acs/acs*_monitor.txt', recursive=True)
        names = []

        for f in files:
            temp = f.split("\\")[-1]
            temp = temp[:-4]
            names.append(temp)
        return names

    def relErr():
        [z, t, Tref, Tj] = Files.read_acs_monitor(name)

        plt.figure()
        z = np.array(z)
        Tref = np.array(Tref)
        for j in range(len(Tj)):
            if j != 21 and j != 4 and j != 12 and j != 22 and j != 24 and j != 15 and j != 14:
                continue
            Ti = np.array(Tj[j])
            dT = np.abs(Tref-Ti)/Tref*100
            plt.plot(t, dT, label="$z =" + str(z[j+1])+"$ m")
        plt.axhline(y=20, color='r', linestyle='--')
        plt.xlabel("$t$[s]")
        plt.ylabel(
            "$\\dfrac{|T_{\\text{point}}-T_\\text{{ref}}|}{T_\\text{{ref}}}\\cdot100\\%$")
        plt.legend()
        plt.grid()
        plt.savefig(Files.p_acs + name[:4]+"_monitor" +
                    '.pdf', bbox_inches='tight')

    def Tref_Tj():
        [z, t, Tref, Tj] = Files.read_acs_monitor(name)

        z = np.array(z)
        Tref = np.array(Tref)
        plt.figure()
        plt.plot(t, Tref, label="$z_{\\text{ref}} =" +
                 str(z[0])+"$ m", markersize=20)
        plt.plot(t, 1.2*Tref, color='r', linestyle='--')
        #plt.plot(t, 0.8*Tref, color='r', linestyle='--')
        for j in range(len(Tj)):
            if j != 21 and j != 4 and j != 12 and j != 22 and j != 24 and j != 15 and j != 14:
                continue
            Ti = np.array(Tj[j])
            plt.plot(t, Ti, label="$z =" + str(z[j+1])+"$ m")
        plt.xlabel("$t$[s]")
        plt.ylabel(
            "$T$[K]")
        plt.legend()
        plt.grid()
        plt.savefig(Files.p_acs + name[:4]+"_Tref_Tj" +
                    '.pdf', bbox_inches='tight')

    def relErr_2():
        [z, t, Tref, Tj] = Files.read_acs_monitor(name)

        plt.figure()
        z = np.array(z)
        Tref = np.array(Tref)
        i = 0
        for j in range(len(Tj)):
            if j != 21 and j != 4 and j != 12 and j != 22 and j != 24 and j != 15 and j != 14:
                continue
            Ti = np.array(Tj[j])
            dT = np.abs(Tref-Ti)/Tref*100
            plt.plot(t, dT, label=p_j[i])
            i = i+1
        plt.axhline(y=20, color='r', linestyle='--')
        plt.xlabel("$t$[s]")
        plt.ylabel(
            "$\\dfrac{|T_{\\text{point}}-T_\\text{{ref}}|}{T_\\text{{ref}}}\\cdot100\\%$")
        plt.legend(bbox_to_anchor=(-0.015,-0.68), loc=3)
        plt.grid()
        plt.savefig(Files.p_acs + name[:4]+"_monitor" +
                    '.pdf', bbox_inches='tight')

    def Tref_Tj_2():
        [z, t, Tref, Tj] = Files.read_acs_monitor(name)

        z = np.array(z)
        Tref = np.array(Tref)
        plt.figure()
        plt.plot(t, Tref, label=p_ref, markersize=20)
        plt.plot(t, 1.2*Tref, color='r', linestyle='--')
        #plt.plot(t, 0.8*Tref, color='r', linestyle='--')
        i = 0
        for j in range(len(Tj)):
            if j != 21 and j != 4 and j != 12 and j != 22 and j != 24 and j != 15 and j != 14:
                continue
            Ti = np.array(Tj[j])
            plt.plot(t, Ti, label=p_j[i])
            i = i+1
        plt.xlabel("$t$[s]")
        plt.ylabel(
            "$T$[K]")
        plt.legend(bbox_to_anchor=(-0.015,-0.75), loc=3)
        plt.grid()
        plt.savefig(Files.p_acs + name[:4]+"_Tref_Tj" +
                    '.pdf', bbox_inches='tight')


monitor = Monitor
names = monitor.getNames()
for name in monitor.getNames():
    monitor.Tref_Tj_2()
    monitor.relErr_2()
