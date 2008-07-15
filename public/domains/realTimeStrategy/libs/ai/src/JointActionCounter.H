
#ifndef JOINTACTIONCOUNTER_H_
#define JOINTACTIONCOUNTER_H_

#include <string>

class JointActionCounter
{
  int numActors; 
  int numActions;  // actions labeled 0 .. numActions - 1
  int maxAction; 
  
  int * array; 
  std::string delimiter; 
  
public:
  JointActionCounter(int _numActors, int _numActions);
  virtual ~JointActionCounter(); 
  
  void next(); 
  std::string to_string(); 
  bool done(); 
  void reset(); 
  
  void set_delimiter(std::string _delimiter) { delimiter = _delimiter; }
};

#endif

