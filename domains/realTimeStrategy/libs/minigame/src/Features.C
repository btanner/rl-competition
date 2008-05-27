
#include "Features.H"
#include "MiniGameState.H"

#include <math.h>
#include <vector>

using namespace std; 

/**
 * Note: features are always in view of player 0!
 */

double Features::realFeature(int num, MiniGameState & state, const string & parms)
{
  switch (num)
  {
    case FTR_TACTADV:       return tactadv(state, parms);
    case FTR_HPDIFF:        return hpdiff(state, parms);
    case FTR_UNITCOSTDIFF:  return unitcostdiff(state, parms);
    default: return 0; 
  }
}

void Features::snapshot(MiniGameState & state, const vector<int> & enabledFtrs,
                        vector<double> & ftrVals)
{
  FORALL(enabledFtrs, i)
  {
    int ftrNum = *i;     
    double ftrVal = realFeature(ftrNum, state, ""); 
    ftrVals.push_back(ftrVal);
  }
}

double Features::tactadv(MiniGameState & state, const string & parms)
{
  vector<GameObj<MiniGameState>*> source_marines; 
  vector<GameObj<MiniGameState>*> target_marines; 
  
  if (parms == "0")
  {
    state.get_marines(0, source_marines);
    state.get_marines(1, target_marines);
  }
  else if (parms == "1")
  {
    state.get_marines(1, source_marines);
    state.get_marines(0, target_marines);
  }
  
  double source_damage_caused = 0; 
  double source_hp = 0;
  double target_damage_caused = 0; 
  double target_hp = 0;
  
  
  int min_hp = 10000000;
  double min_dist = 10000000;
  Marine* weakest_in_sight = NULL;
  Marine* closest = NULL;
  
  // Source dmg 1st
  
  FORALL (source_marines, i) {
    Marine* smar = (Marine*)(*i);
    
    source_hp += smar->hp; 
            
    FORALL (target_marines, j) {
      Marine* tmar = (Marine*)(*j);
            
      if (state.a_sees_b(*smar, *tmar))
      {
        if (tmar->hp < min_hp) 
        {
          min_hp = tmar->hp;
          weakest_in_sight = tmar; 
        }          
      }
      
      double dx = smar->x - tmar->x;
      double dy = smar->y - tmar->y;      
      double dist = sqrt(dx*dx + dy*dy);
      
      if (dist < min_dist) 
      {
        min_dist = dist; 
        closest = tmar; 
      }
    }
    
    if (weakest_in_sight != NULL)
      source_damage_caused += smar->attack_value; 
    else if (closest != NULL)
      source_damage_caused += smar->attack_value/(min_dist/(double)smar->max_speed);     
      //source_damage_caused += smar->attack_value;     
  }
  
  min_hp = 1000000000;
  min_dist = 100000000;
  weakest_in_sight = NULL;
  closest = NULL;

  // target dmg 2nd  
  
  FORALL (target_marines, i) {
    Marine* tmar = (Marine*)(*i);
    
    target_hp += tmar->hp;
            
    FORALL (source_marines, j) {
      Marine* smar = (Marine*)(*j);
            
      if (state.a_sees_b(*tmar, *smar))
      {
        if (smar->hp < min_hp) 
        {
          min_hp = smar->hp;
          weakest_in_sight = smar; 
        }          
      }
      
      double dx = tmar->x - smar->x;
      double dy = tmar->y - smar->y;      
      double dist = sqrt(dx*dx + dy*dy);
      
      if (dist < min_dist) 
      {
        min_dist = dist; 
        closest = smar; 
      }
    }
    
    if (weakest_in_sight != NULL)
      target_damage_caused += tmar->attack_value; 
    else if (closest != NULL)
      target_damage_caused += tmar->attack_value/(min_dist/(double)tmar->max_speed);     
  }
    
  return (source_hp*source_damage_caused - target_hp*target_damage_caused); 
}

double Features::hpdiff(MiniGameState & state, const string & parms)
{
  double hp0 = 0, hp1 = 0;
  
  FORALL (state.all_objs, i)
  {
    GameObj<MiniGameState>* obj = *i; 
    
    if (obj->dead())
      continue; 
    
    if (obj->owner == 0) 
      hp0 += obj->hp; 
    else if (obj->owner == 1)
      hp1 += obj->hp; 
  }
  
  return (hp0 - hp1); 
}

double Features::unitcostdiff(MiniGameState & state, const string & parms)
{
  double unitcost0 = 0, unitcost1 = 0;
  
  FORALL (state.all_objs, i)
  {
    GameObj<MiniGameState>* obj = *i; 
    
    if (obj->dead())
      continue;
    
    double cost = 0;
    
    if (obj->get_type() == "worker")
      cost = state.get_parms().worker_cost; 
    else if (obj->get_type() == "marine")
      cost = state.get_parms().marine_cost; 
    else if (obj->get_type() == "base")
      cost = state.get_parms().base_cost; 
    
    if (obj->owner == 0) 
      unitcost0 += cost;
    else if (obj->owner == 1)
      unitcost1 += cost;     
  }
  
  return (unitcost0 - unitcost1); 
}

