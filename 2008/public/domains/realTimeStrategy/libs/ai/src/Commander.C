
#include <iostream>
#include <sstream>

#include "Commander.H"

using namespace std; 

Commander::Commander() 
{
}

Commander::~Commander()
{
  FORALL(objInfo, iter)
  {
    GameObjPtr objPtr = (*iter).first; 
    void * infoptr = (*iter).second; 
    
    if (infoptr != NULL) 
      disengage(objPtr); 
  }
}

Commander::Commander(MiniGameParameters * _parms, MiniGameState * _state, int _playerNum)
{
  parmsPtr = _parms; 
  statePtr = _state; 
  playerNum = _playerNum; 
}

void Commander::engage(GameObjPtr objPtr)
{
  void * info = new_info(); 
  objInfo[objPtr] = info; 
}

void Commander::disengage(GameObjPtr objPtr)
{
  void * info = objInfo[objPtr];
  free(info); 
  objInfo[objPtr] = NULL; 
}

bool Commander::is_commanding(GameObjPtr objPtr)
{
  return (objInfo[objPtr] != NULL);
}

void Commander::fix(int & x, int & y)
{
  if (x < 0) x = 0; 
  if (y < 0) y = 0; 
  
  if (x >= parmsPtr->width) x = parmsPtr->width-1; 
  if (y >= parmsPtr->height) y = parmsPtr->height-1; 
}

// radius is the radius of the target
bool Commander::a_sees_loc(GameObj<MiniGameState>* obj, int x, int y, int radius)
{
  double d =
        square((double)(obj->x - x)) +
        square((double)(obj->y - y));
  
  return d <= square(radius + obj->sight_range);  
}

bool Commander::a_canattack_loc(GameObj<MiniGameState>* obj, int x, int y, int radius)
{
  double d =
        square((double)(obj->x - x)) +
        square((double)(obj->y - y));
  
  return d <= square(radius + obj->get_atkrange());  
}



// Stolen from Player class
string Commander::compose_action(int id, const string& act)
{
  ostringstream oss; 
  oss << id << " " << act; 
  return oss.str(); 
}

// Stolen from Player class
string Commander::compose_move_action(int id, int x, int y, int speed)
{
  ostringstream oss; 
  if (x < 0 || y < 0)
  {
    //REM2("playerNum = ", playerNum);  
    ERR("Commander.C bad values for move parms!");
  }
  
  oss << id << " move " << x << " " << y << " " << speed; 
  return oss.str(); 
}

// Stolen from Player class
string Commander::compose_attack_action(int id, int target_id)
{
  ostringstream oss; 
  oss << id << " attack " << target_id; 
  return oss.str(); 
}
