#include <string>
#include <vector>
#include <tuple>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
//#include <boost/shared_ptr.hpp>

class GanitaZeroFair
{
protected:
  std::string name;
  unsigned long dimension, dim, dim2;
  unsigned long max_iter;
  int verbose;
  GanitaZeroMat gzfm, gzfm2;
  // gzfm( mm, nn ) stores the position of player mm on day nn.
public:
  GanitaZeroFair(void);
  int init(unsigned long);
  int init2(unsigned long);
  int checkSquareWeak1(unsigned long xx, unsigned long yy);
  int checkSquareWeak2(unsigned long xx, unsigned long yy);
  int checkSquare1(unsigned long xx, unsigned long yy);
  int checkSquare2(unsigned long xx, unsigned long yy);
  int checkSquare3(unsigned long xx, unsigned long yy);
  int setSquare(unsigned long xx, unsigned long yy);
  int setSquare(unsigned long xx, unsigned long yy, double max);
  int backTrackSolve(void);
  int topBalanced(void);
  int fill2(int, int);
  int twoBalanced(void);
  int anyBalanced(void);
  int fillAny(int, int, int);
  int printMat(void);
  int checkMat(void);
  int checkMat2(void);
  int checkMat3(void);
  int squareProp(void);
  int addMachineSeq(void);
  int fieldMult(void);
};

