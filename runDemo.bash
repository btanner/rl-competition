#!/bin/bash


#Variables
basePath=.
systemPath=$basePath/system
libPath=$systemPath/libraries

compLib=$libPath/RLVizLib.jar

glueExe=$systemPath/RL_glue
guiLib=$libPath/forms-1.1.0.jar
envShellLib=$libPath/EnvironmentShell.jar

agentPath=agents/randomAgentJava
demoPath=trainers/guiTrainerJava

			   
RLVIZ_LIB_PATH=$PWD/$libPath

AGENT_CLASSPATH=$compLib:$agentPath/bin
ENV_CLASSPATH=$compLib:$envShellLib
VIZ_CLASSPATH=$compLib:$guiLib:$demoPath/bin/RLVizApp.jar

if [ `uname -o` = "Cygwin" ]
then
	RLVIZ_LIB_PATH=`cygpath -wp $RLVIZ_LIB_PATH`
	AGENT_CLASSPATH=`cygpath -wp $AGENT_CLASSPATH`
	ENV_CLASSPATH=`cygpath -wp $ENV_CLASSPATH`
	VIZ_CLASSPATH=`cygpath -wp $VIZ_CLASSPATH`
	glueExe="$glueExe.exe"
fi

#
# First do a quick check to see if the RL_glue exe exists.  If it does not, they probably have not done a "make all" yet
#
if [ ! -e "$glueExe" ]       # Check if file exists.
  then
    echo "RL_glue not found at $glueExe.  Did you remember to \"make all\"?"; echo
    exit 1                $Quit
   fi
#
#Start the RL_glue program  in the background and get it's process id
#
$glueExe &
gluePID=$!
echo "Starting up RL-glue - PID=$gluePID"

#
#Start the random agent in the background and get it's process id
#
java -Xmx128M -cp $AGENT_CLASSPATH rlglue.agent.AgentLoader RandomAgent.RandomAgent &
agentPID=$!


#
#Start the environment in the background and get it's process id
#
java -Xmx128M -DRLVIZ_LIB_PATH=$RLVIZ_LIB_PATH -cp $ENV_CLASSPATH rlglue.environment.EnvironmentLoader environmentShell.EnvironmentShell &
envShellPID=$!
echo "Starting up dynamic environment loader - PID=$envShellPID"

#
#Start the demo visualizer program
#
macAboutNameCommand=-Dcom.apple.mrj.application.apple.menu.about.name=RLVizApp
java -Xmx128M -DRLVIZ_LIB_PATH=$RLVIZ_LIB_PATH $macAboutNameCommand -cp $VIZ_CLASSPATH btViz.GraphicalDriver

echo "-- Visualizer is finished"

echo "-- Waiting for the Environment to die..."
wait $envShellPID
echo "   + Environment terminated"
echo "-- Waiting for the Agent to die..."
wait $agentPID
echo "   + Agent terminated"
echo "-- Waiting for the Glue to die..."
wait $gluePID
echo "   + Glue terminated"


