// Methods for GanitaZeroSymbolic class. 
#include "ganita/zero/representations/GanitaZeroSymbolic.hpp"

GanitaZeroSymbolic::GanitaZeroSymbolic(void)
{
  name = "symbolic";
  seq_size = 0;
  alphabet_size = 0;
  alphabet_max = 0;
  // Note alphabet array is initialized later. 
  my_hist = new GanitaZeroHist();
  verbose = 0;
}

// GanitaZeroSymbolic::GanitaZeroSymbolic(std::ifstream &sym_file)
// {
//   name = "symbolic";
//   seq_size = 0;
//   alphabet_size = 0;
//   alphabet_max = 0;
//   // Note alphabet array is initialized later. 
//   gzi = new GanitaBuffer(sym_file);
//   my_hist = new GanitaZeroHist();
//   verbose = 0;
// }

// int GanitaZeroSymbolic::initBuffer(std::ifstream &sym_file)
// {
//   gzi = new GanitaBuffer(sym_file);
//   return 1;
// }

int GanitaZeroSymbolic::initBuffer(void)
{
  gzi = new GanitaBuffer();
  return 1;
}

// unsigned long GanitaZeroSymbolic::loadCharSeq(std::ifstream &sym_file)
// {
//   if(seq_size > 0){
//     // possibly allocated sequence
//     return(0);
//   }
//   sym_file.seekg(0, sym_file.end);
//   seq_size = sym_file.tellg();
//   // Allocate seq of size seq_size
//   seq = new unsigned char[seq_size];
//   // seq = (unsigned char *)malloc((size_t)seq_size*sizeof(char));
//   // if(seq == NULL){
//   //   cout<<"Unable to allocate sequence."<<endl;
//   //   return(0);
//   // }
//   sym_file.seekg(0, sym_file.beg);
//   sym_file.read((char *) seq,seq_size);
//   cout<<"Sequence length: "<<seq_size<<endl;
//   return(seq_size);
// }

uint64_t GanitaZeroSymbolic::loadDoubleDiffQuant(void)
{
  uint64_t ii;
  double tmp1, tmp2;

  if(seq_size > 0){
    // possibly allocated sequence
    return(0);
  }
  if(gzi->doubleSize() <= 1){
    return(0);
  }
  seq_size = (gzi->doubleSize() + 7) / 8;
  fprintf(stdout, "Sequence size: %ld\n", seq_size);
  // Note parentheses below ensure initialized to zero! 
  seq = new unsigned char[seq_size]();
  gzi->getDouble(0,tmp1);
  for(ii=0; ii<gzi->doubleSize()-1; ii++){
    gzi->getDouble(ii+1,tmp2);
    if(tmp2 > tmp1){
      seq[ii / 8] |= (0x1 << (7-(ii % 8))) & 0xff;
    }
    tmp1 = tmp2;
  } 

  return(ii);
}

// int GanitaZeroSymbolic::init(std::ifstream &sym_file)
// {
//   unsigned long ii;
//   // load data into array seq
//   // deprecated loadCharSeq
//   if(loadCharSeq(sym_file) == 0) return(-1);
//   // Set alphabet to zero
//   for(ii=0; ii<ALPHABET_ALLOC_SIZE; ii++){
//     alphabet[ii] = 0;
//   }
//   // Determine alphabet size
//   for(ii=0; ii<seq_size; ii++){
//     if(alphabet[seq[ii]] <= 0){
//       alphabet[seq[ii]]++;
//     }
//     if(seq[ii] > alphabet_max){
//       alphabet_max = seq[ii];
//     }
//   }
//   alphabet_size = 0;
//   for(ii=0; ii<ALPHABET_ALLOC_SIZE; ii++){
//     if(alphabet[ii] > 0) alphabet_size++;
//   }

//   //cout<<"Alph size: "<<alphabet_size<<endl;
  
//   return(alphabet_size);
// }

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
  // Using GanitaBuffer.
  gzi = new GanitaBuffer();
  gzi->open(input_file);
  gzi->createInOutBuffer();
 
  return(1);
}

int GanitaZeroSymbolic::buildAlphabet(void)
{
  unsigned long ii;
  // Set alphabet to zero.
  for(ii=0; ii<ALPHABET_ALLOC_SIZE; ii++){
    alphabet[ii] = 0;
  }
  //Determine alphabet size
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
      cout<<std::hex<<ii<<":";
    }
  }

  // cout<<"Scanned "<<std::dec<<gzi->size()<<" bytes.\n";
  // cout<<"Alph size: "<<alphabet_size<<endl;
  return(alphabet_size);
}

