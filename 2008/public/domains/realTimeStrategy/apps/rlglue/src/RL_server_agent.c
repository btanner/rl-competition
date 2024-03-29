/* 
* Copyright (C) 2007, Adam White

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include <string.h> /* strlen */
#include <stdlib.h> /* calloc */
#include <assert.h> /* assert */

#include <stdio.h> /* fprintf : debug */

#include <RL_common.h>
#include <Network/RL_network.h>

static Action theAction = {0};
static rlBuffer theBuffer = {0};
static char* theOutMessage = 0;

/* These are defined in RL_network_agent and RL_direct_agent */
extern void rlSetAgentConnection(int);
extern int rlGetAgentConnection();

/* Send the task spec to the agent */
void agent_init(const Task_specification theTaskSpec) {
  int agentState = kAgentInit;
  unsigned int theTaskSpecLength = 0;
  unsigned int offset = 0;
  
  if (theTaskSpec != NULL)
    theTaskSpecLength = strlen(theTaskSpec);

  if (theBuffer.capacity == 0)
    rlBufferCreate(&theBuffer, 65536);

  /* send across agent_init specific data */
  rlBufferClear(&theBuffer);
  offset = 0;

  /* Strings are always preceeded by their length, and do not include their null terminating character */
  offset = rlBufferWrite(&theBuffer, offset, &theTaskSpecLength, 1, sizeof(int));
  if (theTaskSpecLength > 0) {
    offset = rlBufferWrite(&theBuffer, offset, theTaskSpec, theTaskSpecLength, sizeof(char));
  }
  rlSendBufferData(rlGetAgentConnection(), &theBuffer, agentState);

  /* Receive the receipt from the Client, to ensure that AgentInit has been completed */
  rlBufferClear(&theBuffer);
  rlRecvBufferData(rlGetAgentConnection(), &theBuffer, &agentState);
  assert(agentState == kAgentInit);
}

/* Send the observation to the agent, receive the action and return it */
Action agent_start(Observation theObservation) {
  int agentState = kAgentStart;
  unsigned int offset = 0;

  rlBufferClear(&theBuffer);
  offset = 0;
  offset = rlCopyADTToBuffer(&theObservation, &theBuffer, offset);
  rlSendBufferData(rlGetAgentConnection(), &theBuffer, agentState);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(rlGetAgentConnection(), &theBuffer, &agentState);
  assert(agentState == kAgentStart);
    
  offset = 0;
  offset = rlCopyBufferToADT(&theBuffer, offset, &theAction);

  return theAction;
}

/* Send the reward and the observation to the agent, receive the action and return it */
Action agent_step(Reward theReward, Observation theObservation) {
  int agentState = kAgentStep;
  unsigned int offset = 0;

  rlBufferClear(&theBuffer);
  offset = 0;
  offset = rlBufferWrite(&theBuffer, offset, &theReward, 1, sizeof(Reward));
  offset = rlCopyADTToBuffer(&theObservation, &theBuffer, offset);
  rlSendBufferData(rlGetAgentConnection(), &theBuffer, agentState);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(rlGetAgentConnection(), &theBuffer, &agentState);

  assert(agentState == kAgentStep);

  offset = 0;
  offset = rlCopyBufferToADT(&theBuffer, offset, &theAction);

  return theAction;
}

/* Send the final reward to the agent */
void agent_end(Reward theReward) { 
  int agentState = kAgentEnd;
  unsigned int offset = 0;

  rlBufferClear(&theBuffer);
  /*offset = rlBufferWrite(&theBuffer, offset, &agentState, 1, sizeof(int));*/ /* Removed, shouldn't have been sent. */
  offset = rlBufferWrite(&theBuffer, offset, &theReward, 1, sizeof(Reward));
  rlSendBufferData(rlGetAgentConnection(), &theBuffer, agentState);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(rlGetAgentConnection(), &theBuffer, &agentState);
  assert(agentState == kAgentEnd);
}

/* Tell the agent that we're cleaning up */
void agent_cleanup() {
  int agentState = kAgentCleanup;

  rlBufferClear(&theBuffer);
  rlSendBufferData(rlGetAgentConnection(), &theBuffer, agentState);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(rlGetAgentConnection(), &theBuffer, &agentState);
  assert(agentState == kAgentCleanup);

  if (theAction.intArray != 0) {
    free(theAction.intArray);
    theAction.intArray    = 0;
    theAction.numInts     = 0;
  }

  if (theAction.doubleArray != 0) {
    free(theAction.doubleArray);
    theAction.doubleArray = 0;
    theAction.numDoubles  = 0;
  }

  if (theOutMessage != 0) {
    free(theOutMessage);
    theOutMessage = 0;
  }

  rlBufferDestroy(&theBuffer);
}

void agent_freeze() {
  int agentState = kAgentFreeze;

  rlBufferClear(&theBuffer);
  rlSendBufferData(rlGetAgentConnection(), &theBuffer, agentState);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(rlGetAgentConnection(), &theBuffer, &agentState);
  assert(agentState == kAgentFreeze);
}

Message agent_message(const Message inMessage) {
  int agentState = kAgentMessage;
  unsigned int theInMessageLength = 0;
  unsigned int theOutMessageLength = 0;
  unsigned int offset = 0;

  if (inMessage != 0) {
    theInMessageLength = strlen(inMessage);
  }

  if (theBuffer.capacity == 0)
    rlBufferCreate(&theBuffer, 65536);

  rlBufferClear(&theBuffer);
  offset = 0;
  offset = rlBufferWrite(&theBuffer, offset, &theInMessageLength, 1, sizeof(int));
  if (theInMessageLength > 0) {
    offset = rlBufferWrite(&theBuffer, offset, inMessage, theInMessageLength, sizeof(char));
  }
  rlSendBufferData(rlGetAgentConnection(), &theBuffer, agentState);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(rlGetAgentConnection(), &theBuffer, &agentState);
  assert(agentState == kAgentMessage);

  offset = 0;
  offset = rlBufferRead(&theBuffer, offset, &theOutMessageLength, 1, sizeof(int));
/*Free and point the old message to null */
    if (theOutMessage != 0) {
      free(theOutMessage);
      theOutMessage = 0;
    }
/* Allocated memory for the new message, maybe just 1 byte for the terminator */
    theOutMessage = (char*)calloc(theOutMessageLength+1, sizeof(char));

/* Fill up the string from the buffer */
if (theOutMessageLength > 0) {
    offset = rlBufferRead(&theBuffer, offset, theOutMessage, theOutMessageLength, sizeof(char));
  }
/* Set the terminator */
    theOutMessage[theOutMessageLength] = '\0';
  return theOutMessage;
}
