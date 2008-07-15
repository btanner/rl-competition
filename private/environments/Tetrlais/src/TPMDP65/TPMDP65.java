
/*
 * This code was auto-generated by the parameter script. Do not change these values manually.
 */
package TPMDP65;

import Tetrlais.GameState;
import Tetrlais.TetrlaisPiece;
import java.util.Vector;
import GeneralizedTetris.*;

public class TPMDP65 extends AbstractProvingMDPTetris{

    public TPMDP65(){
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
	height=20;
	rewardExponent=1.222407d;
            
	pieceWeights=new double[]{0.906440d, 0.453796d, 0.353942d, 0.417540d, 0.545800d, 0.696814d, 0.189697d};
	GameState g=new GameState(width,height,possibleBlocks,pieceWeights,rewardExponent);    
        super.gameState=g;
    }
}