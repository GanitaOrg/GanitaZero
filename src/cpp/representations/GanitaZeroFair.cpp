GanitaZeroFair::GanitaZeroFair(void)
{
  name="fair";
  dimension = 8;
  dim = dimension;
  verbose = 0;
  max_iter = 100000000;
}

//Initialize matrix for finding proportional sequences
int GanitaZeroFair::init(unsigned long dd)
{
  unsigned long ii, jj;

  dim = dd;
  dimension = dim;

  gzfm.init(dim+1,dim+1);
  for(ii=0; ii<=dim; ii++){ gzfm.set(0,ii,0); gzfm.set(ii,0,0);}
  for(ii=1; ii<=dim; ii++){
    gzfm.set(ii,1,ii);
    gzfm.set(ii,2,dim-ii+1);
  }
  for(ii=1; ii<=dim; ii++){
    for(jj=3; jj<=dim; jj++){
      gzfm.set(ii,jj,(double) dim);
    }
  }
  return(dim);
}

int GanitaZeroFair::checkSquareWeak1(unsigned long xx, unsigned long yy)
{
  int count;
  unsigned long kk, ii, jj;

  for(ii=1; ii<yy; ii++){
    kk = (unsigned long) floor((((double) ii*dim)/((double) yy)) + 1); 
    if(verbose) cout<<"kk="<<kk<<" ";
    count = 0;
    for(jj=1; jj<=yy; jj++){
      if(gzfm.get(xx,jj)<= (double) kk){
        count++;
      }
    }
    if(count < (int) ii){
      // Not proportional
      if(verbose) cout<<"count="<<count<<" "<<ii<<" ";
      return(count - ii);
    }
  }

  return(1);
}

int GanitaZeroFair::checkSquareWeak2(unsigned long xx, unsigned long yy)
{
  int count;
  unsigned long kk, ii, jj;

  for(ii=1; ii<yy; ii++){
    kk = (unsigned long) floor((((double) (ii+1)*dim)/((double) yy)) + 1); 
    if(verbose) cout<<"kk="<<kk<<" ";
    count = 0;
    for(jj=1; jj<=yy; jj++){
      if(gzfm.get(xx,jj)<= (double) kk){
        count++;
      }
    }
    if(count < (int) ii){
      // Not proportional
      if(verbose) cout<<"count="<<count<<" "<<ii<<" ";
      return(count - ii);
    }
  }

  return(1);
}

int GanitaZeroFair::checkSquare1(unsigned long xx, unsigned long yy)
{
  int count;
  unsigned long kk, ii, jj;

  for(ii=1; ii<yy; ii++){
    kk = (unsigned long) ceil(((double) ii*dim)/((double) yy)); 
    if(verbose) cout<<"kk="<<kk<<" ";
    count = 0;
    for(jj=1; jj<=yy; jj++){
      if(gzfm.get(xx,jj)<= (double) kk){
	count++;
      }
    }
    if(count < (int) ii){
      // Not proportional
      if(verbose) cout<<"count="<<count<<" "<<ii<<" ";
      return(count - ii);
    }
  }

  return(1);
}

int GanitaZeroFair::checkSquare2(unsigned long xx, unsigned long yy)
{
  unsigned long ii;

  for(ii=1; ii<yy; ii++){
    if(gzfm.get(xx,ii) == gzfm.get(xx,yy)){
      // Same value appears twice
      if(verbose) cout<<"checkSquare2 "<<ii<<","<<xx<<endl;
      return(-2);
    }
  }

  return(1);
}

int GanitaZeroFair::checkSquare3(unsigned long xx, unsigned long yy)
{
  unsigned long ii;

  for(ii=1; ii<xx; ii++){
    if(gzfm.get(ii,yy) == gzfm.get(xx,yy)){
      // Same value appears twice
      return(-2);
    }
  }

  return(1);
}

