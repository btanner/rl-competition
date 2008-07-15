
#ifndef ABSTRACTCOMBATRESOLVER_H
#define ABSTRACTCOMBATRESOLVER_H

#include <vector>

#include "MiniGameState.H"

class AbstractCombatResolver
{
  std::vector<GameObj<MiniGameState>*> my_units; 
  std::vector<GameObj<MiniGameState>*> opp_units; 

public:
  AbstractCombatResolver(); 
  virtual ~AbstractCombatResolver(); 
  
  // see MiniTacticalGameState.C
  //static void test(); 
  
  void resolve_combat(std::vector<int> & my_unit_hps, 
                      std::vector<int> & opp_unit_hps);
  
  void clear() 
  { my_units.clear(); opp_units.clear(); }
  
  void add_my_unit(GameObj<MiniGameState>* unit) 
  { my_units.push_back(unit); }
  
  void add_opp_unit(GameObj<MiniGameState>* unit)
  { opp_units.push_back(unit); }      
  
};

#endif

