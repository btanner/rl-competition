#!/bin/bash
basePath=../..
systemPath=$basePath/system
#Source a script that sets all important functions and variables
source $systemPath/rl-competition-includes.sh


#Utility functions from rl-competition-includes.sh

startRTSInBackGround
startRLGlueInBackGround

startRTSGuiTrainer

#Utility functions from rl-competition-includes.sh
waitForRTSToDie
waitForRLGlueToDie


