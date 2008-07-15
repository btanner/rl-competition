
#ifndef IMAPPLAYER_H_
#define IMAPPLAYER_H_

#include <list>

#include "InfluenceMap.H"
#include "Marine.H"
#include "MiniGameState.H"
#include "Player.H"

class IMapPlayer : public DefaultPlayer
{  
  std::list< std::pair<int,int> > * ally_peaks; 
  std::list< std::pair<int,int> > * enemy_peaks; 
  std::map< std::pair<int,int>, std::pair<int,int> > peak_assignments; 
  
protected:
  InfluenceMap * imap;
    
  // Called individually in big loop  
  virtual std::string chooseAction(int objId, Marine* marinePtr, MiniGameState& state);
  
  virtual void pre_objloop(); 
      
public:
  IMapPlayer(int num); 
  virtual ~IMapPlayer(); 
  
  void set_imap(InfluenceMap * _imap) { imap = _imap; }
};


#endif
