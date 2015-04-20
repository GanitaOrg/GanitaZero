// GanitaZeroHist is used to compute and store histograms. 
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <bitset>
#include <cmath>

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
  unsigned long *hist;       // histogram
  unsigned long hist_length; // histogram length
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
  int computeCondHist1(unsigned char *ptr, unsigned long ss);
  int computeCondHist1(GanitaBuffer *input);
  int computeCondHist2(GanitaBuffer *input);
  int computeCondHistAll(GanitaBuffer *input);
  double computeCondEntAll(void);
  GanitaZeroTile getBestTile1(void);
  int bestPatLen1(void);
  double computeCondEnt1(void);
  unsigned long dumpHist(void);
  unsigned long dumpCondHist1(void);
  unsigned long dumpCondHistSep(void);
  uint64_t findMaxCondHist(void);
  uint64_t findMaxCondHist(int hist_ii);
  uint64_t findTopIndices(double ratio);
  double returnPoisson(double lambda, uint64_t k);
  uint64_t returnArc4Rand(char *myran, uint64_t len);
  uint64_t dumpRand(uint64_t len);
  uint64_t dumpHistHist(uint64_t len);
  uint64_t findTopFreq(uint64_t fsize);
};

// Unneeded
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
