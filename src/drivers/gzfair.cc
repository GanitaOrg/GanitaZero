#include "ganita/zero/GanitaZero.hpp"
  
// Driver program for GanitaZero. 

int main(int argc, char *argv[])
{
  //GanitaZero gzero(1);
  unsigned long num;
  int rval;

  GanitaZeroFair gzfair;
  GanitaZeroVersion version;
  if(argc < 2){
    std::cout<<"Usage: gzfair dimension"<<std::endl;
    std::cout<<"Version: "<<version.returnVersion()<<endl;
    exit(1);
  }

  num = atoi(argv[1]);
  if(num < 2){
    num = 8;
  }
  //gzfair.init(num);
  gzfair.init2(num);
  // rval = gzfair.backTrackSolve();
  // if(rval < 1){
  // cout<<"No proportional sequence for n = "<<num<<endl;
  // }
  // gzfair.printMat();
  // gzfair.checkMat2();
  //cout<<"Proportional matrix squared"<<endl;
  //gzfair.squareProp();
  gzfair.vanDerCorput();
  //gzfair.topBalanced();
  //gzfair.printMat();
  //gzfair.anyBalanced();
  gzfair.printMat();
  if(gzfair.isLatinSquare()){
    gzfair.checkMat2();
  }
  else{
    cout<<"Not a Latin Square!"<<endl;
  }
}

