
NOTE:: Please read domains/realTimeStrategy/ INSTALL and README before compiling or running the realTimeStrategyAgentJava.

This java agent for the Real Time Strategy domain.  This has been specifically implemented for the Real Time Strategy domain and will not work with any other competition domains. 


To recompile just the Real Time Strategy java agent:
>> make clean
>> make

in realTimeStrategyAgentJava directory.

Execution begins at main() in the src/RTSAgent/RTSAgent.java. This files
contains action handlers for each unit type; this manual behavior should be
replaced by a method which selects actions based on learning from previous
experience.  

run.bash
- Starts the realTimeStrategyAgentJava process

