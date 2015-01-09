GanitaZeroBinary::GanitaZeroBinary(void)
{
  name = "binary";
  alphabet_size = 2;
  alphabet_max = 1;
  seq_size = 0;
  alloc_size = 0;
}

unsigned long GanitaZeroBinary::loadBinary(std::ifstream &sym_file)
{
  if(seq_size > 0){
    // possibly allocated sequence
    return(0);
  }
  sym_file.seekg(0, sym_file.end);
  seq_size = 8*sym_file.tellg();
  alloc_size = (seq_size + 7)/8;
  // Allocate seq of size seq_size
  seq = new unsigned char[alloc_size];
  sym_file.read((char *)seq,alloc_size);
  cout<<"Binary sequence length: "<<seq_size<<endl;
  return(seq_size);
}

