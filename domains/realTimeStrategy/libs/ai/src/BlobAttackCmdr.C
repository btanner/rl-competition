
#include "BlobAttackCmdr.H"

using namespace std; 



BlobAttackCmdr::BlobAttackCmdr(MiniGameParameters * _parms, MiniGameState * _state, int _playerNum)
  : Commander(_parms, _state, _playerNum)
{
  attacking = false; 
  tx = ty = -1; 
  basePtr = NULL; 
  oppBasePtr = NULL;   
  
  curtarget = -1; 
  ctx = cty = -1;
  
  FORS(i, 8)
    covered[i] = false; 
}


BlobAttackCmdr::~BlobAttackCmdr()
{
}
     
void BlobAttackCmdr::determine_waiting_spot()
{
  int dy = 0, dx = 0; 
  int mag = 20; 
  
  dx = (basePtr->x < (parmsPtr->width/2)) ? mag : -mag; 
  dy = (basePtr->y < (parmsPtr->height/2)) ? mag : -mag; 
  
  tx = basePtr->x + dx; 
  ty = basePtr->y + dy; 
  
  fix(tx,ty); 
}

void BlobAttackCmdr::get_target()
{
  if (ctx < 0)
  {
    // first time
    if (basePtr->x < parmsPtr->width/2 && basePtr->y < parmsPtr->height/2)
      curtarget = 0; 
    else if (basePtr->x >= parmsPtr->width/2 && basePtr->y < parmsPtr->height/2)
      curtarget = 1; 
    else if (basePtr->x < parmsPtr->width/2 && basePtr->y >= parmsPtr->height/2)
      curtarget = 2; 
    else 
      curtarget = 3;       
  }
  
  if (curtarget == 0)
  { ctx = 50; cty = 50; }
  else if (curtarget == 1)
  { ctx = parmsPtr->width - 50; cty = 50; }
  else if (curtarget == 2)
  { ctx = 50; cty = parmsPtr->height - 50; }
  else if (curtarget == 3)
  { ctx = parmsPtr->width - 50; cty = parmsPtr->height - 50; }
} 
   
void BlobAttackCmdr::next_target()
{
  covered[curtarget] = true; 
  
  int i = 0; 
  
  for (i = 0; i < 8; i++) 
  {
    curtarget = (curtarget + 1) % 8;
    if (!covered[curtarget])
      break;
  }
  
  if (curtarget == 0)
  { ctx = 50; cty = 50; }
  else if (curtarget == 1)
  { ctx = parmsPtr->width - 50; cty = 50; }
  else if (curtarget == 2)
  { ctx = 50; cty = parmsPtr->height - 50; }
  else if (curtarget == 3)
  { ctx = parmsPtr->width - 50; cty = parmsPtr->height - 50; }  
  else if (curtarget == 4)
  { ctx = 50; cty = parmsPtr->height/2; }  
  else if (curtarget == 5)
  { ctx = parmsPtr->width/2; cty = 50; }  
  else if (curtarget == 6)
  { ctx = parmsPtr->width - 50; cty = parmsPtr->height/2; }  
  else if (curtarget == 7)
  { ctx = parmsPtr->width/2; cty = parmsPtr->height - 50; }  
}     
     
     
void * BlobAttackCmdr::new_info()
{
  // place holder to show that we are engaged
  bainfo * bai = new bainfo; 
  bai->scouting = false; 
  return bai;  
}

void BlobAttackCmdr::pre_objloop()
{
  if (tx < 0 && basePtr != NULL) 
    determine_waiting_spot(); 
  
  get_target(); 
}

string BlobAttackCmdr::get_action(int objId, GameObjPtr objPtr)
{
  
  //cout << "### " << basePtr->x << " " << basePtr->y << tx << " " << ty << attacking << endl;
  if (oppBasePtr) attacking = true; 
  
  if (!attacking)
    return compose_move_action(objId, tx, ty, objPtr->get_maxspeed()); 
  
  if (oppBasePtr && a_canattack_loc(objPtr, oppBasePtr->x, oppBasePtr->y, parmsPtr->base_radius))
  {
    if (objPtr->get_ismoving())
      return compose_action(objId, "stop"); 
   
    return compose_attack_action(objId, oppBasePtr->get_id(playerNum)); 
  }
  
  if (oppBasePtr)
  {    
    return compose_move_action(objId, oppBasePtr->x, oppBasePtr->y, objPtr->get_maxspeed());
  }
  
  
  if (objPtr->x == ctx && objPtr->y == cty)
    next_target(); 
      
  return compose_move_action(objId, ctx, cty, objPtr->get_maxspeed()); 
}
