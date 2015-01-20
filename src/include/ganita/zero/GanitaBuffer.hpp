#ifndef _GANITABUFFER_
#define _GANITABUFFER_

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>

//default buffer size (1M)
#ifndef GANITA_DEFAULT_BUFFER_SIZE
#define GANITA_DEFAULT_BUFFER_SIZE 1000000
#endif

//default output buffer size
#ifndef GANITA_DEFAULT_OUTPUT_BUFFER_SIZE
#define GANITA_DEFAULT_OUTPUT_BUFFER_SIZE 4096
#endif

using namespace std;

static const string b64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

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
  std::ifstream *gzt_input_file;
  std::ofstream gzt_output_file;
  unsigned char outByte;
  int outByteOffset;
  unsigned char *out_byte_value;
  unsigned long out_buf_size;
  unsigned long out_buf_offset;
public:
  GanitaBuffer();
  GanitaBuffer(std::ifstream &gzt_file);
  unsigned char getByte(unsigned long loc);
  unsigned long size();
  unsigned long open(char *input_file);
  unsigned long openOut(char *out_file);
  unsigned long writeBit(unsigned char bit);
  unsigned long writeByte(unsigned char mybyte);
  inline bool is_base64(unsigned char c);
  string b64Encode(unsigned char const* in_bytes,unsigned long in_len);
  string b64Encode(void);
  string b64Decode(string &enc_str);
  int close(void);
};

#endif