uint64_t GanitaZeroSymbolic::initD(char *input_file)
{
  unsigned long ii;
  // Using GanitaBuffer.
  gzi = new GanitaBuffer();
  gzi->openDoubleLine(input_file);
  // Set alphabet to zero.
  for(ii=0; ii<ALPHABET_ALLOC_SIZE; ii++){
    alphabet[ii] = 0;
  }
  //gzi->dumpDoubles();
  
  return(1);
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

double GanitaZeroSymbolic::computeCondEnt1FromScratch(int h_len)
{
  my_hist->initConditional(h_len);
  my_hist->computeCondHist1(seq, seq_size);
  fprintf(stdout, "Max count: %lld\n", (long long int) my_hist->findMaxCondHist());
  my_hist->findTopIndices(.51);
  return(my_hist->computeCondEnt1());
}

double GanitaZeroSymbolic::computeCondEnt2FromScratch(int h_len)
{
  my_hist->initConditional(h_len);
  my_hist->computeCondHist2(gzi);
  return(my_hist->computeCondEnt1());
}

double GanitaZeroSymbolic::computeCondEntAll(int h_len)
{
  my_hist->initConditional(h_len);
  my_hist->computeCondHistAll(gzi);
  return(my_hist->computeCondEntAll());
}

int GanitaZeroSymbolic::tileSpaceZero(int h_len)
{
  int ntiles, ii, bestPatLen;
  int max_tile_ii;
  uint64_t num;
  GanitaBuffer *newgzi;
  newgzi = new GanitaBuffer();
  newgzi->open(gzi->returnFileName());

  my_hist->initConditional(h_len);
  //my_hist->computeCondHistAll(gzi);
  my_hist->computeCondHistAll(gzi);
  bestPatLen = my_hist->getBestSize();
  //cout<<"Pattern length: "<<bestPatLen<<endl;
  for(ii=0; ii<bestPatLen; ii++){
    addTile();
  }
  ntiles = my_hist->getBestTiles(bestPatLen, mytile);
  //cout<<"Number of tiles: "<<ntiles<<endl;
  for(ii=0; ii<ntiles; ii++){
    mytile[ii]->dumpTile();
    fprintf(stdout, " %ld\n", countBitPat2(mytile[ii]));
  }
  //majorTileSelector();
  max_tile_ii = maxTileSelector();
  if(max_tile_ii >= 0){
    num = updatePatBits(mytile[max_tile_ii]);
    newgzi->createInOutBuffer((char *)"gzero.shrink1", (num+7)/8);
    cout<<shrinkPatBits(newgzi, mytile[max_tile_ii])<<endl;
  }
  newgzi->close();
  return(1);
}

GanitaZeroTile *GanitaZeroSymbolic::getTileSpaceZero(int h_len)
{
  int ntiles, ii, bestPatLen;
  int max_tile_ii;
  uint64_t num;
  GanitaBuffer *newgzi;
  uint64_t bnum;
  newgzi = new GanitaBuffer();
  newgzi->open(gzi->returnFileName());

  my_hist->initConditional(h_len);
  my_hist->computeCondHistAll(gzi);
  bestPatLen = my_hist->getBestSize();
  //cout<<"Pattern length: "<<bestPatLen<<endl;
  for(ii=0; ii<bestPatLen; ii++){
    addTile();
  }
  ntiles = my_hist->getBestTiles(bestPatLen, mytile);
  //cout<<"Number of tiles: "<<ntiles<<endl;
  for(ii=0; ii<ntiles; ii++){
    mytile[ii]->dumpTile();
    fprintf(stdout, " %ld\n", countBitPat2(mytile[ii]));
  }
  //majorTileSelector();
  max_tile_ii = maxTileSelector();
  if(max_tile_ii >= 0){
    countBitPatBlock(mytile[max_tile_ii]);
    bnum = goodBlock(mytile[max_tile_ii]);
    cout<<"Good block: "<<bnum<<endl;
    slideChanges(bnum);
    num = updatePatBits(mytile[max_tile_ii]);
    newgzi->createInOutBuffer((char *)"gzero.shrink1", (num+7)/8);
    cout<<shrinkPatBits(newgzi, mytile[max_tile_ii])<<endl;
  }
  newgzi->close();
  return( mytile[max_tile_ii].get() );
}

int GanitaZeroSymbolic::tileSpace(int h_len)
{
  int ntiles, ii, bestPatLen;
  int max_tile_ii;
  uint64_t num;
  GanitaBuffer *newgzi;
  newgzi = new GanitaBuffer();
  newgzi->open(gzi->returnFileName());

  my_hist->initConditional(h_len);
  //my_hist->computeCondHistAll(gzi);
  my_hist->computeCondHistNested(gzi);
  bestPatLen = my_hist->getBestSize();
  cout<<"Pattern length: "<<bestPatLen<<endl;
  mytile.clear();
  for(ii=0; ii<bestPatLen; ii++){
    addTile();
  }
  ntiles = my_hist->getBestTiles(bestPatLen, mytile);
  cout<<"Number of tiles: "<<ntiles<<endl;
  if(!ntiles) return(0);
  for(ii=0; ii<ntiles; ii++){
    mytile[ii]->dumpTile();
    fprintf(stdout, " %ld\n", countBitPatNested(mytile[ii]));
  }
  //majorTileSelector();
  max_tile_ii = maxTileSelector();
  if(max_tile_ii >= 0){
    num = updatePatBits(mytile[max_tile_ii]);
    newgzi->createInOutBuffer((char *)"gzero.shrink2", (num+7)/8);
    cout<<shrinkPatBits(newgzi, mytile[max_tile_ii])<<endl;
  }
  newgzi->close();
  return(1);
}

int GanitaZeroSymbolic::addTile(void)
{
  GanitaZeroTile *newtile = new GanitaZeroTile();
  mytile.push_back(std::make_shared<GanitaZeroTile>(*newtile));
  delete newtile;
  return(mytile.size());
}

int GanitaZeroSymbolic::addBlock(void)
{
  GanitaZeroBlock *newblock = new GanitaZeroBlock();
  blocks.push_back(std::make_shared<GanitaZeroBlock>(*newblock));
  delete newblock;
  return(blocks.size());
}

uint64_t GanitaZeroSymbolic::addBlocks(uint64_t nblocks)
{
  uint64_t ii;
  for(ii=0; ii<nblocks; ii++){
    addBlock();
  }
  return(blocks.size());
}

uint64_t GanitaZeroSymbolic::countBitPat1(GanitaZeroTile mytile)
{
  uint64_t ii, count;
  uint64_t tarpat;
  int len;
  uint64_t refpat;
  uint64_t fsize;
  fsize = gzi->size();
  len = mytile.returnSize();
  refpat = mytile.getTile();
  tarpat = gzi->getBits(0,len);
  count = 0;
  ii = len - 1;
  while(ii<8*fsize-2*len){
    if(tarpat == refpat){
      count++;
      ii += len;
      tarpat = gzi->getBits(ii, len);
    }
    else {
      ii++;
      tarpat = (tarpat >> 1) | (gzi->getBit(ii) << len);
    }
  }
  return(count);
}

// Need to update this method so that 
// it only counts when getInOutBit == 0.
uint64_t GanitaZeroSymbolic::countBitPat2
(std::shared_ptr<GanitaZeroTile>& mytile)
{
  uint64_t bitsToScan;
  uint64_t ii;
  uint32_t count;
  uint64_t tarpat;
  uint32_t len;
  uint64_t refpat;
  uint64_t fsize;
  fsize = gzi->size();
  len = mytile->returnSize();
  refpat = mytile->getTile();
  tarpat = gzi->getBits(0,len);
  count = 0;
  ii = 0;
  bitsToScan = (fsize << 3) - (len << 1);
  //mytile->dumpTile();
  while(ii<bitsToScan){
    if(tarpat == refpat){
      count++;
      ii += len;
      tarpat = gzi->getBits(ii, len);
      //cout<<"|"<<ii<<" ";
    }
    else {
      //cout<<ii<<"|";
      ii++;
      tarpat = (tarpat >> 1) | (gzi->getBit(ii) << (len - 1));
    }
  }
  mytile->setValue(count);
  return(count);
}

uint64_t GanitaZeroSymbolic::countBitPatBlock
(std::shared_ptr<GanitaZeroTile>& mytile)
{
  uint64_t bitsToScan;
  uint64_t ii, jj;
  uint32_t count;
  uint64_t tarpat;
  uint32_t len;
  uint64_t refpat;
  uint64_t fsize;
  uint64_t nblocks;
  uint64_t blocksize;
  nblocks = 100;
  blocks.clear();
  nblocks = addBlocks(nblocks);
  cout<<"Created "<<nblocks<<" blocks."<<endl;
  fsize = gzi->size();
  len = mytile->returnSize();
  refpat = mytile->getTile();
  tarpat = gzi->getBits(0,len);
  count = 0;
  bitsToScan = (fsize << 3) - (len << 1);
  blocksize = bitsToScan / (nblocks + 1);
  cout<<"Block size: "<<blocksize<<"."<<endl;
  for(ii=0; ii<nblocks; ii++){
    blocks[ii]->setStart(ii*blocksize);
    blocks[ii]->setEnd((ii+1)*blocksize);
  }
  cout<<"Set end of blocks."<<endl;
  ii = 0; jj = 0;
  //mytile->dumpTile();
  while(ii<bitsToScan){
    if(tarpat == refpat){
      count++;
      ii += len;
      tarpat = gzi->getBits(ii, len);
      //cout<<"|"<<ii<<" ";
    }
    else {
      //cout<<ii<<"|";
      ii++;
      tarpat = (tarpat >> 1) | (gzi->getBit(ii) << (len - 1));
    }
    if(ii >= blocks[jj]->returnEnd()){
      blocks[jj]->setCount(count);
      jj++;
      count = 0;
    }
    if(jj >= nblocks) break;
  }
  return(count);
}

uint64_t GanitaZeroSymbolic::goodBlock
(std::shared_ptr<GanitaZeroTile>& mytile)
{
  if(!blocks.size()){
    return(0);
  }
  uint64_t ii, jj;
  uint64_t nn, ws;
  uint64_t max, max_ii;
  //nn = blocks.size();
  //qsort(blocks[0], blocks.size(), sizeof(GanitaZeroBlock *), compar);
  std::sort(blocks.begin(), blocks.end(), gzu::gzCompareBlocks);
  for(ii=0; ii<blocks.size(); ii++){
    cout<<blocks[ii]->returnCount()<<":";
  }
  cout<<endl;
  nn = mytile->getValue() / (blocks.size() + 1) - 1;
  ii = 0;
  cout<<"Target count: "<<nn<<endl;
  while(blocks[ii]->returnCount() < nn){
    ii++;
  }
  jj = 0; ws = nn / 10;
  if(ws < 2) ws = 2;
  max = 0; max_ii = 0;
  while(ii + jj < blocks.size()){
    while(blocks[ii+jj]->returnCount() < nn + ws){
      jj++;
      if(ii + jj >= blocks.size()) break;
    }
    if(jj > max){
      max = jj;
      max_ii = ii;
    }
    nn += ws;
    ii += jj;
    jj = 0;
  }

  return(max_ii);
}

uint64_t GanitaZeroSymbolic::countBitPatNested
(std::shared_ptr<GanitaZeroTile>& mytile)
{
  uint64_t bitsToScan;
  uint64_t ii;
  uint32_t count;
  uint64_t tarpat;
  uint32_t len;
  uint64_t refpat;
  uint64_t fsize;
  fsize = gzi->size();
  len = mytile->returnSize();
  bitsToScan = (fsize << 3) - (len << 1);
  refpat = mytile->getTile();
  ii = 0;
  while(gzi->getInOutBit(ii,len)){
    ii++;
    if(ii >= bitsToScan) return(0);
  }
  tarpat = gzi->getBits(ii,len);
  count = 0;
  //mytile->dumpTile();
  while(ii<bitsToScan){
    if(tarpat == refpat){
      count++;
      ii += len;
      //cout<<"|"<<ii<<" ";
    }
    else {
      //cout<<ii<<"|";
      ii++;
      //tarpat = (tarpat >> 1) | (gzi->getBit(ii) << (len - 1));
    }
    while(gzi->getInOutBit(ii,len)){
      ii++;
      if(ii >= bitsToScan) return(count);
    }
    tarpat = gzi->getBits(ii, len);
  }
  mytile->setValue(count);
  return(count);
}

uint64_t GanitaZeroSymbolic::updatePatBits
(std::shared_ptr<GanitaZeroTile>& mytile)
{
  uint64_t bitsToScan;
  uint64_t ii;
  uint32_t jj;
  uint64_t tarpat;
  uint32_t len;
  uint64_t refpat;
  uint64_t fsize;
  uint64_t count;
  
  if(!mytile->getValue()){
    fprintf(stdout, "No bits to be updated.\n");
    return(0);
  }

  count = 0;
  fsize = gzi->size();
  len = mytile->returnSize();
  refpat = mytile->getTile();
  tarpat = gzi->getBits(0,len);
  ii = 0;
  bitsToScan = (fsize << 3) - (len << 1);
  //mytile->dumpTile();
  while(ii<bitsToScan){
    if(tarpat == refpat){
      //count++;
      for(jj=0; jj<len; jj++){
	gzi->writeBufBitInOut(1,ii+jj);
      }
      ii += len; count++;
      tarpat = gzi->getBits(ii, len);
      //cout<<ii<<" ";
    }
    else {
      //cout<<ii<<"|";
      ii++; count++;
      tarpat = (tarpat >> 1) | (gzi->getBit(ii) << (len - 1));
    }
  }
  gzi->flushInOut();
  //mytile->setValue(count);
  return(count);
}

uint64_t GanitaZeroSymbolic::shrinkPatBits
(GanitaBuffer *newgzi, std::shared_ptr<GanitaZeroTile>& mytile)
{
  uint64_t bitsToScan;
  uint64_t ii;
  uint64_t jj;
  uint64_t tarpat;
  uint32_t len;
  uint64_t refpat;
  uint64_t fsize;
  
  if(!mytile->getValue()){
    fprintf(stdout, "No bits to be updated.\n");
    return(0);
  }

  fsize = gzi->size();
  len = mytile->returnSize();
  refpat = mytile->getTile();
  tarpat = gzi->getBits(0,len);
  ii = 0; jj = 0;
  bitsToScan = (fsize << 3) - (len << 1);
  //mytile->dumpTile();
  while(ii<bitsToScan){
    if(tarpat == refpat){
      //count++;
      newgzi->writeBufBitInOut(0,jj);
      ii += len; jj++;
      tarpat = gzi->getBits(ii, len);
      //cout<<ii<<" ";
    }
    else {
      //cout<<ii<<"|";
      newgzi->writeBufBitInOut(1,jj);
      ii++; jj++;
      tarpat = (tarpat >> 1) | (gzi->getBit(ii) << (len - 1));
    }
  }
  newgzi->flushInOut();
  //mytile->setValue(count);
  return(jj);
}

double GanitaZeroSymbolic::computeCondHist2(int h_len)
{
  my_hist->initConditional(h_len);
  return(my_hist->computeCondHist2(gzi));
}

double GanitaZeroSymbolic::computeCondHistAll(int h_len)
{
  my_hist->initConditional(h_len);
  return(my_hist->computeCondHistAll(gzi));
}

unsigned long GanitaZeroSymbolic::dumpCondHist1(void)
{
  return(my_hist->dumpCondHist1());
}

unsigned long GanitaZeroSymbolic::dumpCondHistSep(void)
{
  return(my_hist->dumpCondHistSep());
}

string GanitaZeroSymbolic::returnB64Encode(void)
{
  return(gzi->b64Encode());
}

int GanitaZeroSymbolic::close(void)
{
  return(gzi->close());
}

uint64_t GanitaZeroSymbolic::dumpHistHist(uint64_t len)
{
  return(my_hist->dumpHistHist(len));
}

uint64_t GanitaZeroSymbolic::findTopFreq(void)
{
  return(my_hist->findTopFreq(gzi->size()));
}

int GanitaZeroSymbolic::majorTileSelector(void)
{
  uint32_t ii, jj;
  uint64_t sum1, sum2;

  if(mytile.size() <= 0){
    return(0);
  }

  GanitaZeroTile tmpTile(mytile[0]->returnSize());
  sum1 = 0;
  for(ii=0; ii<mytile.size(); ii++){
    sum1 += mytile[ii]->getValue();
  }
  for(jj=0; jj<mytile[0]->returnSize(); jj++){
    sum2 = 0;
    for(ii=0; ii<mytile.size(); ii++){
      sum2 += mytile[ii]->getValue() * mytile[ii]->getBit(jj);
    }
    cout << sum2 << endl;
    if((sum2 << 1) > sum1){
      // Set bit to 1
      tmpTile.setBit(jj,0x1);
    }
  }
  tmpTile.dumpTile(); cout<<endl;
  return(1);
}

int GanitaZeroSymbolic::maxTileSelector(void)
{
  uint32_t ii;
  uint64_t max, max_ii;

  if(mytile.size() <= 0){
    return(-1);
  }

  max = 0; max_ii = 0;
  for(ii=0; ii<mytile.size(); ii++){
    if(mytile[ii]->getValue() > max){
      max = mytile[ii]->getValue();
      max_ii = ii;
    }
  }
  cout<<"Max tile:"<<endl;
  mytile[max_ii]->dumpTile(); cout<<endl;
  return(max_ii);
}

int GanitaZeroSymbolic::countInOutBits(void)
{
  uint64_t ii;
  uint64_t count;

  count = 0;
  for(ii=0; ii<8*(gzi->inOutSize()); ii++){
    count += gzi->getInOutBit(ii);
  }
  fprintf(stdout, "InOut File Count: %ld / %ld\n", 
	  count, 8*(gzi->inOutSize()));

  return(1);
}

int GanitaZeroSymbolic::setVerbosity(int vv)
{
  verbose = vv;
  return(verbose);
}

int GanitaZeroSymbolic::computeAutoCorr(int64_t len)
{
  return(my_hist->computeAutoCorr(len, gzi));
}

int GanitaZeroSymbolic::computeDFT(void)
{
  return(my_hist->computeDFT(gzi));
}

int GanitaZeroSymbolic::slideChanges(uint64_t bn)
{
  uint64_t ii, ss, ee;
  int64_t dp;
  uint64_t jj, count, bitsToScan;
  uint64_t blocksize;
  uint64_t max, kk;
  count = 0;
  ss = blocks[bn % blocks.size()]->returnStart();
  ee = blocks[bn % blocks.size()]->returnEnd();
  blocksize = ee - ss;
  bool *ch, *dd;
  ch = (bool *) malloc(sizeof(bool)*(blocksize-1));
  dd = (bool *) malloc(sizeof(bool)*blocksize);
  if((dd == NULL) || (ch == NULL)){
    cout<<"Ran out of memory in slideChanges."<<endl;
    return(-1);
  }
  // 1 false, (-1)^1 = -1 becomes false
  // 0 true, (-1)^0 = 1 becomes true
  for(ii=ss; ii<ee-1; ii++){
    if(gzi->getBit(ii) != gzi->getBit(ii+1)){
      ch[ii-ss] = 1;
      count++;
    }
    else ch[ii-ss] = 0;
  }
  dp = ee - ss;
  for(ii=0; ii<blocksize-1; ii++){
    dd[ii] = ch[ii];
  }
  dd[blocksize-1] = (gzi->getBit(ee) != gzi->getBit(ee-1));
  dp -= 2*count + dd[blocksize-1];
  bitsToScan = 8*gzi->size();
  cout<<"bitsToScan|start|end="<<bitsToScan<<"|"<<ss<<"|"<<ee<<endl;
  for(ii=0; ii<8*(gzi->size()-1); ii++) gzi->getBit(ii);
  max = 0; kk = 1;
  ii = 2;
  //while(ee + ii < bitsToScan){
  for(ii=2; ee + ii < bitsToScan; ii++){
    gzi->getBit(ii);
  }
  for(ii=2; ee + ii < bitsToScan; ii++){
    for(jj=0; jj<blocksize-kk; jj++){
      if(ch[jj]){
	dd[jj+kk] ^= 1;
      }
    }
    for(jj=blocksize-kk; jj<blocksize-1; jj++){
      // if(ch[jj]){
      // 	dd[jj + kk - blocksize] ^= 1;
      // }
    }
    //dd[kk - 1] = (gzi->getBit(ee-1) != gzi->getBit(ee + ii - 1));
    // dd[kk - 1] = 1;
    // //gzi->getByte(0);
    // //ii++;
    kk++;
    if(kk >= blocksize) kk = 1;
    //gzi->getByte(ii/8);
    //for(jj=0; jj<10000; jj++){
    //dd[jj] ^= 1;
    //}
  }

  return(1);
}

