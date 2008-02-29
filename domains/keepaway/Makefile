#This should be an absolute path
AUTO_BOOST_ROOT_RELATIVE = $(shell pwd)/../../system/boost
BOOST_ROOT=$(shell cd $(AUTO_BOOST_ROOT_RELATIVE);pwd)

#if this build fails, try commenting BOOST_ROOT or setting it to nothing, to see if your system can
#auto detect the boost location

#Comment the above lines if you did NOT install boost with our automated script and your system could
#NOT find boost on its own.Then uncomment the next line and set it to the absolute path to your BOOST directory
#BOOST_ROOT=/home/username/

X11_PATH = /usr/X11R6

export RCSSBASE = $(shell pwd)/rcss


export CPPFLAGS = -I$(X11_PATH)/include
export PATH +=:$(RCSSBASE)/bin
export LD_LIBRARY_PATH=$(BOOST_ROOT)/lib:$(RCSSBASE)/lib:$(RCSSBASE)/lib/rcssserver/modules
export DYLD_FALLBACK_LIBRARY_PATH=$(BOOST_ROOT)/lib:$(RCSSBASE)/lib:$(RCSSBASE)/lib/rcssserver/modules

all: rcssbase rcssserver rcssmonitor tools player

rcssbase:	
	cd rcss-src/rcssbase-11.1.0 && ./configure --prefix=$(RCSSBASE) --with-boost=$(BOOST_ROOT)
	cd rcss-src/rcssbase-11.1.0 && make
	cd rcss-src/rcssbase-11.1.0 && make install

rcssserver:
	cd rcss-src/rcssserver-11.1.2 && ./configure --prefix=$(RCSSBASE) --with-boost=$(BOOST_ROOT)
	cd rcss-src/rcssserver-11.1.2 && make
	cd rcss-src/rcssserver-11.1.2 && make install

rcssmonitor:
	cd rcss-src/rcssmonitor-11.1.1 && ./configure --prefix=$(RCSSBASE) --with-boost=$(BOOST_ROOT) --x-includes=$(X11_PATH)/include --x-libraries=$(X11_PATH)/lib
	cd rcss-src/rcssmonitor-11.1.1 && make
	cd rcss-src/rcssmonitor-11.1.1 && make install

tools:
	cd keepaway-0.6/tools && make
	cd keepaway-0.6/tools && cp hist killserver kunzip kwyzipper kzip monitor winsum $(RCSSBASE)/bin

player:
	cd keepaway-0.6/player && make

clean:
	cd rcss-src/rcssbase-11.1.0 && make clean
	cd rcss-src/rcssserver-11.1.2 && make clean
	cd rcss-src/rcssmonitor-11.1.1 && make clean
	cd keepaway-0.6/tools && make clean
	cd keepaway-0.6/player && make clean

