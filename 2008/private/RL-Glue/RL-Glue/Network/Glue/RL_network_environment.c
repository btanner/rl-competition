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

#include <Network/RL_network.h>

static int envConnection = 0;

int rlDidEnvironmentConnect()
{
  return envConnection != 0;
}

void rlCloseEnvironmentConnection()
{
  rlBuffer theBuffer = {0};
  rlBufferCreate(&theBuffer, 8);
  rlSendBufferData(envConnection, &theBuffer, kRLTerm);

  rlClose(envConnection);
  envConnection = 0;

  rlBufferDestroy(&theBuffer);
}

void rlSetEnvironmentConnection(int connection)
{
  if (rlDidEnvironmentConnect())
    rlCloseEnvironmentConnection();

  envConnection = connection;
}

int rlGetEnvironmentConnection()
{
  return envConnection;
}
