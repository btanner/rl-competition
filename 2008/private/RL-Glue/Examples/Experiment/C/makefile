# ------------------------------------------------------------------
# Changes these values to use different source files and different 
# directories 
# ------------------------------------------------------------------
EXPERIMENT = experiment.c
EXP_SRC_PATH = ./src
BUILD_PATH = ./Build
RL_GLUE_BASE = ../../..
# ------------------------------------------------------------------

EXP_OBJECTS = $(basename $(EXPERIMENT)).o
RL_GLUE_PATH = $(RL_GLUE_BASE)/RL-Glue
UTILS_PATH = $(RL_GLUE_BASE)/Utils

CFLAGS = -I$(RL_GLUE_PATH) -I$(UTILS_PATH) -I$(EXP_SRC_PATH)

all: RL_experiment

include $(RL_GLUE_PATH)/RL_glue.makefile

EXP_OBJECTS += RL_client_experiment.o RL_network.o

RL_experiment: $(addprefix $(BUILD_PATH)/,$(EXP_OBJECTS)) 
	$(CC) -o RL_experiment $^

$(BUILD_PATH)/$(basename $(EXPERIMENT)).o: $(EXP_SRC_PATH)/$(EXPERIMENT)
	$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm -rf RL_experiment RL_experiment.exe $(BUILD_PATH)/*
