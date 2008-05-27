
#include "DefendBaseCmdr.H"
#include "Global.H"
#include "GameObj.H"
#include "MiniGameState.H" 
#include "Player.H"
#include "RLComp08Bot3.H"

#include <stdlib.h>
#include <time.h>

#include <map>
#include <string> 
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim.hpp>

using namespace std;

#define SGN(x)          ((x) < 0 ? (-1) : 1)

static bool debug = false; 

RLComp08Bot3::RLComp08Bot3(int num)
  : DefaultPlayer::DefaultPlayer(num)
{
  phase = 0; 
  base_x = base_y = -1; 
  obx = oby = -1;
  basePtr = NULL;
  oppBasePtr = NULL;  
  phase1ctr = 0; 
  
  dbcmdr = NULL; 
  sccmdr = NULL; 
  mncmdr = NULL; 
  bacmdr = NULL; 
  
  train_scheduler = 0;
  marine_scheduler = 0;  
  
  base_free_time = 0; 
  
  closest_mp = NULL; 
}

RLComp08Bot3::~RLComp08Bot3()
{
  FORALL(commanders, iter)
  {
    Commander* cmdrPtr = *iter; 
    delete cmdrPtr; 
  }
}

Commander* RLComp08Bot3::find(GameObj<MiniGameState>* objPtr)
{
  FORALL(commanders, iter)
  {
    Commander* cmdrPtr = *iter; 
    if (cmdrPtr->is_commanding(objPtr))
      return cmdrPtr;
  }
  
  return NULL; 
}

string RLComp08Bot3::follow_orders(int objId, GameObj<MiniGameState>* objPtr)
{    
  Commander* cmdrPtr = find(objPtr); 
  if (cmdrPtr == NULL) return "";
  return cmdrPtr->get_action(objId, objPtr); 
}

void RLComp08Bot3::enter_phase(int phase)
{
  DPR << "Entering phase " << phase << endl; 
  this->phase = phase; 
  
  if (phase == 1)
  {
  }
}

void RLComp08Bot3::create_commanders()
{
  dbcmdr = new DefendBaseCmdr(basePtr, 4, parmsPtr, statePtr, playerNum); 
  sccmdr = new ScoutCmdr(parmsPtr); 
  mncmdr = new MinerCmdr(parmsPtr, statePtr, playerNum); 
  bacmdr = new BlobAttackCmdr(parmsPtr, statePtr, playerNum);
  
  commanders.push_back(dbcmdr); 
  commanders.push_back(sccmdr);   
  commanders.push_back(mncmdr);  
  commanders.push_back(bacmdr); 
}

void RLComp08Bot3::rotating_assignment(GameObjPtr objPtr)
{
  // make sure commanders are created 1st
  if (!dbcmdr) return;
  
  if (objPtr->get_type() == "worker")
  {
    mncmdr->engage(objPtr);
    return; 
  }
    
  switch(marine_scheduler)
  {
    // Scout
    //case 0: 
    //case 1: 
    //case 2: 
      sccmdr->engage(objPtr); 
      break;
      
      
    // Marine wdef
    case 7: 
    case 8: 
    case 9: 
      mncmdr->engage(objPtr); 
      break;
      
    // Marine bdef
    case 0:
    case 1:
    case 2:
    case 3: 
    case 4: 
    case 5: 
    case 6: 
      dbcmdr->engage(objPtr); 
      break;
      
  }
  
  marine_scheduler = (marine_scheduler + 1) % 10; 
}

string RLComp08Bot3::rotating_trainaction(int objId)
{
  string ret = "";
    
  // Exclude workers when we have 10
  if (   my_workers >= 10 
      && (   train_scheduler == 0
          || train_scheduler == 3
          || train_scheduler == 5
         )
     )
  {
    train_scheduler = (train_scheduler + 1) % 8;     
  }
  
  /*if (my_marines < 5)
  {
    train_scheduler = 4;
  }*/ 
         
  if (my_workers < 1)
  {
    train_scheduler = 3;
  }
      
  
  switch(train_scheduler)
  {
    case 0: // Build a worker
      if (can_afford_worker()) {
        ret = compose_train_worker_action(objId); 
        base_free_time = timestep + parmsPtr->worker_training_time;
      }      
    break;
      
    case 1: // Build a scout
      if (can_afford_marine()) {
        ret = compose_train_marine_action(objId); 
        base_free_time = timestep + parmsPtr->marine_training_time;
      }
    break;
      
    case 2: // Build a marine worker defense
      if (can_afford_marine()) {
        ret = compose_train_marine_action(objId); 
        base_free_time = timestep + parmsPtr->marine_training_time;
      }
    break;
      
    case 3: // Build a worker
      if (can_afford_worker()) {
        ret = compose_train_worker_action(objId); 
        base_free_time = timestep + parmsPtr->worker_training_time;
      }      
    break;
      
    case 4: // Build a marine base defense
      if (can_afford_marine()) {
        ret = compose_train_marine_action(objId); 
        base_free_time = timestep + parmsPtr->marine_training_time;
      }
    break;
  
    case 5: // Build a worker
      if (can_afford_worker()) {
        ret = compose_train_worker_action(objId); 
        base_free_time = timestep + parmsPtr->worker_training_time;
      }      
    break;
    
    case 6: // Build a marine worker defense
      if (can_afford_marine()) {
        ret = compose_train_marine_action(objId); 
        base_free_time = timestep + parmsPtr->marine_training_time;
      }
    break;
    
    case 7: // Build a marine base defense
      if (can_afford_marine()) {
        ret = compose_train_marine_action(objId); 
        base_free_time = timestep + parmsPtr->marine_training_time;
      }
    break;
  }
  
  train_scheduler = (train_scheduler + 1) % 8; 
  
  return ret; 
}

