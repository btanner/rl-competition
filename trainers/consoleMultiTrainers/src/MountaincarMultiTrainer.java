/* console Trainer for RL Competition
* Copyright (C) 2007, Brian Tanner brian@tannerpages.com (http://brian.tannerpages.com/)
* 
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA. */
import rlglue.RLGlue;

public class MountaincarMultiTrainer extends AbstractMultiTrainer {

	public static void main(String[] args) throws InterruptedException {
		AbstractMultiTrainer theTrainer=new MountaincarMultiTrainer();
		theTrainer.runAllMDPs();
	}
	
	//maximum number of steps per MDP
	int mountaincarStepLimit=100000;
	int thisMDPNumber;
	
	public MountaincarMultiTrainer(){
		//The number of training MDPs
		super(30);
	}
	
	protected double runCurrentMDP(int mdpStepLimit){
		System.out.print("Mdp " + thisMDPNumber + " Started :: ");
		
		RLGlue.RL_init();
		int stepsRemaining = mdpStepLimit;
		int totalEpisodes = 0;
		double returnThisMDP=0.0d;

		while (stepsRemaining > 0) {
            RLGlue.RL_episode(stepsRemaining);

            int thisStepCount = RLGlue.RL_num_steps();
            stepsRemaining -= thisStepCount;

            returnThisMDP += RLGlue.RL_return();
            totalEpisodes++;
        }
		System.out.println("Mdp " + thisMDPNumber + " completed with " + totalEpisodes + " episodes");
		RLGlue.RL_cleanup();
		return returnThisMDP;
	}

	protected void runAllMDPs(){
		double totalReturn = 0.0d;
		for(int i=0;i<super.numMDP;i++){
			//Uncomment one of the two lines below depending if you want random order or sequential order
			thisMDPNumber=getNextRandomMDPNumber();
			//int thisMDPNumber=i;

			consoleTrainerHelper.loadMountainCar(thisMDPNumber);
			totalReturn+=runCurrentMDP(mountaincarStepLimit);
		}	

		System.out.println("All MDPs have finished with a total reward of "+totalReturn);
	}
}