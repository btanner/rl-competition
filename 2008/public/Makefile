BIN_PATH = ./system
RL_GLUE_PATH = ./system/RL-Glue/RL-Glue
BUILD_PATH = ./system/build

AGENT_NETWORKED=1
ENV_NETWORKED=1
EXP_NETWORKED=1

CFLAGS = -I$(RL_GLUE_PATH) -O2
LDFLAGS = -lm 

all: $(BIN_PATH)/RL_glue
	cd ./agents/randomAgentJava/ && make
	cd ./agents/randomAgentCPP/ && make
	cd ./agents/mountainCarAgentCPP/ && make
	cd ./agents/realTimeStrategyAgentJava/ && make
	cd ./agents/helicopterAgentCPP/ && make
	cd ./agents/zeroAgentCPP/ && make
	cd ./trainers/consoleTrainerJava/ && make
	cd ./trainers/consoleTrainerPython/ && make
	cd ./trainers/consoleTrainerCPP/ && make
	cd ./trainers/consoleTrainerRealTimeStrategyJava/ && make
	cd ./trainers/consoleMultiTrainers/ && make

clean:
	-rm -f $(BUILD_PATH)/*
	-rm -f $(BIN_PATH)/RL_glue
	-rm -f $(BIN_PATH)/RL_glue.exe
	-rm -f ./system/includes/*.pyc
	-rm -f $(RL_GLUE_PATH)/Python/rlglue/*.pyc
	-rm -f $(RL_GLUE_PATH)/Python/rlglue/network/*.pyc
	-rm -f $(RL_GLUE_PATH)/Python/rlglue/agent/*.pyc
	-rm -f $(RL_GLUE_PATH)/Python/rlglue/environment/*.pyc
	-rm -f agents/randomAgentPython/src/*.pyc
	cd ./agents/randomAgentJava/ && make clean
	cd ./agents/randomAgentCPP/ && make clean
	cd ./agents/mountainCarAgentCPP/ && make clean
	cd ./agents/realTimeStrategyAgentJava/ && make clean
	cd ./agents/helicopterAgentCPP/ && make clean
	cd ./agents/zeroAgentCPP/ && make clean
	cd ./trainers/consoleTrainerJava/ && make clean
	cd ./trainers/consoleTrainerPython/ && make clean
	cd ./trainers/consoleTrainerCPP/ && make clean
	cd ./trainers/consoleTrainerRealTimeStrategyJava/ && make clean
	cd ./trainers/consoleMultiTrainers/ && make clean

include $(RL_GLUE_PATH)/RL_glue.makefile
