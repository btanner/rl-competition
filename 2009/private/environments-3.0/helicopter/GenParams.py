import GenParamSamples

method1 = "setWindWaveNS"
method2 = "setWindWaveEW"

f = open('./src/org/rlcommunity/environment/helicopter/generalized/GenHeliParamData.java','w')

f.write("""
/*
 * This code was auto-generated by the parameter script, GenParams.py.
 * Do not change these values manually.
 */
package org.rlcommunity.environment.helicopter.generalized;

public class GenHeliParamData {

    void setParameters(int paramSet, GeneralizedHelicopter thisEnvironment) {
        switch (paramSet) {""")
for i in range(10):
	f.write("""
            case %d:
                thisEnvironment.%s(%fd, %fd, %fd, %fd);
                thisEnvironment.%s(%fd, %fd, %fd, %fd);
                break;""" % (i,
                    method1,
                        GenParamSamples.sampleP1(i),
                        GenParamSamples.sampleP1(i),
                        GenParamSamples.sampleP1(i),
                        GenParamSamples.sampleP1(i),
                    method2,
                        GenParamSamples.sampleP2(i),
                        GenParamSamples.sampleP2(i),
                        GenParamSamples.sampleP2(i),
                        GenParamSamples.sampleP2(i)
                ))

f.write("""
            default:
                System.err.println("Invalid Parameter Set Requested ("+paramSet+") : using Param Set 0");
                setParameters(0, thisEnvironment);
        }
    }
}""")
f.close()