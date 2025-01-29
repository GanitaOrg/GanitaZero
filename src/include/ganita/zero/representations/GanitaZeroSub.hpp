#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
//#include <boost/shared_ptr.hpp>

// Maximum number of stages for substitution dynamical system. 
#ifndef MAX_NUM_SUB_STAGES
#define MAX_NUM_SUB_STAGES 1000
#endif

class GanitaZeroSub : public GanitaZeroSymbolic
{
private:
  std::string name;
  unsigned long alloc_size;
public:
  GanitaZeroSub(void);
  unsigned long loadSub(std::ifstream &sym_file);
  unsigned long generateChacon3(unsigned long len);
  unsigned long generateChacon2(unsigned long len);
};

