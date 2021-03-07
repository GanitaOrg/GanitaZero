#include <string>
#include <vector>
#include <tuple>
#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/shared_ptr.hpp>
#include "ganita/graph/GanitaGraphFile.hpp"
#include "ganita/zero/GanitaZeroTile.hpp"

class GanitaZeroNeural
{
protected:
  std::string name;
  std::vector<std::string> type;
  std::vector<std::string> keys;
  std::vector< std::vector<std::string> > values;
  // Ganita Graph for Neural
  vector< std::shared_ptr<GanitaGraph> > layer;
  uint64_t num_layers;
  uint64_t repeat_pattern_length;
  char layer_delim;
  char node_delim;
  char edge_delim;
  uint64_t weight_denom;
public:
  GanitaZeroNeural(void);
  uint64_t addLayer(void);
  uint64_t addLayer(uint64_t ii, uint64_t val);
  int readFixed(std::ifstream &gzt_file);
  int readRepeat(std::ifstream &gzt_file);
  uint64_t dumpLayerSize(uint64_t ss);
  uint64_t dumpLayer(uint64_t ss);
  GanitaGraph *returnLayer(uint64_t ss);
  uint64_t returnNumLayers(void);
  int buildBase(void);
  int extendLayer(GanitaZeroTile *mytile);
};

