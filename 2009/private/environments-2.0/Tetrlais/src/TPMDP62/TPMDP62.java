
/*
 * This code was auto-generated by the parameter script. Do not change these values manually.
 */
package TPMDP62;

import Tetrlais.GameState;
import Tetrlais.TetrlaisPiece;
import java.util.Vector;
import GeneralizedTetris.*;

public class TPMDP62 extends AbstractProvingMDPTetris{

    public TPMDP62(){
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

	width=12;
	height=23;
	rewardExponent=1.458532d;
            
	pieceWeights=new double[]{0.677281d, 0.149580d, 0.662580d, 0.953828d, 0.133648d, 0.198171d, 0.518066d};
	GameState g=new GameState(width,height,possibleBlocks,pieceWeights,rewardExponent);    
        super.gameState=g;
    }
}