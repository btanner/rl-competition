
#include "ScoutCmdr.H"

using namespace std; 

#define ARR2D(a,i,j)     (a[(i)*rows + (j)])

static bool debug = false;

ScoutCmdr::ScoutCmdr(MiniGameParameters * _parms)
{
  set_parmsptr(_parms); 
  
  target_x = target_y = -1; 
  
  init(15, 15); 
}

ScoutCmdr::~ScoutCmdr()
{
  delete [] covered; 
  
  // ~Commander gets the rest
}

void ScoutCmdr::init(int r, int c)
{
  rows = r; cols = c; 
  
  covered = new bool[r*c]; 
  
  FORS(i, rows)
    FORS(j, cols)
      ARR2D(covered,i,j) = false;
}
  
void ScoutCmdr::new_target(scoutinfo * si)
{
  int x = -1; 
  int y = -1; 
  
  FORS(tries, 15) 
  {
    x = rand() % rows; 
    y = rand() % cols; 
      
    if (!ARR2D(covered,x,y))
    {
      //ARR2D(covered,x,y) = true; 
      break; 
    }      
  }
  
  x = x*(parmsPtr->width/cols + 1);
  y = y*(parmsPtr->height/rows + 1);
      
  fix(x,y);
      
  si->target_x = x; 
  si->target_y = y;  
  
  target_x = x; 
  target_y = y; 
  
  DPR << "New target is " << x << "," << y << endl; 
}
  
  
void * ScoutCmdr::new_info()
{
  scoutinfo * si = (scoutinfo*)malloc(sizeof(scoutinfo)); 
  
  new_target(si); 
  
  return si; 
}

void ScoutCmdr::pre_objloop()
{
}

string ScoutCmdr::get_action(int objId, GameObjPtr objPtr)
{
  scoutinfo* si = static_cast<scoutinfo*>(objInfo[objPtr]);
  
  if (objPtr->x == si->target_x && objPtr->y == si->target_y)
    new_target(si); 
  
  //if (!objPtr->get_ismoving())
  return compose_move_action(objId, target_x, target_y, objPtr->get_maxspeed());
  
  //return "";
}     
