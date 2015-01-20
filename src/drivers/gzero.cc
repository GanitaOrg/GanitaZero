#include "ganita/zero/GanitaZero.hpp"
  
// driver program for GanitaZero. 

int main(int argc, char *argv[])
{
  GanitaZero gzero(1);
  GanitaZeroSub chacon3;

  if(argc < 3){
    std::cout<<"Usage: input-file output-file"<<std::endl;
    exit(1);
  }

  //gzero.readT(argv[1]);
  //cout<<"Number of stages:        "<<gzero.returnNumStages()<<endl;
  //gzero.dumpStage(1);
  gzero.init(argv[1]);
  //gzero.computeByteHist();
  cout<<"Conditional entropy: "<<gzero.computeCondEnt2FromScratch(13)<<endl;
  //cout<<"Base64 encoding: "<<gzero.returnB64Encode()<<endl;
  gzero.close();
  chacon3.initOut(argv[2]);
  chacon3.generateChacon3(1000000);
  chacon3.close();
}

