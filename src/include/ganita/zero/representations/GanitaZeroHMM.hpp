#ifndef _GANITAZEROHMM_
#define _GANITAZEROHMM_

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <random>
#include <chrono>
#include <cstdint>
#include <boost/shared_ptr.hpp>
#include "ganita/zero/GanitaZeroMat.hpp"

#ifndef __APPLE__
#include <bsd/stdlib.h>
#endif

// Maximum number of stages for substitution dynamical system. 
#ifndef MAX_NUM_SUB_STAGES
#define MAX_NUM_SUB_STAGES 1000
#endif

class GanitaZeroHMM : public GanitaZeroSymbolic
{
private:
  std::string name;
  unsigned long alloc_size;
  GanitaZeroMat gztm;
  GanitaZeroMat gzom;
public:
  GanitaZeroHMM(void);
  unsigned long loadHMM(std::ifstream &sym_file);
  unsigned long generateHMM_1(unsigned long len);
  uint64_t returnArc4Rand(char *myran, uint64_t len);
  double returnArc4RandUniform(void);
  uint64_t dumpRand(uint64_t len);
  using GanitaZeroSymbolic::close;
  int close(void);
};

#endif
