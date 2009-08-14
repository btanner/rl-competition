/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package org.rlcommunity.competition.phoneHomeInterface;

/**
 *
 * @author btanner
 */
public enum RLEvent {
	Helicopter(0),
	MountainCar(1),
	Tetris(2),
	RTS(3),
	Polyathlon(4),
        Keepaway(5),
        Testing_Helicopter(6),
	Testing_MountainCar(7),
	Testing_Tetris(8),
	Testing_RTS(9),
	Testing_Polyathlon(10),
        Testing_Keepaway(11);
	private final int id;
	
	RLEvent(int id){
        this.id = id;
    }
    public int id()   {return id;}
    
    public static RLEvent eventFromNumber(int id){
    if(id==RLEvent.Helicopter.id())return RLEvent.Helicopter;
    if(id==RLEvent.MountainCar.id())return RLEvent.MountainCar;
    if(id==RLEvent.Tetris.id())return RLEvent.Tetris;
    if(id==RLEvent.RTS.id())return RLEvent.RTS;
    if(id==RLEvent.Polyathlon.id())return RLEvent.Polyathlon;
    if(id==RLEvent.Keepaway.id())return RLEvent.Keepaway;
    if(id==RLEvent.Testing_Helicopter.id())return RLEvent.Testing_Helicopter;
    if(id==RLEvent.Testing_MountainCar.id())return RLEvent.Testing_MountainCar;
    if(id==RLEvent.Testing_Tetris.id())return RLEvent.Testing_Tetris;
    if(id==RLEvent.Testing_RTS.id())return RLEvent.Testing_RTS;
    if(id==RLEvent.Testing_Polyathlon.id())return RLEvent.Testing_Polyathlon;
    if(id==RLEvent.Testing_Keepaway.id())return RLEvent.Testing_Keepaway;
    
    return null;
        }
    public static int numEvents(){return 12;}
//For convenience
    public static String name(int id){
        if(id == Helicopter.id())return "Helicopter";
        if(id == MountainCar.id())return "MountainCar";
        if(id == Tetris.id())return "Tetris";
        if(id == RTS.id())return "RTS";
        if(id == Polyathlon.id())return "Polyathlon";
        if(id == Keepaway.id())return "Keepaway";
        if(id == Testing_Helicopter.id())return "Testing_Helicopter";
        if(id == Testing_MountainCar.id())return "Testing_MountainCar";
        if(id == Testing_Tetris.id())return "Testing_Tetris";
        if(id == Testing_RTS.id())return "Testing_RTS";
        if(id == Testing_Polyathlon.id())return "Testing_Polyathlon";
        if(id == Testing_Keepaway.id())return "Testing_Keepaway";
        return "Type: "+id+" is unknown Event Type";
    }

    public static String name(RLEvent theEvent) {
        return name(theEvent.id());
    }
}
