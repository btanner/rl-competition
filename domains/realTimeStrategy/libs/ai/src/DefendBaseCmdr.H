
#ifndef DEFENDBASECMDR_H_
#define DEFENDBASECMDR_H_

#include <vector>

#include "Commander.H"
#include "Base.H"
#include "MiniGameState.H"

class DefendBaseCmdr : public Commander
{
  int num_posts; 
  int cur_post; 
  Base * basePtr; 
  GameObjPtr threat; 
  
  void init(); 
  
  std::vector< std::pair<int,int> > posts; 
    
public:
  DefendBaseCmdr(Base * _basePtr, int _numPosts, MiniGameParameters * _parms,
                 MiniGameState * _state, int _playerNum); 
  
  virtual ~DefendBaseCmdr(); 
     
  virtual void * new_info();  
  virtual void pre_objloop();  
  virtual std::string get_action(int objId, GameObjPtr objPtr); 
    
};


#endif

