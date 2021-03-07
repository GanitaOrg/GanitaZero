#include <string>
#include <vector>
#include <tuple>
#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/shared_ptr.hpp>
#include "ganita/graph/GanitaGraphFile.hpp"

class GanitaZeroTGF : public GanitaZeroNeural
{
private:
  std::string name;
  std::vector<int> actseq;
  std::vector<tuple<double, double>> node_output;
  std::vector<tuple<double, double, double, double>> node_input;
  std::vector<tuple<double, double, double, double>> tgf_weights;
public:
  GanitaZeroTGF(void);
  int createTGF(uint64_t numlayers);
  static const double tgf_generator_bkpts[4][6];
  static const double tgf_generator_fun[4][4][2];
  double callGenerator(int fn, double input);
  double callGeneratorSlope(int fn, double input);
  int generateRandActSeq(uint64_t len);
  int generateOrdActSeq(uint64_t len);
  int dumpActSeq(void);
  double composeTGF(double input);
  double convertToWeight(uint64_t val);
  uint64_t convertFromWeight(double ww);
  int setBinaryWeightsRand1(void);
  int setBinaryWeightsId(void);
  int setUniformWeights(void);
  int dumpWeights(void);
  int feedForwardTGF(double input);
  int writeNetwork(void);
  int copyInternalTGF(double input);
  int newCopyInternalTGF(GanitaZeroTGF *copy);
  double feedForwardInternalTGF(double input);
  int backPropTGF(double xx, double yy);
  int sgdTGF(uint64_t num);
  int sgdTGF2(uint64_t num, GanitaZeroTGF target);
  int sgdTGF3(uint64_t num, int gnum);
  int outputTestRandTGF(uint64_t num);
  int outputTestTGF(uint64_t num);
  int flipWeight(void);
};

