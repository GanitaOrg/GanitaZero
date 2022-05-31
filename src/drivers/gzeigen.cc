#include "ganita/zero/GanitaZero.hpp"
#include <Eigen/Dense>
  
// Driver program for GanitaZero. 
// g++ -I /home/terry/projects/src/linear-algebra/eigen/ gzeigen.cc -o gzeigen

using Eigen::MatrixXd;
 
int main(int argc, char *argv[])
{
  GanitaZeroMat radon_mat;
  uint64_t ii, jj;
  double denom, pp;
  uint64_t dim;
  dim = 10;

  if(argc < 2){
    cout<<"gzeigen matrix-file"<<endl;
    exit(1);
  }
  std::ifstream mat_file(argv[1]);
  radon_mat.read(mat_file);
  radon_mat.dumpMat();

  Eigen::MatrixXf A(dim+1,dim+1);
  Eigen::VectorXf b(dim+1);
  for(ii=0; ii<dim+1; ii++){
    for(jj=0; jj<dim+1; jj++){
      A(ii,jj)=radon_mat.get(jj,ii);
    }
  }
  for(ii=0; ii<dim+1; ii++){
    b(ii) = -1*radon_mat.get(dim+1,ii);
  }
  std::cout << "Here is the matrix A:\n" << A << std::endl;
  std::cout << "Here is the vector b:\n" << b << std::endl;
  Eigen::VectorXf x = A.colPivHouseholderQr().solve(b);
  std::cout << "The solution is:\n" << x << std::endl;

  denom = 0;
  for(ii=0; ii<dim+1; ii++){
    if(x(ii) > 0){
      denom += x(ii);
    }
  }
  denom += 1;
  cout<<"Radon point:"<<endl;
  for(ii=0; ii<dim; ii++){
    pp = 0;
    for(jj=0; jj<dim+1; jj++){
      if(x(jj) > 0){
	pp += x(jj)*A(ii,jj)/denom;
      }
    }
    pp += -1*b(ii)/denom;
    cout<<pp<<" ";
  }
  cout<<endl;

  exit(1);
}

