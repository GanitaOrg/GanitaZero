#include "ganita/zero/GanitaZero.hpp"
  
// driver program for GanitaZero. 

int main(int argc, char *argv[])
{
  GanitaZero gzero(1);

  if(argc < 2){
    std::cout<<"Usage: input-file"<<std::endl;
    exit(1);
  }

  //gzero.readT(argv[1]);
  //cout<<"Number of stages:        "<<gzero.returnNumStages()<<endl;
  //gzero.dumpStage(1);
  gzero.init(argv[1]);
  //gzero.computeByteHist();
  //cout<<"Conditional entropy: "<<gzero.computeCondEnt1FromScratch()<<endl;
  cout<<"Conditional entropy: "<<gzero.computeCondEnt2FromScratch()<<endl;
}

