#!/bin/bash

# Keepaway startup script
# 
# No commandline parameters.  All options are set in this file.
#

#  $1 - stop after $1 episodes
#  $2 - size of field is $2 x $2
#  $3 - player speed.
#  $4 - quantize step: 0.$4 and 0.0$4
#  $5 - log file name: $5


export BOOST_ROOT=../system/boost

export RCSSBASE=../domains/keepaway/rcss

export PATH=$PATH:$RCSSBASE/bin
export LD_LIBRARY_PATH=$BOOST_ROOT/lib:$RCSSBASE/lib:$RCSSBASE/lib/rcssserver/modules
export DYLD_FALLBACK_LIBRARY_PATH=$BOOST_ROOT/lib:$RCSSBASE/lib:$RCSSBASE/lib/rcssserver/modules


echo $1
echo $2
echo $3
echo $4
echo $5

# Top-level keepaway directory
keepaway_dir=../domains/keepaway/keepaway-0.6
# Top-level rcssjava directory ** SET THIS OPTION **
rcssjava_dir=$keepaway_dir/rcssjava-0.1

############################################################
# Keeper options                                           #
############################################################

num_keepers=3                    # number of keepers
keeper_load=0                    # should I load previously learned weights?
keeper_load_dir=                 # sub-directory of weight_dir where weights are stored

############################################################
# Taker options                                            #
############################################################

num_takers=2                     # number of takers
taker_load=0                     # should I load previously learned weights?
taker_load_dir=                  # sub-directory of weight_dir where weights are stored
taker_learn=0                    # should learning be turned on for takers?
#taker_policy="learned"          # policy followed by takers
taker_policy="hand"

############################################################
# Client options                                           #
############################################################

save_weights=0                    # should I save learned weights
weight_dir=$keepaway_dir/weights  # top-level weight directory
save_client_log=0                 # should I save client logging info to a file?
log_level="1..1000"               # range of log levels to store
save_client_draw_log=0            # should I save client logged shape info to a file?
client_log_dir=$keepaway_dir/logs # top-level client log directory
client_dir=$keepaway_dir/player   # directory containing player binary
client=keepaway_player            # name of player binary
#stop_after=$1                     # stop after num episodes
stop_after=-1

if [ -n "$1" ]; then
	stop_after=$1                     # stop after num episodes
fi

echo LearningAgent should die after $stop_after episodes
	
start_learning_after=-1           # start learning after num episodes

############################################################
# Server options                                           #
############################################################
ka_width=20
#ka_width=$2                      # Y-axis size of playing region
if [ -n "$2" ]; then
	ka_width=$2
fi
#${ka_width:-20}
ka_length=$ka_width              # X-axis size of playing region

unrestricted_vision=1            # should I use 360-degree vision instead of 90?
synch_mode=0                     # should I speed up with synchronous mode?

save_kwy_log=1                   # should I save episode info to .kwy file?
save_rcg_log=0                   # should I save game log to .rcg file?
save_rcl_log=0                   # should I save message lof to .rcl file?
log_dir=$keepaway_dir/logs       # directory to store kwy, rcg, and rcl logs

port=5800                        # server port used by players and monitor
coach_port=$[port + 1]           # server port used by offline trainer
olcoach_port=$[port + 2]         # server port used by online coach
sleep_time=8                     # time (in seconds) before starting PlayOn mode

launch_monitor=1                 # should I launch rcssmonitor on startup?

###########################################################
# Trainer options                                         #
###########################################################

use_trainer=1                    # should I use a trainer instead of server referee?
save_trainer_log=0               # should I save cycle queue to a file?
launch_trainer_monitor=0         # should I launch trainer monitor on startup?
trainer="rcssjava.trainer.Trainer"   # name of trainer class

############################################################
#                                                          #
############################################################

date=`date +%Y%m%d%H%M`
machine=`hostname`

proc_name=$date-$machine
if [ -n "$5" ]; then
	proc_name=$5
fi

echo The log file will be $proc_name
save_dir=$weight_dir/$proc_name
log_save_dir=$client_log_dir/$proc_name

keepaway_mode=$[!use_trainer]

s="server"

keepaway_opts="$s::keepaway=$keepaway_mode $s::keepaway_start=$sleep_time $s::keepaway_width=$ka_width $s::keepaway_length=$ka_length"

kwy_log_opts="$s::keepaway_logging=$save_kwy_log $s::keepaway_log_dir=$log_dir $s::keepaway_log_fixed=1 $s::keepaway_log_fixed_name=$proc_name"
rcg_log_opts="$s::game_logging=$save_rcg_log $s::game_log_dir=$log_dir $s::game_log_compression=0 $s::game_log_version=3 $s::game_log_fixed=1 $s::game_log_fixed_name=$proc_name"
rcl_log_opts="$s::text_logging=$save_rcl_log $s::text_log_dir=$log_dir $s::text_log_compression=0 $s::text_log_fixed=1 $s::text_log_fixed_name=$proc_name"
log_opts="$kwy_log_opts $rcg_log_opts $rcl_log_opts"

