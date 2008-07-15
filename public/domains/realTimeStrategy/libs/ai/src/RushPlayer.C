
#include <stdlib.h>
#include <time.h>

#include <map>
#include <string> 
#include <sstream>

#include "GameObj.H"
#include "MiniGameState.H" 
#include "Player.H"
#include "Worker.H" 

using namespace std;

//static bool debug = false; 

RushPlayer::RushPlayer(int num)
  : DefaultPlayer(num)
{
  name = "RushPlayer";
  target_x = target_y = -1; 
}

RushPlayer::~RushPlayer()
{
}

string RushPlayer::chooseAction(int objId, Marine* marinePtr, MiniGameState& state)
{  
  if (target_x < 0 || target_y < 0)
    return "";
  else 
    return compose_move_action(objId, target_x, target_y, marinePtr->max_speed);
}


