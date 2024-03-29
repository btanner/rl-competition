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

/* Standard Headers */
#include <assert.h> /* assert */
#include <stdlib.h> /* calloc */
#include <string.h> /* memset */
#include <stdio.h> /* fprintf */

/* Network Headers */
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

/* RL_netlib Library Header */
#include <Network/RL_network.h>

/* Open and configure a socket */
int rlOpen(short thePort) {
  int flag = 1;
  int theSocket = 0;

  theSocket = socket(PF_INET, SOCK_STREAM, 0);
  setsockopt(theSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int)); /* Disable Nagleing */

  return theSocket;
}

/* Calls accept on a socket */
int rlAcceptConnection(int theSocket) {
  int theClient = 0;
  struct sockaddr_in theClientAddress = {0};
  socklen_t theSocketSize = sizeof(struct sockaddr_in);
  theClient = accept(theSocket, (struct sockaddr*)&theClientAddress, &theSocketSize);
  return theClient;
}

/* Connect (TCP/IP) to the given address at the given port */
int rlConnect(int theSocket, const char* theAddress, short thePort) {
  int theStatus = 0;
  struct sockaddr_in theDestination;
  theDestination.sin_family = AF_INET;
  theDestination.sin_port = htons(thePort);
  theDestination.sin_addr.s_addr = inet_addr(theAddress);
  memset(&theDestination.sin_zero, '\0', 8);

  theStatus = connect(theSocket, 
		      (struct sockaddr*)&theDestination, 
		      sizeof(struct sockaddr));

  return theStatus;
}

/* Listen for an incoming connection on a given port.
   This function blocks until it receives a new connection */
