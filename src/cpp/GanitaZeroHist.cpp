#include "ganita/zero/GanitaZeroHist.hpp"

GanitaZeroHist::GanitaZeroHist(void)
{
  hist_length = 0;
  condition_num = 0;
  est_num = 0;
  max_count = 0;
  max_index = 0;
  divisor = 1;
}

GanitaZeroHist::GanitaZeroHist(unsigned long ss)
{
  if(ss <= 0){
    hist_length = DEFAULT_HIST_SIZE;
  }
  else hist_length = ss;
  hist = new unsigned long[ss]();
  condition_num = 0;
  est_num = 0;
  max_count = 0;
  max_index = 0;
  divisor = 1;
}

unsigned long GanitaZeroHist::init(unsigned long ss)
{
  if(ss <= 0){
    hist_length = DEFAULT_HIST_SIZE;
  }
  else hist_length = ss;
  hist = new unsigned long[ss]();
  if(!hist){
    cout<<"Unable to allocate memory for histogram."<<endl;
  }
  condition_num = 0;
  est_num = 0;

  return hist_length;
}

unsigned long GanitaZeroHist::initConditional(void)
{
  if(hist_length <= 0){
    cout<<"previously unallocated\n";
    hist_length = 256;
    hist = new unsigned long[2*hist_length]();
  }
  else {
    cout<<"previously allocated\n";
    delete hist;
    hist_length = 256;
    hist = new unsigned long[2*hist_length]();
  }
  condition_num = 8;
  est_num = 1;

  return hist_length;
}

unsigned long GanitaZeroHist::initConditional(int h_len)
{
  if(h_len > GANITA_MAX_COND_BITS){
    h_len =  GANITA_MAX_COND_BITS;
  }
  if(hist_length <= 0){
    //cout<<"previously unallocated\n";
    hist_length = 0x1 << h_len;
    hist = new(nothrow) unsigned long[2*hist_length]();
  }
  else {
    //cout<<"previously allocated\n";
    delete hist;
    hist_length = 0x1 << h_len;
    hist = new(nothrow) unsigned long[2*hist_length]();
  }
  if(!hist){
    cout<<"Unable to allocate memory for histogram."<<endl;
    hist_length = 1;
    h_len = 0;
    hist = new unsigned long[2*hist_length]();
  }
  condition_num = h_len;
  est_num = 1;

  return hist_length;
}

int GanitaZeroHist::computeByteHist(unsigned char *ptr, unsigned long ss)
{
  unsigned long ii;
  cout<<"Histogram length: "<<ss<<endl;
  for(ii=0; ii<ss; ii++){
    hist[ptr[ii]]++;
  }

  return 1;
}

// Compute an approximation of the conditional entropy
// int GanitaZeroHist::computeCondHist1(unsigned char *ptr, unsigned long ss)
// {
//   unsigned int byte;
//   unsigned int bit;
//   unsigned long ii, b1, bottom;
//   condition_num = 8;
//   est_num = 1;

//   for(ii=0; ii<8*(ss-1); ii++){
//     bottom = ii % 8;
//     b1 = ii / 8;
//     byte = (unsigned int) (((ptr[b1] & 0xff) >> bottom) & 0xff);
//     byte |= (unsigned int) (((ptr[b1+1] & 0xff) << (8 - bottom)) & 0xff);
//     bit = (unsigned int) (((ptr[b1+1] & 0xff) >> bottom) & 0x1);
//     //fprintf(stdout, "Values: %02X, %02X, %01X\n", ptr[b1+1],byte,bit);
//     hist[2*byte + bit]++;
//   }

//   return 1;
// }

//int GanitaZeroHist::computeCondHist1(GanitaBuffer *input)
int GanitaZeroHist::computeCondHist1(unsigned char *ptr, unsigned long ss)
{
  unsigned long cond_bits;
  unsigned long pbit, cbit;
  unsigned long ii, jj;
  unsigned long b1, bottom;

  cout<<"Number of bytes: "<<ss<<endl;

  for(ii=condition_num; ii<8*(ss-1); ii++){
    // Get the probability bit
    b1 = ii / 8;
    bottom = ii % 8;
    pbit = (unsigned long) (((ptr[b1] & 0xff) >> bottom) & 0x1);
    cond_bits = 0;
    for(jj=ii-condition_num; jj<ii; jj++){
      bottom = jj % 8;
      b1 = jj / 8;
      cbit = (unsigned long) (((ptr[b1] & 0xff) >> bottom) & 0x1);
      cond_bits |= (cbit << (jj + condition_num - ii));
    }
    //fprintf(stdout, "Values: %04X, %01X\n", cond_bits,pbit);
    hist[2*cond_bits + pbit]++;
  }

  return 1;
}

