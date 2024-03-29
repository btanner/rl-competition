
#ifndef FUNC_APPROX
#define FUNC_APPROX

#include "SMDPAgent.h"

class FunctionApproximator
{
  int numFeatures, numActions;

  double ranges      [ MAX_STATE_VARS ];
  double minValues   [ MAX_STATE_VARS ];
  double resolutions [ MAX_STATE_VARS ];

 protected:
  double state       [ MAX_STATE_VARS ];

  int getNumFeatures() { return numFeatures; }
  int getNumActions () { return numActions;  }

  double getRange     ( int i ) { return ranges     [ i ]; }
  double getMinValue  ( int i ) { return minValues  [ i ]; }
  double getResolution( int i ) { return resolutions[ i ]; }

 public:
  FunctionApproximator( int numF, int numA, 
			double r[], double m[], double res[] );
  virtual ~FunctionApproximator() {}

  // YL
  // call set state before calling computeQ
  virtual void setState( double s[] );

  virtual double computeQ( int action ) = 0;
  virtual void updateWeights( double delta, double alpha ) = 0;

  virtual void clearTraces( int action ) = 0;
  virtual void decayTraces( double decayRate ) = 0;
  virtual void updateTraces( int action ) = 0;

  virtual void read ( int fd ) = 0;
  virtual void write( int fd ) = 0;

  virtual void doShuffle(int iNumKeepers, int iNumTakers, double dMaxDist, bool setToAverage) = 0; //*met 5/3/05
};

#endif

