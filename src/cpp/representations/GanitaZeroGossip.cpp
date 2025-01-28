GanitaZeroGossip::GanitaZeroGossip(void)
{
  name = "Random Gossip Process";
  gossip_total = 0;
  rounds = 0;
}

// Return random number between 0 and 1.
double GanitaZeroGossip::returnArc4RandUniform(void)
{
  double myran;

  std::random_device rd;  // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<> dis(0.0, 1.0);

  myran = dis(gen);
  return(myran);
}

// Return random integer between 0 and up_bound.
uint32_t GanitaZeroGossip::returnArc4RandUniform(uint32_t up_bound)
{
  uint32_t myint;
  std::random_device r;
  std::default_random_engine e1(r());
  std::uniform_int_distribution<int> uniform_dist(0, up_bound-1);
  myint = uniform_dist(e1);

  return(myint);
}

int GanitaZeroGossip::run(uint64_t num, uint64_t trials)
{
  uint64_t ii, jj;
  uint64_t myran;
  std::vector<uint64_t> member_copy;
  uint64_t hist[100] = {0};
  double sum = 0;

  for(ii=0; ii<num; ii++){
    member.push_back(0);
  }

  for(jj=0; jj<trials; jj++){
    std::fill(member.begin(), member.end(), 0);
    member[0] = 1;
    member_copy = member;
    
    gossip_total = 1;
    rounds = 0;
    while(gossip_total < num){
      rounds++;
      //cout<<"Round-"<<rounds<<":";
      for(ii=0; ii<num; ii++){
	if(member_copy[ii]==1){
	  //myran = (uint64_t) arc4random_uniform(num-1);
	  myran = (uint64_t) returnArc4RandUniform(num-1);
	  if(myran >= ii){
	    myran++;
	  }
	  if(member[myran]==0){
	    //cout<<"Informed:"<<ii<<"->"<<myran<<"|";
	    member[myran]=1;
	    gossip_total++;
	  }
	}
      }
      member_copy = member;
    }
    //cout<<endl<<"Number of rounds = "<<rounds<<endl;
    hist[rounds]++;
  }
  jj=0;
  while((hist[jj]==0) && (jj<99)){
    jj++;
  }
  if(jj == 99){
    //histogram is empty
    return(-1);
  }
  cout<<"Histogram (members:trials)=("<<num<<":"<<trials<<")"<<endl;
  cout<<"rounds,frequency"<<endl;
  while(hist[jj]+hist[jj+1]+hist[jj+2] > 0){
    //cout<<"(rounds,frequency)=("<<jj<<","<<hist[jj]<<")"<<endl;
    cout<<jj<<","<<hist[jj]<<endl;
    sum += jj*hist[jj];
    jj++;
  }
  cout<<"Expected value: "<<sum/trials<<endl;
  
  return(1);
}

int GanitaZeroGossip::run3(uint64_t num, uint64_t trials, uint64_t nbrs)
{
  uint64_t ii, jj;
  //std::vector<uint64_t> myran(nbrs,0);
  std::vector<uint64_t> member_copy;
  uint64_t hist[100] = {0};
  double sum = 0;
  set<uint64_t> myran;
  set<uint64_t, greater<uint64_t> >::iterator itr;

  for(ii=0; ii<num; ii++){
    member.push_back(0);
  }

  for(jj=0; jj<trials; jj++){
    std::fill(member.begin(), member.end(), 0);
    member[0] = 1;
    member_copy = member;
    
    gossip_total = 1;
    rounds = 0;
    while(gossip_total < num){
      rounds++;
      //cout<<"Round-"<<rounds<<":";
      for(ii=0; ii<num; ii++){
	if(member_copy[ii]==1){
	  myran.clear();
	  myran.insert(ii);
	  while(myran.size() <= nbrs){
	    //myran.insert((uint64_t) arc4random_uniform(num));
	    myran.insert((uint64_t) returnArc4RandUniform(num));
	  }
	  for (itr = myran.begin(); itr != myran.end(); itr++){
	    if(member[*itr]==0){
	      //cout<<"Informed:"<<ii<<"->"<<*itr<<"|";
	      member[*itr]=1;
	      gossip_total++;
	    }
	  }
	}
      }
      member_copy = member;
    }
    //cout<<endl<<"Number of rounds = "<<rounds<<endl;
    hist[rounds]++;
  }
  jj=0;
  while((hist[jj]==0) && (jj<99)){
    jj++;
  }
  if(jj == 99){
    //histogram is empty
    return(-1);
  }
  //cout<<"Histogram (members:trials)=("<<num<<":"<<trials<<")"<<endl;
  //cout<<"rounds,frequency"<<endl;
  while(hist[jj]+hist[jj+1]+hist[jj+2] > 0){
    //cout<<"(rounds,frequency)=("<<jj<<","<<hist[jj]<<")"<<endl;
    //cout<<jj<<","<<hist[jj]<<endl;
    sum += jj*hist[jj];
    jj++;
  }
  cout<<"Expected value: "<<sum/trials<<endl;
  
  return(1);
}

