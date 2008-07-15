#!/bin/bash
basePath=../..
systemPath=$basePath/system
#Source a script that sets all important functions and variables
source $systemPath/rl-competition-includes.sh


#Utility functions from rl-competition-includes.sh

startRTSInBackGround
startRLGlueInBackGround

CLASSPATH=$VIZ_CLASSPATH:./bin/
if [[ `uname` == CYGWIN* ]]
then
	CLASSPATH=`cygpath -wp $CLASSPATH`
fi

java -DRLVIZ_LIB_PATH=$RLVIZ_LIB_PATH -Xmx128M -classpath $CLASSPATH RealTimeStrategyMultiTrainer

#Utility functions from rl-competition-includes.sh
waitForRTSToDie
waitForRLGlueToDie



