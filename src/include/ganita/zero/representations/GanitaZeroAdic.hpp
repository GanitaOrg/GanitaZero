#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/shared_ptr.hpp>
#include "ganita/graph/GanitaGraphStage.hpp"

class GanitaZeroAdic
{
private:
  std::string name;
  std::vector<std::string> type;
  std::vector<std::string> keys;
  std::vector< std::vector<std::string> > values;
  // Ganita Graph for Adic
  vector< std::shared_ptr<GanitaGraphStage> > stage;
  unsigned long num_fixed_levels;
  unsigned long repeat_pattern_length;
  char stage_delim;
  char node_delim;
  char edge_delim;
public:
  GanitaZeroAdic(void);
  unsigned long addStage(void);
  unsigned long addStage(unsigned long ii, unsigned long val);
  int readFixed(std::ifstream &gzt_file);
  int readRepeat(std::ifstream &gzt_file);
  unsigned long dumpStageSize(unsigned long ss);
  unsigned long dumpStage(unsigned long ss);
  GanitaGraphStage *returnStage(unsigned long ss);
  unsigned long returnNumStages(void);
};

