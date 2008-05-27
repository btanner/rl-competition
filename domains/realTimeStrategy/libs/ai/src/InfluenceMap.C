
#include <math.h>

#include <iostream>
#include <vector>
#include <list>

#include "InfluenceMap.H"
#include "Global.H"

using namespace std; 

static int debug_level = 0; 

static bool scale_by_atkpot = true;

#define  INBOUNDS(x,y)    ((x) >= 0 && (y) >= 0 && (x) < rows && (y) < cols)
#define  ARR2D(a,x,y)     ((a)[(x)*rows + (y)])


InfluenceMap::InfluenceMap(MiniGameState & _state, MiniGameParameters & _parms)
  : state(_state), parms(_parms)
{
  values = NULL; 
  ally_peaks = NULL; 
  enemy_peaks = NULL; 
  
  init(); 
}

InfluenceMap::~InfluenceMap()
{
  DELETEARR(values);
  DELETE(ally_peaks);
  DELETE(enemy_peaks);
}

void InfluenceMap::init() 
{
  cwidth = parms.marine_radius*2; 
  cheight = parms.marine_radius*2; 
  
  // grow them until there is something divisible
  while (parms.width % cwidth != 0) cwidth++; 
  while (parms.height % cheight != 0) cheight++;
   
  cols = parms.width / cwidth; 
  rows = parms.height / cheight; 
  
  DLPR(1) << "IMAP cwidth,cheight = " << cwidth << "," << cheight 
          << "   Rows,Cols = " << rows << "," << cols << endl; 
  
  values = new double[rows * cols];  
   
  reset(); 
}

void InfluenceMap::reset()
{
  FORS(r, rows)
    FORS(c, cols)
      ARR2D(values,r,c) = 0; 
}

void InfluenceMap::add_ally(GameObj<MiniGameState> * obj)
{
  int acol = (int)obj->x / (int) cwidth; 
  int arow = (int)obj->y / (int) cheight;
   
  double initialValue = 1.0; 
  
  if (scale_by_atkpot)
    initialValue *= (obj->hp*MAX(obj->get_atkvalue(),1));
  
  ARR2D(values,arow,acol) += initialValue;  
}

void InfluenceMap::add_enemy(GameObj<MiniGameState> * obj)
{
  int acol = (int)obj->x / (int) cwidth; 
  int arow = (int)obj->y / (int) cheight;  
  
  double initialValue = 1.0; 
  
  if (scale_by_atkpot)
    initialValue *= (obj->hp*MAX(obj->get_atkvalue(),1));
  
  ARR2D(values,arow,acol) -= initialValue;    
}

void InfluenceMap::print() 
{
  FORS(r, rows)
  {
    FORS(c, cols)
    {
      cout << ARR2D(values,r,c) << " "; 
    }
    
    cout << endl;
  }        
}

double InfluenceMap::get_value(int r, int c)
{
  return ARR2D(values,r,c);
}

pair<int,int> InfluenceMap::get_mapcoord(int r, int c)
{
  int x = (int)(c*cwidth + (double)cwidth/2.0);
  int y = (int)(r*cheight + (double)cheight/2.0);
  
  return make_pair(x,y);
}

