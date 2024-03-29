/* console Trainer for RL Competition
* Copyright (C) 2007, Marc Lanctot
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
import java.io.IOException;

public class RealTimeStrategyMultiTrainer
{	
	public static void main(String[] args) {
		double totalReturn = 0.0d;
		int rtsStepLimit=37500000;
		//Uncomment one of the two lines below depending if you want random order or sequential order
		//int thisMDPNumber=i;
		System.out.print("Mdp Started :: ");
		RLGlue.RL_init();
		int stepsRemaining = rtsStepLimit;
		int totalEpisodes = 0;

		while (stepsRemaining > 0) {
            RLGlue.RL_episode(stepsRemaining);

            int thisStepCount = RLGlue.RL_num_steps();
            stepsRemaining -= thisStepCount;

            totalReturn += RLGlue.RL_return();
            totalEpisodes++;
        }
		System.out.println("Mdp completed with " + totalEpisodes + " episodes");
		RLGlue.RL_cleanup();

		System.out.println("All MDPs have finished with a total reward of "+totalReturn);
	}
}
