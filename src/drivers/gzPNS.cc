#include "ganita/zero/GanitaZero.hpp"
  
// Driver program for GanitaZeroPascalNS.

int main(int argc, char *argv[])
{
  GanitaZero gzero(1);
  GanitaZeroPascalNS gzpns1, gzpns2;
  GanitaZeroVersion version;
  uint64_t ii, jj;
  uint64_t ulimit;
  uint64_t vala, valb;

  if(argc < 5){
    std::cout<<"Usage: bignum smallnum base number"<<std::endl;
    std::cout<<"Version: "<<version.returnVersion()<<endl;
    exit(1);
  }

  gzpns1.setAll(atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),atoi(argv[4]));
  gzpns2.setAll(atoi(argv[1]),atoi(argv[2])+1,atoi(argv[3]),atoi(argv[4]));
  
  ulimit = gzpns1.binomial(atoi(argv[1]),atoi(argv[2]));
  for(ii=0; ii<=ulimit; ii++){
    gzpns1.computeRep(ii);
    vala = gzpns1.returnCoef(gzpns1.size()-1, 0);
    gzpns2.computeRep(ii);
    valb = gzpns2.returnCoef(gzpns2.size()-1, 0);
    cout<<"("<<ii<<","<<vala<<","<<valb<<"): ";
    for(jj=0; jj<gzpns1.size(); jj++){
      cout<<"("<<gzpns1.returnCoef(jj,0)<<" "<<gzpns1.returnCoef(jj,1)<<") ";
    }
    cout<<"| ";
    for(jj=0; jj<gzpns2.size(); jj++){
      cout<<"("<<gzpns2.returnCoef(jj,0)<<" "<<gzpns2.returnCoef(jj,1)<<") ";
    }
    cout<<std::endl;
  }
  
  //gzpns.compareTails();
  //gzpns.printRep();

  exit(1);
}

