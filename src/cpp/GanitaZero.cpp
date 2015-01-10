// This file contains the methods for GanitaZero. 
// Initially we only read and process adic representation. 
// Adic reading and processing included here. 
// Eventually, we may incorporate factory classes 
// to load various representations. 

#include "ganita/zero/GanitaZero.hpp"

// Constructors
GanitaZero::GanitaZero(void)
{
  verbosity = 0;
}

GanitaZero::GanitaZero(int vv)
{
  verbosity = vv;
}

int GanitaZero::readTHeader(ifstream &gzt_file)
{
  GanitaZeroSchema gzs;
  std::string line;

  if(!std::getline(gzt_file,line)){
    return(-1);
  }
  if(line != gzs.returnSchema(0)){
    std::cout<<"Header error!"<<std::endl;
    return(-1);
  }
  if(!std::getline(gzt_file,line)){
    return(-1);
  }
  if(line != gzs.returnSchema(1)){
    std::cout<<"Header error!"<<std::endl;
    return(-1);
  }
  // Read in transformation name.
  if(!std::getline(gzt_file,name)){
    return(-1);
  }
  // Read in the representation. 
  if(!std::getline(gzt_file,representation)){
    return(-1);
  }
  // Read in secondary name. Not stored currently. 
  if(!std::getline(gzt_file,line)){
    return(-1);
  }
  // Read in type. 
  if(!std::getline(gzt_file,type)){
    return(-1);
  }

  return(1);
}

int GanitaZero::readAdic(ifstream &gzt_file)
{
  if(gAdic.readFixed(gzt_file) >= 0){
     return(gAdic.readRepeat(gzt_file));
  }
  
  return(-1);
}

int GanitaZero::readT(char *input_tran)
{
  // Read in the input transformation. 
  std::ifstream gzt_file(input_tran);
  if (!gzt_file.is_open()){
    std::cout<<"Unable to open input file: "<<input_tran<<std::endl;
    return(-1);
  }

  if(readTHeader(gzt_file) < 0){
    gzt_file.close();
    return(-1);
  }

  if(representation != "adic"){
    cout<<"Only able to read adic representation."<<endl;
    gzt_file.close();
    return(-1);
  }

  readAdic(gzt_file);

  if(verbosity > 0){
    dumpTHeader();
  }

  gzt_file.close();
  return(1);
}

int GanitaZero::dumpTHeader(void)
{
  GanitaZeroVersion gzv;
  fprintf(stdout, "GanitaZero version \t %s\n", gzv.returnVersion().c_str());
  fprintf(stdout, "Transformation name\t %s\n", name.c_str());
  fprintf(stdout, "Representation     \t %s\n", representation.c_str());
  fprintf(stdout, "Transformation type\t %s\n", type.c_str());

  return (1);
}

unsigned long GanitaZero::dumpStageSize(unsigned long ss)
{
  return(gAdic.dumpStageSize(ss));
}

unsigned long GanitaZero::dumpStage(unsigned long ss)
{
  return(gAdic.dumpStage(ss));
}

unsigned long GanitaZero::returnNumStages(void)
{
  return(gAdic.returnNumStages());
}

unsigned long GanitaZero::loadCharSeq(ifstream &sym_file)
{
  return(gSym.loadCharSeq(sym_file));
}

int GanitaZero::init(ifstream &sym_file)
{
  return(gSym.init(sym_file));
}

unsigned long GanitaZero::loadCharSeq(char *input_seq)
{
  // Read in the input transformation. 
  unsigned long tmp;
  std::ifstream sym_file(input_seq,std::ifstream::binary);
  if (!sym_file.is_open()){
    std::cout<<"Unable to open input file: "<<input_seq<<std::endl;
    return(0);
  }

  tmp = loadCharSeq(sym_file);
  sym_file.close();
  return(tmp);
}

int GanitaZero::init(char *input_seq)
{
  // Read in the input transformation. 
  unsigned long tmp;
  //std::ifstream sym_file(input_seq,std::ifstream::binary);
  // std::ifstream sym_file(input_seq);
  // if (!sym_file.is_open()){
  //   std::cout<<"Unable to open input file: "<<input_seq<<std::endl;
  //   return(0);
  // }
  
  tmp = gSym.init(input_seq);
  if(verbosity){
    gSym.dumpAlphabet();
  }

  return(tmp);
}

unsigned long GanitaZero::computeByteHist(void)
{
  gSym.computeByteHist();
  return(gSym.dumpHist());
}

double GanitaZero::computeCondEnt1FromScratch(void)
{
  double ent = gSym.computeCondEnt1FromScratch();
  gSym.dumpCondHist1();
  return(ent);
}

double GanitaZero::computeCondEnt2FromScratch(void)
{
  double ent = gSym.computeCondEnt2FromScratch();
  gSym.dumpCondHist1();
  return(ent);
}

