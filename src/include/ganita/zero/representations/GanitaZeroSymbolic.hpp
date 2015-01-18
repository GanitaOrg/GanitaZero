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
#include "ganita/zero/GanitaBuffer.hpp"

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
public:
  GanitaZeroSymbolic(void);
  GanitaZeroSymbolic(std::ifstream &sym_file);
  int initBuffer(std::ifstream &sym_file);
  int initBuffer(void);
  unsigned long loadCharSeq(std::ifstream &sym_file);
  int init(std::ifstream &sym_file);
  int init(void);
  int init(char *input_file);
  int shrinkAlphabet(void);
  int dumpAlphabet(void);
  int computeByteHist(void);
  unsigned long dumpHist(void);
  double computeCondEnt1FromScratch(void);
  double computeCondEnt2FromScratch(void);
  unsigned long dumpCondHist1(void);
  string returnB64Encode(void);
};