void InfluenceMap::propogate_influence()
{
  DLPR(2) << "Propogating influence ... " << endl; 
  
  list<double*> masks; 
  double * mask; 
  
  FORS(r, rows)
    FORS(c, cols)
    {
      /*
        For each "source", do the following: 
            1. Create a mask to insert values (influence for each source)
            2. Propogate 35% diagonally, 50% cardinally to make a 3x3 square
            3. For each corner, create a new corner of the larger square by 
               prop diag then all other cells along the square cardinally
      */      
      
      if (ARR2D(values,r,c) < -0.1 || ARR2D(values,r,c) > 0.1)
      {                
        // Source detected 
        DLPR(3) << "Source detected at " << r << "," << c;         
        DLPR(3) << " val = " << ARR2D(values,r,c) << endl;         
        
        mask = new double[rows*cols]; 
        
        ARR2D(mask,r,c) = ARR2D(values,r,c); 
        
        int tlr = r, tlc = c, brr = r, brc = c;
        
        while(true)
        {
          // enlargen box          
          tlr--; tlc--; 
          brr++; brc++; 
          
          // break when all out of bounds
          if (tlr < 0 && tlc < 0 && brr >= rows && brc >= cols)
            break; 
          
          // 4 corners first. 
          
          if (INBOUNDS(tlr,tlc))   // top left            
            ARR2D(mask,tlr,tlc) = 0.35*ARR2D(mask,tlr+1,tlc+1); 
          
          if (INBOUNDS(tlr,brc))   // top right
            ARR2D(mask,tlr,brc) = 0.35*ARR2D(mask,tlr+1,brc-1); 
                            
          if (INBOUNDS(brr,tlc))   // bottom left
            ARR2D(mask,brr,tlc) = 0.35*ARR2D(mask,brr-1,tlc+1); 
          
          if (INBOUNDS(brr,brc))   // bottom right
            ARR2D(mask,brr,brc) = 0.35*ARR2D(mask,brr-1,brc-1);
           
          // Now the sides of the box
          
          int rp, cp; 
          
          for (rp = tlr, cp = tlc+1; cp < brc; cp++)   // top
            if(INBOUNDS(rp,cp))
              ARR2D(mask,rp,cp) = 0.5*ARR2D(mask,rp+1,cp);
          
          for (rp = brr, cp = tlc+1; cp < brc; cp++)   // bottom
            if(INBOUNDS(rp,cp))
              ARR2D(mask,rp,cp) = 0.5*ARR2D(mask,rp-1,cp);
          
          for (rp = tlr+1, cp = tlc; rp < brr; rp++)   // left side
            if(INBOUNDS(rp,cp))
              ARR2D(mask,rp,cp) = 0.5*ARR2D(mask,rp,cp+1);
          
          for (rp = tlr+1, cp = brc; rp < brr; rp++)   // left side
            if(INBOUNDS(rp,cp))
              ARR2D(mask,rp,cp) = 0.5*ARR2D(mask,rp,cp-1);          
        }
        
        masks.push_back(mask);         
      }      
    }
    
  // Apply masks to values. Note: don't add them to the current values. 
  // Simply replace the values in the inf. map by the sum of the masks
    
  FORS(r, rows)
    FORS(c, cols)
    {
      ARR2D(values,r,c) = 0; 
      
      FORALL(masks, iter) 
      {
        double* mask = *iter;         
        ARR2D(values,r,c) += ARR2D(mask,r,c); 
      }
    }
  
  // Free memory used up by the masks 
    
  FORALL(masks, iter)
  {
    double* mask = *iter;         
    DELETEARR(mask);
  }
}

void InfluenceMap::blur(int passes, int radius)
{
  FORS(p, passes)
  {
    double * mask = new double[rows*cols];
    
    FORS(r, rows)
      FORS(c, cols)  
      {
        int n = 0; 
        double sum = 0; 
        
        for (int rp = r-radius; rp <= r+radius; rp++)
          for (int cp = c-radius; cp <= c+radius; cp++)
          {
            if (INBOUNDS(rp,cp))
            {
              sum += ARR2D(values,r,c); 
              n++; 
            }
          }
          
        ARR2D(mask,r,c) = sum/n; 
      }
      
    // Apply blurring mask            
    FORS(r, rows)
      FORS(c, cols)
      {
        ARR2D(values,r,c) = ARR2D(mask,r,c);       
      }
            
    delete [] mask; 
  }
}

void InfluenceMap::localize(int passes, int radius)
{
  FORS(p, passes)
  {
    double * mask = new double[rows*cols];
    
    FORS(r, rows)
        FORS(c, cols)  
    {
      double sum = 0; 
        
      for (int rp = r-radius; rp <= r+radius; rp++)
        for (int cp = c-radius; cp <= c+radius; cp++)
      {
        if (r == rp && c == cp)
          continue; 
        
        if (INBOUNDS(rp,cp))
        {
          sum += ARR2D(values,r,c)/DIST(r,c,rp,cp); 
        }
      }
          
      ARR2D(mask,r,c) = ARR2D(values,r,c) + sum; 
    }
      
    // Apply localizing mask            
    FORS(r, rows)
        FORS(c, cols)
    {
      ARR2D(values,r,c) = ARR2D(mask,r,c);       
    }
            
    delete [] mask; 
  }
}



