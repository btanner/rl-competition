
#include <stdlib.h>

#include <string> 

#include "GameObj.H"
#include "IMapPlayer.H"
#include "TacticalMissionPlayer.H"

static int debug_level = 3; 

using namespace std; 

TacticalMissionPlayer::TacticalMissionPlayer(int num)
  : IMapPlayer(num)
{
  name = "TacticalMissionPlayer";
  b1 = b2 = NULL; 
  K = 2; 
}

TacticalMissionPlayer::~TacticalMissionPlayer()
{
}

string TacticalMissionPlayer::chooseAction(int objId, Marine* marinePtr, MiniGameState& state)
{
  if (attacking)
    return chooseOffenseAction(objId, marinePtr, state);  
  else
    return chooseDefenseAction(objId, marinePtr, state);      
}

void TacticalMissionPlayer::find_bases()
{
  bases = 0; 
  
  b1 = b2 = NULL; 
  
  FORALL(statePtr->all_objs, iter)
  {
    GameObj<MiniGameState>* objPtr = *iter; 
    if (objPtr->get_type() == "base")
    {
      if (objPtr->in_play())
      {
        if (b1 == NULL)
        {
          bases++; 
          b1 = (Base*)objPtr; 
        }
        else
        {
          bases++;
          b2 = (Base*)objPtr; 
        }
      }
    }
  }  
}

void TacticalMissionPlayer::count_units()
{
  def_units = 0; 
  
  FORALL(statePtr->all_objs, iter)
  {
    GameObj<MiniGameState>* objPtr = *iter; 
    if (objPtr->owner == playerNum && objPtr->get_type() != "base")
      def_units++;
  }  
}

string TacticalMissionPlayer::chooseDefenseAction(int objId, Marine* marinePtr, MiniGameState& state)
{
  if (bases == 0)
    return ""; 
  
  pair<int,int> whereto;
      
  if (def_assignments.find(marinePtr) != def_assignments.end())
  {    
    whereto = def_assignments[marinePtr];
  }
  else
  {
    whereto = def_targets[1];
  } 
    
  // in some pathological cases these become negative numbers, so 
  // we must correct them
  whereto.first = MAX(0, whereto.first); 
  whereto.second = MAX(0, whereto.second); 
  
  return compose_move_action(objId, whereto.first, whereto.second,
                             parmsPtr->marine_max_speed);
}

void TacticalMissionPlayer::pre_objloop_defender()
{
  def_assignments.clear(); 

  // for each base, find the nearest enemy cluster and send the same number of defenders to
  // intercept their path

  find_bases(); 
  
  if (bases == 0)
    return;
  
  if (b2 == NULL || b1->dead() || b2->dead())
  {
    if (b1 != NULL && !b1->dead())
      def_targets[1] = make_pair(b1->x, b1->y); 
    else if (b2 != NULL && !b2->dead())
      def_targets[1] = make_pair(b2->x, b2->y); 
    else 
    {// wtf? why does this ever happen...
      cout << "HERE!!!!!!!!" << endl; 
      def_targets[1] = make_pair(0,0);
    } 
    
    return; 
  }
  
  count_units();
  def_aunits = 0;    
  
  double strengths[2];
  pair<int,int> sources[2]; 
  pair<int,int> targets[2]; 
  
  FORS(i, K)
  {
    Base * b = NULL; 
    double min_dist = 10000000.0;
      
    // b1  
    double dist = DIST(b1->x,b1->y,enemy_centroids[i].first,enemy_centroids[i].second); 
    if (dist < min_dist)
    {
      min_dist = dist; 
      b = b1; 
    }
      
    // b2
    dist = DIST(b2->x,b2->y,enemy_centroids[i].first,enemy_centroids[i].second); 
    if (dist < min_dist)
    {
      min_dist = dist; 
      b = b2; 
    }
    
    if (b == b1) 
    {
      strengths[i] = enemy_cstrengths[i];
      sources[i] = enemy_centroids[i];
      targets[i] = make_pair(b1->x,b1->y); 
    }
    else
    {
      strengths[i] = enemy_cstrengths[i];
      sources[i] = enemy_centroids[i]; 
      targets[i] = make_pair(b2->x,b2->y); 
    }
  }

  def_prop = strengths[0]/(strengths[0]+strengths[1]); 
  
  // make the triangle
  
  FORS(i, K)
  {
    double dx = targets[i].first - sources[i].first;
    double dy = targets[i].second - sources[i].second;
    double len = sqrt(SQR(dy) + SQR(dx)); 
    double newlen = len - parmsPtr->base_radius - 10; 
    double angle = atan(dy/dx); 
    double new_dx = newlen * cos(angle); 
    double new_dy = newlen * sin(angle); 
    def_targets[i].first = sources[i].first + (int)new_dx; 
    def_targets[i].second = sources[i].second + (int)new_dy; 
  }
  
  // now the assignments
  
  //std::map< GameObj<MiniGameState>*, std::pair<int,int> > def_assignments;
  
  int num = (int)(def_prop*def_units);
  
  FORS(i, num)
  {
    // find the closest not in the map
    double min_dist = 1000000.0; 
    GameObj<MiniGameState>* new_candidate = NULL; 
    
    FORALL(statePtr->all_objs, iter)
    {
      GameObj<MiniGameState>* objPtr = *iter; 
      
      if (objPtr->get_type() == "base" || objPtr->owner != playerNum)
        continue; 
      
      if (!objPtr->in_play())
        continue; 
      
      if (def_assignments.find(objPtr) != def_assignments.end())
        continue;  // in the map, accounted for!
      
      double d = DIST(objPtr->x, objPtr->y, def_targets[0].first, def_targets[0].second);       
      if (d < min_dist)
      {
        min_dist = d; 
        new_candidate = objPtr; 
      }
    }
    
    def_assignments[new_candidate] = def_targets[0]; 
  }
}