int GanitaZeroFair::setSquare(unsigned long xx, unsigned long yy)
{
  int ii;

  ii = (int) gzfm.get(xx,yy);
  while(ii>=1){
    gzfm.set(xx,yy,(double) ii);
    if(checkSquare3(xx,yy)>0){
      if(verbose) cout<<"setSquare3 "<<ii<<","<<xx<<endl;
      if(checkSquare2(xx,yy)>0){
	if(verbose) cout<<"setSquare2 "<<ii<<","<<yy<<endl;
	if(checkSquareWeak1(xx,yy)>0){
	  return(1);
	  if(verbose) cout<<"setSquare1 "<<ii<<endl;
	}
      }
    }
    ii--;
  }
  
  return(ii);
}

int GanitaZeroFair::setSquare(unsigned long xx, unsigned long yy, double max)
{
  unsigned long ii;

  ii=(unsigned long) max;
  while(ii>=1){
    gzfm.set(xx,yy,(double) ii);
    if(checkSquare3(xx,yy)>0){
      if(checkSquare2(xx,yy)>0){
	if(checkSquare1(xx,yy)>0){
	  return(1);
	}
      }
    }
    ii--;
  }
    
  return(ii);
}

int GanitaZeroFair::backTrackSolve(void)
{
  unsigned long pos1, pos2;
  unsigned long keep;
  pos1 = 1; pos2 = 2;
  int val, dd;
  double tval;

  dd = (int) dim + 1;
  cout<<"Dimension "<<dim<<"|| Initial Matrix"<<endl;
  for(int ii=0; ii<dd; ii++){
    for(int jj=0; jj<dd; jj++){
      cout<<gzfm.get(ii,jj)<<",";
    }
    cout<<endl;
  }
  if(verbose) cout<<"Start at "<<gzfm.get(pos1,pos2)<<endl;

  keep = 1;
  while(keep){
    //keep++;
    val = setSquare(pos1,pos2);
    if(val>0){
      //std::cout<<val<<std::endl;
      pos1++;
      if(pos1 > dim){
	pos1 = 1;
	pos2++;
	if(verbose) cout<<"New position "<<pos1<<","<<pos2<<endl;
	if(pos2 > dim){
	  //Reached end
	  return 2;
	}
      }
    }
    else{
      // Backtrack
      // Obtain current value and reset to max=dim
      tval=gzfm.get(pos1,pos2);
      tval = (double) dim;
      gzfm.set(pos1,pos2,tval);
      // Start backtracking
      pos1--;
      if(verbose) cout<<"Backtracking pos1="<<pos1<<"tval="<<tval<<endl;
      if(pos1 <= 0){
	pos1 = dim;
	pos2--;
	//std::cout<<"New positions "<<pos1<<", "<<pos2<<std::endl;
	if(pos2 <= 1){
	  // Unable to solve
	  return(-1);
	}
      }
      tval = gzfm.get(pos1,pos2);
      tval -= 1;
      while(tval < 1.0){
	tval = (double) dim;
	gzfm.set(pos1,pos2,tval);
	pos1--;
	if(pos1 <= 0){
	  pos1 = dim;
	  pos2--;
	  if(pos2 <= 1){
	    // Unable to solve
	    return(-1);
	  }
	}
	tval = gzfm.get(pos1,pos2);
	tval -= 1;
      }
      gzfm.set(pos1,pos2,tval);
    }
  }

  return(1);   
}
    
