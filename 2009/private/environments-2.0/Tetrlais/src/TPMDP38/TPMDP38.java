
/*
 * This code was auto-generated by the parameter script. Do not change these values manually.
 */
package TPMDP38;

import Tetrlais.GameState;
import Tetrlais.TetrlaisPiece;
import java.util.Vector;
import GeneralizedTetris.*;

public class TPMDP38 extends AbstractProvingMDPTetris{

    public TPMDP38(){
	super();
	int width=0;
	int height=0;
	double []pieceWeights=null;
	double rewardExponent=1.0d;
		
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
	rewardExponent=1.685223d;
            
	pieceWeights=new double[]{0.031777d, 0.691070d, 0.881135d, 0.888148d, 0.596041d, 0.804911d, 0.469695d};
	GameState g=new GameState(width,height,possibleBlocks,pieceWeights,rewardExponent);    
        super.gameState=g;
    }
}