
/*
 * This code was auto-generated by the parameter script. Do not change these values manually.
 */
package org.rlcommunity.environments.competition2009.Tetris.Proving;

import org.rlcommunity.environments.competition2009.Tetris.GameState;
import org.rlcommunity.environments.competition2009.Tetris.TetrlaisPiece;
import java.util.Vector;
import org.rlcommunity.environments.competition2009.Tetris.Generalized.*;

public class TPMDP19 extends AbstractProvingMDPTetris{

    public TPMDP19(){
	super();
	int width=0;
	int height=0;
	double []pieceWeights=null;
	double rewardExponent=1.0d;
	double evilness = 0.0d;
	int evilAgentType = 0;
	double benchmarkScore = 0.0;
		
	Vector<TetrlaisPiece> possibleBlocks=new Vector<TetrlaisPiece>();
        possibleBlocks.add(TetrlaisPiece.makeLine());
	possibleBlocks.add(TetrlaisPiece.makeSquare());
        possibleBlocks.add(TetrlaisPiece.makeTri());
	possibleBlocks.add(TetrlaisPiece.makeSShape());
	possibleBlocks.add(TetrlaisPiece.makeZShape());
	possibleBlocks.add(TetrlaisPiece.makeLShape());
	possibleBlocks.add(TetrlaisPiece.makeJShape());

	width=10;
	height=20;
	rewardExponent=1.881390d;
	evilness=0.547334d;
	evilAgentType=3;
	benchmarkScore=2499.000000d;
            
	pieceWeights=new double[]{0.876012d, 0.559667d, 0.895819d, 0.610501d, 0.731889d, 0.695255d, 0.885297d};
	GameState g=new GameState(width,height,possibleBlocks,pieceWeights,rewardExponent,evilness,evilAgentType,benchmarkScore);    
        super.gameState=g;
    }
}