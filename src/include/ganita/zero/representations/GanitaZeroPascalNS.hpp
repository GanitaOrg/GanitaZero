#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <boost/shared_ptr.hpp>

class GanitaZeroPascalNS
{
private:
  std::string name;
  uint64_t bignum, smallnum, base;
  uint64_t input;
  vector<vector<uint64_t>> pnsCoefs;
  vector<uint64_t> curCoef;
public:
  GanitaZeroPascalNS(void);
  bool setAll(uint64_t bn, uint64_t sn, uint64_t ba, uint64_t num);
  uint64_t setBigNum(uint64_t num);
  uint64_t setSmallNum(uint64_t num);
  uint64_t setBase(uint64_t num);
  uint64_t printRep(void);
  uint64_t returnCoef(uint64_t index, uint64_t ii);
  uint64_t size(void);
  uint64_t binomial(uint64_t top, uint64_t bot);
  uint64_t computeCoef(uint64_t diff, uint64_t bn, uint64_t sn);
  uint64_t computeRep(uint64_t diff, uint64_t bn, uint64_t sn);
  uint64_t computeRep(void);
  uint64_t computeRep(uint64_t input_num);
  uint64_t compareTails(void);
};

