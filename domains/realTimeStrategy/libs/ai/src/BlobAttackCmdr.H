
#ifndef BLOBATTACKCMDR_H_
#define BLOBATTACKCMDR_H_

#include <vector>

#include "Commander.H"
#include "Base.H"
#include "Marine.H"
#include "MiniGameState.H"
#include "Worker.H"

struct bainfo {
  bool scouting; 
}; 


class BlobAttackCmdr : public Commander
{    
  bool attacking; 
  Base* basePtr; 
  Base* oppBasePtr; 
  int tx, ty; 
  int curtarget, ctx, cty; 
  bool covered[8];
  
  void determine_waiting_spot(); 
  void get_target(); 
  void next_target(); 
  
public:
  BlobAttackCmdr(MiniGameParameters * _parms, MiniGameState * _state, int _playerNum); 
  virtual ~BlobAttackCmdr(); 
     
  virtual void * new_info();  
  virtual void pre_objloop();  
  virtual std::string get_action(int objId, GameObjPtr objPtr);     
  
  void set_base(Base* _bp) { basePtr = _bp; }
  void set_attacking(bool _a) { attacking = _a; }
  void set_obase(Base * _ob) { oppBasePtr = _ob; }
};

#endif

