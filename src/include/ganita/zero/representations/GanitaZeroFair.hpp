#include <string>
#include <vector>
#include <tuple>
#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/shared_ptr.hpp>

class GanitaZeroFair
{
protected:
  std::string name;
  unsigned long dimension, dim;
  unsigned long max_iter;
  int verbose;
  GanitaZeroMat gzfm;
public:
  GanitaZeroFair(void);
  int init(unsigned long);
  int checkSquare1(unsigned long xx, unsigned long yy);
  int checkSquare2(unsigned long xx, unsigned long yy);
  int checkSquare3(unsigned long xx, unsigned long yy);
  int setSquare(unsigned long xx, unsigned long yy);
  int setSquare(unsigned long xx, unsigned long yy, double max);
  int backTrackSolve(void);
  int printMat(void);
  int checkMat(void);
};

