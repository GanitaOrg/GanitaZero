#ifndef _GANITAZEROSYMBOLIC_
#define _GANITAZEROSYMBOLIC_

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <algorithm>
//#include <boost/shared_ptr.hpp>

#ifndef ALPHABET_ALLOC_SIZE
#define ALPHABET_ALLOC_SIZE 256
#endif

#include "ganita/zero/GanitaZeroHist.hpp"
#include "ganita/graph/GanitaBuffer.hpp"
#include "ganita/graph/GanitaGraphList.hpp"
#include "ganita/zero/GanitaZeroBlock.hpp"
#include "ganita/zero/GanitaZeroUtil.hpp"

class GanitaZeroSymbolic
{
protected:
  std::string name;
  unsigned char *seq;   // For efficiency and memory considerations
  unsigned long seq_size;
  unsigned long alphabet_size;
  unsigned long alphabet_max;
  unsigned char alphabet[ALPHABET_ALLOC_SIZE];
  GanitaZeroHist *my_hist;
  GanitaBuffer *gzi;
  vector< std::shared_ptr<GanitaZeroTile> > mytile;
  vector< std::shared_ptr<GanitaZeroBlock> > blocks;
  int verbose;
public:
  GanitaZeroSymbolic(void);
  //GanitaZeroSymbolic(std::ifstream &sym_file);
  //int initBuffer(std::ifstream &sym_file);
  int initBuffer(void);
  //unsigned long loadCharSeq(std::ifstream &sym_file);
  uint64_t loadDoubleDiffQuant(void);
  //int init(std::ifstream &sym_file);
  int init(void);
  int init(char *input_file);
  int buildAlphabet(void);
  uint64_t initD(char *input_file);
  int init(char *input_file, char *out_file);
  unsigned long initOut(char *out_file);
  int shrinkAlphabet(void);
  int dumpAlphabet(void);
  int computeByteHist(void);
  unsigned long dumpHist(void);
  double computeCondEnt1FromScratch(int h_len);
  double computeCondEnt2FromScratch(int h_len);
  double computeCondEntAll(int h_len);
  GanitaZeroTile *getTileSpaceZero(int h_len, int mode);
  int tileSpaceZero(int h_len);
  int tileSpace(int h_len);
  int addTile(void);
  int addBlock(void);
  uint64_t addBlocks(uint64_t nblocks);
  uint64_t countBitPat1(GanitaZeroTile mytile);
  uint64_t countBitPat2(std::shared_ptr<GanitaZeroTile>& mytile);
  uint64_t countBitPatBlock(std::shared_ptr<GanitaZeroTile>& mytile);
  uint64_t goodBlock(std::shared_ptr<GanitaZeroTile>& mytile);
  static bool compar
  (std::shared_ptr<GanitaZeroBlock> b1, std::shared_ptr<GanitaZeroBlock> b2);
  //int compar(GanitaZeroBlock *b1, GanitaZeroBlock *b2);
  //(std::shared_ptr<GanitaZeroBlock>& b1, std::shared_ptr<GanitaZeroBlock>& b2);
  //(void *b1, void *b2);
  uint64_t countBitPatNested(std::shared_ptr<GanitaZeroTile>& mytile);
  uint64_t updatePatBits(std::shared_ptr<GanitaZeroTile>& mytile);
  uint64_t shrinkPatBits
  (GanitaBuffer *newgzi, std::shared_ptr<GanitaZeroTile>& mytile);
  uint64_t shrinkPatBits
  (GanitaBuffer *newgzi, std::shared_ptr<GanitaZeroTile>& mytile, 
   GanitaGraphList *mylist);
  double computeCondHist2(int h_len);
  double computeCondHistAll(int h_len);
  unsigned long dumpCondHist1(void);
  unsigned long dumpCondHistSep(void);
  string returnB64Encode(void);
  virtual int close(void);
  int delHist(void);
  uint64_t dumpHistHist(uint64_t len);
  uint64_t findTopFreq(void);
  int majorTileSelector(void);
  int maxTileSelector(void);
  int countInOutBits(void);
  int setVerbosity(int);
  int computeAutoCorr(int64_t len);
  int computeDFT(void);
  int slideChanges(uint64_t bn, GanitaGraphList *mylist);
  int computeAutoCorr2(uint64_t bn, GanitaGraphList *mylist);
  int buildStationarySeq(uint64_t ws);
  int buildStationarySeq3(uint64_t ws, int32_t nn, double *kmeans);
  int updateStationarySeq(double *domMap, uint64_t addV, uint64_t subV);
  int updateStationarySeq2(double *domMap, uint64_t addV, uint64_t subV);
  int computeDomMap(double *domMap, uint64_t *ss);
  int binSeq1(int32_t nmeans, double *kmeans);
};

#endif
