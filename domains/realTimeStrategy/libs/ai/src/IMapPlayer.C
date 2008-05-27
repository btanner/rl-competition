
#include <stdlib.h>

#include <string> 

#include "GameObj.H"
#include "IMapPlayer.H"

static int debug_level = 0; 

using namespace std; 

IMapPlayer::IMapPlayer(int num)
  : DefaultPlayer(num)
{
  name = "IMapPlayer";
  imap = NULL; 
}

IMapPlayer::~IMapPlayer()
{
  DELETE(imap);
}

string IMapPlayer::chooseAction(int objId, Marine* marinePtr, MiniGameState& state)
{
  // Find the assignments source nearest to us, then head for its dest
  
  pair<int,int> loc = make_pair(marinePtr->x, marinePtr->y);
  
  double min_dist = 1000000.0; 
  pair<int,int> target = make_pair(-1,-1); 
  
  FORALL(peak_assignments, iter)
  {
    pair<int,int> source = (*iter).first; 
    pair<int,int> dest = (*iter).second;
    
    double dist = PDIST(loc, source); 
    
    if (dist < min_dist)
    {
      min_dist = dist; 
      target = dest; 
    }
  } 
  
  // convert back to map coordinates
  pair<int,int> maptarget = imap->get_mapcoord(target.first, target.second);
  
  return compose_move_action(objId, maptarget.first, maptarget.second, 
                             parmsPtr->marine_max_speed); 
}
  
void IMapPlayer::pre_objloop()
{
  if (imap == NULL) 
    imap = new InfluenceMap(*statePtr, *parmsPtr); 
  
  imap->reset(); 
  
  // add allies and enemies
  
  FORALL(statePtr->all_objs, iter)
  {
    GameObj<MiniGameState> * objPtr = *iter; 
    
    if (objPtr->owner == playerNum)
    {
      DLPR(4) << "Adding ally" << endl;
      imap->add_ally(objPtr); 
    }
    else if (objPtr->owner != 2)       
    { // Note: != 2 .. don't want to consider mineral patches as enemies :) 
      DLPR(4) << "Adding enemy" << endl; 
      imap->add_enemy(objPtr); 
    }
    else
    {
      cout << "Found owner = " << objPtr->owner << endl;
    }
  }
  
  imap->propogate_influence(); 
  
  //imap->blur(1, 5);   
  //imap->localize(1, 3); 
  
  imap->calc_peaks(); 
  
  ally_peaks = imap->get_ally_peaks(); 
  enemy_peaks = imap->get_enemy_peaks(); 
  
  /* 
     For each ally peak find closest enemy peak and closest ally peak
     If going towards enemy peak shows a win, go to it. If not, go 
     towards nearest ally peak.   
   */
  
  peak_assignments.clear();
  
  FORALL(*ally_peaks, iter1) 
  {
    pair<int,int> ally_peak = *iter1;   
    
    double min_dist = 100000.0; 
    pair<int,int> target = make_pair(-1,-1);
            
    FORALL(*enemy_peaks, iter2) 
    {
      pair<int,int> enemy_peak = *iter2; 
      
      double dist = PDIST(ally_peak, enemy_peak); 
      
      if (dist < min_dist)
      {
        min_dist = dist; 
        target = enemy_peak; 
      }
    }
    
    double outcome = 0; 
    
    if (target.first >= 0 && target.second >= 0)
    {    
      outcome =   imap->get_value(ally_peak.first, ally_peak.second) 
                + imap->get_value(target.first, target.second);
      
      //cout << "outcome = " << outcome << endl; 
      
      if (outcome > 0)
      {
        DLPR(3) << "Winning outcome " << outcome << " peak " << to_string(ally_peak);
        DLPR(3) << " target " << to_string(target) << endl; 
        
        peak_assignments[ally_peak] = target;       
        continue; 
      }
    }
    
    // losing outcome or no enemy peaks? go towards nearest other ally peak
    
    min_dist = 100000.0;
    target = make_pair(-1,-1); 
    
    FORALL(*ally_peaks, iter3) 
    {
      pair<int,int> other_ally_peak = *iter3; 
      
      if (other_ally_peak == ally_peak)
        continue;
      
      double dist = PDIST(ally_peak, other_ally_peak); 
      
      if (dist < min_dist)
      {
        min_dist = dist; 
        target = other_ally_peak; 
      }
    }
    
    // If no peak is chosen there is only one peak (ours), 
    // which is ok.. just tighten it to get full strength
    if (target.first < 0 && target.second < 0) 
      target = ally_peak; 
    
    DLPR(3) << "Losing outcome " << outcome << " peak " << to_string(ally_peak);
    DLPR(3) << " target " << to_string(target) << endl; 
    
    peak_assignments[ally_peak] = target;     
  }
}



      