vision_opts="$s::visible_angle=360"
if [ -n "$4" ]; then
  vision_opts="$s::visible_angle=360 $s::quantize_step=0.$4 $s::quantize_step_l=.0$4"
fi

player_speed=1.2
if [ -n "$3" ]; then
	player_speed=$3
fi

server_opts="$s::port=$port $s::coach_port=$coach_port $s::olcoach_port=$olcoach_port $s::half_time=-1 $s::forbid_kick_off_offside=0 $s::use_offside=0 $s::stamina_inc_max=3500 $s::synch_mode=$synch_mode $s::synch_offset=80 $keepaway_opts $log_opts $vision_opts $s::coach=$use_trainer $s::slow_down_factor=1 $s::player_speed_max=$player_speed"
	

keeper_opts_hand="-t keepers -e 0 -q hand"
keeper_opts_learn="-t keepers -e 1 -q learned"
taker_opts="-t takers -e $taker_learn -q $taker_policy"

client_opts="-p $port -k $num_keepers -j $num_takers -x $stop_after -y $start_learning_after"

if (( $save_weights && $taker_learn )); then
  echo Creating weights directory: $save_dir
  mkdir $save_dir
  if [ ! -d $save_dir ]; then
    echo Unable to create weight directory: $save_dir
    exit
  fi
fi

if (( $save_client_log || $save_client_draw_log )); then
  mkdir -p $log_save_dir
  if [ ! -d $log_save_dir ]; then
    echo Unable to create client log directory: $log_save_dir
    exit
  fi
fi

if (( $save_kwy_log || $save_rcg_log || $save_rcl_log || $save_trainer_log )); then
  mkdir -p $log_dir
  if [ ! -d $log_dir ]; then
    echo Unable to find log directory: $log_dir
    exit
  fi
fi

if (( $save_kwy_log && $use_trainer )); then
  trainer_log_opts="-kwy $log_dir/$proc_name.kwy"
fi

if (( $save_trainer_log && $use_trainer )); then
  trainer_log_opts="$trainer_log_opts -queue_file $log_dir/$proc_name.log"
fi

echo Starting Server....
#echo rcssserver $server_opts
echo rcssserver $server_opts
rcssserver $server_opts &

server_pid=$!

sleep 2
echo Starting Clients....
pushd $client_dir

for (( i=1; i<=$num_keepers; i++ ))
do
  if (( $save_client_log )); then
    klog_opts="-l $log_level -o $log_save_dir/k$i.log"
  fi
  if (( $save_client_draw_log )); then
    kdraw_opts="-d 1 -a $log_save_dir/k$i-draw.log"
  fi
  if (( $save_weights )); then
    kweight_opts="-f $save_dir/k$i-weights.dat"
  fi
  if (( $keeper_load )); then
    kweight_opts="$kweight_opts -w $weight_dir/$keeper_load_dir/k$i-weights.dat"
  fi
  if (( $i==1 )); then
    kcmd_line="./$client $client_opts $keeper_opts_learn $klog_opts $kdraw_opts $kweight_opts"
  else
    kcmd_line="./$client $client_opts $keeper_opts_hand $klog_opts $kdraw_opts $kweight_opts"
  fi
  echo Starting Keeper \#$i...
  #echo $kcmd_line
  $kcmd_line &
done

sleep 2
for (( i=1; i<=$num_takers; i++ ))
do
  if (( $save_client_log )); then
    tlog_opts="-l $log_level -o $log_save_dir/t$i.log"
  fi
  if (( $save_client_draw_log )); then
    tdraw_opts="-d 1 -a $log_save_dir/t$i-draw.log"
  fi
  if (( $save_weights && $taker_learn )); then
    tweight_opts="-f $save_dir/t$i-weights.dat"
  fi
  if (( $taker_load )); then
    tweight_opts="$tweight_opts -w $weight_dir/$taker_load_dir/t$i-weights.dat"
  fi
  tcmd_line="./$client $client_opts $taker_opts $tlog_opts $tdraw_opts $tweight_opts"
  echo Starting Taker \#$i...
  #echo $tcmd_line
  $tcmd_line &
done

sleep 2
popd

if (( $use_trainer )); then
  trcmd_line="java -cp $rcssjava_dir/rcssjava.jar $trainer -port $coach_port -keepers $num_keepers -width $ka_width -length $ka_length -takers $num_takers -monitor $launch_trainer_monitor $trainer_log_opts"
  echo Starting Trainer...
  echo $trcmd_line
  $trcmd_line &
  trainer_pid=$!
fi

if (( $launch_monitor )); then
  monitor_opts="rcssmonitor -m_keepaway 1 -m_keepaway_length $ka_length -m_keepaway_width $ka_width -m_port $port"
  echo Starting Monitor...
  rcssmonitor $monitor_opts &
  monitor_pid=$!
fi

if (( $use_trainer )); then
  wait $trainer_pid
  kill -INT $server_pid
  kill -9 $monitor_pid
fi
