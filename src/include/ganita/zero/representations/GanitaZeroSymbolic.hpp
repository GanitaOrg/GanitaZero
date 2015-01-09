#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <boost/shared_ptr.hpp>

#ifndef ALPHABET_ALLOC_SIZE
#define ALPHABET_ALLOC_SIZE 256
#endif

#include "ganita/zero/GanitaZeroHist.hpp"

class GanitaZeroSymbolic
{
protected:
  std::string name;
  unsigned char *seq;   // For efficiency and memory considerations
  unsigned long seq_size;
  unsigned long alphabet_size;
  unsigned long alphabet_max;
  unsigned char alphabet[ALPHABET_ALLOC_SIZE];
  GanitaZeroHist my_hist;
public:
  GanitaZeroSymbolic(void);
  unsigned long loadCharSeq(std::ifstream &sym_file);
  int init(std::ifstream &sym_file);
  int shrinkAlphabet(void);
  int dumpAlphabet(void);
  int computeByteHist(void);
  unsigned long dumpHist(void);
  double computeCondEnt1FromScratch(void);
  unsigned long dumpCondHist1(void);
};

