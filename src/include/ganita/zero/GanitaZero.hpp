// This class implements algorithms that use Adic transformations. 
// It uses GanitaGraph to implement the graphical representation 
// of an Adic transformation. 
// Adic transformations are representations of measure preserving 
// transformations on Lebesgue spaces. 
// Any (standard) e.m.p.t. can be represented as an Adic transformation. 
// Adic transformations are defined on Bratteli diagrams 
// from the field of operator algebras. 
// Every stationary ergodic transformation can be represented 
// as an Adic transformation, including Markov, hmm and pgm. 

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>

#include "ganita/graph/GanitaGraph.hpp"
#include "ganita/zero/GanitaZeroSchema.hpp"
#include "ganita/zero/GanitaZeroRepresentations.hpp"
#include "ganita/zero/GanitaZeroVersion.hpp"

class GanitaZero
{
private:
  std::vector<GanitaGraph> gAdic;
  std::string name;
  std::string representation;
  std::string type;
  int verbosity;
public:
  GanitaZero(void);
  GanitaZero(int vv);                   // set verbosity=vv
  int readTHeader(ifstream &gzt_file);
  int readT(char *input_tran);
  int dumpTHeader(void);
  int readAdic(ifstream &gzt_file);
};

