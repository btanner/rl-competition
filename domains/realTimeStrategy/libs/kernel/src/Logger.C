
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include "Logger.H"


using namespace std; 

Logger::~Logger() 
{
  // used for testing
  //dump_to_file("/tmp/rlgenv_episode.log");
}

void Logger::append(const string & str)
{
  outstream << str; 
}

string Logger::str() 
{
  return outstream.str(); 
}

std::string Logger::str(int chunkSize, int startChar)
{
  if (startChar < 0) 
    REM("Logger::str startChar < 0");
  
  if (chunkSize < 0) 
    REM("Logger::str chunkSize < 0");
  
  if (((unsigned int)startChar) >= outstream.str().length())
    return "";
  else
    return outstream.str().substr(startChar, chunkSize); 
}


void Logger::dump_to_file(const char * filename)
{
  ofstream fout(filename, ios::out);
  fout << str() << endl; 
  fout.close(); 
}

