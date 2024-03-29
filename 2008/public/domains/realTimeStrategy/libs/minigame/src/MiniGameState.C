
#include "MiniGameState.H"
#include "Worker.H"
#include "Marine.H"
#include "Base.H"
#include "MineralPatch.H"

#include <vector>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>

using namespace std;

// these are not used!
bool MiniGameState::finished() const { return true; }
int MiniGameState::score(int /*player*/) const { return 0; }


void MiniGameState::object_setup()
{
  int width = gp.width, height = gp.height; 
  //int mid_x = width/2, mid_y = height/2;
  
  Worker *w = new Worker(this);
  new_obj(w, 0);  
  w->x = rand() % width;
  w->y = rand() % height;
  
  w = new Worker(this);
  new_obj(w, 1);
  w->x = rand() % width;
  w->y = rand() % height;

  for (int i = 0; i < gp.mineral_patches; i++)
  {
    MineralPatch *mp = new MineralPatch(this);
    new_obj(mp, 2);
    mp->x = rand() % width;
    mp->y = rand() % height;
  }
}

void MiniGameState::setMPstr(const string& mpstr)
{
  vector<string> parts;
  boost::split(parts, mpstr, boost::is_any_of("-"));
  
  for (unsigned int i = 0; i < parts.size(); i++)
  {
    int x = to_int(parts[i]);
    i++;
    int y = to_int(parts[i]); 
    
    MineralPatch *mp = new MineralPatch(this);
    new_obj(mp, 2);
    mp->x = x;
    mp->y = y;    
  }
  
  apply_new_objs();
}


GameObj<MiniGameState>* MiniGameState::new_game_object(const std::string & type)
{
  //std::cout << "calling minigame new_game_object" << std::endl; 
  GameObj<MiniGameState>* ptr = 0;
  
  if (type == "worker")
    ptr = new Worker(this);
  else if (type == "marine") 
    ptr = new Marine(this);
  else if (type == "base") 
    ptr = new Base(this);
  else if (type == "mineral_patch") 
    ptr = new MineralPatch(this);
  
  return ptr;
}

GameObj<MiniGameState>* MiniGameState::getorcreate_game_object(int id, int player, const std::string & type, bool & newobj)
{
  PlayerInfo &pi = player_infos[player];
  
  GameObj<MiniGameState>* ptr = pi.id2obj[id];
  
  if (ptr == 0)
  {
    newobj = true;
    return new_game_object(type);
  }
  
  newobj = false;
  return ptr; 
}

int MiniGameState::check_win()
{
  int p0_base = 0, p1_base = 0;
  int p0_units = 0, p1_units = 0; 
  
  FORALL (all_objs, i) {
    GameObj<MiniGameState>* objPtr = *i;
   
    if (objPtr->hp >= 1 && objPtr->owner == 0)
    {
      p0_units++;
      if (objPtr->get_type() == "base") {
        p0_base_constructed = true;
        p0_base = 1;
      }
    }
    else if (objPtr->hp >= 1 && objPtr->owner == 1)
    {
      p1_units++;
      if (objPtr->get_type() == "base") {
        p1_base_constructed = true;
        p1_base = 1;
      }      
    }
  }
  
  if (p0_units <= 0 && p1_units > 0)
    return 1;
  
  if (p1_units <= 0 && p0_units > 0)
    return 0;
  
  if (p0_units <= 0 && p1_units <= 0)
    return 2;
  
  if (p0_base_constructed && p0_base == 0)
  {
    if (p1_base_constructed && p1_base == 0) // destroyed at same time
      return 2;
    else
      return 1;
  }
  
  if (p1_base_constructed && p1_base == 0)
  {
    if (p0_base_constructed && p0_base == 0) // destroyed at same time
      return 2;
    else
      return 0;
  }

  return -1;
}

