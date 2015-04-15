#include "ganita/zero/GanitaZero.hpp"
  
// driver program for GanitaZero. 

int main(int argc, char *argv[])
{
  GanitaZero gzero(1);
  //int ii;
  //double entropy[32][2];
  //double max;
  //int max_ii;
  GanitaZeroHist testHist;
  string testStr(" ");
  int bit_len;

  if(argc < 2){
    std::cout<<"Usage: bit_length"<<std::endl;
    exit(1);
  }
  // if(argc < 3){
  //   std::cout<<"Usage: input-file output-file"<<std::endl;
  //   exit(1);
  // }

  sscanf(argv[1],"%d",&bit_len);

  //gzero.readT(argv[1]);
  //cout<<"Number of stages:        "<<gzero.returnNumStages()<<endl;
  //gzero.dumpStage(1);
  //gzero.initD(argv[1]);
  //gzero.loadDoubleDiffQuant();
  //gzero.computeByteHist();

  // max = 0;
  // max_ii = 4;
  // entropy[4][0] = gzero.computeCondEnt1FromScratch(4);
  // for(ii=5; ii<25; ii++){
  //   entropy[ii][0] = gzero.computeCondEnt1FromScratch(ii);
  //   entropy[ii][1] = (entropy[ii-1][0] - entropy[ii][0]) / entropy[ii][0];
  //   fprintf(stdout, "Bits %2d C_Entropy %lf Reduction %lf\n", 
  // 	    ii, entropy[ii][0], entropy[ii][1]);
  //   if(entropy[ii][1] > max){
  //     max_ii = ii-1;
  //     max = entropy[ii][1];
  //   }
  //   else if(max_ii > 4){
  //     // Just look for first local max.
  //     break;
  //   }
  // }

  //fprintf(stdout, "Conditional bits: %d\n", max_ii);
  
  //gzero.computeCondEnt1FromScratch(max_ii);
  
  //gzero.dumpCondHistSep();
  //gzero.dumpCondHist();

  //cout<<"Poisson: "<<testHist.returnPoisson(1,2)<<endl;
  //testHist.returnArc4Rand(testStr);
  //fprintf(stdout, "Random bits: %02X\n", testStr.c_str()[0]);
  testHist.dumpRand(pow(2,bit_len)+1);
  //cout<<"Base64 encoding: "<<gzero.returnB64Encode()<<endl;
  //gzero.close();
}

