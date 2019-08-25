#!/usr/bin/env python3

import sys

LROWS = 1001
NCOLUMNS = 30

# mode class definition
class Mode:
    fitted = False
    missing = False
    toBeFitted = False

# initialize 2D modes array
modes = [[Mode() for x in range(NCOLUMNS)] for y in range(LROWS)]

def addModesFromFile(filepath, fitted, missing, hasRedundantValues):
    global modes
    
    with open(filepath) as fp:
        line = fp.readline()
        while line:
            values = line.split(' ', 3)
            
            if not hasRedundantValues and len(values) > 1:
                l = int(values[0])
                n = int(values[1])
                modes[l][n].fitted = fitted
            elif hasRedundantValues and len(values) > 2:
                l = int(values[0])
                n = int(values[2])
                modes[l][n].missing = missing

            line = fp.readline()

def primeModeSearch(l, n, depth):
    global LROWS
    global NCOLUMNS
    global modes
    found = 0

    # search up
    for i in range(depth):
        if l - i >= 0 and modes[l - i][n].fitted:
            found += 1
            break
    
    # search down
    for i in range(depth):
        if l + i < LROWS and modes[l + i][n].fitted:
            found += 1
            break

    # search left
    for i in range(depth):
        if n - i >= 0 and modes[l][n - i].fitted:
            found += 1
            break

    # search right
    for i in range(depth):
        if n + i < NCOLUMNS and modes[l][n + i].fitted:
            found += 1
            break
    
    return found
        
def findModesToBeFitted():
    global LROWS
    global NCOLUMNS
    global modes
    
    for l in range(LROWS):
        for n in range(NCOLUMNS):
            if modes[l][n].missing and primeModeSearch(l, n, 1100) >= 3:
                modes[l][n].toBeFitted = True

def printModesToBeFitted():
    global LROWS
    global NCOLUMNS
    global modes

    for l in range(LROWS):
        for n in range(NCOLUMNS):
            if modes[l][n].toBeFitted:
                print(l, n)

def printAllModes():
    global LROWS
    global NCOLUMNS
    global modes

    for l in range(LROWS):
        for n in range(NCOLUMNS):
            if modes[l][n].toBeFitted:
                print("T", end = " ")
            elif modes[l][n].fitted:
                print("F", end = " ")
            elif modes[l][n].missing:
                print("M", end = " ")
            else:
                print("0", end = " ")
        print("")
                
def main():
    addModesFromFile(sys.argv[1], True, False, False)
    addModesFromFile(sys.argv[2], False, True, True)
    findModesToBeFitted()
    printModesToBeFitted()
    #printAllModes()
    
main()
