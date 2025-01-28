GanitaZeroPascalNS::GanitaZeroPascalNS(void)
{
  name = "PascalNumberSystem";
  bignum = 0;
  smallnum = 0;
  base = 0;
  input = 0;
  curCoef.push_back(0);
  curCoef.push_back(0);
}

bool GanitaZeroPascalNS::setAll
(uint64_t bn, uint64_t sn, uint64_t ba, uint64_t num)
{
  bignum = bn;
  smallnum = sn;
  base = ba;
  input = num;
  return(ba < bn);
}

uint64_t GanitaZeroPascalNS::setBigNum(uint64_t num)
{
  bignum = num;
  return(bignum);
}

uint64_t GanitaZeroPascalNS::setSmallNum(uint64_t num)
{
  smallnum = num;
  return(smallnum);
}

uint64_t GanitaZeroPascalNS::setBase(uint64_t num)
{
  base = num;
  return(base);
}

uint64_t GanitaZeroPascalNS::printRep(void)
{
  for(uint64_t ii=0; ii<pnsCoefs.size(); ii++){
    cout<<"("<<pnsCoefs[ii][0]<<" "<<pnsCoefs[ii][1]<<")"<<std::endl;
  }
  
  return(pnsCoefs.size());
}

uint64_t GanitaZeroPascalNS::returnCoef(uint64_t index, uint64_t ii)
{
  return(pnsCoefs[index % pnsCoefs.size()][ii % 2]);
}

uint64_t GanitaZeroPascalNS::size(void)
{
  return(pnsCoefs.size());
}

uint64_t GanitaZeroPascalNS::binomial(uint64_t top, uint64_t bot)
{
  uint64_t num;
  uint64_t ii;
  
  if(2*bot > top){
    bot = top - bot;
  }
  num = 1;
  for(ii=0; ii<bot; ii++){
    num = num*(top-ii);
  }
  for(ii=2; ii<=bot; ii++){
    num = num / ii;
  }
  return(num);
}

uint64_t GanitaZeroPascalNS::computeCoef
(uint64_t diff, uint64_t bn, uint64_t sn)
{
  uint64_t tt, bb;
  uint64_t count;

  count = 0;
  tt = bn; bb = sn;
  while( binomial(tt, bb) > diff ){
    tt--; bb--;
    count++;
  }

  return(count);
}

uint64_t GanitaZeroPascalNS::computeRep
(uint64_t num, uint64_t bn, uint64_t sn)
{
  uint64_t tt, bb;
  uint64_t count, diff;

  if(binomial(bn,sn) < num){
    cout<<"Starting binomial coefficient less than integer."<<std::endl;
    return(0);
  }

  count = 0;
  tt = bn; bb = sn;
  diff = num;
  while(tt > base){
    count = computeCoef(diff,tt,bb);
    tt -= count; bb -= count;
    diff -= binomial(tt,bb);
    curCoef[0]=tt; curCoef[1]=bb;
    pnsCoefs.push_back(curCoef);
    tt--;
  }

  return(count);
}

uint64_t GanitaZeroPascalNS::computeRep(void)
{
  uint64_t tt, bb;
  uint64_t count, diff;

  if(binomial(bignum,smallnum) < input){
    cout<<"Starting binomial coefficient less than integer."<<std::endl;
    return(0);
  }

  pnsCoefs.clear();
  count = 0;
  tt = bignum; bb = smallnum;
  diff = input;
  //while((tt >= base) && (diff > 0)){
  while(diff >= binomial(base,bb+base-tt)){
    count = computeCoef(diff,tt,bb);
    cout<<"("<<diff<<","<<tt<<","<<bb<<","<<count<<")"<<std::endl;
    tt -= count; bb -= count;
    diff -= binomial(tt,bb);
    curCoef[0]=tt; curCoef[1]=bb;
    pnsCoefs.push_back(curCoef);
    tt--;
  }
  curCoef[0]=diff; curCoef[1]=1;
  pnsCoefs.push_back(curCoef);

  return(count);
}

uint64_t GanitaZeroPascalNS::computeRep(uint64_t input_num)
{
  uint64_t tt, bb;
  uint64_t count, diff;

  if(binomial(bignum,smallnum) < input){
    cout<<"Starting binomial coefficient less than integer."<<std::endl;
    return(0);
  }

  pnsCoefs.clear();
  count = 0;
  tt = bignum; bb = smallnum;
  diff = input_num;
  while((tt >= base) && (diff > 0)){
    //while(diff >= binomial(base,bb+base-tt)){
    count = computeCoef(diff,tt,bb);
    //cout<<"("<<diff<<","<<tt<<","<<bb<<","<<count<<")"<<std::endl;
    tt -= count; bb -= count;
    if(tt >= base){
      diff -= binomial(tt,bb);
      curCoef[0]=tt; curCoef[1]=bb;
      pnsCoefs.push_back(curCoef);
      tt--;
    }
  }
  curCoef[0]=diff; curCoef[1]=1;
  pnsCoefs.push_back(curCoef);

  return(count);
}

uint64_t GanitaZeroPascalNS::compareTails(void)
{
  uint64_t ii;
  uint64_t ulimit;
  uint64_t vala, valb;

  ulimit = binomial(bignum, smallnum);
  for(ii=0; ii<=ulimit; ii++){
    computeRep(ii);
    vala = pnsCoefs.back()[0];
    setSmallNum(smallnum+1);
    computeRep(ii);
    valb = pnsCoefs.back()[0];
    setSmallNum(smallnum-1);
    cout<<"("<<ii<<","<<vala<<","<<valb<<")"<<std::endl;
  }
  
  return(ulimit);
}

