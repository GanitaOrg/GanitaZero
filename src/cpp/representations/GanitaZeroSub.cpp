GanitaZeroSub::GanitaZeroSub(void)
{
  name = "substitution";
  alphabet_size = 2;
  alphabet_max = 1;
  seq_size = 0;
  alloc_size = 0;
}

unsigned long GanitaZeroSub::loadSub(std::ifstream &sym_file)
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

// Generate the Chacon3 substitution bit sequence. 
// This is zero entropy with weak mixing and long term dependencies. 
unsigned long GanitaZeroSub::generateChacon3(unsigned long len)
{
  unsigned long *substage = new unsigned long[100]();
  unsigned long count = 0;
  unsigned char byte1, byte2;
  unsigned long offset;

  while(count < len){
    byte1 = 4;
    byte1 |= (byte1 << 4);
    byte2 = (byte1 << 1) | 0x1;
    offset = 0;
      substage[0]++;
    if(substage[0] == 2){
      // insert 1
    }
  }

  return(count);
}

