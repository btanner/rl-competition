
#include "AbstractCombatResolver.H"
#include "MiniGameTests.H"
#include "MiniTacticalGameState.H"
#include "Marine.H"

using namespace std; 

void MiniTacticalGameState::testACR()
{  
  cout << "Executing testACR" << endl; 
  
  Marine * m;
  AbstractCombatResolver acr; 
  
  int start_myhp = 0, start_opphp = 0; 
  int start_myunits = 0, start_oppunits = 0; 
 
  for (int i = 0; i < 3; i++)
  {
    m = new Marine(this);
    new_obj(m, 0);      
    acr.add_my_unit(m);
    start_myunits++;
    start_myhp += m->hp; 
  }
  
  for (int i = 0; i < 2; i++)
  {
    m = new Marine(this);
    new_obj(m, 1);
    acr.add_opp_unit(m);
    start_oppunits++;
    start_opphp += m->hp; 
  }
  
  int delta_hp = start_myhp - start_opphp; 
  int delta_units = start_myunits - start_oppunits; 
  
  //cout << delta_hp << " " << delta_units << endl; 
  cout << start_myhp << "," << start_myunits << " vs " 
       << start_opphp << "," << start_oppunits << endl; 
  
  vector<int> myhps, opphps;
  
  acr.resolve_combat(myhps, opphps);
  
  int end_myhp = 0, end_opphp = 0; 
  int end_myunits = myhps.size(), end_oppunits = opphps.size();   
  
  FORALL(myhps, iter) {
    end_myhp += *iter; 
  }
  
  FORALL(opphps, iter) {
    end_opphp += *iter; 
  }
  
  delta_hp = end_myhp - end_opphp; 
  delta_units = end_myunits - end_oppunits; 
  
  //cout << delta_hp << " " << delta_units << endl; 
  cout << end_myhp << "," << end_myunits << " vs " 
       << end_opphp << "," << end_oppunits << endl; 
  
  cout << "exiting ... " << endl; 
  exit(-1);
}