// Stolen from bot2 : this is to make sure we don't cheat
void RLComp08Bot3::computeVisible()
{
  my_vobjs.clear();
  opp_vobjs.clear();
  vmps.clear();
   
  my_workers = my_marines = mps = 0; 
  
  //oppw_x = oppw_y = 0; int oppw = 0;
  
  map<int, bool> id2vis; // used for groups calc 
  
  // Collect ours first
  FORALL(statePtr->all_objs, iter)
  {
    GameObj<MiniGameState> * objPtr = (*iter);
    if (objPtr->owner == playerNum)
    {
      my_vobjs.push_back(objPtr);
      int objId = objPtr->view_ids[playerNum];
      id2vis[objId] = true; 
      
      if (objPtr->get_type() == "worker")
        my_workers++;
      else if (objPtr->get_type() == "marine")
        my_marines++; 
    }
  }
  
  FORALL(statePtr->all_objs, iter)
  {
    GameObj<MiniGameState> * objPtr = (*iter);
    
    if (objPtr->owner == playerNum)
      continue;
    
    FORALL(my_vobjs, iter2)
    {
      GameObj<MiniGameState> * objPtr2 = (*iter2);
      if (MiniGameState::a_sees_b(*objPtr2, *objPtr))
      {
        if (objPtr->owner == 2)
        {
          vmps.push_back(objPtr);
          
          MineralPatch * mp = (MineralPatch*)objPtr; 
          
          if (mp->minerals_left > 0)
            amps[mp] = true; 
          else
            amps[mp] = false;  
                   
          mps++; 
        }
        else {
          opp_vobjs.push_back(objPtr);
          
          if (objPtr->get_type() == "worker")
          {
            //oppw_x += objPtr->x; 
            //oppw_y += objPtr->y;
            //oppw++; 
          }
          else if (objPtr->get_type() == "base")
          {
            oppBasePtr = (Base*)objPtr;
            obx = objPtr->x;
            oby = objPtr->y;
            //oppb_x = objPtr->x; 
            //oppb_y = objPtr->y;
            //oppb_id = objPtr->view_ids[playerNum];
          }
        }
        
        break;
      }
    }
  }
  
  /*
  if (oppw > 0)
  {
    oppw_x = (int)(((double)(oppw_x))/oppw);
    oppw_y = (int)(((double)(oppw_y))/oppw);
  } 
  */ 
  
  // now unsubscribe from groups whoever is no longer around
  /*
  groupsizes.clear();
  FORALL(gid2info, iter)
  {
    int gid = (*iter).first; 
    groupinfo & gi = (*iter).second;
    
    FORALL(gi.objs, iter2)
    {
      int id = (*iter2).first;
      
      if (!id2vis[id])
        unsubscribe(id, gid);
      else
        groupsizes[gid]++;        
    }
    
    grouprolls[gid] = drand48(); 
  }*/
 
}

// radius here is the target's radius
bool RLComp08Bot3::a_sees_loc(GameObj<MiniGameState>* obj, int x, int y, int radius)
{
  double d =
        square((double)(obj->x - x)) +
        square((double)(obj->y - y));
  
  return d <= square(radius + obj->sight_range);  
}


bool RLComp08Bot3::is_mpgone(MineralPatch * mp)
{
  // see the spot?
  bool see_spot = false;  
  
  FORALL(my_vobjs, iter)
  {
    GameObjPtr objPtr = (GameObjPtr)(*iter);
    if (a_sees_loc(objPtr, mp->x, mp->y, parmsPtr->mineral_patch_radius))
    {
      see_spot = true; 
      break; 
    }    
  }
  
  if (!see_spot)
    return false; 
  
  FORALL(vmps, iter)
  {
    MineralPatch * mpp = (MineralPatch*)(*iter);
    if (mpp->x == mp->x && mpp->y == mp->y)
    {
      // found it, so it's not gone!
      return false; 
    }
  }
  
  // see it and can't find it in our list of visible mps... :( 
  return true; 
}

