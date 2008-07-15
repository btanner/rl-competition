#!/bin/bash
basePath=../..
systemPath=$basePath/system
#Source a script that sets all important functions and variables
source $systemPath/rl-competition-includes.sh

#Utility functions from rl-competition-includes.sh
startRLGlueInBackGround
startEnvShellInBackGround

CLASSPATH=$VIZ_CLASSPATH:./bin/
if [[ `uname` == CYGWIN* ]]
then
	CLASSPATH=`cygpath -wp $CLASSPATH`
fi

java -Xmx128M -classpath $CLASSPATH TetrisMultiTrainer

#Utility functions from rl-competition-includes.sh
waitForEnvShellToDie
waitForRLGlueToDie
