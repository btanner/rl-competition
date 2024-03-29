/*
 * BaseGameEntity.java
 * 
 * Created on Oct 16, 2007, 9:23:57 PM
 * 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package GeneralGameCode;



/**
 *
 * @author btanner
 */
public class BaseGameEntity {
//  enum {default_entity_type = -1};
  //each entity has a unique ID
private  int         m_ID;

  //every entity has a type associated with it (health, troll, ammo etc)
private  int         m_EntityType;

  //this is a generic flag. 
 private boolean        m_bTag;

  //used by the ructor to give each entity a unique ID
 static int NextID=0;
 private int NextValidID(){return NextID++;}


  
  //its location in the environment
protected  Vector2D m_vPos=null;

 protected Vector2D m_vScale=null;

  //the length of this object's bounding radius
protected  double    m_dBoundingRadius;

//-1 is default entity type  
 public BaseGameEntity(){
     this(-1);
}
 
 public BaseGameEntity(int entity_type){
     this(entity_type,new Vector2D(), 0.0d);
 }
   
  BaseGameEntity(int entity_type, Vector2D pos, double r){
      this.m_vPos=pos;
      this.m_dBoundingRadius=r;
      this.m_EntityType=entity_type;
      m_ID=NextValidID();
      m_vScale=new Vector2D(1.0d,1.0d);
      m_bTag=false;
 }
  
  
  void Update(double time_elapsed){}; 

   void Render(){};

   boolean HandleMessage(String  msg){return false;}
  
  //entities should be able to read/write their data to a stream
//   void Write(std::ostream&  os){}
  // void Read (std::ifstream& is){}
  


  


  Vector2D     Pos(){return m_vPos;}
  void         SetPos(Vector2D new_pos){m_vPos = new_pos;}

  double        BRadius(){return m_dBoundingRadius;}
  void         SetBRadius(double r){m_dBoundingRadius = r;}
  int          ID(){return m_ID;}

  boolean         IsTagged(){return m_bTag;}
  void         Tag(){m_bTag = true;}
  void         UnTag(){m_bTag = false;}

  Vector2D     Scale(){return m_vScale;}
  void         SetScale(Vector2D val){m_dBoundingRadius *= Math.max(val.x, val.y)/Math.max(m_vScale.x, m_vScale.y); m_vScale = val;}
  void         SetScale(double val){m_dBoundingRadius *= (val/Math.max(m_vScale.x, m_vScale.y)); m_vScale =new Vector2D(val, val);} 


  int          getEntityType(){return m_EntityType;}
  void         SetEntityType(int new_type){m_EntityType = new_type;}
 
}
