#!/bin/bash


#Variables
basePath=../..
systemPath=$basePath/system
libPath=$systemPath/libraries

compLib=$libPath/RLVizLib.jar

glueExe=$systemPath/RL_glue
guiLib=$libPath/forms-1.1.0.jar
envShellLib=$libPath/EnvironmentShell.jar

RLVIZ_LIB_PATH=$PWD/$libPath

ENV_CLASSPATH=$compLib:$envShellLib
VIZ_CLASSPATH=$compLib:$guiLib:./bin/RLVizApp.jar

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

java -Xmx128M -DRLVIZ_LIB_PATH=$RLVIZ_LIB_PATH -classpath $ENV_CLASSPATH rlglue.environment.EnvironmentLoader environmentShell.EnvironmentShell &
envShellPID=$!
echo "Starting up dynamic environment loader - PID=$envShellPID"

#
#Start the visualizer program
#
macAboutNameCommand=-Dcom.apple.mrj.application.apple.menu.about.name=RLVizApp
java -Xmx128M -DRLVIZ_LIB_PATH=$RLVIZ_LIB_PATH $macAboutNameCommand -classpath $VIZ_CLASSPATH btViz.GraphicalDriver

echo "-- Visualizer is finished"

echo "-- Waiting for the Environment to die..."
wait $envShellPID
echo "   + Environment terminated"
echo "-- Waiting for the Glue to die..."
wait $gluePID
echo "   + Glue terminated"