int GanitaZeroHist::computeCondHist1(GanitaBuffer *input)
{
  unsigned long cond_bits;
  unsigned long pbit, cbit;
  unsigned long ii, jj;
  unsigned long b1, bottom;

  cout<<"Number of bytes: "<<input->size()<<endl;

  for(ii=condition_num; ii<8*(input->size()-1); ii++){
    // Get the probability bit
    b1 = ii / 8;
    bottom = ii % 8;
    pbit = (unsigned long) (((input->getByte(b1) & 0xff) >> bottom) & 0x1);
    cond_bits = 0;
    for(jj=ii-condition_num; jj<ii; jj++){
      bottom = jj % 8;
      b1 = jj / 8;
      cbit = (unsigned long) (((input->getByte(b1) & 0xff) >> bottom) & 0x1);
      cond_bits |= (cbit << (jj + condition_num - ii));
    }
    //fprintf(stdout, "Values: %04X, %01X\n", cond_bits,pbit);
    hist[2*cond_bits + pbit]++;
  }

  return 1;
}

// Compute an approximation of the conditional entropy
double GanitaZeroHist::computeCondEnt1(void)
{
  unsigned long ii, total;
  double pp, entropy;

  entropy = 0;
  total = 0;
  for(ii=0; ii<2*hist_length; ii++){
    total += hist[ii];
  }

  //fprintf(stdout, "Bit histogram: %ld %ld\n", hist[0],hist[1]);
  for(ii=0; ii<hist_length; ii++){
    if((hist[2*ii] != 0) || (hist[2*ii+1] != 0)){
      pp = ((double) hist[2*ii+1]) / ((double) (hist[2*ii] + hist[2*ii+1]));
      entropy += 
	4*pp*(1-pp)*((double) (hist[2*ii] + hist[2*ii+1])) / ((double) total);
    }
  }
  
  return(entropy);
}

unsigned long GanitaZeroHist::dumpHist(void)
{
  unsigned long ii;
  //unsigned long total;
  //total = 0;
  for(ii=0; ii<hist_length; ii++){
    fprintf(stdout, "Index / Count %3ld / %ld\n", ii, hist[ii]);
    //total += hist[ii];
  }

  //cout<<"Total "<<total<<endl;
  return ii;
}

unsigned long GanitaZeroHist::dumpCondHist1(void)
{
  unsigned long ii;
  unsigned long total = 0;

  for(ii=0; ii<hist_length; ii++){
    fprintf(stdout, "Index / Count %3ld / %ld, %ld\n", ii, hist[2*ii], hist[2*ii+1]);
    total += hist[2*ii] + hist[2*ii+1];
  }

  cout<<"Total "<<total<<endl;
  return ii;
}

unsigned long GanitaZeroHist::dumpCondHistSep(void)
{
  unsigned int ii;
  unsigned long total = 0;

  for(ii=0; ii<hist_length; ii++){
    fprintf(stdout, "Index %6X Count %5ld\n", 2*ii, hist[2*ii]);
    fprintf(stdout, "Index %6X Count %5ld\n", 2*ii+1, hist[2*ii+1]);
    total += hist[2*ii] + hist[2*ii+1];
  }

  cout<<"Total "<<total<<endl;
  return ii;
}

uint64_t GanitaZeroHist::findMaxCondHist(void)
{
  uint64_t ii;
  uint64_t max;
  uint64_t max_ii;

  max = 0;
  max_ii = 0;
  for(ii=0; ii<2*hist_length; ii++){
    if(hist[ii] > max){
      max_ii = ii;
      max = hist[ii];
    }
  }
  max_count = max;
  max_index = max_ii;
  return(max);
}

uint64_t GanitaZeroHist::findTopIndices(double ratio)
{
  uint64_t ii;
  uint64_t tmp_thresh = (uint64_t)(ratio * ((double)max_count));
  uint64_t count; 

  count = 0;
  for(ii=0; ii<2*hist_length; ii++){
    if(hist[ii] > tmp_thresh){
      fprintf(stdout, "High Index: %lld\n", (long long int) ii);
      count++;
    }
  }

  return(count);
}

double GanitaZeroHist::returnPoisson(double lambda, uint64_t k)
{
  return(pow(lambda,k) * exp(-1*lambda) / tgamma((double) k+1));
}

uint64_t GanitaZeroHist::returnArc4Rand(char *myran, uint64_t len)
{
  arc4random_buf((char *)myran, (size_t) len);
  return(len);
}

uint64_t GanitaZeroHist::dumpRand(uint64_t len)
{
  uint64_t ii;
  char *mychar = new char[len];
  returnArc4Rand(mychar, len);
  fprintf(stdout, "Random %lld bits:\n", (long long int) 8*len);
  for(ii=0; ii<len; ii++){
    //fprintf(stdout, "%02X", mychar[ii] & 0xff);
    fprintf(stdout, "%c", mychar[ii] & 0xff);
  }
  fprintf(stdout, "\n");
  return(len);
}

uint64_t GanitaZeroHist::dumpHistHist(uint64_t len)
{
  uint64_t *hh = new uint64_t[len]();
  uint64_t ii;
  fprintf(stdout, "Poisson histogram length %ld:\n", hist_length);
  for(ii=0; ii<hist_length; ii++){
    if(hist[ii] < len){
      hh[hist[ii]]++;
    }
  }
  //fprintf(stdout, "Poisson histogram:\n");
  for(ii=0; ii<len; ii++){
    fprintf(stdout, "Index / Count: %lld / %lld\n", 
	    (long long int) ii, (long long int) hh[ii]);
  }

  return(len);
}

