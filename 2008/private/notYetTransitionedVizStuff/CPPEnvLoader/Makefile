BIN_PATH=../../bin
OUT_PATH=../../libraries
INCLUDE_PATH=../../includes

ENV_HEADERS=../../system/rl-viz
GET_BACK=../../notYetTransitionedVizStuff/CPPEnvLoader

ifndef JAVA_HOME
	JAVA_HOME=/Library/Java/Home
endif

# supported substrings of OSTYPE
# The following words must be uppercase
# If they are found in the uppercased OSTYPE variable
# the particular O/S matches
OSTYPE_LINUX  := LINUX
OSTYPE_MAC    := DARWIN
OSTYPE_CYGWIN := CYGWIN
OSTYPE_MINGW  := MSYS

# convert OSTYPE to uppercase

ifeq ("$(OSTYPE)", "") 
	OSTYPE := $(shell uname | grep -i -E "(Darwin)|(Linux)|(CYGWIN)")
	OSTYPE := $(shell echo "$(OSTYPE)" | tr a-z A-Z )
else
	OSTYPE := $(shell echo -n "$(OSTYPE)" | tr a-z A-Z )
endif

CC=g++

LIB_HEADERS=environmentShell_LocalCPlusPlusEnvironmentLoader.h environmentShell_JNIEnvironment.h

ifeq ("$(OSTYPE)","$(OSTYPE_MAC)")
	SFLAGS := -dynamiclib
else 
	SFLAGS := -shared
endif

CPPENV.dylib: $(LIB_HEADERS)
	$(CC) -o $(OUT_PATH)/CPPENV.dylib $(SFLAGS) -fPIC -I $(JAVA_HOME)/include -I../../includes -I../../RL-Glue/RL-Glue src/CPlusPlusEnvironmentLoader.cpp $(INCLUDE_PATH)/ParameterHolder.cpp
	
environmentShell_LocalCPlusPlusEnvironmentLoader.h:
	cd $(ENV_HEADERS) && unzip EnvironmentShell.jar
	cd $(ENV_HEADERS) && javah environmentShell.JNIEnvironment && mv environmentShell_JNIEnvironment.h $(GET_BACK)/src

environmentShell_JNIEnvironment.h:
	cd $(ENV_HEADERS) && javah environmentShell.LocalCPlusPlusEnvironmentLoader && mv environmentShell_LocalCPlusPlusEnvironmentLoader.h $(GET_BACK)/src

clean:
	cd $(ENV_HEADERS) && rm -rf META-INF
	cd $(ENV_HEADERS) && rm -rf environmentShell
	rm -f $(OUT_PATH)/CPPENV.dylib
