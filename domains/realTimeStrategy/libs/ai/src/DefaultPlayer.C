
#include "GameObj.H"
#include "MiniGameState.H" 
#include "Player.H"
#include "Worker.H" 

#include <stdlib.h>
#include <time.h>

#include <map>
#include <string> 
#include <sstream>

using namespace std;

static bool debug = false; 

DefaultPlayer::DefaultPlayer(int num)
  : Player(num)
{
  name = "DefaultPlayer";
  
  minerals = 0; 
  timestep = 0; 
}

DefaultPlayer::~DefaultPlayer()
{
}

void DefaultPlayer::pre_objloop()
{
}

bool DefaultPlayer::can_afford_worker()
{
  return (minerals >= parmsPtr->worker_cost); 
}

bool DefaultPlayer::can_afford_marine()
{
  return (minerals >= parmsPtr->marine_cost); 
}


string DefaultPlayer::receive_actions(string view)
{
  if (!fullview)  // If fullview, the state is set using set_state
    build_state(view);            
  
  timestep++; 
  minerals = statePtr->player_infos[playerNum].pd.minerals;
  
  vector<string> actions;
 
  DPR << "TP" << playerNum << ": view is " << view << endl;
  DPR << "TP" << playerNum << ": Iterating through objects" << endl; 
  
  int numObjs = 0;
  
  pre_objloop();
  
  FORALL(statePtr->all_objs, iter)
  {
    numObjs++;
    
    GameObj<MiniGameState> * objPtr = (*iter);
    
    ostringstream oss;
    objPtr->serialize(true, oss);
    
    //cout << "obj->owner = " << objPtr->owner << ", playerNum = " << playerNum << endl; 
    
    // iterate over each one, choose an object
    
    int objId = objPtr->view_ids[playerNum];
    
    if (objPtr->owner == playerNum && objPtr->get_type() == "worker")
    {
      Worker* workerPtr  = (Worker*)objPtr;            
      string act = chooseAction(objId, workerPtr, *statePtr);
      actions.push_back(act); 
        
      //cout << "  found worker, id=" << objId << " : " << oss.str() << endl;
      //cout << "   --> action is " << act << endl;           
    }
    else if (objPtr->owner == playerNum && objPtr->get_type() == "marine")     
    {
      Marine* marinePtr  = (Marine*)objPtr;
      string act = chooseAction(objId, marinePtr, *statePtr);      
      actions.push_back(act);       
       
      //cout << "  found marine, id=" << objId << " : " << oss.str() << endl;
      //cout << "   --> action is " << act << endl; 
    }
    else if (objPtr->owner == playerNum && objPtr->get_type() == "base")             
    {
      Base* basePtr = (Base*)objPtr;
      
      string act = chooseAction(objId, basePtr, *statePtr);      
      actions.push_back(act);       
        
      //cout << "  found base, id=" << objId << " : " << oss.str() << endl;
      //cout << "   --> action is " << act << endl; 
    }
    else if (objPtr->get_type() == "mineral_patch")
    {
      //mp_x = objPtr->x;
      //mp_y = objPtr->y;
      
      //cout << "  found mineral_patch, id=" << objId << " : " << oss.str() << endl;      
    }
   
    
  }
 
  DPR << "numObjs = " << numObjs << endl;
  
  string actionstr = join(actions, "#");
  
  DPR << "actionstr = " << actionstr << endl; 
  
  return actionstr;
}

string DefaultPlayer::chooseAction(int objId, Worker* marinePtr, MiniGameState& state)
{  
  return "";
}

string DefaultPlayer::chooseAction(int objId, Marine* marinePtr, MiniGameState& state)
{  
  return "";
}

string DefaultPlayer::chooseAction(int objId, Base* basePtr, MiniGameState& state)
{
  return "";
}

