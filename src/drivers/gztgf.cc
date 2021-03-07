#include "ganita/zero/GanitaZero.hpp"
  
// Driver program for GanitaZero. 

int main(int argc, char *argv[])
{
  GanitaZero gzero(1);
  GanitaZeroTGF tgf, tgf_test;
  GanitaZeroVersion version;
  if(argc < 1){
    std::cout<<"Usage: input-file hist_len"<<std::endl;
    std::cout<<"Version: "<<version.returnVersion()<<endl;
    exit(1);
  }

  tgf.createTGF(4);
  //tgf.newCopyInternalTGF(&tgf_test);
  //tgf_test.flipWeight();
  tgf.sgdTGF(10000);
  tgf.outputTestTGF(9);
  //tgf_test.outputTestTGF(9);
}

