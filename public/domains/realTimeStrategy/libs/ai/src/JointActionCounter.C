
#include "JointActionCounter.H"

using namespace std; 

JointActionCounter::JointActionCounter(int _numActors, int _numActions)
{
  numActors = _numActors; 
  numActions = _numActions; 
  maxAction = numActions-1; 
  
  array = new int[numActors];
  reset();
  
  delimiter = "";
}

JointActionCounter::~JointActionCounter()
{
  delete [] array; 
}

void JointActionCounter::next()
{
  for (int i = numActors-1; i >= 0; i--)
  {
    if (array[i] != maxAction)
    { 
      array[i]++; 
      
      // reset the ones to the right
      for (int j = i+1; j < numActors-1; j++)
        array[j] = 0; 
        
      break; 
    }
  }
}

bool JointActionCounter::done()
{
  for (int i = 0; i < numActors; i++) 
    if (array[i] != (numActions-1))
      return false; 
  
  return true;   
}

void JointActionCounter::reset()
{
  for (int i = 0; i < numActors; i++)
    array[i] = 0;   
}

std::string JointActionCounter::to_string()
{
  return ""; 
}
