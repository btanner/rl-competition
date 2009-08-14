
#ifndef MINERCMDR_H_
#define MINERCMDR_H_

#include <vector>

#include "Commander.H"
#include "Base.H"
#include "Marine.H"
#include "MiniGameState.H"
#include "Worker.H"

struct minerinfo { 
  int current_target; // 0 = mp, 1 = base
}; 

class MinerCmdr : public Commander
{  
  int base_x, base_y; 
  int mp_x, mp_y; 
  int threat_x, threat_y; 
  
  
  std::string get_action(int objId, Worker * worker);     
  std::string get_action(int objId, Marine * marine);     
  
public:
  MinerCmdr(MiniGameParameters * _parms, MiniGameState * _state, int _playerNum); 
  virtual ~MinerCmdr(); 
     
  virtual void * new_info();  
  virtual void pre_objloop();  
  virtual std::string get_action(int objId, GameObjPtr objPtr);     
  
  void set_base_loc(int x, int y); 
  void set_mp_loc(int x, int y); 
};

#endif


