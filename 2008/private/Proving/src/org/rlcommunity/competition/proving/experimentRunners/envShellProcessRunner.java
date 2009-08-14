/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package org.rlcommunity.competition.proving.experimentRunners;

import java.io.File;

/**
 *
 * @author btanner
 */
public class envShellProcessRunner extends externalProcessRunner{

    public envShellProcessRunner(){
        super("envShell");
    }
    @Override
    protected Process createProcess() {
            try {
                String basePath = "..";
                String systemPath = basePath + File.separator+"system";
                String libPath = systemPath + File.separator+"libraries";
                String provingPath = systemPath + File.separator+"proving";
                String compLib = libPath + File.separator+"RLVizLib.jar";
//                String compLib = "C:\\cygwin\\home\\Administrator\\rl-competition\\system\\libraries\\RLVizLib.jar";

                String envShellLib = libPath + File.separator+"EnvironmentShell.jar";

                String userDir = System.getProperty("user.dir");
                String rlVizLibPathArg = "-DRLVIZ_LIB_PATH=" + userDir + File.separator+ provingPath;

                ProcessBuilder theProcessBuilder = new ProcessBuilder("java", rlVizLibPathArg, "-Xmx384M", "-classpath",compLib + File.pathSeparator + envShellLib,"rlglue.environment.EnvironmentLoader", "environmentShell.EnvironmentShell");
                theProcessBuilder.redirectErrorStream(true);

                return theProcessBuilder.start();
        }catch(Exception e){
            System.err.println("envShellProcessRunner died when trying to create envShellProcess with exception: "+e);
        }
            return null;
    }
    
        @Override
    public void subClassKillProcess() {
        theProcess.destroy();
    }

}