int GanitaZeroFair::topBalanced(void)
{
  unsigned long pos1, pos2;
  unsigned long keep;
  pos1 = 1; pos2 = 2;
  int val, dd;
  double tval;

  dd = (int) dim + 1;
  cout<<"Dimension "<<dim<<"|| Initial Matrix"<<endl;
  for(int ii=0; ii<dd; ii++){
    for(int jj=0; jj<dd; jj++){
      cout<<gzfm.get(ii,jj)<<",";
    }
    cout<<endl;
  }
  if(verbose) cout<<"Start at "<<gzfm.get(pos1,pos2)<<endl;

  keep = 1;
  while(keep){
    //keep++;
    val = setSquare(pos1,pos2);
    if(val>0){
      //std::cout<<val<<std::endl;
      pos1++;
      if(pos1 > dim){
	pos1 = 1;
	pos2++;
	if(verbose) cout<<"New position "<<pos1<<","<<pos2<<endl;
	if(pos2 > dim){
	  //Reached end
	  return 2;
	}
      }
    }
    else{
      // Backtrack
      // Obtain current value and reset to max=dim
      tval=gzfm.get(pos1,pos2);
      tval = (double) dim;
      gzfm.set(pos1,pos2,tval);
      // Start backtracking
      pos1--;
      if(verbose) cout<<"Backtracking pos1="<<pos1<<"tval="<<tval<<endl;
      if(pos1 <= 0){
	pos1 = dim;
	pos2--;
	//std::cout<<"New positions "<<pos1<<", "<<pos2<<std::endl;
	if(pos2 <= 1){
	  // Unable to solve
	  return(-1);
	}
      }
      tval = gzfm.get(pos1,pos2);
      tval -= 1;
      while(tval < 1.0){
	tval = (double) dim;
	gzfm.set(pos1,pos2,tval);
	pos1--;
	if(pos1 <= 0){
	  pos1 = dim;
	  pos2--;
	  if(pos2 <= 1){
	    // Unable to solve
	    return(-1);
	  }
	}
	tval = gzfm.get(pos1,pos2);
	tval -= 1;
      }
      gzfm.set(pos1,pos2,tval);
    }
  }

  return(1);   
}
    
int GanitaZeroFair::printMat(void)
{
  int dd;
  dd = (int) dim + 1;
  cout<<"Dimension "<<dim<<endl;
  for(int ii=0; ii<dd; ii++){
    for(int jj=0; jj<dd; jj++){
      cout<<gzfm.get(ii,jj)<<",";
    }
    cout<<endl;
  }
  return(1);
}

int GanitaZeroFair::checkMat(void)
{
  int counter, kk, bb;
  int dd;

  bb = 0; dd = (int) dim-1;
  for(int ii=1; ii<=dd+1; ii++){
    // ii = days
    for(int jj=1; jj<ii; jj++){
      // jj = occurences
      kk = (int) floor((((double) dim*jj)/((double) ii))+1);
      // kk = positions
      for(int mm=1; mm<=dim; mm++){
	counter = 0;
	for(int nn=1; nn<=ii; nn++){
	  if(gzfm.get(mm,nn)<=kk){
	    counter++;
	  }
	}
	if(counter<jj){
	  printf("Not fair: %d, %d, %d, %d\n", mm, kk, ii, jj);
	  bb++;
	}
      }
    }
  }
  if(bb==0){
    cout<<"PROPORTIONAL SEQUENCE!"<<endl;
  }

  return(1);
}

int GanitaZeroFair::checkMat2(void)
{
  int counter, kk, bb;
  int dd;

  bb = 0; dd = (int) dim;
  for(int ii=3; ii<=dd; ii++){
    // ii = days
    for(int jj=1; jj<ii-1; jj++){
      // jj = occurences
      kk = (int) floor((((double) dd*(jj+1))/((double) ii))+1);
      // kk = positions
      for(int mm=1; mm<=dd; mm++){
        counter = 0;
        for(int nn=1; nn<=ii; nn++){
          if(gzfm.get(mm,nn)<=kk){
            counter++;
          }
        }
        if(counter<jj){
          printf("Not fair: %d, %d, %d, %d\n", mm, kk, ii, jj);
          bb++;
        }
      }
    }
  }
  if(bb==0){
    cout<<"PROP2 PROPORTIONAL SEQUENCE!"<<endl;
  }

  return(1);
}

