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
  //double *ent;               // histogram
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
  double computeCondEnt1(void);
  unsigned long dumpHist(void);
  unsigned long dumpCondHist1(void);
  unsigned long dumpCondHistSep(void);
  uint64_t findMaxCondHist(void);
  uint64_t findTopIndices(double ratio);
  double returnPoisson(double lambda, uint64_t k);
  uint64_t returnArc4Rand(char *myran, uint64_t len);
  uint64_t dumpRand(uint64_t len);
  uint64_t dumpHistHist(uint64_t len);
};