void RLComp08Bot3::find_closest_mp()
{
  if (basePtr == NULL)
    return; 
  
  // maximize (dist_to_enemy_base / dist_to_my_base)
    
  double max_fact = -1; 
  double mydist;
  double oppdist;
  MineralPatch * cmp = NULL; 
  
  FORALL(amps, iter)
  {
    MineralPatch * mp = (MineralPatch*)iter->first; 
    bool good = iter->second;
    
    if (is_mpgone(mp))
    {
      good = false; 
      amps[mp] = false;
      continue; 
    }
     
    if (good)
    {      
      if (obx < 0 || oby < 0)     
        oppdist = 10; 
      else
        oppdist = DIST(obx, oby, mp->x, mp->y);
      
      mydist = DIST(basePtr->x, basePtr->y, mp->x, mp->y);
      
      if ((oppdist/mydist) > max_fact)
      {
        max_fact = (oppdist/mydist);
        cmp = mp; 
      }
    }
  }
  
  closest_mp = cmp; 
}


void RLComp08Bot3::pre_objloop()
{
  //cout << "minerals = " << minerals << endl; 
  
  computeVisible(); 
  find_closest_mp();
   
  if (mncmdr)
  {
    if (basePtr) {
      //DPR << "Setting base loc " << endl; 
      mncmdr->set_base_loc(basePtr->x, basePtr->y);
    }
    
    if (closest_mp) { 
      //DPR << "Setting closest mp" << endl; 
      mncmdr->set_mp_loc(closest_mp->x, closest_mp->y);
    }
  }
  
  if (bacmdr)
  {
    if (basePtr)
      bacmdr->set_base(basePtr);
    
    if (oppBasePtr)
      bacmdr->set_obase(oppBasePtr);
  }
  
  FORALL(commanders, iter)
  {
    Commander* cmdrPtr = *iter; 
    cmdrPtr->pre_objloop(); 
  }
  
  if (timestep >= 500 && phase < 2)  
    enter_phase(2); 
  
  if (basePtr && basePtr->hp != basePtr->get_maxhp() && phase < 2)
    enter_phase(2); 
}

string RLComp08Bot3::chooseAction(int objId, Worker* workerPtr, MiniGameState& state)
{
  if (phase == 0 && !workerPtr->off_map()) 
  {
    enter_phase(1); 
    return compose_build_base_action(objId); 
  }
  
  if (workerPtr->off_map())
    return "";
  
  //if (closest_mp != NULL)
  //  return compose_move_action(objId, closest_mp->x, closest_mp->y, workerPtr->get_maxspeed()); 
  
  if (phase >= 1)
  {
    string order = follow_orders(objId, workerPtr); 
    
    if (order == "") // unassigned!
      rotating_assignment(workerPtr); 
    else 
      return order; 
  }
  
  return follow_orders(objId, workerPtr);   
}

string RLComp08Bot3::chooseAction(int objId, Marine* marinePtr, MiniGameState& state)
{
  if (timestep > 9000 && obx >= 0 && oby >= 0)
  {
    return compose_move_action(objId, obx, oby, marinePtr->get_maxspeed()); 
  }
  
  if (phase == 1)
  {
    string order = follow_orders(objId, marinePtr); 
    
    if (order == "")
    {
      //cout << "BA engaging" << endl; 
      phase1ctr++; 
      bacmdr->engage(marinePtr); 
      
      if (phase1ctr == 5)
        bacmdr->set_attacking(true);
      
      return follow_orders(objId, marinePtr);
    }
    else
      return order; 
  }
  
  if (phase == 2)
  {
    string order = follow_orders(objId, marinePtr); 
    
    if (order == "") // unassigned!
      rotating_assignment(marinePtr); 
    else 
      return order; 
  }
  
  return follow_orders(objId, marinePtr); 
}

string RLComp08Bot3::chooseAction(int objId, Base* basePtr, MiniGameState& state)
{
  if (this->basePtr == NULL)
  {
    this->basePtr = basePtr; 
    
    create_commanders(); 
  }  
  
  if (phase == 1)
  {
    if (timestep >= base_free_time && can_afford_marine())
    {
      base_free_time = timestep + parmsPtr->marine_training_time; 
      //cout << "Building marine!" << endl; 
      return compose_train_marine_action(objId); 
    }
    else if (!can_afford_marine())
    {
      //cout << "Can't afford marine :( Launching attack!" << endl; 
      //exit(-1);
      bacmdr->set_attacking(true); 
    }
    else if (timestep >= base_free_time && can_afford_worker())
    {
      base_free_time = timestep + parmsPtr->worker_training_time;
      return compose_train_worker_action(objId); 
    }
  }
  
  if (phase == 2)
  {
    if (timestep >= base_free_time)
    {
      //DPR << "Calling rotating_train_action" << endl; 
      return rotating_trainaction(objId); 
    }
  }
  
  return follow_orders(objId, basePtr); 
}


