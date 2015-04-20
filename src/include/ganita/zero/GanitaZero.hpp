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

#include "ganita/zero/GanitaZeroSchema.hpp"
#include "ganita/zero/GanitaZeroRepresentations.hpp"
#include "ganita/zero/GanitaZeroVersion.hpp"

class GanitaZero
{
private:
  GanitaZeroAdic gAdic;
  GanitaZeroSymbolic gSym;
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
  unsigned long dumpStageSize(unsigned long ss);
  unsigned long dumpStage(unsigned long ss);
  unsigned long returnNumStages(void);
  unsigned long loadCharSeq(ifstream &sym_file);
  int init(ifstream &sym_file);
  unsigned long loadCharSeq(char *input_seq);
  int init(char *input_seq);
  int initD(char *input_seq);
  uint64_t loadDoubleDiffQuant(void);
  int init(char *input_seq, char *out_file);
  unsigned long computeByteHist(void);
  double computeCondEnt1FromScratch(int h_len);
  double computeCondEnt2FromScratch(int h_len);
  double computeCondEntAll(int h_len);
  int tileSpace(int h_len);
  double computeCondHist2(int h_len);
  double computeCondHistAll(int h_len);
  string returnB64Encode(void);
  int close(void);
  int dumpCondHist(void);
  int dumpCondHistSep(void);
  uint64_t dumpHistHist(uint64_t len);
  uint64_t findTopFreq(void);
};

