#!/bin/bash


#Variables
basePath=../..
systemPath=$basePath/system
libPath=$systemPath/libraries

compLib=$libPath/RLVizLib.jar
envShellLib=$libPath/EnvironmentShell.jar 

glueExe=$systemPath/RL_glue

RLVIZ_LIB_PATH=$PWD/$libPath
ENV_CLASSPATH=$compLib:$envShellLib
VIZ_CLASSPATH=$compLib:./bin/

if [[ `uname` == CYGWIN* ]]
then
	glueExe="$glueExe.exe"
	RLVIZ_LIB_PATH=`cygpath -wp $RLVIZ_LIB_PATH`
	ENV_CLASSPATH=`cygpath -wp $ENV_CLASSPATH`
	VIZ_CLASSPATH=`cygpath -wp $VIZ_CLASSPATH`
fi

$glueExe &
gluePID=$!
echo "Starting up RL-glue - PID=$gluePID"

java -DRLVIZ_LIB_PATH=$RLVIZ_LIB_PATH -Xmx128M -classpath $ENV_CLASSPATH rlglue.environment.EnvironmentLoader environmentShell.EnvironmentShell  &
envShellPID=$!

echo "Starting up dynamic environment loader - PID=$envShellPID"

java -Xmx128M -classpath $VIZ_CLASSPATH consoleTrainer

echo "-- Console Trainer finished"

echo "-- Waiting for the Environment to die..."
wait $envShellPID
echo "   + Environment terminated"
echo "-- Waiting for the Glue to die..."
wait $gluePID
echo "   + Glue terminated"


