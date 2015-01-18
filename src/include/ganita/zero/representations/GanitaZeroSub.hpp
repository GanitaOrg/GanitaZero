#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <boost/shared_ptr.hpp>

class GanitaZeroSub : public GanitaZeroSymbolic
{
private:
  std::string name;
  unsigned long alloc_size;
public:
  GanitaZeroSub(void);
  unsigned long loadSub(std::ifstream &sym_file);
  unsigned long generateChacon3(unsigned long len);
};

