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
  unsigned long *substage = new unsigned long[MAX_NUM_SUB_STAGES]();
  unsigned long ii, count;
  char *myrands = new char[len+1];
  char myrandbit;
  my_hist->returnArc4Rand(myrands, len+1);

  count = 0;
  while(count < len){
    myrandbit = 0;
    if((myrands[count] & 0xf) == 0){
      myrandbit = (myrands[count] >> 7) & 0x1;
    }
    if(myrandbit == 0){ 
      gzi->writeBit(0x0);
    }
    else gzi->writeBit(0x1);
    count++;
    ii = 0;
    while(substage[ii] == 2){
      substage[ii] = 0;
      ii++;
      if(ii >= MAX_NUM_SUB_STAGES){
	cout<<"Reached max number of stages."<<endl;
	delete substage;
	return(count);
      }
    }
    if(substage[ii] == 1){
      myrandbit = 0;
      if((myrands[count] & 0xf) == 0){
        myrandbit = (myrands[count] >> 7) & 0x1;
      }
      if(myrandbit == 0){ 
        gzi->writeBit(0x1);
      }
      else gzi->writeBit(0x0);
      count++;
    }
    substage[ii]++;
  }

  delete substage;
  return(count);
}

// Generate the Chacon2 substitution bit sequence. 
// This is zero entropy with weak mixing and long term dependencies. 
unsigned long GanitaZeroSub::generateChacon2(unsigned long len)
{
  unsigned long *substage = new unsigned long[MAX_NUM_SUB_STAGES]();
  unsigned long ii, count;
  char *myrands = new char[len+1];
  char myrandbit;
  my_hist->returnArc4Rand(myrands, len+1);

  count = 0;
  while(count < len){
    myrandbit = 0;
    if((myrands[count] & 0xff) == 0){
      myrandbit = (myrands[count] >> 7) & 0x1;
    }
    if(myrandbit == 0){ 
      gzi->writeBit(0x0);
    }
    else gzi->writeBit(0x1);
    //gzi->writeBit(0x0);
    count++;
    ii = 0;
    while(substage[ii] == 1){
      myrandbit = 0;
      if((myrands[count] & 0xff) == 0){
        myrandbit = (myrands[count] >> 7) & 0x1;
      }
      if(myrandbit == 0){ 
        gzi->writeBit(0x1);
      }
      else gzi->writeBit(0x0);
      //gzi->writeBit(0x1);
      count++;
      substage[ii] = 0;
      ii++;
      if(ii >= MAX_NUM_SUB_STAGES){
	cout<<"Reached max number of stages."<<endl;
	delete substage;
	return(count);
      }
    }
    // if(substage[ii] == 1){
    //   gzi->writeBit(0x1);
    //   count++;
    // }
    substage[ii]++;
  }

  delete substage;
  return(count);
}

