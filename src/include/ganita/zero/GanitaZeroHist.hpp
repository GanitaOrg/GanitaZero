// GanitaZeroHist is used to compute and store histograms. 
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <bitset>

#include "ganita/zero/GanitaBuffer.hpp"

#ifndef DEFAULT_HIST_SIZE
#define DEFAULT_HIST_SIZE 256
#endif

using namespace std;

class GanitaZeroHist
{
private:
  unsigned long *hist;       // histogram
  unsigned long hist_length; // histogram length
  int condition_num;         // number of bits to condition on
  int est_num;               // number of bits to estimate probability on
  //double *ent;               // histogram
public:
  GanitaZeroHist(void);
  GanitaZeroHist(unsigned long size);
  unsigned long init(unsigned long size);
  unsigned long initConditional(void);
  int computeByteHist(unsigned char *ptr, unsigned long ss);
  int computeCondHist1(unsigned char *ptr, unsigned long ss);
  int computeCondHist1(GanitaBuffer *input);
  double computeCondEnt1(void);
  unsigned long dumpHist(void);
  unsigned long dumpCondHist1(void);
};

