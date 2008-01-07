import java.util.Random;

public abstract class AbstractMultiTrainer{
	int currentMDPNumber=-1;
	int iterValue;
	int numMDP = 10;
	
	Random generator = new Random();
	
	
	public AbstractMultiTrainer(int numMDP){
		this.numMDP=numMDP;
	}
	
	abstract protected void runAllMDPs();
	abstract protected double runCurrentMDP(int mdpStepLimit);
	
	private void initRandomStuff(){
		currentMDPNumber = generator.nextInt( numMDP ); // select the starting mdp
		iterValue = generator.nextInt(numMDP); //this value will be used to iterate through
		while(gcd(numMDP, iterValue)!=1){ //insure we get get an iter value that gives us all training MDP's
			iterValue = generator.nextInt(numMDP);
		}
	}
	protected int getNextRandomMDPNumber(){
		if(currentMDPNumber==-1){
			initRandomStuff();
		}else{
			currentMDPNumber = (currentMDPNumber + iterValue)% numMDP;
		}

		return currentMDPNumber;
	}
	
	/**
	  * Euclid's greatest common divisor algorithm.
	  * Find the largest number that evenly divides into both n and d.
	  * require: n >= 0, d >= 0.
	  * fastest if n >= d.
	  */
	private int gcd(int n, int d)
	   {
	   if ( d == 0 )
	      {
	      return 1;
	      }
	   int r = n % d;
	   return( r == 0 ) ? d : gcd( d , r );
	   }
	
	
}