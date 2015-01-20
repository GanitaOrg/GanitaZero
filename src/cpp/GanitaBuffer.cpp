#include "ganita/zero/GanitaBuffer.hpp"

GanitaBuffer::GanitaBuffer(void)
{
  file_loc = 0;
  buffer_num = 0;
  buffer_start = 0;
  byte_loc = 0;
  fixed_buffer_size = GANITA_DEFAULT_BUFFER_SIZE;
  buffer_size = fixed_buffer_size;
  out_buf_size = GANITA_DEFAULT_OUTPUT_BUFFER_SIZE;
  //cout<<"Out buf size: "<<out_buf_size<<endl;
  buf_read_flag = 0;
  gzt_input_file = new std::ifstream();
  outByte = 0;
  outByteOffset = 0;
  out_buf_offset = 0;
}

GanitaBuffer::GanitaBuffer(std::ifstream &gzt_file)
{
  file_loc = 0;
  buffer_num = 0;
  buffer_start = 0;
  byte_loc = 0;
  fixed_buffer_size = GANITA_DEFAULT_BUFFER_SIZE;
  buffer_size = fixed_buffer_size;
  //out_buf_size = GANITA_DEFAULT_OUTPUT_BUFFER_SIZE;
  outByte = 0;
  outByteOffset = 0;
  out_buf_offset = 0;
  if (!gzt_file.is_open()){
    std::cout<<"Unable to open input file: "<<std::endl;
    buf_read_flag = 0;
  }
  else {
    byte_value = new unsigned char[buffer_size];
    gzt_input_file = new std::ifstream();
    gzt_input_file = &gzt_file;
    gzt_input_file->seekg(0, gzt_input_file->end);
    file_size = gzt_input_file->tellg();
    if(file_size < buffer_size){
      buffer_size = file_size;
    }
    gzt_input_file->seekg(0, gzt_input_file->beg);
    gzt_input_file->read((char *) byte_value,buffer_size);
    buf_read_flag = 1;
  }
}

unsigned char GanitaBuffer::getByte(unsigned long loc)
{
  unsigned char byte;

  if(loc >= file_size){
    cout<<"Location beyond file end: "<<loc<<" >= "<<file_size<<endl;
    loc = loc % file_size;
  }

  if(buf_read_flag <= 0){
    std::cout<<"Did not open input file."<<std::endl;
    return(0);
  }

  file_loc = loc;
  if((loc < buffer_start) || (loc >= buffer_start + buffer_size))
    {
      // Need to read in a new buffer
      //cout<<"Read new buffer "<<loc<<" "<<buffer_start<<"\n";
      buffer_num = loc / fixed_buffer_size;
      buffer_start = buffer_num * fixed_buffer_size;
      byte_loc = file_loc - buffer_start;
      gzt_input_file->seekg(0, gzt_input_file->beg);
      gzt_input_file->seekg(buffer_start);
      if(buffer_start + buffer_size >= file_size){
	buffer_size = file_size - buffer_start;
      }
      else buffer_size = fixed_buffer_size;
      gzt_input_file->read((char *) byte_value,buffer_size);
      byte = byte_value[byte_loc];
    }
  else {
    // Get byte from current buffer
    byte_loc = file_loc - buffer_start;
    byte = byte_value[byte_loc];
  }

  return(byte & 0xff);
}

unsigned long GanitaBuffer::size()
{
  return(file_size);
}

unsigned long GanitaBuffer::open(char *input_file)
{
  // Open buffer for reading.
  gzt_input_file->open(input_file);
  if (!gzt_input_file->is_open()){
    std::cout<<"Unable to open input file: "<<input_file<<std::endl;
    return(0);
  }

  byte_value = new unsigned char[buffer_size];
  gzt_input_file->seekg(0, gzt_input_file->end);
  file_size = gzt_input_file->tellg();
  if(file_size < buffer_size){
    buffer_size = file_size;
  }
  gzt_input_file->seekg(0, gzt_input_file->beg);
  gzt_input_file->read((char *) byte_value,buffer_size);
  buf_read_flag = 1;
  
  return(1);
}

unsigned long GanitaBuffer::openOut(char *out_file)
{
  // Open output file for writing.
  gzt_output_file.open(out_file);
  if (!gzt_output_file.is_open()){
    std::cout<<"Unable to open output file: "<<out_file<<std::endl;
    return(0);
  }
  //cout<<"Out buf size: "<<out_buf_size<<endl;
  out_byte_value = new unsigned char[out_buf_size];
  
  return(1);
}

unsigned long GanitaBuffer::writeBit(unsigned char bit)
{
  outByte |= ((bit & 0x1) << outByteOffset);
  outByteOffset++;
  //cout<<"Offset: "<<outByteOffset<<"| ";
  if(outByteOffset >= 8){
    // save byte to output buffer
    out_byte_value[out_buf_offset] = outByte;
    out_buf_offset++;
    outByte = 0;
    outByteOffset = 0;
  }
  if(out_buf_offset >= out_buf_size){
    // save output buffer to file
    gzt_output_file.write((char *)out_byte_value,out_buf_size);
    out_buf_offset = 0;
  }

  return(out_buf_offset);
}

