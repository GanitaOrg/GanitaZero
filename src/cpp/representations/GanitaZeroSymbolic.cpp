// Methods for GanitaZeroSymbolic class. 

GanitaZeroSymbolic::GanitaZeroSymbolic(void)
{
  name = "symbolic";
  seq_size = 0;
  alphabet_size = 0;
  alphabet_max = 0;
  // Note alphabet array is initialized later. 
}

unsigned long GanitaZeroSymbolic::loadCharSeq(std::ifstream &sym_file)
{
  if(seq_size > 0){
    // possibly allocated sequence
    return(0);
  }
  sym_file.seekg(0, sym_file.end);
  seq_size = sym_file.tellg();
  // Allocate seq of size seq_size
  seq = new unsigned char[seq_size];
  // seq = (unsigned char *)malloc((size_t)seq_size*sizeof(char));
  // if(seq == NULL){
  //   cout<<"Unable to allocate sequence."<<endl;
  //   return(0);
  // }
  sym_file.seekg(0, sym_file.beg);
  sym_file.read((char *) seq,seq_size);
  cout<<"Sequence length: "<<seq_size<<endl;
  return(seq_size);
}

int GanitaZeroSymbolic::init(std::ifstream &sym_file)
{
  unsigned long ii;
  // load data into array seq
  if(loadCharSeq(sym_file) == 0) return(-1);
  // Set alphabet to zero
  for(ii=0; ii<ALPHABET_ALLOC_SIZE; ii++){
    alphabet[ii] = 0;
  }
  // Determine alphabet size
  for(ii=0; ii<seq_size; ii++){
    if(alphabet[seq[ii]] <= 0){
      alphabet[seq[ii]]++;
    }
    if(seq[ii] > alphabet_max){
      alphabet_max = seq[ii];
    }
  }
  alphabet_size = 0;
  for(ii=0; ii<ALPHABET_ALLOC_SIZE; ii++){
    if(alphabet[ii] > 0) alphabet_size++;
  }

  //cout<<"Alph size: "<<alphabet_size<<endl;
  
  return(alphabet_size);
}

int GanitaZeroSymbolic::shrinkAlphabet(void)
{
  int ii, jj;
  if(alphabet_size == 0){
    // Need to load seq first.
    return(-1);
  }
  if(alphabet_size > alphabet_max + 1){
    // This shouldn't happen. 
    return(-2);
  }
  if(alphabet_size == alphabet_max + 1){
    // Nothing to shrink
    return(0);
  }
  jj = 0;
  for(ii=0; ii<ALPHABET_ALLOC_SIZE; ii++){
    if(alphabet[ii] > 0){
      alphabet[jj] = ii;
      jj++;
    }
  }
  return(jj);
}

int GanitaZeroSymbolic::dumpAlphabet(void)
{
  int tmp;
  unsigned long ii;
  if((tmp = shrinkAlphabet()) <= 0){
    return(tmp);
  }

  cout<<"Alphabet: "<<std::dec<<(unsigned int)alphabet[0];
  for(ii=1; ii<alphabet_size; ii++){
    cout<<","<<std::dec<<(unsigned int)alphabet[ii];
  }
  cout<<endl<<"Alphabet size: "<<alphabet_size<<endl;

  return(ii);
}

int GanitaZeroSymbolic::computeByteHist(void)
{
  my_hist.init(256);
  return(my_hist.computeByteHist(seq, seq_size));
}

unsigned long GanitaZeroSymbolic::dumpHist(void)
{
  return(my_hist.dumpHist());
}

double GanitaZeroSymbolic::computeCondEnt1FromScratch(void)
{
  my_hist.initConditional();
  my_hist.computeCondHist1(seq, seq_size);
  return(my_hist.computeCondEnt1());
}

unsigned long GanitaZeroSymbolic::dumpCondHist1(void)
{
  return(my_hist.dumpCondHist1());
}

