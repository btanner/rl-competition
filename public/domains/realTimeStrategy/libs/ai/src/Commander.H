
#ifndef COMMANDER_H_
#define COMMANDER_H_

#include <stdlib.h>

#include <ext/hash_map>

#include "GameObj.H"
#include "MiniGameState.H"

typedef GameObj<MiniGameState>* GameObjPtr; 

struct StrictGameObjEqual {
  bool operator()(const GameObjPtr objPtr1, const GameObjPtr objPtr2)
  { 
    return (   objPtr1 != NULL 
            && objPtr2 != NULL
            && objPtr1 == objPtr2 
            && objPtr1->view_ids[0] == objPtr2->view_ids[0]
            && objPtr1->view_ids[1] == objPtr2->view_ids[1]
            // ASSUMES only 2 plaers! Fix this eventually
           ); 
  }
};

struct GameObjEqual {
  bool operator()(const GameObjPtr objPtr1, const GameObjPtr objPtr2)
  { 
    return (   objPtr1 != NULL 
            && objPtr2 != NULL
            && objPtr1 == objPtr2 
           ); 
  }
};


struct IntegerHash {
  size_t operator()(const int & integer) const
  {
    return (size_t)(integer % INT_MAX); 
  }
};

struct ObjPtrHash {
  size_t operator()(const GameObjPtr ptr) const
  {    
    // INT_MAX may not always be a good choice.. won't work as well on 64-bit systems
    return (size_t)(reinterpret_cast<size_t>(ptr) % INT_MAX); 
  }
};

// The pointer is to a structure which contains info for that gameobj
typedef __gnu_cxx::hash_map<GameObjPtr, void*, ObjPtrHash, GameObjEqual > GameObjInfo;

class Commander
{
protected:
  GameObjInfo objInfo;
  MiniGameState* statePtr;
  MiniGameParameters* parmsPtr;
  int playerNum; 
     
  std::string compose_action(int id, const std::string& act);  
  std::string compose_move_action(int id, int x, int y, int speed);
  std::string compose_attack_action(int id, int target_id);
  
  std::string compose_build_base_action(int id)
  { return compose_action(id, "build_base"); }
  
  std::string compose_train_marine_action(int id)
  { return compose_action(id, "train marine"); }
  
  std::string compose_train_worker_action(int id)
  { return compose_action(id, "train worker"); }  
  
  void fix(int & x, int & y); 
  bool a_sees_loc(GameObj<MiniGameState>* obj, int x, int y, int radius); 
  bool a_canattack_loc(GameObj<MiniGameState>* obj, int x, int y, int radius); 
  
public:
  Commander(); 
  Commander(MiniGameParameters * _parms, MiniGameState * _state, int _playerNum);
  virtual ~Commander();
   
  void set_stateptr(MiniGameState* _statePtr) { statePtr = _statePtr; }
  void set_parmsptr(MiniGameParameters* _p) { parmsPtr = _p; }
  void set_playernum(int _n) { playerNum = _n; }
  
  // Can't return null because used as a value for engaged
  virtual void * new_info() = 0; 
  virtual void pre_objloop() = 0; 
  virtual std::string get_action(int objId, GameObjPtr objPtr) = 0; 
  
  void engage(GameObjPtr objPtr);
  void disengage(GameObjPtr objPtr);
  bool is_commanding(GameObjPtr objPtr); 
  
};

#endif
