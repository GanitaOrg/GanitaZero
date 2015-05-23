#include "ganita/zero/GanitaZeroHist.hpp"

#ifndef DEFAULT_NUM_TOP_FREQ
#define DEFAULT_NUM_TOP_FREQ 100
#endif

#ifndef DEFAULT_RAT_TOP_FREQ
#define DEFAULT_RAT_TOP_FREQ 4
#endif

GanitaZeroHist::GanitaZeroHist(void)
{
  hist_length = 0;
  condition_num = 0;
  est_num = 0;
  max_count = 0;
  max_index = 0;
  divisor = 1;
  longone = 0x1;
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
  longone = 0x1;
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
    hist_length = longone << h_len;
    hist = new(nothrow) unsigned long[2*hist_length]();
  }
  else {
    //cout<<"previously allocated\n";
    delete hist;
    hist_length = longone << h_len;
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
    pbit = (unsigned long) (((ptr[b1] & 0xff) >> bottom) & longone);
    cond_bits = 0;
    for(jj=ii-condition_num; jj<ii; jj++){
      bottom = jj % 8;
      b1 = jj / 8;
      cbit = (unsigned long) (((ptr[b1] & 0xff) >> bottom) & longone);
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
    pbit = (unsigned long) (((input->getByte(b1) & 0xff) >> bottom) & longone);
    cond_bits = 0;
    for(jj=ii-condition_num; jj<ii; jj++){
      bottom = jj % 8;
      b1 = jj / 8;
      cbit = (unsigned long) (((input->getByte(b1) & 0xff) >> bottom) & longone);
      cond_bits |= (cbit << (jj + condition_num - ii));
    }
    //fprintf(stdout, "Values: %04X, %01X\n", cond_bits,pbit);
    hist[2*cond_bits + pbit]++;
  }

  return 1;
}

int GanitaZeroHist::computeCondHist2(GanitaBuffer *input)
{
  unsigned long cond_bits;
  unsigned long pbit, cbit;
  unsigned long ii, jj;
  unsigned long b1, bottom;

  //cout<<"Number of bytes: "<<input->size()<<endl;
  cout<<input->size()<<" ";

  for(ii=condition_num; ii<8*(input->size()-1); ii++){
    // Get the probability bit
    b1 = ii / 8;
    bottom = ii % 8;
    pbit = (unsigned long) (((input->getByte(b1) & 0xff) >> bottom) & longone);
    cond_bits = 0;
    for(jj=ii-condition_num; jj<ii; jj++){
      bottom = jj % 8;
      b1 = jj / 8;
      cbit = (unsigned long) (((input->getByte(b1) & 0xff) >> bottom) & longone);
      cond_bits |= (cbit << (jj + condition_num - ii));
      //fprintf(stdout, "iValues: %04X, %01X %02X %d\n", cond_bits,pbit,input->getByte(b1),condition_num);
    }
    //fprintf(stdout, "fValues: %04X, %01X %02X\n", cond_bits,pbit,input->getByte(b1));
    hist[cond_bits + (pbit << condition_num)]++;
  }

  return 1;
}

// This computes the conditional histograms for 
// all patterns of length less than condition_num. 
// This stores the values for multiple histograms 
// at the same time in hist. 
int GanitaZeroHist::computeCondHistAll(GanitaBuffer *input)
{
  unsigned long cond_bits;
  uint64_t ii;
  int jj;
  uint64_t count1, count2;

  //cout<<"Number of bytes: "<<input->size()<<endl;
  //cout<<input->size()<<" ";

  count1 = 0;
  count2 = 0;
  cond_bits = 0;
  for(jj=0; jj<condition_num; jj++){
    cond_bits = (cond_bits << 1) | (input->getBit(condition_num - 1 - jj));
    hist[cond_bits + count2]++;
    count1++;
    count2 += (longone << count1);
    //fprintf(stdout, "iValues: %04X, %01X %02X %d\n", cond_bits,pbit,input->getByte(b1),condition_num);
  }
  for(ii=condition_num; ii<8*(input->size()-1); ii++){
    count1 = 0;
    count2 = 0;
    cond_bits = (cond_bits >> 1) | ((input->getBit(ii)) << (condition_num - 1));
    for(jj=0; jj<condition_num; jj++){
      hist[(cond_bits >> (condition_num - 1 - jj)) + count2]++;
      count1++;
      count2 += (longone << count1);
    }
  }

  return 1;
}

