# -*- coding: utf-8 -*-
"""
Created on Fri Apr 29 19:57:04 2022

@author: arekj

"""
import matplotlib.pyplot as plt
import numpy as np

def TtoColor(T):
    T=T-273
    colors = ['darkblue', 'blue', 'royalblue', 'cornflowerblue', 'lightsteelblue',
              'turquoise','aquamarine',
              'springgreen','greenyellow',
              'yellow','gold',
              'orange','darkorange',
              'tomato', 'firebrick', 'red' ]
    if T <= -20: return colors[0]
    if T >-20 and T <= -15: return colors[1]
    if T >-15 and T <= -10: return colors[2]
    if T >-10 and T <= -5: return colors[3]
    if T >-5 and T <= 0: return colors[4]
    #green/blue
    if T >0 and T <= 5: return colors[5]
    if T >5 and T <= 10: return colors[6]
    #green
    if T >10 and T <= 15: return colors[7]
    if T >15 and T <= 20: return colors[8]
    #yellow
    if T >20 and T <= 25: return colors[9]
    if T >25 and T <= 30: return colors[10]
    #orange
    if T >30 and T <= 35: return colors[11]
    if T >35 and T <= 40: return colors[12]
    #red
    if T >40 and T <= 45: return colors[13]
    if T >45 and T <= 50: return colors[14]
    if T >50: return colors[15]
  