string TacticalMissionPlayer::chooseOffenseAction(int objId, Marine* marinePtr, MiniGameState& state)
{
  if (bases == 0)
    return ""; 
  
  if (atk_target.first < 0) 
    return ""; 
  
  return compose_move_action(objId, atk_target.first, atk_target.second, 
                             parmsPtr->marine_max_speed); 
}

void TacticalMissionPlayer::pre_objloop_attacker()
{
  
  // Attacker: rush towards weakest-defended base
  
  find_bases();
     
  if (bases == 0)
    return; 
  
  bool onebase = (bases == 1);
  if (b2 != NULL) 
  {
    if (b1->dead() || b2->dead())
      onebase = true; 
  }
  
  double defenses[2]; 
  defenses[0] = defenses[1] = -100; 
  double min_dist = 1000000.0; 
  
  if (!onebase)
  {
    FORS(i, K)
    {
      Base * b = NULL; 
      
      // b1  
      double dist = DIST(b1->x,b1->y,enemy_centroids[i].first,enemy_centroids[i].second); 
      if (dist < min_dist)
      {
        min_dist = dist; 
        b = b1; 
      }
      
      // b2
      dist = DIST(b2->x,b2->y,enemy_centroids[i].first,enemy_centroids[i].second); 
      if (dist < min_dist)
      {
        min_dist = dist; 
        b = b2; 
      }
    
      if (b == b1) 
        defenses[0] += enemy_cstrengths[i]; 
      else
        defenses[1] += enemy_cstrengths[i]; 
    }
    
    Base * target = (defenses[0] < defenses[1] ? b1 : b2); 
      
    atk_target.first = target->x;
    atk_target.second = target->y; 
    
    return;
  }
  
  if (b1 == NULL) {
    // no bases
    atk_target = make_pair(-1,-1);
    return;  
  }
  
  atk_target.first = b1->x; 
  atk_target.second = b1->y;
}  


void TacticalMissionPlayer::pre_objloop()
{
  calc_clusters(); 
  
  /*
     Used this test code as a sanity test for k-means
  cout << endl;
  cout << "Player " << playerNum << endl;
  
  FORALL(ally_centroids, iter)
    cout << "ally centroid " << to_string(*iter) << endl; 
  
  FORALL(enemy_centroids, iter)
      cout << "enemy centroid " << to_string(*iter) << endl;   
  */
  
  // Calc the cluster strengths
  
  ally_cstrengths.clear(); 
  enemy_cstrengths.clear();
  
  FORS(i, K)
  {
    double ally_str = 0; 
    double enemy_str = 0; 
    
    pair<int,int> & ally_centroid = ally_centroids[i]; 
    pair<int,int> & enemy_centroid = enemy_centroids[i]; 
    
    FORALL(ally_memberships, iter)
    {
      GameObj<MiniGameState>* objPtr = (*iter).first; 
      int c = (*iter).second; 
      
      if (i == c) 
      {  // This unit belongs to this cluster
        double d = DIST(objPtr->x,objPtr->y,ally_centroid.first,ally_centroid.second);         
        if (d == 0) d = 0.00001; 
        
        ally_str += ((objPtr->hp)*objPtr->get_atkvalue())/d; 
      }
    }
    
    FORALL(enemy_memberships, iter)
    {
      GameObj<MiniGameState>* objPtr = (*iter).first; 
      int c = (*iter).second; 
      
      if (i == c) 
      {  // This unit belongs to this cluster
        double d = DIST(objPtr->x,objPtr->y,enemy_centroid.first,enemy_centroid.second); 
        if (d == 0) d = 0.00001; 
        
        enemy_str += ((objPtr->hp)*objPtr->get_atkvalue())/d;
      }
    }
    
    DLPR(5) << "cstr " << ally_str << " " << enemy_str << endl; 
    
    ally_cstrengths.push_back(ally_str);
    enemy_cstrengths.push_back(enemy_str);    
  }
  
  // Now specific stuff
  
  if (attacking)
    pre_objloop_attacker();
  else
    pre_objloop_defender();
}

