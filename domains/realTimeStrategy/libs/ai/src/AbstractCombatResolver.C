
#include "AbstractCombatResolver.H"

using namespace std; 

AbstractCombatResolver::AbstractCombatResolver()
{
}

AbstractCombatResolver::~AbstractCombatResolver()
{
}

/* Assume everyone is in range and each side shoots at the 
   the one with highest damage index (atkvalue/hp) */
   
void AbstractCombatResolver::resolve_combat(
  std::vector<int> & my_unit_hps, std::vector<int> & opp_unit_hps)
{
  while (!my_units.empty() && !opp_units.empty())
  {
    GameObj<MiniGameState>* my_target = NULL;  
    GameObj<MiniGameState>* opp_target = NULL; 
    vector<GameObj<MiniGameState>*>::iterator my_target_iter; 
    vector<GameObj<MiniGameState>*>::iterator opp_target_iter; 
    
    double my_max_damageindex = -100000; 
    double opp_max_damageindex = -100000; 
    int my_target_lifetime = 0;
    int opp_target_lifetime = 0; 
    int my_dmgpertick = 0; 
    int opp_dmgpertick = 0; 
    
    // first get the targets & calc dmg per tick
    
    FORALL (my_units, iter) {
      GameObj<MiniGameState>* obj = *iter; 
      
      double damage_index = ((double)obj->get_atkvalue())/((double)obj->hp); 
      
      if (damage_index > my_max_damageindex)
      {
        my_max_damageindex = damage_index; 
        my_target = obj;
        my_target_iter = iter; 
      }
      
      my_dmgpertick += obj->get_atkvalue();     
    }
    
    FORALL (opp_units, iter) {
      GameObj<MiniGameState>* obj = *iter; 
      
      double damage_index = ((double)obj->get_atkvalue())/((double)obj->hp); 
      
      if (damage_index > opp_max_damageindex)
      {
        opp_max_damageindex = damage_index; 
        opp_target = obj;
        opp_target_iter = iter; 
      }
    
      opp_dmgpertick += obj->get_atkvalue();     
    }
    
    // now calc how long each target has to live
    
    my_target_lifetime = opp_dmgpertick / my_target->hp + 1; 
    opp_target_lifetime = my_dmgpertick / opp_target->hp + 1; 
    
    // find the minimum of this to see who dies and calc damage
    // on both targets. if they are equal eliminate both targets
    
    if (my_target_lifetime == opp_target_lifetime)
    {
      my_units.erase(my_target_iter); 
      opp_units.erase(opp_target_iter); 
    }  
    else if (my_target_lifetime > opp_target_lifetime)
    {
      // opp dies
      int fighting_time = opp_target_lifetime;     
      opp_units.erase(opp_target_iter); 
      my_target->hp -= fighting_time*opp_dmgpertick; 
    }
    else if (my_target_lifetime < opp_target_lifetime)
    {
      // my dies
      int fighting_time = my_target_lifetime;     
      my_units.erase(my_target_iter); 
      opp_target->hp -= fighting_time*my_dmgpertick;
    }
  }
   
  // report on remaining hps
  
  FORALL (my_units, iter) {
    GameObj<MiniGameState>* obj = *iter;     
    my_unit_hps.push_back(obj->hp); 
  }
  
  FORALL (opp_units, iter) {
    GameObj<MiniGameState>* obj = *iter;     
    opp_unit_hps.push_back(obj->hp); 
  }
  
}

