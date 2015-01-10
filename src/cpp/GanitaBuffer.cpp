#include "ganita/zero/GanitaBuffer.hpp"

GanitaBuffer::GanitaBuffer(void)
{
  file_loc = 0;
  buffer_num = 0;
  buffer_start = 0;
  byte_loc = 0;
  fixed_buffer_size = GANITA_DEFAULT_BUFFER_SIZE;
  buffer_size = fixed_buffer_size;
  buf_read_flag = 0;
}

GanitaBuffer::GanitaBuffer(std::ifstream &gzt_file)
{
  file_loc = 0;
  buffer_num = 0;
  buffer_start = 0;
  byte_loc = 0;
  fixed_buffer_size = GANITA_DEFAULT_BUFFER_SIZE;
  buffer_size = fixed_buffer_size;
  if (!gzt_file.is_open()){
    std::cout<<"Unable to open input file: "<<std::endl;
    buf_read_flag = 0;
  }
  else {
    byte_value = new unsigned char[buffer_size];
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
  gzt_input_file = new std::ifstream(input_file);
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

