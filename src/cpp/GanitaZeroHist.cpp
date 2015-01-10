#include "ganita/zero/GanitaZeroHist.hpp"

GanitaZeroHist::GanitaZeroHist(void)
{
  hist_length = 0;
  condition_num = 0;
  est_num = 0;
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
}

unsigned long GanitaZeroHist::init(unsigned long ss)
{
  if(ss <= 0){
    hist_length = DEFAULT_HIST_SIZE;
  }
  else hist_length = ss;
  hist = new unsigned long[ss]();
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

int GanitaZeroHist::computeByteHist(unsigned char *ptr, unsigned long ss)
{
  unsigned long ii;
  for(ii=0; ii<ss; ii++){
    hist[ptr[ii]]++;
  }

  return 1;
}

// Compute an approximation of the conditional entropy
int GanitaZeroHist::computeCondHist1(unsigned char *ptr, unsigned long ss)
{
  unsigned int byte;
  unsigned int bit;
  unsigned long ii, b1, bottom;
  condition_num = 8;
  est_num = 1;

  for(ii=0; ii<8*(ss-1); ii++){
    bottom = ii % 8;
    b1 = ii / 8;
    byte = (unsigned int) (((ptr[b1] & 0xff) >> bottom) & 0xff);
    byte |= (unsigned int) (((ptr[b1+1] & 0xff) << (8 - bottom)) & 0xff);
    bit = (unsigned int) (((ptr[b1+1] & 0xff) >> bottom) & 0x1);
    //fprintf(stdout, "Values: %02X, %02X, %01X\n", ptr[b1+1],byte,bit);
    hist[2*byte + bit]++;
  }

  return 1;
}

// Compute an approximation of the conditional entropy
int GanitaZeroHist::computeCondHist1(GanitaBuffer *input)
{
  unsigned int byte;
  unsigned int bit;
  unsigned long ii, b1, bottom;
  condition_num = 8;
  est_num = 1;

  cout<<"Number of bytes: "<<input->size()<<endl;

  for(ii=0; ii<8*(input->size()-1); ii++){
    bottom = ii % 8;
    b1 = ii / 8;
    byte = (unsigned int) (((input->getByte(b1) & 0xff) >> bottom) & 0xff);
    byte |= (unsigned int) (((input->getByte(b1+1) & 0xff) << (8 - bottom)) & 0xff);
    bit = (unsigned int) (((input->getByte(b1+1) & 0xff) >> bottom) & 0x1);
    //fprintf(stdout, "Values: %02X, %02X, %01X\n", input->getByte(b1+1),byte,bit);
    hist[2*byte + bit]++;
  }

  return 1;
}

double GanitaZeroHist::computeCondEnt1(void)
{
  unsigned long ii, total;
  double pp, entropy;

  entropy = 0;
  total = 0;
  for(ii=0; ii<2*hist_length; ii++){
    total += hist[ii];
  }

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

