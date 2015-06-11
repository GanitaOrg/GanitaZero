#include "ganita/zero/GanitaZeroMat.hpp"

GanitaZeroMat::GanitaZeroMat(void)
{
  gzmat = 0;
  ncols = 0;
  nrows = 0;
}

GanitaZeroMat::GanitaZeroMat(unsigned long nn, unsigned long mm)
{
  unsigned long ii;
  ncols = nn; nrows = mm;
  gzmat = new double *[nn] ;
  //memory allocated for elements of each column.
  
  for( ii = 0 ; ii < nn; ii++ )
    gzmat[ii] = new double[mm]();
}

int GanitaZeroMat::close(void)
{
  //free the allocated memory
  
  for( unsigned long i = 0 ; i < ncols ; i++ )
    delete [] gzmat[i] ;
  delete [] gzmat ;
  return(1);
}

unsigned long GanitaZeroMat::returnNRows(void)
{
  return(nrows);
}

unsigned long GanitaZeroMat::returnNCols(void)
{
  return(ncols);
}

int GanitaZeroMat::set(unsigned long cc, unsigned long rr, double value)
{
  if(cc >= ncols) cc = cc % ncols;
  if(rr >= nrows) rr = rr % nrows;
  gzmat[cc][rr] = value;
  return(1);
}

double GanitaZeroMat::get(unsigned long cc, unsigned long rr)
{
  if(cc >= ncols) cc = cc % ncols;
  if(rr >= nrows) rr = rr % nrows;
  return(gzmat[cc][rr]);
}

int GanitaZeroMat::leftMultiply(GanitaZeroMat mymat)
{
  if(ncols != mymat.returnNRows()){
    // cannot multiply
    return(-1);
  }
  unsigned long ii, jj, kk;
  unsigned long nc;
  double sum;
  GanitaZeroMat acol(1,nrows);
  nc = mymat.returnNCols();
  for(jj=0; jj<nc; jj++){
    for(ii=0; ii<nrows; ii++){
      sum = 0;
      for(kk=0; kk<ncols; kk++){
	sum += gzmat[kk][ii] * mymat.get(jj,kk);
      }
      acol.set(0,ii,sum);
    }
    for(ii=0; ii<nrows; ii++){
      mymat.set(jj,ii,acol.get(0,ii));
    }
  }
  acol.close();
  return(1);
}

int GanitaZeroMat::leftMultiply(GanitaZeroMat mymat, GanitaZeroMat result)
{
  if(ncols != mymat.returnNRows()){
    // cannot multiply
    return(-1);
  }
  unsigned long ii, jj, kk;
  unsigned long nc;
  double sum;
  nc = mymat.returnNCols();
  //result.init(nrows,mymat.returnNCols());
  for(jj=0; jj<nc; jj++){
    for(ii=0; ii<nrows; ii++){
      sum = 0;
      for(kk=0; kk<ncols; kk++){
	sum += gzmat[kk][ii] * mymat.get(jj,kk);
      }
      result.set(jj,ii,sum);
    }
  }
  return(1);
}

int GanitaZeroMat::square(void)
{
  if(ncols != nrows){
    // cannot multiply
    return(-1);
  }
  unsigned long ii, jj, kk;
  double sum;
  GanitaZeroMat tmat(ncols,nrows);
  for(jj=0; jj<ncols; jj++){
    for(ii=0; ii<nrows; ii++){
      sum = 0;
      for(kk=0; kk<ncols; kk++){
	sum += gzmat[kk][ii] * gzmat[jj][kk];
      }
      tmat.set(jj,ii,sum);
    }
  }
  for(jj=0; jj<ncols; jj++){
    for(ii=0; ii<nrows; ii++){
      gzmat[jj][ii] = tmat.get(jj,ii);
    }
  }
  tmat.close();

  return(1);
}

int GanitaZeroMat::dumpMat(void)
{
  unsigned long ii, jj;
  for(ii=0; ii<nrows; ii++){
      for(jj=0; jj<ncols; jj++){
	fprintf(stdout, "%lf ", gzmat[jj][ii]);
      }
      fprintf(stdout, "\n");
  }
  return(1);
}

int GanitaZeroMat::setEx1(void)
{
  // gzmat[1][0] = 1;
  // gzmat[2][1] = 1;
  // gzmat[0][2] = .33;
  // gzmat[1][2] = .33;
  // gzmat[2][2] = .34;
  gzmat[0][1] = 1;
  gzmat[1][2] = 1;
  gzmat[2][0] = .33;
  gzmat[2][1] = .33;
  gzmat[2][2] = .34;
  return(1);
}

int GanitaZeroMat::init(unsigned long nn, unsigned long mm)
{
  ncols = nn; nrows = mm;
  gzmat = new double *[nn] ;
  //memory allocated for elements of each column.
  
  for( unsigned long i = 0 ; i < nn; i++ )
    gzmat[i] = new double[mm]();

  return(1);
}

int GanitaZeroMat::setUnit(unsigned long ii)
{
  unsigned long jj, kk;
  if(ii >= nrows){
    ii = ii % nrows;
  }
  for(jj=0; jj<ncols; jj++){
    for(kk=0; kk<nrows; kk++){
      gzmat[jj][kk] = 0;
    }
  }
  gzmat[0][ii] = 1;
  return(1);
}

