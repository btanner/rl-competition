
/*
 * This code was auto-generated by the parameter script. Do not change these values manually.
 */
package org.rlcommunity.environments.competition2009.Tetris.Proving;

import org.rlcommunity.environments.competition2009.Tetris.GameState;
import org.rlcommunity.environments.competition2009.Tetris.TetrlaisPiece;
import java.util.Vector;
import org.rlcommunity.environments.competition2009.Tetris.Generalized.*;

public class TPMDP30 extends AbstractProvingMDPTetris{

    public TPMDP30(){
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

	width=7;
	height=17;
	rewardExponent=1.516454d;
	evilness=0.289478d;
	evilAgentType=0;
	benchmarkScore=5256.000000d;
            
	pieceWeights=new double[]{0.875672d, 0.539714d, 0.525560d, 0.593800d, 0.917697d, 0.928974d, 0.940417d};
	GameState g=new GameState(width,height,possibleBlocks,pieceWeights,rewardExponent,evilness,evilAgentType,benchmarkScore);    
        super.gameState=g;
    }
}