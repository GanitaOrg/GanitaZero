#include "ganita/zero/GanitaZero.hpp"
  
// Driver program for GanitaZero. 

int main(int argc, char *argv[])
{
  GanitaZero gzero(1);
  //GanitaZeroSub chacon;
  //int hist_len;
  GanitaZeroVersion version;
  if(argc < 3){
    std::cout<<"Usage: input-file output-file"<<std::endl;
    std::cout<<"Version: "<<version.returnVersion()<<endl;
    exit(1);
  }

  gzero.s1(argv[1], argv[2]);
}

