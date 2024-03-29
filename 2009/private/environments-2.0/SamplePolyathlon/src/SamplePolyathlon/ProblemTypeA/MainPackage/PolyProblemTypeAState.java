/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package SamplePolyathlon.ProblemTypeA.MainPackage;

import java.text.DecimalFormat;
import java.text.NumberFormat;
import java.util.Random;
import rlVizLib.utilities.random.myBeta;

/**
 *
 * @author btanner
 */
public class PolyProblemTypeAState {
        NumberFormat formatter = new DecimalFormat ( "000.00" ) ;
    
    
//	Current State Information
	public double position;
	public double velocity;

//Some of these are fixed
	public double minPosition = -1.2;
	public double maxPosition = 0.6;
	public double minVelocity = -0.07;    
	public double maxVelocity = 0.07;    
	public double goalPosition = 0.5;

	public double accelerationFactor = 0.001;
	public double gravityFactor = -0.0025;
	public double hillPeakFrequency = 3.0;

	public double defaultInitPosition=-0.5d;
	public double defaultInitVelocity=0.0d;



	public double rewardPerStep=-1.0d;
	public double rewardAtGoal=0.0d;

//These are configurable
//Remember to clone these
	public  boolean randomStarts=false;
	public double scaleP = 1.0d;
	public double scaleV = 1.0d;


	public double pOffset = 0.0d;
	public double vOffset = 0.0d;
                
    
        private Random randomGenerator;
        private myBeta betaGenerator;



        PolyProblemTypeAState(Random randomGenerator, myBeta betaGenerator) {
                this.randomGenerator=randomGenerator;
                this.betaGenerator=betaGenerator;
        }

        public PolyProblemTypeAState(PolyProblemTypeAState stateToCopy){
            this.position=stateToCopy.position;
            this.velocity=stateToCopy.velocity;
            this.minPosition=stateToCopy.minPosition;
            this.maxPosition=stateToCopy.maxPosition;
            this.minVelocity=stateToCopy.minVelocity;
            this.maxVelocity=stateToCopy.maxVelocity;
            this.goalPosition=stateToCopy.goalPosition;
            this.accelerationFactor=stateToCopy.accelerationFactor;
            this.gravityFactor=stateToCopy.gravityFactor;
            this.hillPeakFrequency=stateToCopy.hillPeakFrequency;
            this.defaultInitPosition=stateToCopy.defaultInitPosition;
            this.defaultInitVelocity=stateToCopy.defaultInitVelocity;
            this.rewardPerStep=stateToCopy.rewardPerStep;
            this.rewardAtGoal=stateToCopy.rewardAtGoal;

            this.randomStarts=stateToCopy.randomStarts;
            this.scaleP=stateToCopy.scaleP;
            this.scaleV=stateToCopy.scaleV;
            this.pOffset=stateToCopy.pOffset;
            this.vOffset=stateToCopy.vOffset;
            this.rewardAtGoal=stateToCopy.rewardAtGoal;
            this.rewardAtGoal=stateToCopy.rewardAtGoal;

//These are pointers but that's ok

            this.randomGenerator=stateToCopy.randomGenerator;
            this.betaGenerator=stateToCopy.betaGenerator;


            
        }

        
        //	Stopping condition
	public boolean inGoalRegion(){
		return position >= goalPosition;
	}

    String stringSerialize() {

       StringBuffer b=new StringBuffer();
       //There are 2 things we're putting in
       b.append("2");
        //It's a double
       b.append("_d_");
       b.append(formatter.format(position));
       b.append("_d_");
       b.append(formatter.format(velocity));
       b.append("_");
       return b.toString();
    }

void update(int a) {
        double variedAccel = accelerationFactor;

        velocity += ((a - 1)) * variedAccel + getSlope(position) * (gravityFactor);
        if (velocity > maxVelocity) {
            velocity = maxVelocity;
        }
        if (velocity < minVelocity) {
            velocity = minVelocity;
        }
        position += velocity;
        if (position > maxPosition) {
            position = maxPosition;
        }
        if (position < minPosition) {
            position = minPosition;
        }
        if (position == minPosition && velocity < 0) {
            velocity = 0;
        }

    }

    
    	public double getHeightAtPosition(double queryPosition){
		return -Math.sin(hillPeakFrequency*(queryPosition));
	}

	public double getSlope(double queryPosition){
		/*The curve is generated by cos(hillPeakFrequency(x-pi/2)) so the 
		 * pseudo-derivative is cos(hillPeakFrequency* x) 
		 */
		return Math.cos(hillPeakFrequency*queryPosition);
	}
        
        
	public double getReward(){
		if(inGoalRegion())
			return rewardAtGoal;
		else
			return rewardPerStep;
	}


}
