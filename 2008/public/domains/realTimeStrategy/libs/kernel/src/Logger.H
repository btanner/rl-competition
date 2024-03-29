
#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <string>
#include <iostream>
#include <sstream>

#include "Global.H"

/* At the moment, this is a simple wrapper around ostringstream. */
class Logger
{
  std::ostringstream outstream;
  
public:
  
  Logger() { }
  virtual ~Logger(); 

  void append(const std::string & str);
  std::string str();  
  std::string str(int chunkSize, int startChar);  
  void dump_to_file(const char * filename);  
};

#endif

