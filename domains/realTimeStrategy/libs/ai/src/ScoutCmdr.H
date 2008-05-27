
#ifndef SCOUTCMDR_H_
#define SCOUTCMDR_H_

#include <vector>

#include "Commander.H"
#include "Base.H"
#include "MiniGameState.H"

typedef struct _si {
  int target_x, target_y; 
} scoutinfo; 

class ScoutCmdr : public Commander
{  
  void init(int r, int c); 
  int rows, cols; 
  bool * covered; 
  
  int target_x, target_y; 
  
  void new_target(scoutinfo * si); 
  
public:
  ScoutCmdr(MiniGameParameters * _parms); 
  virtual ~ScoutCmdr(); 
     
  virtual void * new_info();  
  virtual void pre_objloop();  
  virtual std::string get_action(int objId, GameObjPtr objPtr);     
};

#endif

