#ifndef _GANITAZEROMAT_
#define _GANITAZEROMAT_

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <bitset>
#include <cmath>
#include <string>
#include <iostream>
#include <vector>
//#include <boost/shared_ptr.hpp>

#ifndef __APPLE__
#include <bsd/stdlib.h>
#endif

using namespace std;

class GanitaZeroMat
{
private:
  unsigned long ncols;
  unsigned long nrows;
  double **gzmat;
public:
  GanitaZeroMat(void);
  GanitaZeroMat(unsigned long nn, unsigned long mm);
  int close(void);
  unsigned long returnNCols(void);
  unsigned long returnNRows(void);
  int set(unsigned long row, unsigned long column, double value);
  double get(unsigned long row, unsigned long column);
  int leftMultiply(GanitaZeroMat mymat);
  int leftMultiply(GanitaZeroMat mymat, GanitaZeroMat result);
  int square(void);
  int dumpMat(void);
  int setEx1(void);
  int init(unsigned long nn, unsigned long mm);
  int setUnit(unsigned long ii);
  int read(std::ifstream &gzt_file);
};

#endif
