#!/usr/bin/env python

import subprocess
import datetime

def buildExecutables(executlableNames):
    for executlableName in executlableNames:
        subprocess.call('make -C ../' + executlableName, shell=True)

def cleanExecutables(executlableNames):
    for executlableName in executlableNames:
        subprocess.call('make clean -C ../' + executlableName, shell=True)

def getUniqueName(prefix, numOfPoints, shape):
    now = datetime.datetime.now()
    return '_'.join([prefix, str(numOfPoints), shape, str(now.month), str(now.day), str(now.hour), str(now.minute),
                     str(now.second)])

def getCorrectOutput(numOfPoints, dataFile, algorithmRunner):
    output = subprocess.check_output(
        'cat ' + dataFile + ' | ' + algorithmRunner + ' 1 ' + str(numOfPoints) + ' graham', shell=True)
    return parseOutput(output)

def isOutputCorrect(outputToCheck, correctOutput):
    parsedOutputToCheck = parseOutput(outputToCheck)
    if len(parsedOutputToCheck) == len(correctOutput):
        for point in parsedOutputToCheck:
            if not point in correctOutput:
                return False
        return True
    else:
        return False

def parseOutput(output):
    allParts = output.split('\n')
    points = [];
    for i in range(4, len(allParts)):
        xy = allParts[i].split(' ')
        if (len(xy) > 2):
            points.append((xy[0], xy[1]))
    return points