
#include "MinerCmdr.H"
#include "Worker.H"

using namespace std; 

MinerCmdr::MinerCmdr(MiniGameParameters * _parms, MiniGameState * _state, int _playerNum)
  : Commander(_parms, _state, _playerNum)
{
  base_x = base_y = -1; 
  mp_x = -1; 
  mp_y = -1; 
}

MinerCmdr::~MinerCmdr()
{
}
     
void MinerCmdr::set_base_loc(int x, int y)
{
  base_x = x; base_y = y;
}

void MinerCmdr::set_mp_loc(int x, int y)
{
  mp_x = x; mp_y = y; 
}
         
void * MinerCmdr::new_info()
{
  minerinfo * mi = (minerinfo*)malloc(sizeof(minerinfo)); 
  mi->current_target = 0; 
  return mi;  
}

void MinerCmdr::pre_objloop()
{  
  threat_x = -1; 
  threat_y = -1; 
}

std::string MinerCmdr::get_action(int objId, Worker * worker)
{ 
  minerinfo * mi = (minerinfo*)objInfo[worker];   
  
  //if (mp_threatened)
  //  return compose_move_action(objId, base_x, base_y, worker->get_maxspeed());
    
  if (   mi->current_target == 0 
         && worker->carried_minerals == parmsPtr->worker_mineral_capacity)
  {
    mi->current_target = 1; 
    return compose_move_action(objId, base_x, base_y, worker->get_maxspeed()); 
  }    
  else if (mi->current_target == 0 && mp_x >= 0 && mp_y >= 0)
  {
    return compose_move_action(objId, mp_x, mp_y, worker->get_maxspeed());       
  }
    
  if (mi->current_target == 1 && worker->carried_minerals == 0)
  {
    mi->current_target = 0; 
    return compose_move_action(objId, mp_x, mp_y, worker->get_maxspeed());       
  }
  
  return "";
}

std::string MinerCmdr::get_action(int objId, Marine * marine)
{
  minerinfo * mi = (minerinfo*)objInfo[marine];     
  
  if (mp_x >= 0 && mp_y >= 0 && mi->current_target == 0)
    return compose_move_action(objId, mp_x, mp_y, marine->get_maxspeed()); 
  
  
  return ""; 
}


string MinerCmdr::get_action(int objId, GameObjPtr objPtr)
{
  if (objPtr->get_type() == "worker")
  {
    Worker * worker = (Worker*)objPtr; 
    return get_action(objId, worker); 
  }
  else if (objPtr->get_type() == "marine")
  {
    Marine * marine = (Marine*)objPtr; 
    return get_action(objId, marine);
  } 
  
  return "";
}
  
  
