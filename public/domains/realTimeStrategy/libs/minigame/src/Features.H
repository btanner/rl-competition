#ifndef FEATURES_H
#define FEATURES_H

#include "State.H"
#include "MiniGameState.H"
#include "Worker.H" 
#include "Marine.H" 
#include "Base.H" 

#define FTR_TACTADV         1
#define FTR_HPDIFF          2
#define FTR_UNITCOSTDIFF    3

class Features
{   
protected:
  static double tactadv(MiniGameState & state, const std::string & parms);
  static double hpdiff(MiniGameState & state, const std::string & parms);
  static double unitcostdiff(MiniGameState & state, const std::string & parms);
  
public:
  static double realFeature(int num, MiniGameState & state, const std::string & parms);
  static void snapshot(MiniGameState & state, const std::vector<int> & enabledFtrs, 
                       std::vector<double> & ftrVals);
};

#endif