void TacticalMissionPlayer::calc_clusters()
{
  // Run k-means with k = 2 for each player 
    
  ally_centroids.clear();  
  ally_centroids.push_back(make_pair(0,0)); 
  ally_centroids.push_back(make_pair(parmsPtr->width-1,parmsPtr->height-1)); 
  
  enemy_centroids.clear();  
  enemy_centroids.push_back(make_pair(0,0)); 
  enemy_centroids.push_back(make_pair(parmsPtr->width-1,parmsPtr->height-1)); 
  
  
  /*
  
  // Random Initial values will lead to diff clusters ! See log for May 6th 2008
  
  ally_centroids.clear();
    //if (ally_centroids.size() == 0)
  FORS(i, K)
  ally_centroids.push_back(make_pair(, rand() % parmsPtr->height));        
      //ally_centroids.push_back(make_pair(rand() % parmsPtr->width, rand() % parmsPtr->height));
  
  enemy_centroids.clear();
    //if (enemy_centroids.size() == 0)
  FORS(i, K)
  enemy_centroids.push_back(make_pair(rand() % parmsPtr->width, rand() % parmsPtr->height));     
  */
  
  // For each player p 
  FORS(p, 2)  
  {      
    
    vector< pair<int,int> > * centroidsPtr = NULL; 
    vector< pair<int,int> > old_centroids; 
    map< GameObj<MiniGameState>*,int > * mapPtr = NULL; 
    //map< GameObj<MiniGameState>*,int > old_map; 
    
    if (p == playerNum)
    {
      centroidsPtr = &ally_centroids; 
      mapPtr = &ally_memberships;
    }
    else
    {
      centroidsPtr = &enemy_centroids; 
      mapPtr = &enemy_memberships;      
    }    
    
    mapPtr->clear(); 
    int reps = 0; 
    
    do 
    {
      old_centroids = (*centroidsPtr);      
      //old_map = (*mapPtr);
      
      // assign units to their closest centroid
      
      FORALL(statePtr->all_objs, iter)
      {
        GameObj<MiniGameState>* objPtr = *iter; 
        
        if (objPtr->owner == p)
        {
          double min_dist = 100000000.0;
          int i = 0; 
          int c = 0;
          
          // find closest centroid 
          
          FORALL(*centroidsPtr, iter2)
          {
            int cx = (*iter2).first; 
            int cy = (*iter2).second; 
            
            double dist = DIST(objPtr->x, objPtr->y, cx, cy); 
            
            if (dist < min_dist) 
            {
              min_dist = dist; 
              c = i; 
            }
            
            i++;
          }
                        
          // assign unit to centroid 
          
          (*mapPtr)[objPtr] = c; 
        }
      }
      
      // Recalc centroids
      
      double * xcoords = new double[K];
      double * ycoords = new double[K];
      int * num = new int[K];
      
      FORS(i, K)
      {
        xcoords[i] = ycoords[i] = 0; 
        num[i] = 0; 
      }
      
      FORALL(*mapPtr, iter)
      {
        GameObj<MiniGameState> * objPtr = (*iter).first; 
        int c = (*iter).second; 
        
        xcoords[c] += objPtr->x; 
        ycoords[c] += objPtr->y; 
        num[c]++; 
      }
      
      FORS(c, K)
        if (num[c] > 0)
          (*centroidsPtr)[c] = make_pair((int)(xcoords[c]/num[c]), (int)(ycoords[c]/num[c])); 
    
      DELETEARR(xcoords);
      DELETEARR(ycoords);
      DELETEARR(num);
      
      reps++;
    }
    while (!setwise_equal(old_centroids, (*centroidsPtr)));
    //while (!((*mapPtr) == old_map));
    //while (reps < 50);
    
  }
}


