#!/bin/bash
basePath=../..
systemPath=$basePath/system
#Source a script that sets all important functions and variables
source $systemPath/rl-competition-includes.sh

#Name of the package the Agent is in.
packageName=RTSAgent 
className=RTSAgent    #Name of the agent class
maxMemory=128M			 #Max amount of memory to give the agent (Java default is often too low)
extraPath=./bin		 #Item for the class path so your agent can be found
startJavaAgent $extraPath $packageName $className $maxMemory

# 
# #!/bin/bash
# #Variables
# compLib=../../system/libraries/RLVizLib.jar
# 
# 
# java -Xmx128M -cp $compLib:./bin rlglue.agent.AgentLoader OLAgent
# 
# echo "-- Agent is complete"
# 
