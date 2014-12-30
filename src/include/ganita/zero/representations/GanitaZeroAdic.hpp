#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "ganita/graph/GanitaGraph.hpp"

class GanitaZeroAdic
{
private:
  std::string name;
  std::vector<std::string> type;
  std::vector<std::string> keys;
  std::vector< std::vector<std::string> > values;
  // Ganita Graph for Adic
  GanitaGraph *agg = new GanitaGraph();
public:
  GanitaZeroAdic(void);
  int read(std::ifstream &gzt_file);
};

