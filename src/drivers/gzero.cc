#include "ganita/zero/GanitaZero.hpp"
  
// Driver program for GanitaZero. 

int main(int argc, char *argv[])
{
  GanitaZero gzero(1);
  //GanitaZeroSub chacon;
  //int hist_len;
  GanitaZeroVersion version;
  GanitaZeroMat gzm(7,7);
  GanitaZeroMat gzprod(7,7);
  GanitaZeroMat gzmean(7,7);
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
  //gzero.build1(argv[1], 25, 4);

  for(int ii=0; ii<7; ii++){
   for(int jj=0; jj<7; jj++){
     gzm.set(ii,jj,0);
     //gzmean.set(ii,jj,0);
   }
  }
  gzm.set(0,2,1);
  gzm.set(1,0,.6666667);
  gzm.set(1,3,.3333333);
  gzm.set(2,1,.6666667);
  gzm.set(2,4,.3333333);
  gzm.set(3,2,.6666667);
  gzm.set(3,5,.3333333);
  gzm.set(4,3,.6666667);
  gzm.set(4,6,.3333333);
  gzm.set(5,4,1);
  gzm.set(6,5,1);
  for(int ii=0; ii<7; ii++){
   for(int jj=0; jj<7; jj++){
     gzprod.set(ii,jj,gzm.get(ii,jj));
     gzmean.set(ii,jj,gzm.get(ii,jj));
   }
  }
  //gzm.setEx1();
  //gzm.dumpMat();
  for(int ii=0; ii<9999; ii++){
     gzm.leftMultiply(gzprod);
     for(int jj=0; jj<7; jj++){
       for(int kk=0; kk<7; kk++){
         gzmean.set(jj,kk,gzmean.get(jj,kk)+gzprod.get(jj,kk));
       }
     }
  }
  for(int jj=0; jj<7; jj++){
    for(int kk=0; kk<7; kk++){
      gzmean.set(jj,kk,gzmean.get(jj,kk)/10000.00);
    }
  }
  cout<<"Number of iterations = 9999"<<endl;
  cout<<"-----------"<<endl;
  gzmean.dumpMat();
  cout<<"-----------"<<endl;
  //gzhmm.initOut(argv[2]);
  //gzhmm.generateHMM_1(1000000);
  //gzhmm.close();
  //gzm.close();
  //cout<<"Base64 encoding: "<<gzero.returnB64Encode()<<endl;
  //gzero.close();
  //chacon.initOut(argv[2]);
  //chacon.generateChacon3(100000000);
  //chacon.close();
}