int MiniGameState::encode_view_rlg(int player, int* & array, int attrs_per_unit)
{
  PlayerInfo &pi = player_infos[player];

  // fixme: speed this up if # of objects gets big

  Vector<GameObjBase*> visibleObjs;
  Vector<GameObjBase*> playerObjs;

  // collect player objs
  
  FORALL (all_objs, i)
    if ((*i)->owner == player)
      playerObjs.push_back(*i);
  
  FORALL (all_objs, i) {

    bool visible = (*i)->owner == player; // visible if owner

    if (!visible) {

      // check if other player's object is visible by player

      FORALL (playerObjs, j) {

        if (a_sees_b(**j, **i))
        { visible = true; break; }
      }
    }
        
    if (visible)
      visibleObjs.push_back(*i);
    //else 
    //  invisibleObjs.push_back(*i);
  }
  
  int units = visibleObjs.size(); 
  array = (int *)realloc(array, (1+units*attrs_per_unit)*sizeof(int));  

  array[0] = pi.pd.minerals;   
  
  int index = 1; 
    
  FORALL (visibleObjs, i) {

    if ((*i)->view_ids[player] != 0) {
      
    } else {
      
      // new objects assign id
      ObjId id = (*i)->view_ids[player] = pi.next_obj_id(); // for next round
      // add id to map
      pi.id2obj[id] = *i;
    }

    GameObjBase* objPtr = *i;
    
    // type
    if      (objPtr->get_type() == "worker") array[index] = 0;
    else if (objPtr->get_type() == "marine") array[index] = 1;
    else if (objPtr->get_type() == "base") array[index] = 2;
    else if (objPtr->get_type() == "mineral_patch") array[index] = 3;
    else array[index] = -1;
    index++;
    
    // id
    array[index++] = objPtr->view_ids[player];
    
    array[index++] = objPtr->owner;
    array[index++] = objPtr->x;
    array[index++] = objPtr->y;
    array[index++] = objPtr->radius;
    array[index++] = objPtr->sight_range;
    array[index++] = objPtr->hp;
    array[index++] = objPtr->armor;
    
    if (objPtr->get_type() == "worker")
    {
      Worker* workerPtr = (Worker*)objPtr;
      
      array[index++] = workerPtr->max_speed;
      array[index++] = workerPtr->is_moving;
      array[index++] = workerPtr->carried_minerals;
    }
    else if (objPtr->get_type() == "marine")
    {
      Marine* marinePtr = (Marine*)objPtr;
      
      array[index++] = marinePtr->max_speed;
      array[index++] = marinePtr->is_moving;
      array[index++] = -1;     
    }
    else if (objPtr->get_type() == "mineral_patch")
    {
      MineralPatch* mpPtr = (MineralPatch*)objPtr;
      
      array[index++] = -1;     
      array[index++] = -1;     
      array[index++] = mpPtr->minerals_left;
    }
    else
    {
      array[index++] = -1;
      array[index++] = -1;
      array[index++] = -1;
    }
  }
  
  return index; 
    
}

int MiniGameState::get_score(int player)
{
  PlayerInfo &pi = player_infos[player];
  
  int score = pi.pd.minerals/2;
  score += destruct_cost[player]; 
  
  FORALL (all_objs, i)
  {
    GameObjBase* objPtr = *i; 
    
    if (objPtr->dead())
      continue;
    
    if (objPtr->get_type() == "worker")
      score += gp.worker_cost;
    else if (objPtr->get_type() == "marine")
      score += gp.marine_cost;
    else if (objPtr->get_type() == "base")
      score += gp.base_cost; 
  }
  
  return score;
}

void MiniGameState::update_score(GameObjBase* obj)
{
  int target_player = (obj->owner == 0 ? 1 : 0);
  
  if (obj->get_type() == "worker")
    destruct_cost[target_player] += gp.worker_cost;
  else if (obj->get_type() == "marine")
    destruct_cost[target_player] += gp.marine_cost;
  else if (obj->get_type() == "base")
    destruct_cost[target_player] += gp.base_cost;
}

string MiniGameState::to_string() 
{
  string str = "";
  
  FORALL (all_objs, i) 
  {
    str = str + (*i)->to_string(); 
    str = str + " "; 
  }
  
  str = str.substr(0, str.length()-1);
  
  return str; 
}

// Right now this assumes certain types of objects for each owner
// only to be used at startup!!
void MiniGameState::from_string(const std::string & statestr)
{
  vector<string> objs; 
  boost::split(objs, statestr, boost::is_any_of(" "));
  
  vector<string>::iterator iter;
  
  for (iter = objs.begin(); iter != objs.end(); iter++)
  {
    string str = *iter;

    string prefix = str.substr(0, 3);
    
    if (prefix == "o=0")
    {
      Worker *w = new Worker(this);
      w->deserialize(str); 
      new_obj(w, 0);
    }
    else if (prefix == "o=1")
    {
      Worker *w = new Worker(this);
      w->deserialize(str); 
      new_obj(w, 1);
    }
    else if (prefix == "o=2")
    {
      MineralPatch* mp = new MineralPatch(this);
      mp->deserialize(str);
      new_obj(mp, 2);
    }
  }
   
  apply_new_objs(); 
}

