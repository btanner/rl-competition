
#ifndef RLCOMP08BOT3_H
#define RLCOMP08BOT3_H

#include <map>
#include <string>
#include <vector>
#include <sstream> 

#include "BlobAttackCmdr.H"
#include "DefendBaseCmdr.H"
#include "Commander.H"
#include "GameObj.H"
#include "MiniGameState.H"
#include "Worker.H"
#include "Marine.H"
#include "MineralPatch.H" 
#include "MinerCmdr.H" 
#include "Base.H" 
#include "Player.H"
#include "Profiler.H"
#include "ScoutCmdr.H"

#define ADD_ACTION(oss, str)        (oss) << (str) << "#"         

class RLComp08Bot3 : public DefaultPlayer
{
  std::vector<Commander*> commanders; 
  int phase, base_x, base_y;
  int obx, oby; 
  int train_scheduler;
  int marine_scheduler;  
  int base_free_time;
  int my_workers, my_marines, mps; 
  int phase1ctr; 
  
  Base* basePtr;
  Base* oppBasePtr; 
  MineralPatch * closest_mp; 
  
  DefendBaseCmdr * dbcmdr;
  ScoutCmdr * sccmdr;
  MinerCmdr * mncmdr;
  BlobAttackCmdr * bacmdr; 
  
  std::vector<GameObj<MiniGameState>* > my_vobjs;   // visible objs only 
  std::vector<GameObj<MiniGameState>* > opp_vobjs;  // opponent's visible objs only
  std::vector<GameObj<MiniGameState>* > vmps;       // visible mps only    
  std::map<MineralPatch*, bool> amps;
  
  Commander* find(GameObj<MiniGameState>* objPtr); 
  std::string follow_orders(int objId, GameObj<MiniGameState>* objPtr); 
  
  void enter_phase(int phase);
  void create_commanders();  
  void rotating_assignment(GameObjPtr objPtr);
  std::string rotating_trainaction(int objId);
  void computeVisible(); 
  void find_closest_mp(); 
  bool is_mpgone(MineralPatch * mp); 
  bool a_sees_loc(GameObj<MiniGameState>* obj, int x, int y, int radius);
  
protected:  
  // Called individually in big loop
  virtual std::string chooseAction(int objId, Worker* workerPtr, MiniGameState& state);  
  virtual std::string chooseAction(int objId, Marine* marinePtr, MiniGameState& state);
  virtual std::string chooseAction(int objId, Base* basePtr, MiniGameState& state);  
  virtual void pre_objloop();   
  
public:  
  RLComp08Bot3(int num); 
  virtual ~RLComp08Bot3();
  
};

#endif

