RLVizPath = ../../system/libraries
SRC_PATH = src
BIN_PATH = bin

all: 
	javac $(SRC_PATH)/**/*.java -d $(BIN_PATH) -cp $(RLVizPath)/RLVizLib.jar

RL_agent:
	javac $(SRC_PATH)/RTSAgent/RTSAgent.java -d $(BIN_PATH)/ -cp $(RLVizPath)/RLVizLib.jar

OLJ:
	javac $(SRC_PATH)/RTSAgent/Parameters.java -d $(BIN_PATH)

clean:
	-rm -rf $(BIN_PATH)/*.class