int GanitaZeroGossip::runEV(uint64_t num, uint64_t trials)
{
  uint64_t ii, jj;
  uint64_t myran;
  std::vector<uint64_t> member_copy;
  uint64_t hist[100] = {0};
  double sum = 0;

  for(ii=0; ii<num; ii++){
    member.push_back(0);
  }

  for(jj=0; jj<trials; jj++){
    std::fill(member.begin(), member.end(), 0);
    member[0] = 1;
    member_copy = member;
    
    gossip_total = 1;
    rounds = 0;
    while(gossip_total < num){
      rounds++;
      //cout<<"Round-"<<rounds<<":";
      for(ii=0; ii<num; ii++){
	if(member_copy[ii]==1){
	  //myran = (uint64_t) arc4random_uniform(num-1);
	  myran = (uint64_t) returnArc4RandUniform(num-1);
	  if(myran >= ii){
	    myran++;
	  }
	  if(member[myran]==0){
	    //cout<<"Informed:"<<ii<<"->"<<myran<<"|";
	    member[myran]=1;
	    gossip_total++;
	  }
	}
      }
      member_copy = member;
    }
    //cout<<endl<<"Number of rounds = "<<rounds<<endl;
    hist[rounds]++;
  }
  jj=0;
  while((hist[jj]==0) && (jj<99)){
    jj++;
  }
  if(jj == 99){
    //histogram is empty
    return(-1);
  }
  //cout<<"Histogram (members:trials)=("<<num<<":"<<trials<<")"<<endl;
  //cout<<"rounds,frequency"<<endl;
  while(hist[jj]+hist[jj+1]+hist[jj+2] > 0){
    //cout<<"(rounds,frequency)=("<<jj<<","<<hist[jj]<<")"<<endl;
    //cout<<jj<<","<<hist[jj]<<endl;
    sum += jj*hist[jj];
    jj++;
  }
  cout<<sum/trials<<endl;
  
  return(1);
}

int GanitaZeroGossip::runAll(uint64_t num)
{
  uint64_t ii, jj;
  uint64_t num2, myran;
  std::vector<vector<uint64_t>> all_members(num, vector<uint64_t>(num,0));
  std::vector<uint64_t> member_copy(num,0);
  std::vector<uint64_t> myrans(num,0);

//   for(ii=0; ii<num; ii++){
//     member.push_back(0);
//     myrans.push_back(0);
//   }
  for(ii=0; ii<num; ii++){
    all_members[ii][ii] = 1;
  }

  gossip_total = num;
  num2 = num*num;
  rounds = 0;
  while(gossip_total < num2){
    rounds++;
    cout<<"Round-"<<rounds<<":";
    for(ii=0; ii<num; ii++){
      //myran = (uint64_t) arc4random_uniform(num);
      myran = (uint64_t) returnArc4RandUniform(num);
      if(myran >= ii){
	myran++;
      }
      myrans[ii] = myran;
    }
    for(ii=0; ii<num; ii++){
      member_copy = all_members[ii];
      for(jj=0; jj<num; jj++){
	if((member_copy[jj]==1) && (all_members[ii][myrans[jj]]==0)){
	  //cout<<"Informed:"<<ii<<"->"<<myran<<"|";
	  all_members[ii][myrans[jj]]=1;
	  gossip_total++;
	}
      }
    }
  }
  cout<<endl<<"Number of rounds = "<<rounds<<endl;
  cout<<endl<<"Total = "<<gossip_total<<endl;
  
  return(1);
}

int GanitaZeroGossip::estProb(uint64_t num, uint64_t jj)
{
  uint64_t kk, rounds, nn;
  uint64_t informed, uninformed;
  double multiplier;

  kk = num; rounds = 1;
  nn = num - 1;
  informed = 2; uninformed = num - 2;
  while(informed < kk){
    multiplier = exp(-1*((double)jj*informed)/((double)nn));
    uninformed = (uint64_t) round(multiplier*uninformed);
    informed = num - uninformed;
    //cout<<"(informed,multiplier)="<<"("<<informed<<","<<multiplier<<")"<<endl;
    rounds++;
  }
  //cout<<"Informed="<<informed<<"|"<<"Rounds="<<rounds<<endl;
  //cout<<informed<<","
  cout<<rounds<<endl;

  return(1);
}

