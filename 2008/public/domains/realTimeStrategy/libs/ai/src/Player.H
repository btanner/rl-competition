#ifndef PLAYER_H
#define PLAYER_H

#include "MiniGameState.H"
#include "Worker.H"
#include "Marine.H"
#include "Base.H"

#include <map>
#include <string>

class Player
{
  bool do_not_free;
  
protected:
  std::string name;
  std::string view;
  MiniGameState* statePtr;
  MiniGameParameters* parmsPtr;
  int playerNum;
  bool fullview;
  
  void build_state(const std::string & view);
  
  std::string compose_action(int id, const std::string& act);  
  std::string compose_move_action(int id, int x, int y, int speed);
  std::string compose_attack_action(int id, int target_id);
  
  std::string compose_build_base_action(int id)
  { return compose_action(id, "build_base"); }
  
  std::string compose_train_marine_action(int id)
  { return compose_action(id, "train marine"); }
  
  std::string compose_train_worker_action(int id)
  { return compose_action(id, "train worker"); }  
  
  bool onMap(GameObj<MiniGameState>* ptr); 
    
public: 
  Player(); 
  Player(int num); 
  virtual ~Player();
  
  bool get_fullview() { return fullview; }
    
  void set_state(MiniGameState * stateptr); 
  void set_parms(MiniGameParameters * parmsptr);
  void set_fullview(bool _fv);
  
  virtual std::string receive_actions(std::string view)  = 0;  

  std::string rnd_move_action(int objId, int speed);
};

class NullPlayer : public Player
{
public:
  NullPlayer(int num); 
  virtual ~NullPlayer(); 
  virtual std::string receive_actions(std::string view);  
};

// Add player classes with diff policies here ...

class RandomPlayer : public Player
{
public:
  RandomPlayer(int num); 
  virtual ~RandomPlayer(); 
  virtual std::string receive_actions(std::string view);  
};

class DefaultPlayer : public Player
{
protected:
  int minerals;  
  int timestep; 
  
  bool can_afford_worker(); 
  bool can_afford_marine(); 
  
  // Called individually in big loop
  virtual std::string chooseAction(int objId, Worker* workerPtr, MiniGameState& state);  
  virtual std::string chooseAction(int objId, Marine* marinePtr, MiniGameState& state);  
  virtual std::string chooseAction(int objId, Base* basePtr, MiniGameState& state);
  
  virtual void pre_objloop();   

public:
  DefaultPlayer(int num);
  virtual ~DefaultPlayer();
  virtual std::string receive_actions(std::string view);
};


class TestPlayer : public DefaultPlayer
{
protected:
  bool init; 
  int time;   
  int done_base_time, done_marine_time, done_worker_time;
  bool have_base; 
  int base_x, base_y;
  int mp_x, mp_y; // mineral patch
  int minerals; 
  
  // Called individually in big loop
  virtual std::string chooseAction(int objId, Worker* workerPtr, MiniGameState& state);
  
  virtual std::string chooseAction(int objId, Marine* marinePtr, MiniGameState& state);
  
  virtual std::string chooseAction(int objId, Base* basePtr, MiniGameState& state);
    
public:
  TestPlayer(int num); 
  virtual ~TestPlayer(); 
  virtual std::string receive_actions(std::string view);  
  
};

class RushPlayer : public DefaultPlayer
{  
  int target_x, target_y;
  
protected:
  // Called individually in big loop  
  virtual std::string chooseAction(int objId, Marine* marinePtr, MiniGameState& state);
      
public:
  RushPlayer(int num); 
  virtual ~RushPlayer(); 
  
  void set_target(int x, int y) { target_x = x; target_y = y; }
};

#endif

