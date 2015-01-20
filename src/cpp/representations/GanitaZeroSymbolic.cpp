// Methods for GanitaZeroSymbolic class. 

GanitaZeroSymbolic::GanitaZeroSymbolic(void)
{
  name = "symbolic";
  seq_size = 0;
  alphabet_size = 0;
  alphabet_max = 0;
  // Note alphabet array is initialized later. 
  my_hist = new GanitaZeroHist();
}

GanitaZeroSymbolic::GanitaZeroSymbolic(std::ifstream &sym_file)
{
  name = "symbolic";
  seq_size = 0;
  alphabet_size = 0;
  alphabet_max = 0;
  // Note alphabet array is initialized later. 
  gzi = new GanitaBuffer(sym_file);
  my_hist = new GanitaZeroHist();
}

int GanitaZeroSymbolic::initBuffer(std::ifstream &sym_file)
{
  gzi = new GanitaBuffer(sym_file);
  return 1;
}

int GanitaZeroSymbolic::initBuffer(void)
{
  gzi = new GanitaBuffer();
  return 1;
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
  // deprecated loadCharSeq
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

int GanitaZeroSymbolic::init(void)
{
  unsigned long ii;
  // Using GanitaBuffer.
  // Set alphabet to zero.
  for(ii=0; ii<ALPHABET_ALLOC_SIZE; ii++){
    alphabet[ii] = 0;
  }
  // Determine alphabet size
  for(ii=0; ii<gzi->size(); ii++){
    if(alphabet[gzi->getByte(ii)] <= 0){
      alphabet[gzi->getByte(ii)]++;
    }
    if(gzi->getByte(ii) > alphabet_max){
      alphabet_max = gzi->getByte(ii);
    }
  }
  alphabet_size = 0;
  for(ii=0; ii<ALPHABET_ALLOC_SIZE; ii++){
    if(alphabet[ii] > 0) alphabet_size++;
  }

  //cout<<"Alph size: "<<alphabet_size<<endl;
  
  return(alphabet_size);
}

int GanitaZeroSymbolic::init(char *input_file)
{
  unsigned long ii;
  // Using GanitaBuffer.
  gzi = new GanitaBuffer();
  gzi->open(input_file);
  // Set alphabet to zero.
  for(ii=0; ii<ALPHABET_ALLOC_SIZE; ii++){
    alphabet[ii] = 0;
  }
  // Determine alphabet size
  for(ii=0; ii<gzi->size(); ii++){
    if(alphabet[gzi->getByte(ii)] <= 0){
      alphabet[gzi->getByte(ii)]++;
    }
    if(gzi->getByte(ii) > alphabet_max){
      alphabet_max = gzi->getByte(ii);
    }
  }
  alphabet_size = 0;
  for(ii=0; ii<ALPHABET_ALLOC_SIZE; ii++){
    if(alphabet[ii] > 0){
      alphabet_size++;
      //cout<<std::hex<<ii<<":";
    }
  }

  //cout<<"Scanned "<<std::dec<<gzi->size()<<" bytes.\n";
  //cout<<"Alph size: "<<alphabet_size<<endl;
  
  return(alphabet_size);
}

int GanitaZeroSymbolic::init(char *input_file, char *out_file)
{
  unsigned long ii;
  unsigned long tmp1, tmp2;
  // Using GanitaBuffer.
  gzi = new GanitaBuffer();
  tmp1 = gzi->open(input_file);
  tmp2 = gzi->openOut(out_file);
  if((tmp1 <= 0) || (tmp2 <= 0)){
    cout<<"Unable to open both input and output files."<<endl;
    return(-1);
  }
  // Set alphabet to zero.
  for(ii=0; ii<ALPHABET_ALLOC_SIZE; ii++){
    alphabet[ii] = 0;
  }
  // Determine alphabet size
  for(ii=0; ii<gzi->size(); ii++){
    if(alphabet[gzi->getByte(ii)] <= 0){
      alphabet[gzi->getByte(ii)]++;
    }
    if(gzi->getByte(ii) > alphabet_max){
      alphabet_max = gzi->getByte(ii);
    }
  }
  alphabet_size = 0;
  for(ii=0; ii<ALPHABET_ALLOC_SIZE; ii++){
    if(alphabet[ii] > 0) alphabet_size++;
  }

  //cout<<"Alph size: "<<alphabet_size<<endl;
  
  return(alphabet_size);
}

unsigned long GanitaZeroSymbolic::initOut(char *out_file)
{
  unsigned long tmp;
  // Using GanitaBuffer.
  gzi = new GanitaBuffer();
  tmp = gzi->openOut(out_file);
  if(tmp <= 0){
    cout<<"Unable to open output file: "<<out_file<<"."<<endl;
    return(-1);
  }
  // Set alphabet to zero.
  // for(ii=0; ii<ALPHABET_ALLOC_SIZE; ii++){
  //   alphabet[ii] = 0;
  // }
  alphabet_size = 0;
  
  return(tmp);
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

  cout<<"Alphabet: "<<std::hex<<(unsigned int)alphabet[0];
  for(ii=1; ii<alphabet_size; ii++){
    cout<<","<<std::hex<<(unsigned int)alphabet[ii];
  }
  cout<<endl<<"Alphabet size: "<<std::dec<<alphabet_size<<endl;

  return(ii);
}

int GanitaZeroSymbolic::computeByteHist(void)
{
  my_hist->init(256);
  return(my_hist->computeByteHist(seq, seq_size));
}

unsigned long GanitaZeroSymbolic::dumpHist(void)
{
  return(my_hist->dumpHist());
}

double GanitaZeroSymbolic::computeCondEnt1FromScratch(void)
{
  my_hist->initConditional();
  my_hist->computeCondHist1(seq, seq_size);
  return(my_hist->computeCondEnt1());
}

double GanitaZeroSymbolic::computeCondEnt2FromScratch(int h_len)
{
  my_hist->initConditional(h_len);
  my_hist->computeCondHist1(gzi);
  return(my_hist->computeCondEnt1());
}

unsigned long GanitaZeroSymbolic::dumpCondHist1(void)
{
  return(my_hist->dumpCondHist1());
}

string GanitaZeroSymbolic::returnB64Encode(void)
{
  return(gzi->b64Encode());
}

int GanitaZeroSymbolic::close(void)
{
  return(gzi->close());
}

