#include "GanitaZero.hpp"
  
// driver program for GanitaZero. 

int main(int argc, char *argv[])
{
  GanitaZero gzero(1);

  if(argc < 2){
    std::cout<<"Usage: input-file"<<std::endl;
    exit(1);
  }

  gzero.readT(argv[1]);
}