// This produces too many peaks :( 
void InfluenceMap::calc_peaks_stat()
{
  if (ally_peaks == NULL) 
    ally_peaks = new list< pair<int,int> >; 
  
  if (enemy_peaks == NULL) 
    enemy_peaks = new list< pair<int,int> >; 
   
  ally_peaks->clear();
  enemy_peaks->clear(); 
  
  // In one pass, calc mean and variance of positives (allies) 
  // and negatives (enemies) ... see Alg ||| from 
  // http://en.wikipedia.org/wiki/Algorithms_for_calculating_variance
  
  double ally_sum = 0, ally_sumsqrs = 0, ally_n = 0; 
  double enemy_sum = 0, enemy_sumsqrs = 0, enemy_n = 0; 
   
  FORS(r, rows)
      FORS(c, cols)
  {            
    double x = ARR2D(values,r,c); 
      
    if (x > 0)
    {
      ally_n++;
      ally_sum += x; 
      ally_sumsqrs += (x*x); 
    }
    else if (x < 0)
    {
      enemy_n++;
      enemy_sum += x; 
      enemy_sumsqrs += (x*x); 
    }      
  }
    
  double ally_mean = ally_sum / ally_n;   
  double enemy_mean = enemy_sum / enemy_n;   
  
  double ally_variance = (ally_sumsqrs - (ally_sum*ally_sum)/ally_n)/ally_n; 
  double enemy_variance = (enemy_sumsqrs - (enemy_sum*enemy_sum)/enemy_n)/enemy_n; 
  
  double ally_stddev = sqrt(ally_variance); 
  double enemy_stddev = sqrt(enemy_variance); 
  
  DLPR(3) << "Ally mean/stddev is " << ally_mean << "/" << ally_stddev << endl; 
  DLPR(3) << "Enemy mean/stddev is " << enemy_mean << "/" << enemy_stddev << endl; 
  
  // now get the peaks
  
  FORS(r, rows)
      FORS(c, cols)
  {            
    double x = ARR2D(values,r,c); 
          
    if (x > 0)
    {
      if (x > (ally_mean + 3*ally_stddev))
      {
        DLPR(3) << "Ally peak " << x << endl; 
      }
    }
    else if (x < 0)
    {
      if (x < (enemy_mean - 3*enemy_stddev))
      {
        DLPR(3) << "Enemy peak " << x << endl; 
      }
    }      
  }
  
}

void InfluenceMap::calc_peaks()
{
  if (ally_peaks == NULL) 
    ally_peaks = new list< pair<int,int> >; 
  
  if (enemy_peaks == NULL) 
    enemy_peaks = new list< pair<int,int> >;
  
  ally_peaks->clear();
  enemy_peaks->clear();   
  
  int AR = 5; // area radius for peaks
  
  FORS(r, rows)
    FORS(c, cols)
  {
    bool done = false; 
    
    // look for ally peaks 
    
    if (ARR2D(values,r,c) > 0)
    {
      for (int rp = r-AR; rp <= r+AR && !done; rp++)
        for (int cp = c-AR; cp <= c+AR; cp++)
        {
          if (INBOUNDS(rp, cp) && ARR2D(values,rp,cp) > ARR2D(values,r,c))
          {
            done = true; 
            break; 
          }
        }
        
      if (!done) 
      {
        DLPR(3) << "Found ally peak " << r << "," << c << " " << ARR2D(values,r,c) << endl;  
        ally_peaks->push_back(make_pair(r,c)); 
      }
    }    
    
    done = false; 
    
    // look for enemy peaks
    
    if (ARR2D(values,r,c) < 0)
    {
      for (int rp = r-AR; rp <= r+AR && !done; rp++)
        for (int cp = c-AR; cp <= c+AR; cp++)
        {
          if (INBOUNDS(rp, cp) && ARR2D(values,rp,cp) < ARR2D(values,r,c))
          {
            done = true; 
            break; 
          }
        }
      
      if (!done) 
      {
        DLPR(3) << "Found enemy peak " << r << "," << c << " " << ARR2D(values,r,c) << endl;
        enemy_peaks->push_back(make_pair(r,c));         
      }
    }    
      
            
  }
  
}
