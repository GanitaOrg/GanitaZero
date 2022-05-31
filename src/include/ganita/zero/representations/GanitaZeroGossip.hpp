#include <string>
#include <vector>
#include <tuple>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <set>
#include <boost/shared_ptr.hpp>
#include "ganita/graph/GanitaGraphFile.hpp"

class GanitaZeroGossip
{
private:
  std::string name;
  std::vector<uint64_t> member;
  uint64_t gossip_total;
  uint64_t rounds;
public:
  GanitaZeroGossip(void);
  int run(uint64_t num, uint64_t trials);
  int run3(uint64_t num, uint64_t trials, uint64_t nbrs);
  int runEV(uint64_t num, uint64_t trials);
  int runAll(uint64_t num);
  int estProb(uint64_t num, uint64_t jj);
};