unsigned long GanitaBuffer::writeByte(unsigned char mybyte)
{
  // save byte to output buffer
  out_byte_value[out_buf_offset] = mybyte;
  out_buf_offset++;
  outByte = 0;
  outByteOffset = 0;
  if(out_buf_offset >= out_buf_size){
    // save output buffer to file
    gzt_output_file.write((char *)out_byte_value,out_buf_size);
    out_buf_offset = 0;
  }

  return(out_buf_offset);
}

inline bool GanitaBuffer::is_base64(unsigned char c) 
{
  return (isalnum(c) || (c == '+') || (c == '/'));
}

string GanitaBuffer::b64Encode(unsigned char const* in_bytes, 
			       unsigned long in_len)
{
  std::string ret;
  int i = 0;
  int j = 0;
  unsigned char char_arr_3[3];
  unsigned char char_arr_4[4];

  while (in_len--) {
    char_arr_3[i++] = *(in_bytes++);
    if (i == 3) {
      char_arr_4[0] = (char_arr_3[0] & 0xfc) >> 2;
      char_arr_4[1] = ((char_arr_3[0] & 0x03) << 4) + ((char_arr_3[1] & 0xf0) >> 4);
      char_arr_4[2] = ((char_arr_3[1] & 0x0f) << 2) + ((char_arr_3[2] & 0xc0) >> 6);
      char_arr_4[3] = char_arr_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        ret += b64_chars[char_arr_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for(j = i; j < 3; j++)
      char_arr_3[j] = '\0';

    char_arr_4[0] = (char_arr_3[0] & 0xfc) >> 2;
    char_arr_4[1] = ((char_arr_3[0] & 0x03) << 4) + ((char_arr_3[1] & 0xf0) >> 4);
    char_arr_4[2] = ((char_arr_3[1] & 0x0f) << 2) + ((char_arr_3[2] & 0xc0) >> 6);
    char_arr_4[3] = char_arr_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      ret += b64_chars[char_arr_4[j]];

    while((i++ < 3))
      ret += '=';

  }

  return ret;
}

string GanitaBuffer::b64Encode(void)
{
  std::string ret;
  int i = 0;
  int j = 0;
  unsigned char char_arr_3[3];
  unsigned char char_arr_4[4];
  unsigned long byte_loc;
  unsigned long in_len;

  byte_loc = 0;
  in_len = file_size;
  while (in_len--) {
    char_arr_3[i++] = getByte(byte_loc++);
    if (i == 3) {
      char_arr_4[0] = (char_arr_3[0] & 0xfc) >> 2;
      char_arr_4[1] = ((char_arr_3[0] & 0x03) << 4) + ((char_arr_3[1] & 0xf0) >> 4);
      char_arr_4[2] = ((char_arr_3[1] & 0x0f) << 2) + ((char_arr_3[2] & 0xc0) >> 6);
      char_arr_4[3] = char_arr_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        ret += b64_chars[char_arr_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for(j = i; j < 3; j++)
      char_arr_3[j] = '\0';

    char_arr_4[0] = (char_arr_3[0] & 0xfc) >> 2;
    char_arr_4[1] = ((char_arr_3[0] & 0x03) << 4) + ((char_arr_3[1] & 0xf0) >> 4);
    char_arr_4[2] = ((char_arr_3[1] & 0x0f) << 2) + ((char_arr_3[2] & 0xc0) >> 6);
    char_arr_4[3] = char_arr_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      ret += b64_chars[char_arr_4[j]];

    while((i++ < 3))
      ret += '=';

  }

  return ret;
}

string GanitaBuffer::b64Decode(string &enc_str)
{
  int in_len = enc_str.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_arr_4[4], char_arr_3[3];
  std::string ret;

  while (in_len-- && ( enc_str[in_] != '=') && is_base64(enc_str[in_])) {
    char_arr_4[i++] = enc_str[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_arr_4[i] = b64_chars.find(char_arr_4[i]);

      char_arr_3[0] = (char_arr_4[0] << 2) + ((char_arr_4[1] & 0x30) >> 4);
      char_arr_3[1] = ((char_arr_4[1] & 0xf) << 4) + ((char_arr_4[2] & 0x3c) >> 2);
      char_arr_3[2] = ((char_arr_4[2] & 0x3) << 6) + char_arr_4[3];

      for (i = 0; (i < 3); i++)
        ret += char_arr_3[i];
      i = 0;
    }
  }

  if (i) {
    for (j = i; j <4; j++)
      char_arr_4[j] = 0;

    for (j = 0; j <4; j++)
      char_arr_4[j] = b64_chars.find(char_arr_4[j]);

    char_arr_3[0] = (char_arr_4[0] << 2) + ((char_arr_4[1] & 0x30) >> 4);
    char_arr_3[1] = ((char_arr_4[1] & 0xf) << 4) + ((char_arr_4[2] & 0x3c) >> 2);
    char_arr_3[2] = ((char_arr_4[2] & 0x3) << 6) + char_arr_4[3];

    for (j = 0; (j < i - 1); j++) ret += char_arr_3[j];
  }

  return ret;
}

int GanitaBuffer::close(void)
{
  int count;
  count = 0;
  if (gzt_output_file.is_open()){
    // write out remaining bytes in output buffer
    // cout<<"Closing output file."<<endl;
    gzt_output_file.write((char *)out_byte_value,out_buf_offset);
    gzt_output_file.close();
    count++;
  }
  if (gzt_input_file->is_open()){
    // cout<<"Closing input file."<<endl;
    gzt_input_file->close();
    count++;
  }
  
  return(count);
}

