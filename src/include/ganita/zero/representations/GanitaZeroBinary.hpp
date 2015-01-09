#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <boost/shared_ptr.hpp>

//#include "ganita/zero/representations/GanitaZeroSymbolic.hpp"

class GanitaZeroBinary : public GanitaZeroSymbolic
{
private:
  std::string name;
  unsigned long alloc_size;
public:
  GanitaZeroBinary(void);
  unsigned long loadBinary(std::ifstream &sym_file);
};

