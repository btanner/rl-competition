RL_GLUE_BASE_PATH =  ../../system/RL-Glue
RL_GLUE_PATH = $(RL_GLUE_BASE_PATH)/RL-Glue
UTILS_PATH   = $(RL_GLUE_BASE_PATH)/Utils

BUILD_PATH = Build
SRC_PATH = src

CFLAGS  = -I$(RL_GLUE_PATH)/ -I$(UTILS_PATH) -ansi -pedantic -Wall

AGENT = RandomAgent.c
AGENT_OBJECTS = RandomAgent.o Glue_utilities.o

all: RL_agent

include $(RL_GLUE_PATH)/RL_glue.makefile

AGENT_OBJECTS += RL_client_agent.o RL_network.o

RL_agent: $(addprefix $(BUILD_PATH)/, $(AGENT_OBJECTS))
	$(CC) -o $@ $^ -lm

$(BUILD_PATH)/RandomAgent.o: $(SRC_PATH)/RandomAgent.c $(SRC_PATH)/RandomAgent.h
	$(CC) -c $(CFLAGS)  $< -o $@

clean:
	rm -rf RL_agent RL_agent.exe $(BUILD_PATH)/*

