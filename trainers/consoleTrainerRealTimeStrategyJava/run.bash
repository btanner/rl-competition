#!/bin/bash


#Variables
basePath=../..
systemPath=$basePath/system
libPath=$systemPath/libraries
RLVIZ_LIB_PATH=$PWD/$libPath

compLib=$libPath/RLVizLib.jar

glueExe=$systemPath/RL_glue
rtsExe=$basePath/domains/realTimeStrategy/bin/rlgenv

VIZ_CLASSPATH=$compLib:./bin/

if [[ `uname` == CYGWIN* ]]
then
	glueExe="$glueExe.exe"
	rtsExe="$rtsExe.exe"
	RLVIZ_LIB_PATH=`cygpath -wp $RLVIZ_LIB_PATH`
	VIZ_CLASSPATH=`cygpath -wp $VIZ_CLASSPATH`
fi

#
# First do a quick check to see if the RL_glue exe exists.  If it does not, they probably have not done a "make all" yet
#
if [ ! -e "$rtsExe" ]       # Check if file exists.
  then
    echo "rlgenv not found at $rtsExe.  Did you remember to \"make rlgenv\" in domains/realTimeStrategy ?"; echo
    exit 1                $Quit
   fi

#
# First do a quick check to see if the RL_glue exe exists.  If it does not, they probably have not done a "make all" yet
#
if [ ! -e "$glueExe" ]       # Check if file exists.
  then
    echo "RL_glue not found at $glueExe.  Did you remember to \"make all\"?"; echo
    exit 1                $Quit
   fi




$glueExe &
gluePID=$!
echo "Starting up RL-glue - PID=$gluePID"

$rtsExe &
rtsPID=$!
echo "Starting up real time strategy game - PID=$rtsPID"

java -DRLVIZ_LIB_PATH=$RLVIZ_LIB_PATH -Xmx128M -classpath $VIZ_CLASSPATH realTimeStrategyTrainerJava

echo "-- Console Real Time Strategy Trainer finished"

echo "-- Waiting for the real time strategy game to die..."
wait $rtsPID
echo "   + real time strategy game terminated"
echo "-- Waiting for the Glue to die..."
wait $gluePID
echo "   + Glue terminated"