// Not ready yet ... 
// This computes the conditional histograms for 
// all patterns of length less than condition_num. 
int GanitaZeroHist::computeCondHistNested(GanitaBuffer *input)
{
  uint64_t cond_bits[condition_num];
  uint64_t cond_bits2[condition_num];
  uint64_t hs[condition_num];
  uint64_t ii; 
  int jj, kk;
  uint64_t mybitlen;
  uint64_t mybit;
  
  mybitlen = 8*input->size()-condition_num-8;

  hs[0] = 0; cond_bits[0] = 0;
  for(jj=1; jj<condition_num; jj++){
    hs[jj] = hs[jj-1] + (longone << jj);
    cond_bits[jj] = 0;
  }
  
  ii = condition_num - 1;;
  while(ii<mybitlen){
    if(input->getInOutBit(ii) == 1){
      for(jj=0; jj<condition_num; jj++){
	cond_bits[jj] = 0;
      }
      while(input->getInOutBit(ii) == 1){
	ii++;
	if(ii >= mybitlen){
	  return(1);
	}
      }
    }
    mybit = input->getBit(ii);
    hist[mybit]++; cond_bits2[0] = mybit;
    jj = 1;
    while(jj<condition_num){
      if(input->getInOutBit(ii-jj) == 1){
	break;
      }
      jj++;
    }
    for(kk=1; kk<jj; kk++){
      cond_bits2[kk] = cond_bits[kk-1] | (mybit << kk);
      hist[cond_bits2[kk] + hs[kk]]++;
    }
    for(kk=0; kk<jj; kk++){
      cond_bits[kk] = cond_bits2[kk];
    }
    ii++;
  }

  return 1;
}

// Compute an approximation of the conditional entropy
double GanitaZeroHist::computeCondEntAll(void)
{
  uint64_t ii, total, combo;
  double pp, entropy;
  uint64_t count1, count2;
  uint64_t s1, s2, s3;
  int jj;

  entropy = 0;
  count1 = 0;
  count2 = 2;
  for(jj=0; jj<condition_num; jj++){
    total = 0;
    entropy = 0;
    for(ii=count1; ii<count2; ii++){
      total += hist[ii];
    }    
    s2 = count2 - count1;
    s1 = s2 >> 1;
    s3 = s1 + count1;
    for(ii=0; ii<s1; ii++){
      combo = hist[ii+count1] + hist[ii+s3];
      if(combo != 0){
	pp = ((double) hist[ii+count1]) / ((double) combo);
	entropy += 
	  4*pp*(1-pp)*((double) combo) / ((double) total);
      }
    }
    fprintf(stdout, "Conditional bits: %d, Entropy: %lf\n", jj, entropy);
    stat.push_back(entropy);
    count1 = count2;
    count2 += (longone << (jj+2));
  }
  
  return(entropy);
}

// Compute an approximation of the conditional entropy
GanitaZeroTile GanitaZeroHist::getBestTile1(void)
{
  GanitaZeroTile tile;
  int bestPatLen;
  
  computeCondEntAll();
  bestPatLen = bestPatLen1();
  fprintf(stdout, "Choose pattern length: %d.\n", bestPatLen);
  tile.set(findMaxCondHist(bestPatLen), bestPatLen);
  fprintf(stdout, "Best tile: %16lX\n", tile.getTile());

  return(tile);
}

// Compute an approximation of the conditional entropy
int GanitaZeroHist::getBestSize(void)
{
  int bestPatLen;
  
  computeCondEntAll();
  bestPatLen = bestPatLen1();
  fprintf(stdout, "Choose pattern length: %d.\n", bestPatLen);
  return(bestPatLen);
}

int GanitaZeroHist::getBestTiles
(int bpL, vector< std::shared_ptr<GanitaZeroTile> > tile)
{
  //int bestPatLen;
  
  //computeCondEntAll();
  //bestPatLen = bestPatLen1();
  //fprintf(stdout, "Choose pattern length: %d.\n", bestPatLen);
  return(findTopCondHist(bpL, tile));
}