int GanitaZeroFair::squareProp(void)
{
  int ival, jval;
  unsigned long mval;
  unsigned long ii, jj;
  unsigned long kk, mm;

  //cout<<"Start algorithm "<<endl;
  dim2 = dim*dim; 
  gzfm2.init(dim2, dim2);

  //cout<<"Find squared matrix "<<dim*dim<<endl;
  for(ii=0; ii<dim; ii++){
    for(jj=0; jj<dim; jj++){
      gzfm2.set(ii*dim,jj*dim,gzfm.get(ii+1,jj+1));
     }
  }

  for(kk=0; kk<dim; kk++){
  for(mm=0; mm<dim; mm++){
    for(ii=0; ii<dim; ii++){
      for(jj=0; jj<dim; jj++){
        mval = (unsigned long) gzfm.get(mm+1,ii+1);
        ival = (int) gzfm.get(kk+1,jj+1)-1;
        jval = (int) gzfm.get(mval,jj+1);
        gzfm2.set(dim*kk+mm,ii*dim+jj,dim*ival+jval);
      }
    }
  }
  }

  //unsigned long dd;
  //dd = dim*dim;
  cout<<"Dimension of squared matrix "<<dim2<<endl;
  for(ii=0; ii<dim2; ii++){
    for(jj=0; jj<dim2; jj++){
      cout<<std::setw(2)<<gzfm2.get(ii,jj)<<",";
    }
    cout<<endl;
  }

  checkMat2();

  return(1);
}

int GanitaZeroFair::addMachineSeq(void)
{
  //unsigned long pos1, pos2;
  unsigned long result, ii, jj;
  unsigned long pp, qq, rr;
  unsigned long val;
  //pos1 = 1; pos2 = 2;
  unsigned int dd;
  //double tval;

  dd = 0;
  pp = 1;
  while (pp < dim+1) {
    //if ( (pp << 1) >= dim) break;
    pp <<= 1;
    dd++;
  }
  pp >>= 1;
  qq = 0; val = 0;
  
  cout<<"Dimension "<<dim<<"|| Initial Matrix"<<endl;
  // for(int ii=0; ii<dd; ii++){
  //   for(int jj=0; jj<dd; jj++){
  //     cout<<gzfm.get(ii,jj)<<",";
  //   }
  //   cout<<endl;
  // }
  //if(verbose) cout<<"Start at "<<gzfm.get(pos1,pos2)<<endl;

  cout<<pp<<endl;
  ii = 1;
  while(ii < pp+1){
    gzfm.set(1,ii,val+1); ii++;
    cout<<val<<":";
    qq += 1; rr = qq;
    result = 0;
    for (int jj = 0; jj < dd-1; ++jj) {
      // Left shift the result to make space for the next bit
      result <<= 1;
      // Extract the least significant bit of 'rr' and add it to the result
      // The expression `rr & 1` gets the rightmost bit
      // The `|` operator sets that bit in the result
      result |= (rr & 1);
      // Right shift 'rr' by 1 to process the next bit in the next iteration
      rr >>= 1;
    }
    val = result;
  }
    
  for(int ii=0; ii<dim; ii++) {
    cout<<gzfm.get(1,ii+1)<<"|";
  }
  cout<<endl;

  // Set the order for all other agents
  for(int ii=2; ii<dim+1; ii++){
    for(int jj=1; jj<dim+1; jj++){
      gzfm.set(ii,jj,gzfm.get(ii-1,(jj % dim) + 1));
    }
  }

  return(1);
}

int GanitaZeroFair::fieldMult(void)
{
  //unsigned long pos1, pos2;
  unsigned long ii, jj;
  //unsigned long pp, qq, rr;
  unsigned long val;
  //pos1 = 1; pos2 = 2;
  unsigned int dd;
  //double tval;

  dd = 0; val = 0;
  
  cout<<"Dimension "<<dim<<"|| Initial Matrix"<<endl;
  // for(int ii=0; ii<dd; ii++){
  //   for(int jj=0; jj<dd; jj++){
  //     cout<<gzfm.get(ii,jj)<<",";
  //   }
  //   cout<<endl;
  // }
  //if(verbose) cout<<"Start at "<<gzfm.get(pos1,pos2)<<endl;

  for(ii=0; ii < dim; ii++){
    val = 21*ii % dim;
    gzfm.set(1,ii+1,val+1);
    cout<<val<<":"<<ii<<":"<<dim<<";";
  }
  for(int ii=0; ii<dim; ii++) {
    cout<<gzfm.get(1,ii+1)<<",";
  }
  cout<<endl;

  // Set the order for all other agents
  for(int ii=2; ii<dim+1; ii++){
    for(int jj=1; jj<dim+1; jj++){
      gzfm.set(ii,jj,gzfm.get(ii-1,(jj % dim) + 1));
    }
  }


  return(1);
}

