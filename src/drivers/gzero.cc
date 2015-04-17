#include "ganita/zero/GanitaZero.hpp"
  
// driver program for GanitaZero. 

int main(int argc, char *argv[])
{
  GanitaZero gzero(1);
  GanitaZeroSub chacon;
  int hist_len;
  GanitaZeroVersion version;
  double max;
  int max_ii, ii;
  double entropy[32][2];

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

  max = 0;
  max_ii = 4;
  entropy[4][0] = gzero.computeCondEnt2FromScratch(4);
  for(ii=5; ii<25; ii++){
    entropy[ii][0] = gzero.computeCondEnt2FromScratch(ii);
    entropy[ii][1] = (entropy[ii-1][0] - entropy[ii][0]) / entropy[ii][0];
    fprintf(stdout, "Bits %2d C_Entropy %lf Reduction %lf\n", 
  	    ii, entropy[ii][0], entropy[ii][1]);
    //fprintf(stdout, "Max entropy = %lf, max ii = %d\n", max, max_ii);
    if(entropy[ii][1] > max){
      max_ii = ii-1;
      max = entropy[ii][1];
    }
    else if(max_ii > 4){
      // Just look for first local max.
      break;
    }
  }

  hist_len = max_ii - 5;
  cout<<"Conditional Histogram: "<<gzero.computeCondHist2(hist_len-1);
  cout<<" "<<hist_len-1<<endl;
  gzero.dumpHistHist(1000000);
  //cout<<"Base64 encoding: "<<gzero.returnB64Encode()<<endl;
  gzero.close();
  chacon.initOut(argv[2]);
  chacon.generateChacon3(1000000);
  chacon.close();
}

