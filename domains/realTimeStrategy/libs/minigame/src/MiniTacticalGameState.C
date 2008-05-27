
#include "MiniGameTests.H"
#include "MiniTacticalGameState.H"
#include "Worker.H"
#include "Marine.H"
#include "Base.H"
#include "MineralPatch.H"

/**
 *  Coordinate System looks like this: 
 *  
 *           x
 *  (0,0) ----------->
 *    |
 *    |
 * y  |
 *    v
 *
 */

// Initial field size is 560x560

void MiniTacticalGameState::totally_random_setup(int p0_marines, int p1_marines)
{
  int width = gp.width, height = gp.height; 
  
  Marine * m;
 
  for (int i = 0; i < p0_marines; i++)
  {
    m = new Marine(this);
    new_obj(m, 0);  
    m->x = rand() % width;
    m->y = rand() % height;
  }
  
  for (int i = 0; i < p1_marines; i++)
  {
    m = new Marine(this);
    new_obj(m, 1);
    m->x = rand() % width;
    m->y = rand() % height;
  }
}

// Put player 0's units on the left and player 1's on the right
void MiniTacticalGameState::sectioned_setup(int p0_marines, int p1_marines)
{
  int width = gp.width, height = gp.height; 
  int mid_x = width/2; //, mid_y = height/2;
  
  Marine * m;
 
  for (int i = 0; i < p0_marines; i++)
  {
    m = new Marine(this);
    new_obj(m, 0);  
    m->x = rand() % mid_x;
    m->y = rand() % height;
  }
  
  for (int i = 0; i < p1_marines; i++)
  {
    m = new Marine(this);
    new_obj(m, 1);
    m->x = (rand() % mid_x) + mid_x;
    m->y = rand() % height;
  }  
}

void MiniTacticalGameState::setup_2base_mission(int p0_marines, int p1_marines)
{
  sectioned_setup(p0_marines, p1_marines); 
    
  int width = gp.width, height = gp.height;
  
  // now place 2 bases for p1  
  Base * b1 = new Base(this); 
  new_obj(b1, 1); 
  b1->x = (int)(0.75*width); 
  b1->y = (int)(0.1*height);
   
  Base * b2 = new Base(this); 
  new_obj(b2, 1); 
  b2->x = (int)(0.75*width); 
  b2->y = (int)(0.9*height); 

  // now place 2 bases for p0
  Base * b3 = new Base(this); 
  new_obj(b3, 0); 
  b3->x = (int)(0.25*width); 
  b3->y = (int)(0.1*height);
   
  Base * b4 = new Base(this); 
  new_obj(b4, 0); 
  b4->x = (int)(0.25*width); 
  b4->y = (int)(0.9*height); 

}

void MiniTacticalGameState::object_setup()
{
  //totally_random_setup(10,10); 
  //sectioned_setup(10,8); 
  setup_2base_mission(10, 10); 
  
  //testACR(); 
}

