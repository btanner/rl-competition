libPath=$systemPath/libraries
compLib=$libPath/RLVizLib.jar

glueExe=$systemPath/RL_glue
rtsExe=$basePath/domains/realTimeStrategy/bin/rlgenv

guiLib=$libPath/forms-1.1.0.jar
envShellLib=$libPath/EnvironmentShell.jar

RLVIZ_LIB_PATH=$PWD/$libPath
ENV_CLASSPATH=$compLib:$envShellLib
VIZ_CLASSPATH=$compLib:$guiLib:./bin/RLVizApp.jar

setMacAboutName ()
{ # This is about as simple as functions get.
if [ "$(uname)" == "Darwin" ]; then
	osExtras="-Dcom.apple.mrj.application.apple.menu.about.name=RLVizApp"
fi
}

setCygwinPaths ()
{
if [[ `uname` == CYGWIN* ]]
then
	glueExe="$glueExe.exe"
	rtsExe="$rtsExe.exe"
	RLVIZ_LIB_PATH=`cygpath -wp $RLVIZ_LIB_PATH`
	ENV_CLASSPATH=`cygpath -wp $ENV_CLASSPATH`
	VIZ_CLASSPATH=`cygpath -wp $VIZ_CLASSPATH`
fi
}

makeLine(){
echo "--------------------------------------------------------------------"
}
checkIfRTSExists(){
if [ ! -e "$rtsExe" ]       # Check if file exists.
  then
  	makeLine
    echo "Error :: rlgenv not found at $rtsExe."
  	echo "Did you remember to \"make rlgenv\" in domains/realTimeStrategy ?"
  	makeLine
    exit 1
   fi
}

checkIfRLGlueExists(){
if [ ! -e "$glueExe" ]       # Check if file exists.
  then
    echo "RL_glue not found at $glueExe.  Did you remember to \"make \" from the main competition directory?"; echo
    exit
   fi
}


startRTSInBackGround(){
checkIfRTSExists
$rtsExe &
rtsPID=$!
echo "Starting up Real Time Strategy - PID=$rtsPID"
}
waitForRTSToDie(){
echo "-- Waiting for  Real Time Strategy to die..."
wait $rtsPID
echo "++ Real Time Strategy terminated"
}


startRLGlueInBackGround(){
checkIfRLGlueExists
$glueExe &
gluePID=$!
echo "Starting up RL-glue - PID=$gluePID"
}

startEnvShellInBackGround(){
java -Xmx128M -DRLVIZ_LIB_PATH=$RLVIZ_LIB_PATH -classpath $ENV_CLASSPATH rlglue.environment.EnvironmentLoader environmentShell.EnvironmentShell &
envShellPID=$!
echo "-- Starting up dynamic environment loader - PID=$envShellPID"
}

startGuiTrainer(){
echo "-- Starting up Gui Trainer"
java -Xmx128M -DRLVIZ_LIB_PATH=$RLVIZ_LIB_PATH $osExtras -classpath $VIZ_CLASSPATH btViz.GraphicalDriver
echo "++ Gui Trainer is finished"
}

startRTSGuiTrainer(){
echo "-- Starting up Gui Trainer"
java -DRLVIZ_LIB_PATH=$RLVIZ_LIB_PATH $osExtras -Xmx128M -classpath $VIZ_CLASSPATH btViz.NoDynamicLoadingGraphicalDriver
echo "++ Gui Trainer is finished"
}

waitForEnvShellToDie(){
echo "-- Waiting for the dynamic environment loader to die..."
wait $envShellPID
echo "++ Dynamic environment loader terminated"
}

waitForRLGlueToDie(){
echo "-- Waiting for RL_glue to die..."
wait $gluePID
echo "++ RL_glue terminated"
}

startJavaAgent(){
privateExtraPath="$1"
privatePackageName="$2"
privateClassName="$3"
privateMaxMemory="$4"
echo "Extra Path is $privateExtraPath"
echo "Package is $privatePackageName"
echo "Class is $privateClassName"
echo "Max Memory is $privateMaxMemory"
java -Xmx$privateMaxMemory -cp $compLib:$privateExtraPath rlglue.agent.AgentLoader $privatePackageName.$privateClassName
}
setMacAboutName
setCygwinPaths
