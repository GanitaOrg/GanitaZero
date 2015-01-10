#ifndef _GANITABUFFER_
#define _GANITABUFFER_

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>

//default buffer size (100M)
#ifndef GANITA_DEFAULT_BUFFER_SIZE
#define GANITA_DEFAULT_BUFFER_SIZE 100000000
#endif

using namespace std;

class GanitaBuffer
{
private:
  unsigned long file_loc;
  unsigned long buffer_num;
  unsigned long byte_loc;
  unsigned char *byte_value;
  unsigned long buffer_size;
  unsigned long fixed_buffer_size;
  unsigned long file_size;
  unsigned long buffer_start;
  int buf_read_flag;
  std::ifstream *gzt_input_file = new std::ifstream();
public:
  GanitaBuffer();
  GanitaBuffer(std::ifstream &gzt_file);
  unsigned char getByte(unsigned long loc);
  unsigned long size();
  unsigned long open(char *input_file);
};

#endif
