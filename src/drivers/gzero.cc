#include "ganita/zero/GanitaZero.hpp"
  
// Driver program for GanitaZero. 

int main(int argc, char *argv[])
{
  GanitaZero gzero(1);
  GanitaZeroSub chacon;
  //int hist_len;
  GanitaZeroVersion version;
  //GanitaZeroMat gzm(3,3);
  //GanitaZeroHMM gzhmm;
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
  //gzero.init(argv[1]);
  //gzero.loadCharSeq(argv[1]);
  //gzero.computeByteHist();

  //gzero.computeCondEntAll(25);

  //gzero.tileSpace(25);
  gzero.build1(argv[1], 25, 4);

  //gzm.setEx1();
  //gzm.dumpMat();
  //for(int ii=0; ii<10; ii++){
    //gzm.square();
  //}
  //cout<<"-----------"<<endl;
  //gzm.dumpMat();
  cout<<"-----------"<<endl;
  //gzhmm.initOut(argv[2]);
  //gzhmm.generateHMM_1(1000000);
  //gzhmm.close();
  //gzm.close();
  //cout<<"Base64 encoding: "<<gzero.returnB64Encode()<<endl;
  //gzero.close();
  chacon.initOut(argv[2]);
  chacon.generateChacon3(100000000);
  chacon.close();
}

