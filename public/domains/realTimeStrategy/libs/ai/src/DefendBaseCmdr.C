
#include <math.h>
#include <stdlib.h>

#include "DefendBaseCmdr.H"

#ifndef PI
#define PI (3.141592653589793)
#endif

using namespace std; 

typedef struct _info 
{
  int target_x, target_y; 
} info; 

DefendBaseCmdr::DefendBaseCmdr(Base * _basePtr, int _numPosts, MiniGameParameters * _parms, 
                               MiniGameState * _state, int _playerNum)
{
  basePtr = _basePtr; 
  num_posts = _numPosts; 
  cur_post = 0; 
  
  parmsPtr = _parms; 
  statePtr = _state; 
  playerNum = _playerNum; 
  
  init(); 
}

DefendBaseCmdr::~DefendBaseCmdr()
{
}
     
void DefendBaseCmdr::init()
{
  double angle_inc = (2*PI) / num_posts; 
  double dist = basePtr->radius + 15; 
  
  double angle = 0; 
  
  FORS(i, num_posts)
  {
    int x = (int)(basePtr->x + dist*cos(angle));       
    int y = (int)(basePtr->y + dist*sin(angle));
    fix(x,y); 
    
    angle += angle_inc; 
    posts.push_back(make_pair(x,y)); 
  }
}     
     
void* DefendBaseCmdr::new_info()
{
  info * infoptr = static_cast<info*>(malloc(sizeof(info))); 
  
  infoptr->target_x = -1; 
  infoptr->target_y = -1; 
  
  return infoptr; 
}
  
void DefendBaseCmdr::pre_objloop()
{
  // find closest enemy
  if (basePtr == NULL) return; 
  threat = NULL; 
  double dist, min_dist = 10000000; 
  
  FORALL(statePtr->all_objs, iter)
  {
    GameObjPtr objPtr = (GameObjPtr)(*iter); 
    
    if (   objPtr->get_type() == "marine" 
           && objPtr->owner != playerNum)
    {
      // potential threat
      dist = DIST(basePtr->x, basePtr->y, objPtr->x, objPtr->y); 
      if (dist <= (parmsPtr->base_radius + parmsPtr->marine_atk_range + 10))
      {
        if (dist < min_dist)
        {
          min_dist = dist; 
          threat = objPtr;
        }
      }
    }
  }
}

string DefendBaseCmdr::get_action(int objId, GameObjPtr objPtr)
{
  info* oi = (info*)objInfo[objPtr];
  
  if (oi->target_x < 0 || oi->target_y < 0)
  {
    // assign guard post    
    oi->target_x = posts[cur_post].first; 
    oi->target_y = posts[cur_post].second;
     
    cur_post = (cur_post + 1) % num_posts;   
  }
  
  //if (objPtr->x == oi->target_x && objPtr->y == oi->target_y)
  //  return "";
  
  //if (objPtr->get_ismoving())
  //  return "";
  
  // threat gets first priority
  if (threat)  
    return compose_move_action(objId, threat->x, threat->y, objPtr->get_maxspeed());   
  
  
  return compose_move_action(objId, oi->target_x, oi->target_y, objPtr->get_maxspeed());   
  
  
}
