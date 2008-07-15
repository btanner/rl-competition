/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package org.rlcommunity.competition.proving.experimentRunners;

import org.rlcommunity.competition.proving.*;

import org.rlcommunity.competition.phoneHomeInterface.RLEvent;

/**
 *
 * @author btanner
 */
public class ExperimentRunningFactory {
    

    public static AbstractExperimentRunner makeExperimentRunner(RLEvent theEvent, Controller theController) {
       if(theEvent==RLEvent.Helicopter)return new JarExperimentRunner(theEvent,theController);
       if(theEvent==RLEvent.MountainCar)return new JarExperimentRunner(theEvent,theController);
       if(theEvent==RLEvent.Tetris)return new JarExperimentRunner(theEvent,theController);
       if(theEvent==RLEvent.RTS)return new CPPExperimentRunner(theEvent,theController);
       if(theEvent==RLEvent.Polyathlon)return new JarExperimentRunner(theEvent,theController);
       if(theEvent==RLEvent.Keepaway)return new ScriptExperimentRunner(theEvent, theController, "../system/proving/");
       
       //hack for testing?
       if(theEvent==RLEvent.Testing_Helicopter)return new JarExperimentRunner(theEvent,theController);
       if(theEvent==RLEvent.Testing_MountainCar)return new JarExperimentRunner(theEvent,theController);
       if(theEvent==RLEvent.Testing_Tetris)return new JarExperimentRunner(theEvent,theController);
       if(theEvent==RLEvent.Testing_RTS)return new CPPExperimentRunner(theEvent,theController);
       if(theEvent==RLEvent.Testing_Polyathlon)return new JarExperimentRunner(theEvent,theController);
       if(theEvent==RLEvent.Testing_Keepaway)return new ScriptExperimentRunner(theEvent, theController, "../system/proving/");
       //if(theEvent==RLEvent.Keepaway)return new keepawayExperimentRunner(theEvent,theController);
       
        System.err.println("Unknown event");
        return null;
    }

}
