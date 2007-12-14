#!/bin/bash


basePath=.
systemPath=$basePath/system
#Source a script that sets all important functions and variables
source $systemPath/rl-competition-includes.sh

##Agent Stuff
AgentExtraPath=agents/randomAgentJava/bin		 #Item for the class path so your agent can be found
AgentPackageName=RandomAgent  #Name of the package the Agent is in.
AgentClassName=RandomAgent    #Name of the agent class
AgentMaxMemory=128M			 #Max amount of memory to give the agent (Java default is often too low)

startRLGlueInBackGround
startEnvShellInBackGround
startJavaAgentInBackGround $AgentExtraPath $AgentPackageName $AgentClassName $AgentMaxMemory
startGuiTrainer

waitForAgentToDie
waitForEnvShellToDie
waitForRLGlueToDie
