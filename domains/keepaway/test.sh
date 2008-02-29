#!/bin/bash

# Keepaway startup script
#
# No commandline parameters.  All options are set in this file.
#

export BOOST_ROOT=../../system/boost

export RCSSBASE=./rcss

export PATH=$PATH:$RCSSBASE/bin
export LD_LIBRARY_PATH=$BOOST_ROOT/lib:$RCSSBASE/lib:$RCSSBASE/lib/rcssserver/modules
export DYLD_FALLBACK_LIBRARY_PATH=$BOOST_ROOT/lib:$RCSSBASE/lib:$RCSSBASE/lib/rcssserver/modules

# Top-level keepaway directory
keepaway_dir=`pwd`/keepaway-0.6
# Top-level rcssjava directory ** SET THIS OPTION **
rcssjava_dir=$keepaway_dir/rcssjava-0.1

############################################################
# Keeper options                                           #
############################################################

num_keepers=3                    # number of keepers
keeper_load=0                    # should I load previously learned weights?
keeper_load_dir=                 # sub-directory of weight_dir where weights are stored

i=0
while [ "$i" -ne "$num_keepers" ]
do
 echo Starting Taker \#$i...
 i=`expr $i + 1`
 echo $i
done