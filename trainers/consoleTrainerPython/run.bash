#!/bin/bash

#Variables

systemPath=../../system
libPath=$systemPath/libraries
RLVIZ_LIB_PATH=$PWD/$libPath


compLib=$libPath/RLVizLib.jar
envShellLib=$libPath/EnvironmentShell.jar

glueExe=$systemPath/RL_glue

ENV_CLASSPATH=$compLib:$envShellLib

if [ `uname -o` = "Cygwin" ]
then
	glueExe="$glueExe.exe"
	RLVIZ_LIB_PATH=`cygpath -wp $RLVIZ_LIB_PATH`
	ENV_CLASSPATH=`cygpath -wp $ENV_CLASSPATH`
fi

$glueExe &
gluePID=$!
echo "Starting up RL-glue - PID=$gluePID"


java -DRLVIZ_LIB_PATH=$RLVIZ_LIB_PATH -Xmx128M -classpath $ENV_CLASSPATH rlglue.environment.EnvironmentLoader environmentShell.EnvironmentShell &
envShellPID=$!
echo "Starting up dynamic environment loader - PID=$envShellPID"

./RL_experiment

echo "-- Console Trainer finished"

echo "-- Waiting for the Environment to die..."
wait $envShellPID
echo "   + Environment terminated"
echo "-- Waiting for the Glue to die..."
wait $gluePID
echo "   + Glue terminated"
