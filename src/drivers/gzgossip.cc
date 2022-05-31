#include "ganita/zero/GanitaZero.hpp"
  
// Driver program for GanitaZero. 

int main(int argc, char *argv[])
{
  GanitaZero gzero(1);
  GanitaZeroGossip gossip;
  GanitaZeroVersion version;
  int ii;

  if(argc < 1){
    std::cout<<"Usage: input-file hist_len"<<std::endl;
    std::cout<<"Version: "<<version.returnVersion()<<endl;
    exit(1);
  }

  cout<<"members,rounds-1,rounds-2"<<endl;
  for(ii=4; ii<41; ii++){
    gossip.estProb((uint64_t) pow(2,ii)+1, 4); 
    //cout<<endl;
    //gossip.runEV((uint64_t) pow(2,ii)+1, 1000);
    //gossip.run3((uint64_t) pow(2,ii)+1, 1000, 1);
  }

  exit(1);
}

