
/*
 * This code was auto-generated by the parameter script. Do not change these values manually.
 */
package TPMDP57;

import Tetrlais.GameState;
import Tetrlais.TetrlaisPiece;
import java.util.Vector;
import GeneralizedTetris.*;

public class TPMDP57 extends AbstractProvingMDPTetris{

    public TPMDP57(){
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

	width=7;
	height=18;
	rewardExponent=1.450916d;
            
	pieceWeights=new double[]{0.595607d, 0.766946d, 0.766515d, 0.037595d, 0.205174d, 0.335769d, 0.945741d};
	GameState g=new GameState(width,height,possibleBlocks,pieceWeights,rewardExponent);    
        super.gameState=g;
    }
}