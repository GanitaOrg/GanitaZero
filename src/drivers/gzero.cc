#include "ganita/zero/GanitaZero.hpp"
  
// Driver program for GanitaZero. 

int main(int argc, char *argv[])
{
  GanitaZero gzero(1);
  //GanitaZeroSub chacon;
  //int hist_len;
  GanitaZeroVersion version;
  //double max;
  //int max_ii, ii;
  //double entropy[32][2];

  // if(argc < 3){
  //   std::cout<<"Usage: input-file output-file"<<std::endl;
  //   exit(1);
  // }
  if(argc < 3){
    std::cout<<"Usage: input-file hist_len"<<std::endl;
    std::cout<<"Version: "<<version.returnVersion()<<endl;
    exit(1);
  }

  //sscanf(argv[2],"%d",&hist_len);

  //gzero.readT(argv[1]);
  //cout<<"Number of stages:        "<<gzero.returnNumStages()<<endl;
  //gzero.dumpStage(1);
  gzero.init(argv[1]);
  //gzero.loadCharSeq(argv[1]);
  //gzero.computeByteHist();

  //gzero.computeCondEntAll(25);

  gzero.tileSpace(25);

  //cout<<"Base64 encoding: "<<gzero.returnB64Encode()<<endl;
  gzero.close();
  //chacon.initOut(argv[2]);
  //chacon.generateChacon2(1000000);
  //chacon.close();
}

