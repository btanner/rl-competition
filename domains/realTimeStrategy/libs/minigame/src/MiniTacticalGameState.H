
#ifndef MiniTacticalGameState_H
#define MiniTacticalGameState_H

#include "State.H"
#include "MiniGameState.H"

#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <iostream>
#include <sstream>

class MiniTacticalGameState : public MiniGameState
{
  void testACR(); 
  
  void totally_random_setup(int p0_marines, int p1_marines); 
  void sectioned_setup(int p0_marines, int p1_marines); 
  void setup_2base_mission(int p0_marines, int p1_marines);   
  
public: 
  
  virtual void object_setup();
};

#endif

