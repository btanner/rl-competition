
#ifndef TACTICALMISSIONPLAYER_H_
#define TACTICALMISSIONPLAYER_H_

#include <list>
#include <map>

#include "Base.H"
#include "InfluenceMap.H"
#include "IMapPlayer.H"
#include "Marine.H"
#include "MiniGameState.H"
#include "Player.H"

class TacticalMissionPlayer : public IMapPlayer
{  
  bool attacking; 
  int K; // k-means.. number of clusters
  
  std::vector< std::pair<int,int> > ally_centroids; 
  std::vector< std::pair<int,int> > enemy_centroids; 
  std::map< GameObj<MiniGameState>*,int > ally_memberships; 
  std::map< GameObj<MiniGameState>*,int > enemy_memberships;   
  std::vector<double> ally_cstrengths; 
  std::vector<double> enemy_cstrengths;  
  Base *b1; 
  Base *b2; 
  
  std::pair<int,int> atk_target;
  std::pair<int,int> def_targets[2];
  double def_prop; // Proportion of resources to send to first target
  int def_units;  
  int def_aunits; 
  std::map< GameObj<MiniGameState>*, std::pair<int,int> > def_assignments; 
  int bases; 
  
  std::string chooseDefenseAction(int objId, Marine* marinePtr, MiniGameState& state);
  std::string chooseOffenseAction(int objId, Marine* marinePtr, MiniGameState& state);
  void calc_clusters();
  void find_bases(); 
  void count_units(); 
  
protected:
  // Called individually in big loop  
  virtual std::string chooseAction(int objId, Marine* marinePtr, MiniGameState& state);
  
  virtual void pre_objloop(); 
  
  void pre_objloop_attacker(); 
  void pre_objloop_defender(); 
      
public:
  TacticalMissionPlayer(int num); 
  virtual ~TacticalMissionPlayer(); 
  
  void set_attacking(bool _a) { attacking = _a; }
  
};


#endif
