
#ifndef INFLUENCEMAP_H_
#define INFLUENCEMAP_H_

#include <list>

#include "Global.H"
#include "GameObj.H"
#include "MiniGameState.H"

class InfluenceMap
{
    MiniGameState & state; 
    MiniGameParameters & parms; 
    
    int cwidth, cheight; 
    int rows, cols; 
        
    double * values; 
    
    std::list< std::pair<int,int> > * ally_peaks; 
    std::list< std::pair<int,int> > * enemy_peaks; 
    
    void init(); 
    
public:
     
    InfluenceMap(MiniGameState & _state, MiniGameParameters & _parms); 
    virtual ~InfluenceMap(); 
    
    std::list< std::pair<int,int> > * get_ally_peaks()
    { return ally_peaks; }
    
    std::list< std::pair<int,int> > * get_enemy_peaks()
    { return enemy_peaks; }
        
    double get_value(int r, int c); 
    std::pair<int,int> get_mapcoord(int r, int c); 
    
    void reset();     
    void add_ally(GameObj<MiniGameState> * obj); 
    void add_enemy(GameObj<MiniGameState> * obj); 
    void propogate_influence(); 
    void blur(int passes, int radius); 
    void localize(int passes, int radius); 
        
    void print(); 
    
    void calc_peaks_stat(); 
    void calc_peaks(); 
};

#endif
