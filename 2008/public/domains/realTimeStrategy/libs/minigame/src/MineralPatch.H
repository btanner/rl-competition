#ifndef MineralPatch_H
#define MineralPatch_H

#include "GameObj.H"
#include "MiniGameState.H"

class MineralPatch : public GameObj<MiniGameState>
{
public:

  int minerals_left;

  virtual bool deserialize_member(const std::string & key, const std::string & val);

  virtual void serialize_members(bool allied_view, std::ostream &os) const;
  
  
  MineralPatch(MiniGameState *s) : GameObj<MiniGameState>(s)
  {
    MiniGameState::GP &gp = state->gp;
    
    radius      = gp.mineral_patch_radius;
    sight_range = gp.mineral_patch_sight_range;
    hp          = gp.mineral_patch_hp;
    armor       = gp.mineral_patch_armor;
    max_hp      = gp.mineral_patch_hp; 

    minerals_left = state->gp.mineral_patch_capacity;
  }

  virtual int get_maxhp() const 
  { return state->gp.mineral_patch_hp; }  
  
  void execute();
  const std::string &get_type() const;
  
};

#endif
