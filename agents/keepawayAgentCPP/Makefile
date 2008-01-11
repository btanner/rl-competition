AGENT = RandomAgent.c
AGENT_OBJECTS = RandomAgent.o Glue_utilities.o

AGENT_SRC_PATH = ./src
BIN_PATH = ./bin
BUILD_PATH = Build

RL_GLUE_BASE_PATH =  ../../system/RL-Glue
RL_GLUE_PATH = $(RL_GLUE_BASE_PATH)/RL-Glue
UTILS_PATH   = $(RL_GLUE_BASE_PATH)/Utils

CFLAGS  = -I$(RL_GLUE_PATH)/ -I$(UTILS_PATH) -I$(AGENT_SRC_PATH) -ansi -pedantic -Wall

ifeq ($(suffix $(AGENT)),.cpp)
	CC = $(CXX)
endif

all: RL_agent

include $(RL_GLUE_PATH)/RL_glue.makefile

AGENT_OBJECTS += RL_client_agent.o RL_network.o

$(BIN_PATH)/RL_agent: $(addprefix $(BUILD_PATH)/, $(AGENT_OBJECTS))
	$(CC) -o $@ $^ -lm

$(BUILD_PATH)/$(basename $(AGENT)).o: $(AGENT_SRC_PATH)/$(AGENT) $(AGENT_SRC_PATH)/$(basename $(AGENT)).h
	$(CC) -c $(CFLAGS)  $< -o $@

clean:
	rm -rf $(BIN_PATH)/RL_agent $(BIN_PATH)/RL_agent.exe $(BUILD_PATH)/*

