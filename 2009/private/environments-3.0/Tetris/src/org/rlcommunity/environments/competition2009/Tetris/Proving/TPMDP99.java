
/*
 * This code was auto-generated by the parameter script. Do not change these values manually.
 */
package org.rlcommunity.environments.competition2009.Tetris.Proving;

import org.rlcommunity.environments.competition2009.Tetris.GameState;
import org.rlcommunity.environments.competition2009.Tetris.TetrlaisPiece;
import java.util.Vector;
import org.rlcommunity.environments.competition2009.Tetris.Generalized.*;

public class TPMDP99 extends AbstractProvingMDPTetris{

    public TPMDP99(){
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

	width=8;
	height=21;
	rewardExponent=1.802715d;
	evilness=0.074345d;
	evilAgentType=1;
	benchmarkScore=4723.000000d;
            
	pieceWeights=new double[]{0.816336d, 0.742430d, 0.588339d, 0.555255d, 0.786908d, 0.597024d, 0.769748d};
	GameState g=new GameState(width,height,possibleBlocks,pieceWeights,rewardExponent,evilness,evilAgentType,benchmarkScore);    
        super.gameState=g;
    }
}