int GanitaZeroHist::bestPatLen1(void)
{
  uint32_t ii, max_ii;
  double max;
  double tmp;

  max = 0;
  max_ii = 0;
  for(ii=1; ii<stat.size()-1; ii++){
    //tmp = stat[ii-1] + stat[ii+1] - 2*stat[ii];
    if(stat[ii+1] == 0){
      //choose max_ii = ii
      max_ii = ii;
      break;
    }
    tmp = (stat[ii-1]/stat[ii]) - (stat[ii]/stat[ii+1]);
    if(tmp > max){
      max = tmp;
      max_ii = ii - 1;
    }
  }
  
  return(max_ii);
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

uint64_t GanitaZeroHist::findMaxCondHist(int hist_ii)
{
  int ii;
  uint64_t jj;
  uint64_t max;
  uint64_t max_ii;
  uint64_t start_ii, end_ii;

  if(hist_ii >= condition_num){
    fprintf(stderr, "Histogram index is out of bounds.\n");
    hist_ii = hist_ii % condition_num;
  }
  start_ii = 0;
  for(ii=1; ii<hist_ii; ii++){
    start_ii += (longone << ii);
  }
  end_ii = start_ii + (longone << hist_ii);

  max = 0;
  max_ii = 0;
  for(jj=start_ii; jj<end_ii; jj++){
    if(hist[jj] >= max){
      max_ii = jj - start_ii;
      max = hist[jj];
    }
  }
  max_count = max;
  max_index = max_ii;
  return(max_ii);
}

int GanitaZeroHist::findTopCondHist(int hist_ii, vector< std::shared_ptr<GanitaZeroTile> > mytiles)
{ 
  int ii;
  uint64_t jj;
  uint64_t start_ii, end_ii;
  uint64_t min, min_ii;

  if(mytiles.size() <= 0){
    fprintf(stderr, "Tile vector not created.\n");
    return(-1);
  }

  if(hist_ii >= condition_num){
    fprintf(stderr, "Histogram index is out of bounds.\n");
    hist_ii = hist_ii % condition_num;
  }
  start_ii = 0;
  for(ii=1; ii<hist_ii; ii++){
    start_ii += (longone << ii);
  }
  end_ii = start_ii + (longone << hist_ii);

  for(ii=0; ii<(int)mytiles.size(); ii++){
    mytiles[ii]->setValue(0);
  }

  min = 0;
  min_ii = 0;
  for(jj=start_ii; jj<end_ii; jj++){
    if(hist[jj] > mytiles[min_ii]->getValue()){
      mytiles[min_ii]->setValue(hist[jj]);
      mytiles[min_ii]->set(jj - start_ii, hist_ii);
      // Find new min.
      min = mytiles[min_ii]->getValue();
      for(ii=0; ii<(int)mytiles.size(); ii++){
	if(mytiles[ii]->getValue() < min){
	  min = mytiles[ii]->getValue();
	  min_ii = ii;
	}
      }
    }
  }

  ii = mytiles.size() - 1;
  while(mytiles[ii]->getValue() <= 0){
    ii--;
  }

  return(ii+1);
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
  //fprintf(stdout, "Random %lld bits:\n", (long long int) 8*len);
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
  //fprintf(stdout, "Poisson histogram length %ld:\n", hist_length<<1);
  for(ii=0; ii<(hist_length<<1); ii++){
    if(hist[ii] < len){
      hh[hist[ii]]++;
    }
  }
  //fprintf(stdout, "Poisson histogram:\n");
  for(ii=0; ii<len; ii++){
    //fprintf(stdout, "Index / Count: %lld / %lld\n", 
    //	    (long long int) ii, (long long int) hh[ii]);
    fprintf(stdout, "%ld ", hh[ii]);
  }
  fprintf(stdout, "\n");

  return(len);
}

// Use ratio to find top patterns. 
uint64_t GanitaZeroHist::findTopFreq(uint64_t fsize)
{
  uint64_t ii;
  uint64_t hh;
  uint64_t count;
  double thresh;
  uint64_t htotal;
  
  htotal = 0;
  count = 0;
  hh = (hist_length<<1);
  thresh = DEFAULT_RAT_TOP_FREQ * ((fsize / hh) + 1);

  for(ii=0; ii<hh; ii++){
    if(hist[ii] > thresh){
      fprintf(stdout, "Common pattern: %04X %ld\n", (uint32_t) ii, hist[ii]);
      count++;
      htotal += hist[ii];
    }
  }
  fprintf(stdout, "Total: %ld %ld\n", count, htotal);

  return(count);
}

