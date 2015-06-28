#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/shared_ptr.hpp>
#include "ganita/graph/GanitaGraphFile.hpp"
#include "ganita/zero/GanitaZeroTile.hpp"

class GanitaZeroAdic
{
private:
  std::string name;
  std::vector<std::string> type;
  std::vector<std::string> keys;
  std::vector< std::vector<std::string> > values;
  // Ganita Graph for Adic
  vector< std::shared_ptr<GanitaGraph> > stage;
  uint64_t num_fixed_levels;
  uint64_t repeat_pattern_length;
  char stage_delim;
  char node_delim;
  char edge_delim;
public:
  GanitaZeroAdic(void);
  uint64_t addStage(void);
  uint64_t addStage(uint64_t ii, uint64_t val);
  int readFixed(std::ifstream &gzt_file);
  int readRepeat(std::ifstream &gzt_file);
  uint64_t dumpStageSize(uint64_t ss);
  uint64_t dumpStage(uint64_t ss);
  GanitaGraph *returnStage(uint64_t ss);
  uint64_t returnNumStages(void);
  int buildBase(void);
  int extendStage(GanitaZeroTile *mytile);
};

