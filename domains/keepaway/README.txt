Installation instructions for Keep-away
==========================================


This documentation isn't finalized yet, the Keepaway code won't be intergrated with the competition software until the Proving-SP1 release (probably December 7/2007).

 
Overview
Three separate pieces of software need to be installed in order to run Keepaway for the RL competition:

   1. X11: Linux or Mac
   2. Boost
   3. RL-GLue Keepaway Module

Below are instructions for doing the first step under Linux and Mac OS X. The second and third steps are the same, regardless of the platform. The Linux installation has been tested on Ubuntu, and the MAC OS X on Tiger. The shell commands listed below are for the bash shell, and may have to be changed slightly for other shells. For instance, "export variable=value" under bash should be replaced with "setenv variable value" under csh.

We have put a significant amount of time into this code and have attempted to make the install procedure as simple as possible. That said, it's certianly not trivial. If you have questions about installing Keepaway, we encourage you to ask us! We plan to continually update this document as people find confusing bits.

In all of this, when we write rl-competition/, we mean /path/to/wherever/you/installed/rl-competition  

X11 - Linux

Your sysadmin may have put X11 in a nonstandard location. To get around this, first try to find your system's X11 include directory:
$> locate X11/Xlib.h
Add the path you find that contains this X11 directory (called something like /blah/X11R6/include) as a default compilation option:
$> export CPPFLAGS=-I//X11R6/include
For example:
$>export CPPFLAGS=-I/usr/X11R6/include

Mac: Developer Tools and X11
1) You need to install the latest version of Xcode, from http://developer.apple.com/tools/xcode. If you are a power user and for some reason had to change the default gcc to 3.3 at some point, be sure to change it it back to 4.0 using gcc_select 4.0
2) You need to have X11 installed. If you don't have it already, go to http://developer.apple.com/opensource/tools/runningx11.html and http://developer.apple.com/opensource/tools/X11.html.
Quoting from the website:

    "X11 is available as an optional install on the Mac OS X v10.3 Panther, and Mac OS X v10.4 Tiger install disks. Run the Installer, select the X11 option, and follow the instructions. You should install the X11 SDK as well, which is included on the Panther Developer CD. If you intend to download X11 source code and build your own binaries, you will need the tools and headers included in the SDK."

I'm not sure what the case is with Leopard, but it's probably about the same.
3) Some people don't know that they can still use X11 apps from inside Terminal.app. You just need to set your DISPLAY environment variable: export DISPLAY=:0.0
or
setenv DISPLAY :0.0
and it's also helpful to add /usr/X11R6/bin to your PATH.

Boost
boost >= 1.33

       If running on a Linux system, this is probably already done
       for you. Try building the boost test in ../support/misc/boost_test. If it
       compiles and runs, then boost is already installed. If not, then
       it's likely easily obtained by using your package manager; search
       for packages named "boost" and/or "boost-dev". Otherwise, follow
       instructions below.

        Note: If you are not comfortable installing boost or are not sure
        what to do next, we have included a script to install it for you. 
        This script will install a local copy of boost, inside the
        rl-competition software distribution.  You do not need root access to
        install it.  Be warned, if some files are missing on your computer,
        you may see a MASSIVE amount of errors when building.  It is probably
        still working.  Also note, this can take up to 30 minutes. 
       
        To run the script:

$>cd ../support  (to rl-competition/domains/support)
$>bash install-boost.bash

         Or, to manually install boost to your home directory, do the following:
$>cd ../support/misc
$>tar xjvf boost_1_34_1.tar.bz2
$>cd boost_1_34_1
$>./configure --prefix=$HOME
$>make
$>make install

       If you see a message indicating that not all the libraries were
       built properly, that is OK. It is likely that the optional python
       libraries were not built.

       Lastly, create this symbolic link.

$>cd $HOME/include
$>ln -s boost-1_34_1/boost

       Now, build and run the test program in misc/boost_test to verify
       that it worked.

RL-Glue Keepaway Module

1. Edit rl-competition/domains/keepaway/Makefile and set BOOST_ROOT and X11_PATH to the appropriate locaitons.

2. Build the project:
$> cd rl-competition/domains/keepaway
$> make
If the build process fails then most likely Boost or X11 are not set up correctly.

3. The next step is to build a sample RL-Glue agent that behaves randomly. Your agent for the RL Competition may be in C, Java, or Python. We have included a C agent so that you can see what is required.
$> cd rl-competition/agents/keepawayAgentCPP
$> make

4. Now that both the RL-Glue keepers and sample RL-Glue agent have been installed, try running keepaway.
First, start the RL-Glue agent:
$> cd rl-competiton/agents/keepawayAgentCPP
$> ./run.bash
Then, start the Soccer Server and the Keepaway players:
$> cd rl-competition/domains/keepaway
$> ./keepaway_RLGlue.sh

If everything is working correctly, one keeper will be controlled by the random RL-Glue agent, and two keepers will use the hand-coded keeper policy.