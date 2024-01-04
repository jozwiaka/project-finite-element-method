# -*- coding: utf-8 -*-
import os
import glob

def rm(ext):
    files = glob.glob('../plt/**/*'+ext, recursive=True)
    
    for f in files:
        try:
            os.remove(f)
        except OSError as e:
            print("Error: %s : %s" % (f, e.strerror))
            
def rm():
    files = glob.glob('../plt/**/*'+".txt", recursive=True)
    
    for f in files:
        try:
            os.remove(f)
        except OSError as e:
            print("Error: %s : %s" % (f, e.strerror))
    
    files = glob.glob('../plt/**/*'+".pdf", recursive=True)
    
    for f in files:
        try:
            os.remove(f)
        except OSError as e:
            print("Error: %s : %s" % (f, e.strerror))

