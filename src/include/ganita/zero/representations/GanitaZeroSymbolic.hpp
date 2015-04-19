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
#include "ganita/graph/GanitaBuffer.hpp"

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
  uint64_t loadDoubleDiffQuant(void);
  int init(std::ifstream &sym_file);
  int init(void);
  int init(char *input_file);
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
  double computeCondHist2(int h_len);
  double computeCondHistAll(int h_len);
  unsigned long dumpCondHist1(void);
  unsigned long dumpCondHistSep(void);
  string returnB64Encode(void);
  int close(void);
  uint64_t dumpHistHist(uint64_t len);
  uint64_t findTopFreq(void);
};

