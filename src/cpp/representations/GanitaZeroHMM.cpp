GanitaZeroHMM::GanitaZeroHMM(void)
{
  name = "hmm";
  alphabet_size = 2;
  alphabet_max = 1;
  seq_size = 0;
  alloc_size = 0;
}

unsigned long GanitaZeroHMM::loadHMM(std::ifstream &sym_file)
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

// Generate sample HMM bit sequence. 
unsigned long GanitaZeroHMM::generateHMM_1(unsigned long len)
{
  unsigned long ii;
  double myran;
  gztm.init(3,3);
  gzom.init(3,2);
  gzom.set(0,0,1);
  gzom.set(1,1,1);
  gzom.set(2,0,1);
  GanitaZeroMat state(1,3);
  GanitaZeroMat obs(1,2);
  state.setUnit(0);
  gztm.setEx1();
  cout<<"Transition matrix:"<<endl;
  gztm.dumpMat();
  cout<<"Observation matrix:"<<endl;
  gzom.dumpMat();
  setVerbosity(0);
  for(ii=0; ii<len; ii++){
    gzom.leftMultiply(state,obs);
    myran = returnArc4RandUniform();
    if(verbose){
      state.dumpMat();
      cout<<"state -----------"<<endl;
      obs.dumpMat();
      cout<<"obs -----------"<<endl;
    }
    if(myran < obs.get(0,0)){
      obs.setUnit(0);
      gzi->writeBit(0x0);
    }
    else{
      obs.setUnit(1);
      gzi->writeBit(0x1);
    }
    if(verbose){
      obs.dumpMat();
      cout<<"obs: "<<ii<<" -----------"<<endl;
    }
    gztm.leftMultiply(state);
    myran = returnArc4RandUniform();
    if(verbose){
      fprintf(stdout, "Random number: %lf\n", myran);
      state.dumpMat();
      cout<<"-----------"<<endl;
    }
    if(myran < state.get(0,0)){
      state.setUnit(0);
    }
    else if(myran < state.get(0,0) + state.get(0,1)){
      state.setUnit(1);
    }
    else state.setUnit(2);
  }
  
  return(0);
}

uint64_t GanitaZeroHMM::returnArc4Rand(char *myran, uint64_t len)
{
  // obtain a seed from the timer
  typedef std::chrono::high_resolution_clock myclock;
  myclock::time_point beginning = myclock::now();
  myclock::duration d = myclock::now() - beginning;
  unsigned seed1 = d.count();
  std::independent_bits_engine<std::mt19937,8,std::uint8_t> generator (seed1);
  //std::cout << "Your seed produced: " << generator() << std::endl;
  for(uint64_t ii=0; ii<len; ii++){
    myran[ii] = generator();
  }
  //arc4random_buf((char *)myran, (size_t) len);

  return(len);
}

// Return random number between 0 and 1.
double GanitaZeroHMM::returnArc4RandUniform(void)
{
  uint32_t myint, up_bound;
  double myran;
  up_bound = 0x1 << 31;
  std::random_device r;
  std::default_random_engine e1(r());
  std::uniform_int_distribution<int> uniform_dist(0, up_bound);
  myint = uniform_dist(e1);
  //myint = arc4random_uniform(up_bound);
  myran = ((double) myint) / ((double) up_bound);
  return(myran);
}

uint64_t GanitaZeroHMM::dumpRand(uint64_t len)
{
  uint64_t ii;
  char *mychar = new char[len];
  returnArc4Rand(mychar, len);
  //fprintf(stdout, "Random %lld bits:\n", (long long int) 8*len);
  for(ii=0; ii<len; ii++){
    //fprintf(stdout, "%02X", mychar[ii] & 0xff);
    fprintf(stdout, "%c", mychar[ii] & 0xff);
  }
  fprintf(stdout, "\n");
  return(len);
}

int GanitaZeroHMM::close(void)
{
  gztm.close();
  gzom.close();
  return(GanitaZeroSymbolic::close());
}