int rlListen(int theSocket, short thePort) {
  struct sockaddr_in theServer;
  int theStatus = 0;
  int yes = 1;
  
  theServer.sin_family = AF_INET;
  theServer.sin_port = htons(thePort);
  theServer.sin_addr.s_addr = INADDR_ANY;
  memset(&theServer.sin_zero, '\0', 8);
  
  /* We don't really care if this fails, it just lets us reuse the port quickly */
  setsockopt(theSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

  theStatus = bind(theSocket, 
		   (struct sockaddr*)&theServer, 
		   sizeof(struct sockaddr));

  if (theStatus == -1)
	{
		fprintf(stderr,"Could not open socket\n");
		exit(1);
	}
  
  theStatus = listen(theSocket, 10);
  if (theStatus == -1) return -1;

  return theStatus;
}

int rlClose(int theSocket) {
  return close(theSocket);
}

int rlIsValidSocket(int theSocket) {
  return theSocket != -1;
}

/* send doesn't guarantee that all the data will be sent.
   rlSendData calls send continually until it is all sent, or until an error occurs.
   the amount of data sent is returned */

int rlSendData(int theSocket, const void* theData, int theLength) {
  int theBytesSent = 0;
  int theMsgError = 0;
  const char* theDataBuffer = (const char*)theData;
  
  while (theBytesSent < theLength) {
    theMsgError = send(theSocket, theDataBuffer + theBytesSent, theLength - theBytesSent, 0);
    if (theMsgError == -1) break;
    else theBytesSent += theMsgError;
  }

  return theBytesSent;
}

/* Calls recv repeatedly until "theLength" data has been received, or an error occurs */
int rlRecvData(int theSocket, void* theData, int theLength) {
  int theBytesRecv = 0;
  int theMsgError = 0;
  char* theDataBuffer = (char*)theData;
  
  while (theBytesRecv < theLength) {
    theMsgError = recv(theSocket, theDataBuffer + theBytesRecv, theLength - theBytesRecv, 0);
    if (theMsgError <= 0) break;
    else theBytesRecv += theMsgError;
  }

  return theBytesRecv;
}

/* rlBuffer API */

/* We use buffers for sending and receiving network data.
   Buffers are written to in network byte order (rlBufferWrite) and read in 
   system byte order (rlBufferRead). */

void rlBufferCreate(rlBuffer *buffer, unsigned int capacity) {
  buffer->size     = 0;
  buffer->capacity = 0;
  buffer->data     = 0;

  if (capacity > 0) {
    rlBufferReserve(buffer, capacity);
  }
}

/* free up the buffer */
void rlBufferDestroy(rlBuffer *buffer) {
  if (buffer->data != 0) {
    free(buffer->data);
  }
  buffer->data = 0;
  buffer->size = 0;
  buffer->capacity = 0;
}

/* clear the buffer */
void rlBufferClear(rlBuffer *buffer) {
  buffer->size = 0;
}

/* Ensure that the buffer contains at least "capacity" memory */
void rlBufferReserve(rlBuffer *buffer, unsigned int capacity) {
  unsigned char* new_data = 0;
  unsigned int new_capacity = 0;

  /* Ensure the buffer can hold the new data */
  if (capacity > buffer->capacity) {
    /* Allocate enough memory for the additional data */
    new_capacity = capacity + (capacity - buffer->capacity) * 2;
    assert(new_capacity > 0);
    new_data = (unsigned char*)calloc(new_capacity, sizeof(unsigned char));
    assert(new_data != 0);

    /* Copy the existing data into the the larger memory allocation */
    if (buffer->size > 0) {
      memcpy(new_data, buffer->data, buffer->size);
    }

    /* Free the original data */
    free(buffer->data);
 
    /* Set the buffers data to the new data pointer */
    buffer->data = new_data;

    /* Set the new capacity */
    buffer->capacity = new_capacity;
  }
}

/* Write to the buffer in network byte order - will resize the buffer to facilitate a write that is too large for the 
   current capacity

   buffer: buffer to write to
   offset: the offset to start writing into the buffer at
   count: the number of items of size "size" to write into the buffer
   size: the size of each individual item
   return: the next sequential byte offset to write to, used as "offset" to the next call of rlBufferWrite
*/
unsigned int rlBufferWrite(rlBuffer *buffer, unsigned int offset, const void* sendData, unsigned int count, unsigned int size) {
  const unsigned char* data = (const unsigned char*)sendData;
  unsigned char* data_ptr = 0;
  unsigned int i = 0;

  if (buffer->capacity < offset + count * size ) {
    rlBufferReserve(buffer, offset + count * size);
  }

  /* Get the offset to the place in the buffer we want to start inserting */
  data_ptr = buffer->data + offset;

  /* For each of the new data items, swap the endianness and add them to the buffer */
  for (i = 0; i < count; ++i) {
    if (rlGetSystemByteOrder() == 1) {
      rlSwapData(&data_ptr[i * size], &data[i * size], size);
    }
    else {
      memcpy(&data_ptr[i * size], &data[i * size], size);
    }
  }

  buffer->size += count * size;
  return offset + count * size;
}

/* Read data in network byte order
   buffer: buffer to read from
   offset: the offset to start reading from the buffer at
   count: the number of items of size "size" to read from the buffer
   size: the size of each individual item
   return: the next sequential byte offset to read from, used as offset in the next call to rlBufferRead
*/
unsigned int rlBufferRead(const rlBuffer *buffer, unsigned int offset, void* recvData, unsigned int count, unsigned int size) {
  unsigned char* data = (unsigned char*)recvData;
  unsigned int i = 0;

  /* For each of the new data items, swap the endianness and read them from the buffer */
  for (i = 0; i < count; ++i) {
    if (rlGetSystemByteOrder() == 1) {
      rlSwapData(&data[i * size], &buffer->data[(i * size) + offset], size);
    }
    else {
      memcpy(&data[i * size], &buffer->data[(i * size) + offset], size);
    }
  }

  return offset + (count * size);
}

/* Send the buffer across the network. Sends target and the size of the buffer across the network
   before sending the data in the buffer itself.  This could be refactored so that it just sends the
   buffer and the caller is responsible for writing any header data into the buffer before hand.
   This was more convenient at the time */

unsigned int rlSendBufferData(int theSocket, const rlBuffer* buffer, const int target) {
  int sendTarget = target;
  unsigned int sendSize = buffer->size;
  unsigned int header[2] = {0};
  
  /* sendSize needs to go across in network byte order, swap it if we're little endian */
  if (rlGetSystemByteOrder() == 1) {
    rlSwapData(&sendTarget, &target, sizeof(int));
    rlSwapData(&sendSize, &buffer->size, sizeof(unsigned int));
  }
  
  header[0] = (unsigned int)sendTarget;
  header[1] = (unsigned int)sendSize;

  /*  rlSendData(theSocket, &sendTarget, sizeof(int));
      rlSendData(theSocket, &sendSize, sizeof(unsigned int));*/

  rlSendData(theSocket, header, sizeof(unsigned int) * 2);

  if (buffer->size > 0) {
    rlSendData(theSocket, buffer->data, buffer->size);
  }

  return (sizeof(unsigned int) * 2) + buffer->size; /* Returns payload size, not actual data sent ! */
}


/* Corresponds to calls made by rlSendBufferData.  Reads a "target" and a size from the network,
   then reads "size" data into rlBuffer.  See rlSendBufferData for more */

unsigned int rlRecvBufferData(int theSocket, rlBuffer* buffer, int *target) {
  unsigned int recvTarget = 0;
  unsigned int recvSize = 0;
  unsigned int header[2] = {0};
  unsigned int totalSize = 0;

  if (rlRecvData(theSocket, header, sizeof(unsigned int) * 2) > 0)
  {
    if (rlGetSystemByteOrder() == 1)  /* Little Endian */
    {
      rlSwapData(&recvTarget, &header[0], sizeof(unsigned int));
      rlSwapData(&recvSize, &header[1], sizeof(unsigned int));
    }
    else
    {
      recvTarget = header[0];
      recvSize = header[1];
    }

    *target = (int)recvTarget;
    rlBufferReserve(buffer, recvSize);

    if (recvSize > 0) {
      rlRecvData(theSocket, buffer->data, recvSize);
    }

    totalSize = (sizeof(unsigned int) * 2) + buffer->size;
  }

  return totalSize;
}

/* Utilities */
int rlGetSystemByteOrder() {
  /*
    Endian will be 1 when we are on a little endian machine,
    and not 1 on a big endian machine.
  */

  const int one = 1;
  const char endian = *(char*)&one;

  return endian;
}

/* Notice that the pointers "in" and "out" are not allowed to be the same.
   When dealing with IEEE floating point numbers, you still need to swap endianness.
   For sanity, we disallow swaping back into the same memory space to discourage 
   swapping a double/float back into its own memory.  Once these have been swapped, they should
   not be treated as doubles/floats again until they are back into their native endianness. */

void rlSwapData(void* out, const void* in, const unsigned int size) {
  const unsigned char *src = (const unsigned char *)in;
  unsigned char *dst = (unsigned char *)out;
  unsigned int i = 0;

  assert(out != in);

  for (i = 0; i < size; ++i) {
    dst[i] = src[size-i-1];
  }
}


int rlWaitForConnection(const char *address, const short port, const int retryTimeout) {
  int theConnection = 0;
  int isConnected = -1;

  while(isConnected == -1) {
    theConnection = rlOpen(port);
    assert(rlIsValidSocket(theConnection));
    isConnected = rlConnect(theConnection, address, port);
    if (isConnected == -1) { 
      rlClose(theConnection);
      sleep(retryTimeout);
    }
  }

  return theConnection;
}

unsigned int rlCopyADTToBuffer(const RL_abstract_type* src, rlBuffer* dst, unsigned int offset) {
  const int headerSize = sizeof(unsigned int) * 2;
  const int dataSize   = src->numInts * sizeof(int) + src->numDoubles * sizeof(double);

  rlBufferReserve(dst, dst->size + headerSize + dataSize);

  /* fprintf(stderr, "send 1 offset = %u\n", offset); */
  offset = rlBufferWrite(dst, offset, &src->numInts, 1, sizeof(unsigned int));
  /* fprintf(stderr, "send 2 offset = %u\n", offset); */
  offset = rlBufferWrite(dst, offset, &src->numDoubles, 1, sizeof(unsigned int));
  /* fprintf(stderr, "send 3 offset = %u\n", offset); */

  if (src->numInts > 0) {
    offset = rlBufferWrite(dst, offset, src->intArray, src->numInts, sizeof(int));
  }

  if (src->numDoubles > 0) {
    offset = rlBufferWrite(dst, offset, src->doubleArray, src->numDoubles, sizeof(double));  
  }

  return offset;
}

unsigned int rlCopyBufferToADT(const rlBuffer* src, unsigned int offset, RL_abstract_type* dst) {
  unsigned int numInts    = 0;
  unsigned int numDoubles = 0;

  int* intArray = 0;
  double * doubleArray = 0;

  /* fprintf(stderr, "recv 1 offset = %u\n", offset); */
  offset = rlBufferRead(src, offset, &numInts, 1, sizeof(unsigned int));
  /* fprintf(stderr, "recv 2 offset = %u\n", offset); */
  offset = rlBufferRead(src, offset, &numDoubles, 1, sizeof(unsigned int));
  /* fprintf(stderr, "recv 3 offset = %u\n", offset); */

  if (numInts > dst->numInts) {
		/* printf("Needed to allocate more than %d\n",numInts); */
    intArray = (int*)calloc(numInts, sizeof(int));
    memcpy(intArray, dst->intArray, dst->numInts * sizeof(int));
    free(dst->intArray);
    dst->intArray = intArray;
  }
  dst->numInts = numInts;


  if (numDoubles > dst->numDoubles) {
    doubleArray = (double*)calloc(numDoubles, sizeof(double));
    memcpy(doubleArray, dst->doubleArray, dst->numDoubles * sizeof(double));
    free(dst->doubleArray);
    dst->doubleArray = doubleArray;
  }
  dst->numDoubles = numDoubles;

  if (dst->numInts > 0) {
    offset = rlBufferRead(src, offset, dst->intArray, dst->numInts, sizeof(int));
  }

  if (dst->numDoubles > 0) {
    offset = rlBufferRead(src, offset, dst->doubleArray, dst->numDoubles, sizeof(double));
  }

  return offset;
}
