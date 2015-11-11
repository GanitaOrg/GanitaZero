// GanitaZeroHist is used to compute and store histograms. 
// We give a description of the following method:
// int computeCondHistAll(GanitaBuffer *input);
// This stores the values for multiple histograms 
// at the same time in hist. 
// Each histogram is stored in hist at a different offset. 
// The offset is count2. 
// The value range below gives the range of histogram 
// for each given offset. 
// count1   0 |      1 |        2 |          3 |            4 ...
// count2   0 |      2 |        6 |         14 |           30 ...
// range  0,1 | 00->11 | 000->111 | 0000->1111 | 00000->11111 ...

#ifndef _GANITAZEROHIST_
#define _GANITAZEROHIST_

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <bitset>
#include <cmath>
#include <boost/shared_ptr.hpp>

#ifndef __APPLE__
#include <bsd/stdlib.h>
#endif

#include "ganita/graph/GanitaBuffer.hpp"
#include "ganita/zero/GanitaZeroTile.hpp"

#ifndef DEFAULT_HIST_SIZE
#define DEFAULT_HIST_SIZE 256
#endif

#ifndef GANITA_MAX_COND_BITS
#define GANITA_MAX_COND_BITS 32
#endif

using namespace std;

class GanitaZeroHist
{
private:
  uint64_t *hist;       // histogram
  uint64_t hist_length; // histogram length
  int condition_num;         // number of bits to condition on
  int est_num;               // number of bits to estimate probability on
  uint64_t max_count;
  uint64_t max_index;
  uint64_t divisor;
  vector<double> stat;
  uint64_t longone;
public:
  GanitaZeroHist(void);
  GanitaZeroHist(unsigned long size);
  unsigned long init(unsigned long size);
  unsigned long initConditional(void);
  unsigned long initConditional(int h_len);
  int computeByteHist(unsigned char *ptr, unsigned long ss);
  int computeByteHist(GanitaBuffer *input);
  int computeByteHist(GanitaBuffer *input, uint64_t len);
  int computeCondHist1(unsigned char *ptr, unsigned long ss);
  int computeCondHist1(GanitaBuffer *input);
  int computeCondHist2(GanitaBuffer *input);
  int computeCondHistAll(GanitaBuffer *input);
  int computeCondHistNested(GanitaBuffer *input);
  double computeCondEntAll(void);
  double computeCondEntAll2(void);
  double computeCondEntAll3(void);
  GanitaZeroTile getBestTile1(void);
  int getBestSize(void);
  int getBestTiles(int bpL, vector< std::shared_ptr<GanitaZeroTile> > tile);
  int bestPatLen1(void);
  int bestPatLen2(void);
  double computeCondEnt1(void);
  unsigned long dumpHist(void);
  unsigned long dumpCondHist1(void);
  unsigned long dumpCondHistSep(void);
  uint64_t findMaxCondHist(void);
  uint64_t findMaxCondHist(int hist_ii);
  int findTopCondHist(int hist_ii, vector< std::shared_ptr<GanitaZeroTile> > mytiles);
  uint64_t findTopIndices(double ratio);
  double returnPoisson(double lambda, uint64_t k);
  uint64_t returnArc4Rand(char *myran, uint64_t len);
  uint64_t dumpRand(uint64_t len);
  uint64_t dumpHistHist(uint64_t len);
  uint64_t findTopFreq(uint64_t fsize);
  int computeAutoCorr(int64_t len, GanitaBuffer *input);
  int computeDFT(GanitaBuffer *input);
  int close(void);
  int radonNikodym(uint64_t *targetHist, uint64_t *der);
  int add(uint64_t val);
  int subtract(uint64_t val);
  uint64_t returnValue(uint64_t xx);
  int computeCD(uint64_t *cd);
  int32_t byteKMeansIter(uint64_t len, double *mm);
  int32_t byteKMeans(uint64_t len, double *kmeans, uint64_t iter);
};

// unneeded
// uint64_t gh_mask[56] = {
//   0x1, 0x3, 0x7, 0xf, 0x1f, 0x3f, 0x7f, 0xff,
//   0x1ff, 0x3ff, 0x7ff, 0xfff, 0x1fff, 0x3fff, 0x7fff, 0xffff,
//   0x1ffff, 0x3ffff, 0x7ffff, 0xfffff, 0x1fffff, 0x3fffff, 0x7fffff, 0xffffff,
//   0x1ffffff, 0x3ffffff, 0x7ffffff, 0xfffffff, 
//   0x1fffffff, 0x3fffffff, 0x7fffffff, 0xffffffff,
//   0x1ffffffff, 0x3ffffffff, 0x7ffffffff, 0xfffffffff, 
//   0x1fffffffff, 0x3fffffffff, 0x7fffffffff, 0xffffffffff,
//   0x1ffffffffff, 0x3ffffffffff, 0x7ffffffffff, 0xfffffffffff, 
//   0x1fffffffffff, 0x3fffffffffff, 0x7fffffffffff, 0xffffffffffff,
//   0x1ffffffffffff, 0x3ffffffffffff, 0x7ffffffffffff, 0xfffffffffffff,
//   0x1fffffffffffff, 0x3fffffffffffff, 0x7fffffffffffff, 0xffffffffffffff
// };

#endif