void MiniGameState::get_marines(int player, vector<GameObj<MiniGameState>*> & marines)
{
  FORALL (all_objs, i) {
    GameObj<MiniGameState>* objPtr = *i;
   
    if (   objPtr->get_type() == "marine" && objPtr->hp >= 1 
        && objPtr->owner == player)
    {
      //Marine* mptr = (Marine*)objPtr; 
      marines.push_back(objPtr);
    }
  }
}


void set_parm(MiniGameParameters * mgpPtr, string parm, int val)
{
  if      (parm == "width")                 mgpPtr->width = val;
  else if (parm == "height")                mgpPtr->height = val;
  else if (parm == "mineralpatches")        mgpPtr->mineral_patches = val;
  
  else if (parm == "baseradius")            mgpPtr->base_radius = val;
  else if (parm == "basesightrange")        mgpPtr->base_sight_range = val;
  else if (parm == "baseatkrange")          mgpPtr->base_atk_range = val;
  else if (parm == "basehp")                mgpPtr->base_hp = val;
  else if (parm == "basearmor")             mgpPtr->base_armor = val;
  else if (parm == "basecost")              mgpPtr->base_cost = val;
  else if (parm == "basebuildtime")         mgpPtr->base_build_time = val;
  else if (parm == "baseatkvalue")          mgpPtr->base_atk_value = val;
  
  else if (parm == "marineradius")            mgpPtr->marine_radius = val;
  else if (parm == "marinesightrange")        mgpPtr->marine_sight_range = val;
  else if (parm == "marineatkrange")          mgpPtr->marine_atk_range = val;
  else if (parm == "marinehp")                mgpPtr->marine_hp = val;
  else if (parm == "marinearmor")             mgpPtr->marine_armor = val;
  else if (parm == "marinemaxspeed")          mgpPtr->marine_max_speed = val;
  else if (parm == "marinecost")              mgpPtr->marine_cost = val;
  else if (parm == "marinetrainingtime")      mgpPtr->marine_training_time = val;
  else if (parm == "marineatkvalue")          mgpPtr->marine_atk_value = val;

  else if (parm == "workerradius")            mgpPtr->worker_radius = val;
  else if (parm == "workersightrange")        mgpPtr->worker_sight_range = val;
  else if (parm == "workeratkrange")          mgpPtr->worker_atk_range = val;
  else if (parm == "workerhp")                mgpPtr->worker_hp = val;
  else if (parm == "workerarmor")             mgpPtr->worker_armor = val;
  else if (parm == "workermaxspeed")          mgpPtr->worker_max_speed = val;
  else if (parm == "workercost")              mgpPtr->worker_cost = val;
  else if (parm == "workertrainingtime")      mgpPtr->worker_training_time = val;
  else if (parm == "workeratkvalue")          mgpPtr->worker_atk_value = val;
  else if (parm == "workerminingtime")        mgpPtr->worker_mining_time = val;
  else if (parm == "workermineralcapacity")   mgpPtr->worker_mineral_capacity = val;

  else if (parm == "mineralpatchradius")      mgpPtr->mineral_patch_radius = val;
  else if (parm == "mineralpatchsightrange")  mgpPtr->mineral_patch_sight_range = val;
  else if (parm == "mineralpatchhp")          mgpPtr->mineral_patch_hp = val;
  else if (parm == "mineralpatcharmor")       mgpPtr->mineral_patch_armor = val;  
  else if (parm == "mineralpatchcapacity")    mgpPtr->mineral_patch_capacity = val;   
   
  else 
    cout << "Undefined parm " << parm << endl; 
}

void load_parms(char * parmsfile, MiniGameParameters * mgpPtr)
{
  ifstream file(parmsfile, ios::in);
  
  if (!file) {
    cerr << "Error opening file" << endl;
    exit(-1);
  }
  
  string line; 
  
  while (getline(file, line)) {
    //cout << line << endl;
    
    if (line.at(0) == '#')
      continue; 
    
    vector<string> parts;
    boost::split(parts, line, boost::is_any_of(","));
    
    if (parts[0] == "int")
    {
      int val = to_int(parts[2]); 
      string key = parts[1];
      set_parm(mgpPtr, key, val);
    }
    else if (parts[0] == "str")
    {
      if (parts[1] == "bot0")
        mgpPtr->bot0 = parts[2];
    }    
  }
  
  file.close();
}



