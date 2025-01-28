GanitaZeroTGF::GanitaZeroTGF(void)
{
  name = "DNN based on Thompson Group F";
}

int GanitaZeroTGF::createTGF(uint64_t numlayers)
{
  uint64_t ii;

  num_layers = numlayers;

  addLayer(0, 0);
  layer[layer.size()-1]->addNode1(0,"input");
  layer[0]->returnNode(0)->addEdge(0,0);
  layer[0]->returnNode(0)->addEdge(1,0);
  dumpLayer(layer.size()-1);
  for(ii=1; ii<=numlayers; ii++){
    addLayer(ii, ii);
    layer[layer.size()-1]->addNode1(0,"hidden-add");
    layer[layer.size()-1]->addNode1(0,"hidden-apply-tgf");
    layer[ii]->returnNode(0)->addEdge(0,0);
    layer[ii]->returnNode(0)->addEdge(1,0);
    layer[ii]->returnNode(1)->addEdge(0,0);
    layer[ii]->returnNode(1)->addEdge(1,0);
    dumpLayer(ii);
  }
  addLayer(ii, ii);
  layer[layer.size()-1]->addNode1(0,"hidden-add");
  layer[layer.size()-1]->addNode1(0,"hidden-apply-tgf");
  layer[layer.size()-1]->returnNode(0)->addEdge(0,0);
  layer[layer.size()-1]->returnNode(1)->addEdge(0,0);
  dumpLayer(layer.size()-1);
  //addLayer(ii+1,ii+1);
  //layer[layer.size()-1]->addNode1(0,"output");
  //dumpLayer(layer.size()-1);
  num_layers = layer.size();
  std::cout << tgf_generator_bkpts[0][1] << std::endl;
  std::cout << callGenerator(2,.3) << std::endl;
  generateOrdActSeq(num_layers+2);
  dumpActSeq();
  cout<<composeTGF(0.30)<<endl;
  setUniformWeights(); dumpWeights();
  copyInternalTGF(.50);

  //writeNetwork();

  return(1);
}

// Return random number between 0 and 1.
double GanitaZeroTGF::returnArc4RandUniform(void)
{
  double myran;

  std::random_device rd;  // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<> dis(0.0, 1.0);

  myran = dis(gen);
  return(myran);
}

// Return random integer between 0 and up_bound.
uint32_t GanitaZeroTGF::returnArc4RandUniform(uint32_t up_bound)
{
  uint32_t myint;
  std::random_device r;
  std::default_random_engine e1(r());
  std::uniform_int_distribution<int> uniform_dist(0, up_bound-1);
  myint = uniform_dist(e1);

  return(myint);
}

int GanitaZeroTGF::sgdTGF(uint64_t num)
{
  uint64_t ii;
  double myran;

  for(ii=0; ii<num; ii++){
    //myran = ((double) arc4random_uniform(100000))/100000.0;
    myran = returnArc4RandUniform();
    cout<<"ii="<<ii<<"|";
    //backPropTGF(myran, callGenerator(0,myran));
    backPropTGF(myran, myran*myran);
    feedForwardInternalTGF(myran);
    //backPropTGF(myran, callGenerator(0,myran));
    backPropTGF(myran, myran*myran);
    feedForwardInternalTGF(myran);
  }
  
  return(1);
}

int GanitaZeroTGF::sgdTGF2(uint64_t num, GanitaZeroTGF target)
{
  uint64_t ii;
  double myran;

  for(ii=0; ii<num; ii++){
    //myran = ((double) arc4random_uniform(100000))/100000.0;
    myran = returnArc4RandUniform();
    cout<<"ii="<<ii<<"|";
    //backPropTGF(myran, callGenerator(0,myran));
    backPropTGF(myran, target.feedForwardInternalTGF(myran));
    feedForwardInternalTGF(myran);
    //backPropTGF(myran, callGenerator(0,myran));
    backPropTGF(myran, target.feedForwardInternalTGF(myran));
    feedForwardInternalTGF(myran);
  }
  
  return(1);
}

int GanitaZeroTGF::sgdTGF3(uint64_t num, int gnum)
{
  uint64_t ii;
  double myran;

  for(ii=0; ii<num; ii++){
    //myran = ((double) arc4random_uniform(100000))/100000.0;
    myran = returnArc4RandUniform();
    cout<<"ii="<<ii<<"|";
    backPropTGF(myran, callGenerator(gnum,myran));
    feedForwardInternalTGF(myran);
    backPropTGF(myran, callGenerator(gnum,myran));
    feedForwardInternalTGF(myran);
  }
  
  return(1);
}

int GanitaZeroTGF::outputTestRandTGF(uint64_t num)
{
  uint64_t ii;
  double myran;

  for(ii=0; ii<num; ii++){
    //myran = ((double) arc4random_uniform(100000))/100000.0;
    myran = returnArc4RandUniform();
    cout<<myran<<", "<<feedForwardInternalTGF(myran)<<endl;
  }

  return(1);
}

int GanitaZeroTGF::outputTestTGF(uint64_t num)
{
  uint64_t ii;
  double mypoint;

  for(ii=0; ii<num; ii++){
    mypoint = ((double) ii)/num;
    cout<<mypoint<<", "<<feedForwardInternalTGF(mypoint)<<endl;
  }

  return(1);
}

// Break points on [0,1] for two generators 
// of the Thompson Group F and their inverses.
// First two arrays are for generators a and b.
// Second two arrays are for inverses a^{-1}, b^{-1}. 
const double GanitaZeroTGF::tgf_generator_bkpts[][6] = 
  {{0, .25, .50, 1, 2, 3}, {0, .50, .625, .75, 1, 2}, 
   {0, .50, .75, 1, 2, 3}, {0, .50, .75, .875, 1, 2}};

// Slopes of piecewise linear generator functions for 
// the Thompson Group F that will be used as activation 
// functions for a deep neural network. 
// First two arrays are for generators a and b.
// Second two arrays are for inverses a^{-1}, b^{-1}. 
// For each pair of numbers, the first is the slope and 
// the second is the y-intercept. 
const double GanitaZeroTGF::tgf_generator_fun[4][4][2] = {
  {{2,0},{1,.25}, {.50,.50},{-1,-1}}, 
  {{1,0},{2,-.50},{1,.125}, {.50,.50}},
  {{.50,0},{1,-.25},{2,-1},{-1,-1}},
  {{1,0},{.50,.25},{1,-.125},{2,-1}}
};

double GanitaZeroTGF::callGenerator(int fn, double input)
{
  int ii;
  double output;
  
  if((input < 0) || (input > 1)){
    // invalid domain
    return(-1);
  }
  if((fn < 0) || (fn > 3)){
    //invalid function call
    return(-2);
  }
  ii=0;
  while(tgf_generator_bkpts[fn][ii] < input){
    ii++;
  }
  if(ii == 0) ii++;
  output = tgf_generator_fun[fn][ii-1][0] * input 
    +  tgf_generator_fun[fn][ii-1][1];
  //cout<<"tgf_generator "<<tgf_generator_fun[fn][ii-1][0]<<","<<input<<","
  //    <<tgf_generator_fun[fn][ii-1][1]<<endl;

  return(output);
}

double GanitaZeroTGF::callGeneratorSlope(int fn, double input)
{
  int ii;
  double output;
  
  if((input < 0) || (input > 1)){
    // invalid domain
    return(-1);
  }
  if((fn < 0) || (fn > 3)){
    //invalid function call
    return(-2);
  }
  ii=0;
  while(tgf_generator_bkpts[fn][ii] < input){
    ii++;
  }
  if(ii == 0) ii++;
  output = tgf_generator_fun[fn][ii-1][0];
  //cout<<"tgf_generator "<<tgf_generator_fun[fn][ii-1][0]<<","<<input<<","
  //    <<tgf_generator_fun[fn][ii-1][1]<<endl;

  return(output);
}

int GanitaZeroTGF::generateRandActSeq(uint64_t len)
{
  uint32_t myran, skip;
  uint64_t ii;

  if(len < 1){
    return(-1);
  }

  //myran = arc4random_uniform(4);
  myran = returnArc4RandUniform(4);
  actseq.push_back(myran);
  for(ii=1; ii<len; ii++){
    skip = (myran + 2) % 4;
    //myran = arc4random_uniform(3);
    myran = returnArc4RandUniform(3);
    if(skip <= myran){
      myran++;
    }
    actseq.push_back(myran);
  }

  return(1);
}

int GanitaZeroTGF::generateOrdActSeq(uint64_t len)
{
  uint64_t ii;

  if(len < 1){
    return(-1);
  }

  for(ii=0; ii<len; ii++){
    actseq.push_back(ii % 4);
  }

  return(1);
}

int GanitaZeroTGF::dumpActSeq(void)
{
  uint64_t ii;
  for(ii=0; ii<actseq.size(); ii++){
    cout<<actseq[ii]<<",";
  }
  cout<<endl;

  return(1);
}

double GanitaZeroTGF::composeTGF(double input)
{
  uint64_t ii;
  double output;

  if((input < 0) || (input > 1)){
    //input outside domain
    return(-1);
  }

  output = input;
  for(ii=0; ii<actseq.size(); ii++){
    cout<<actseq[ii]<<","<<output<<"|";
    output = callGenerator(actseq[ii], output);
  }

  return(output);
}

double GanitaZeroTGF::convertToWeight(uint64_t val)
{
  long double tstore;
  tstore = val;
  tstore /= weight_denom;

  return((double) tstore);
}

uint64_t GanitaZeroTGF::convertFromWeight(double ww)
{
  return((uint64_t) (weight_denom*ww));
}

int GanitaZeroTGF::setBinaryWeightsRand1(void)
{
  uint32_t myran, ii;
  
  if(num_layers < 3){
    return(-1);
  }
  // Set weights to 1 for top branches
  //myran = arc4random_uniform(2);
  //if(myran == 1){
  layer[0]->returnNode(0)->setEdgeValue(0, weight_denom);
    //}
    //else{
  layer[0]->returnNode(0)->setEdgeValue(1, weight_denom);
    //}

  for(ii=1; ii<num_layers-1; ii++){
    //myran = arc4random_uniform(2);
    myran = returnArc4RandUniform(2);
    if(myran == 1){
      layer[ii]->returnNode(0)->setEdgeValue(0, weight_denom);
    }
    else{
      layer[ii]->returnNode(1)->setEdgeValue(0, weight_denom);
    }
    //myran = arc4random_uniform(2);
    myran = returnArc4RandUniform(2);
    if(myran == 1){
      layer[ii]->returnNode(0)->setEdgeValue(1, weight_denom);
    }
    else{
      layer[ii]->returnNode(1)->setEdgeValue(1, weight_denom);
    }
  }
  //myran = arc4random_uniform(2);
  myran = returnArc4RandUniform(2);
  if(myran == 1){
    layer[num_layers-1]->returnNode(0)->setEdgeValue(0, weight_denom);
  }
  else{
    layer[num_layers-1]->returnNode(1)->setEdgeValue(0, weight_denom);
  }

  return(1);
}

int GanitaZeroTGF::setBinaryWeightsId(void)
{
  uint32_t ii;
  
  if(num_layers < 3){
    return(-1);
  }
  layer[0]->returnNode(0)->setEdgeValue(0, weight_denom);
  layer[0]->returnNode(0)->setEdgeValue(1, weight_denom);
  for(ii=1; ii<num_layers-1; ii++){
    layer[ii]->returnNode(0)->setEdgeValue(0, weight_denom);
    layer[ii]->returnNode(1)->setEdgeValue(1, weight_denom);
  }
  layer[num_layers-1]->returnNode(0)->setEdgeValue(0, weight_denom);

  return(1);
}

int GanitaZeroTGF::setUniformWeights(void)
{
  uint32_t ii;
  
  if(num_layers < 3){
    return(-1);
  }
  layer[0]->returnNode(0)->setEdgeValue(0, weight_denom/2);
  layer[0]->returnNode(0)->setEdgeValue(1, weight_denom/2);
  for(ii=1; ii<num_layers-1; ii++){
    layer[ii]->returnNode(0)->setEdgeValue(0, weight_denom/2);
    layer[ii]->returnNode(0)->setEdgeValue(1, weight_denom/2);
    layer[ii]->returnNode(1)->setEdgeValue(0, weight_denom/2);
    layer[ii]->returnNode(1)->setEdgeValue(1, weight_denom/2);
  }
  layer[num_layers-1]->returnNode(0)->setEdgeValue(0, weight_denom/2);
  layer[num_layers-1]->returnNode(1)->setEdgeValue(0, weight_denom/2);

  return(1);
}

int GanitaZeroTGF::dumpWeights(void)
{
  uint64_t ii, jj;
  
  for(ii=0; ii<num_layers; ii++){
    for(jj=0; jj<layer[ii]->returnNumNodes(); jj++){
      cout<<"Layer "<<ii<<" Node "<<jj<<" Edges ";
      layer[ii]->returnNode(jj)->dumpEdgeAll();
    }
  }

  return(1);
}

int GanitaZeroTGF::feedForwardTGF(double input)
{
  uint64_t ii;
  double ww1,ww2;
  GanitaEdge ed;
  double xx, xx1, xx2;
  
  if(num_layers < 2){
    return(-1);
  }
  layer[0]->returnNode(0)->returnEdge(0, ed);
  ww1 = convertToWeight(ed.returnValue());
  cout<<"Edge "<<ed.returnValue()<<","<<ww1;
  layer[0]->returnNode(0)->returnEdge(1, ed);
  ww2 = convertToWeight(ed.returnValue());
  cout<<" Edge "<<ed.returnValue()<<","<<ww2<<endl;
  //xx = ww1*input + ww2*callGenerator(actseq[0],input);
  //cout<<"Value "<<xx<<endl;
  xx1 = ww1*input;
  xx2 = ww2*callGenerator(actseq[0],input);
  cout<<"(ww1,ww2,xx1,xx2)="<<"("<<ww1<<","<<ww2<<","
      <<xx1<<","<<xx2<<")"<<endl;
  for(ii=1; ii<num_layers-1; ii++){
    //compute xx1
    layer[ii]->returnNode(0)->returnEdge(0, ed);
    ww1 = convertToWeight(ed.returnValue());
    //cout<<"Edge "<<ed.returnValue()<<","<<ww1;
    layer[ii]->returnNode(1)->returnEdge(0, ed);
    ww2 = convertToWeight(ed.returnValue());
    xx = ww1*xx1 + ww2*xx2;
    cout<<"(ww1,ww2,xx1,xx2)="<<"("<<ww1<<","<<ww2<<","
	<<xx1<<","<<xx2<<")"<<endl;
    //compute xx2
    layer[ii]->returnNode(0)->returnEdge(1, ed);
    ww1 = convertToWeight(ed.returnValue());
    //cout<<"Edge "<<ed.returnValue()<<","<<ww1;
    layer[ii]->returnNode(1)->returnEdge(1, ed);
    ww2 = convertToWeight(ed.returnValue());
    xx2 = callGenerator(actseq[ii],ww1*xx1 + ww2*xx2);
    // set xx1 to xx
    xx1 = xx;
    cout<<"(ww1,ww2,xx1,xx2)="<<"("<<ww1<<","<<ww2<<","
	<<xx1<<","<<xx2<<")"<<endl;
  }
  layer[ii]->returnNode(0)->returnEdge(0, ed);
  ww1 = convertToWeight(ed.returnValue());
  layer[ii]->returnNode(1)->returnEdge(0, ed);
  ww2 = convertToWeight(ed.returnValue());
  xx = ww1*xx1 + ww2*xx2;
  cout<<"Output "<<xx<<endl;

  return(1);
}

int GanitaZeroTGF::copyInternalTGF(double input)
{
  uint64_t ii;
  double ww1,ww2, ww3, ww4;
  GanitaEdge ed;
  double xx1, xx2, xx3, xx4;
  double ss1, ss2;
  
  if(num_layers < 2){
    return(-1);
  }
  ww1 = 0; ww2 = 0;
  ww3 = 0; ww4 = 0;
  layer[0]->returnNode(0)->returnEdge(0, ed);
  ww1 = convertToWeight(ed.returnValue());
  //cout<<"Edge "<<ed.returnValue()<<","<<ww1;
  layer[0]->returnNode(0)->returnEdge(1, ed);
  ww3 = convertToWeight(ed.returnValue());
  //cout<<" Edge "<<ed.returnValue()<<","<<ww2<<endl;
  //xx = ww1*input + ww2*callGenerator(actseq[0],input);
  //cout<<"Value "<<xx<<endl;
  xx1 = ww1*input;
  xx2 = ww3*input;
  node_input.push_back(tuple<double,double,double,double>(ww1*xx1,ww2*xx2,ww3*xx1,ww4*xx2));
  node_output.push_back(tuple<double,double>(input,input));
  tgf_weights.push_back(tuple<double,double,double,double>(ww1,ww2,ww3,ww4));  
  //  cout<<"(ww1,ww2,xx1,xx2)="<<"("<<ww1<<","<<ww2<<","
  //  <<xx1<<","<<xx2<<")"<<endl;
  for(ii=1; ii<num_layers-1; ii++){
    xx1 = get<0>(node_input.at(ii-1));
    xx2 = get<1>(node_input.at(ii-1));
    xx3 = get<2>(node_input.at(ii-1));
    xx4 = get<3>(node_input.at(ii-1));
    ss1 = xx1 + xx2;
    ss2 = callGenerator(actseq[ii-1],xx3 + xx4);
    node_output.push_back(tuple<double,double>(ss1, ss2));
    layer[ii]->returnNode(0)->returnEdge(0, ed);
    ww1 = convertToWeight(ed.returnValue());
    //cout<<"Edge "<<ed.returnValue()<<","<<ww1;
    layer[ii]->returnNode(1)->returnEdge(0, ed);
    ww2 = convertToWeight(ed.returnValue());
    //cout<<"(ww1,ww2,xx1,xx2)="<<"("<<ww1<<","<<ww2<<","
    //<<xx1<<","<<xx2<<")"<<endl;
    //compute xx2
    layer[ii]->returnNode(0)->returnEdge(1, ed);
    ww3 = convertToWeight(ed.returnValue());
    //cout<<"Edge "<<ed.returnValue()<<","<<ww1;
    layer[ii]->returnNode(1)->returnEdge(1, ed);
    ww4 = convertToWeight(ed.returnValue());
    node_input.push_back(tuple<double,double,double,double>(ww1*ss1,ww2*ss2,ww3*ss1,ww4*ss2));
    tgf_weights.push_back(tuple<double,double,double,double>(ww1,ww2,ww3,ww4));  
  }
  xx1 = get<0>(node_input.at(ii-1));
  xx2 = get<1>(node_input.at(ii-1));
  xx3 = get<2>(node_input.at(ii-1));
  xx4 = get<3>(node_input.at(ii-1));
  ss1 = xx1 + xx2;
  ss2 = callGenerator(actseq[ii-1],xx3 + xx4);
  node_output.push_back(tuple<double,double>(ss1, ss2));
  layer[ii]->returnNode(0)->returnEdge(0, ed);
  ww1 = convertToWeight(ed.returnValue());
  layer[ii]->returnNode(1)->returnEdge(0, ed);
  ww2 = convertToWeight(ed.returnValue());
  tgf_weights.push_back(tuple<double,double,double,double>(ww1,ww2,0,0));  
  node_output.push_back(tuple<double,double>(ww1*ss1+ww2*ss2,0));
  node_input.push_back(tuple<double,double,double,double>(ww1*ss1,ww2*ss2,0,0));
  cout<<"output "<<ww1*ss1+ww2*ss2<<endl;

  return(1);
}

int GanitaZeroTGF::newCopyInternalTGF(GanitaZeroTGF *copy)
{
  uint64_t ii;
  double ww1,ww2, ww3, ww4;
  double yy1, yy2;
  
  copy->num_layers = num_layers;
  for(ii=0; ii<tgf_weights.size(); ii++){
    ww1 = get<0>(tgf_weights.at(ii));
    ww2 = get<1>(tgf_weights.at(ii));
    ww3 = get<2>(tgf_weights.at(ii));
    ww4 = get<3>(tgf_weights.at(ii));
    copy->tgf_weights.push_back(tuple<double,double,double,double>(ww1,ww2,ww3,ww4));  
  }
  for(ii=0; ii<node_output.size(); ii++){
    yy1 = get<0>(node_output.at(ii));
    yy1 = get<1>(node_output.at(ii));
    yy2 = 0;
    copy->node_output.push_back(tuple<double,double>(yy1,yy2));
  }
  for(ii=0; ii<actseq.size(); ii++){
    copy->actseq.push_back(actseq.at(ii));
  }

  return(1);
}

double GanitaZeroTGF::feedForwardInternalTGF(double input)
{
  uint64_t ii;
  double ww1,ww2, ww3, ww4;
  GanitaEdge ed;
  double xx1, xx2;
  double ss1, ss2;
  
  if(num_layers < 2){
    return(-1);
  }
  ww1 = 0; ww2 = 0;
  ww3 = 0; ww4 = 0;
  ww1 = get<0>(tgf_weights.at(0));
  ww3 = get<2>(tgf_weights.at(0));
  xx1 = ww1*input;
  xx2 = ww3*input;
  get<0>(node_output.at(0)) = input;
  get<1>(node_output.at(0)) = input;
  for(ii=1; ii<num_layers-1; ii++){
    xx1 = get<0>(node_output.at(ii-1));
    xx2 = get<1>(node_output.at(ii-1));
    ss1 = ww1*xx1 + ww2*xx2;
    ss2 = callGenerator(actseq[ii-1],ww3*xx1 + ww4*xx2);
    if(ss2 < 0){
      cout<<"Warning: "<<ss2<<","<<ww3<<","<<ww4<<","<<xx1<<","<<xx2<<" actseq[ii-1} "
	  <<ii-1<<","<<actseq[ii-1]<<endl;
    }
    get<0>(node_output.at(ii)) = ss1;
    get<1>(node_output.at(ii)) = ss2;
    ww1 = get<0>(tgf_weights.at(ii));
    ww2 = get<1>(tgf_weights.at(ii));
    //cout<<"(ww1,ww2,xx1,xx2)="<<"("<<ww1<<","<<ww2<<",";
    ww3 = get<2>(tgf_weights.at(ii));
    ww4 = get<3>(tgf_weights.at(ii));
  }
  xx1 = get<0>(node_output.at(ii-1));
  xx2 = get<1>(node_output.at(ii-1));
  ss1 = ww1*xx1 + ww2*xx2;
  ss2 = callGenerator(actseq[ii-1],ww3*xx1 + ww4*xx2);
  get<0>(node_output.at(ii)) = ss1;
  get<1>(node_output.at(ii)) = ss2;
  ww1 = get<0>(tgf_weights.at(ii));
  ww2 = get<1>(tgf_weights.at(ii));
  get<0>(node_output.at(ii+1)) = ww1*ss1 + ww2*ss2;
  //cout<<"output "<<ww1*ss1+ww2*ss2<<endl;
  //cout<<"Weights 2 "<<ww1<<","<<get<0>(tgf_weights.at(tgf_weights.size()-1))<<endl;

  return(ww1*ss1 + ww2*ss2);
}

int GanitaZeroTGF::writeNetwork(void)
{
  GanitaGraphFile ggf;
  GanitaGraph gg;
  uint64_t ii;
  
  gg.addNode1(0,"input");
  gg.returnNode(0)->addEdge(0,2);
  gg.returnNode(0)->addEdge(1,3);
  for(ii=1; ii<num_layers-1; ii++){
    gg.addNode1(2*ii-1,"hidden");
    gg.returnNode(2*ii-1)->addEdge(4*ii, 2*(ii+1));
    gg.returnNode(2*ii-1)->addEdge(4*ii+1,2*ii+3);
    gg.addNode1(2*ii,"hidden");
    gg.returnNode(2*ii)->addEdge(4*ii+2, 2*ii+2);
    gg.returnNode(2*ii)->addEdge(4*ii+3, 2*ii+3);
  }
  gg.addNode1(2*ii-1,"hidden");
  gg.returnNode(2*ii-1)->addEdge(4*ii, 2*ii+2);
  gg.returnNode(2*ii-1)->addEdge(4*ii+1, 2*ii+2);
  gg.addNode1(2*ii,"hidden");
  gg.addNode1(2*ii+1,"output");

  ggf.writeVisTables(&gg);

  return(1);
}

// This is the first algorithm for implementing back propagation 
// on the TGF-based neural network. 
// The cost function used is C = 0.5*(out1 - yy)^2. 
// A standard update of weights has been tried using gradient descent. 
// Also tried taking the inverse of TGF activation slopes in computation of gradients.
// Since the functions are homeomorphisms, then TGF slopes are positive and do not vanish. 
int GanitaZeroTGF::backPropTGF(double xx, double yy)
{
  double out1, out2;
  double ww1, ww2, ww3, ww4;  // weights
  double eta, diff, tw;       // step size, error, sum of two weights
  double pc1, pc2, pc3, pc4;  // store previous layers partial derivative of cost w/r output
  double qc1, qc2, qc3, qc4;  // partial derivative of cost w/r to output from nodes in layer
  double qw1, qw2, qw3, qw4;  // partial derivative of cost w/r to weights for nodes in layer
  eta = 5.0;                  // step size
  uint64_t ii;

  feedForwardInternalTGF(xx);
  out1 = get<0>(node_output.at(node_output.size()-1));
  diff = out1 - yy;
  
  ww1 = get<0>(tgf_weights.at(tgf_weights.size()-1));
  ww2 = get<1>(tgf_weights.at(tgf_weights.size()-1));
  cout<<"Weights 0: "<<ww2<<","<<get<0>(tgf_weights.at(tgf_weights.size()-1))<<endl;
  pc1 = diff*ww1; pc2 = diff*ww2;
  pc3 = 0; pc4 = 0;
  cout<<"xx="<<xx<<" yy="<<yy<<" Diff "<<diff<<endl;
  out1 = get<0>(node_output.at(node_output.size()-2));
  out2 = get<1>(node_output.at(node_output.size()-2));
  ww1 -= eta*diff*out1;
  ww2 -= eta*diff*out2;
  // normalize weights to be a convex combination
  if(ww1 <= 0){
    ww1 = 0; ww2 = 1;
  }
  else if(ww2 <= 0){
    ww2 = 0; ww1 = 1;
  }
  else{
    tw = ww1 + ww2;
    ww1 /= tw;
    ww2 /= tw;
  }
  get<0>(tgf_weights.at(tgf_weights.size()-1)) = ww1;
  get<1>(tgf_weights.at(tgf_weights.size()-1)) = ww2;
  cout<<"Weights 1 "<<ww2<<","<<get<0>(tgf_weights.at(tgf_weights.size()-1))<<endl;
  
  for(ii=2; ii<tgf_weights.size(); ii++){
    ww1 = get<0>(tgf_weights.at(tgf_weights.size()-ii));
    ww2 = get<1>(tgf_weights.at(tgf_weights.size()-ii));
    qc1 = (pc1 + pc3)*ww1;
    qc2 = (pc1 + pc3)*ww2;
    out1 = get<0>(node_output.at(node_output.size()-ii-1));
    out2 = get<1>(node_output.at(node_output.size()-ii-1));
    ww3 = get<2>(tgf_weights.at(tgf_weights.size()-ii));
    ww4 = get<3>(tgf_weights.at(tgf_weights.size()-ii));
    qc3 = (pc2 + pc4)*callGeneratorSlope(actseq[tgf_weights.size()-ii],ww3*out1 + ww4*out2)*ww3;
    qc4 = (pc2 + pc4)*callGeneratorSlope(actseq[tgf_weights.size()-ii],ww3*out1 + ww4*out2)*ww4;
    //qc3 = (pc2 + pc4)*ww3/callGeneratorSlope(actseq[tgf_weights.size()-ii],ww3*out1 + ww4*out2);
    //qc4 = (pc2 + pc4)*ww4/callGeneratorSlope(actseq[tgf_weights.size()-ii],ww3*out1 + ww4*out2);
    
    qw1 = (qc1 + qc3)*out1; qw2 = (qc1 + qc3)*out2; 
    qw3 = (qc2 + qc4)*callGeneratorSlope(actseq[tgf_weights.size()-ii],ww3*out1 + ww4*out2)*out1;
    qw4 = (qc2 + qc4)*callGeneratorSlope(actseq[tgf_weights.size()-ii],ww3*out1 + ww4*out2)*out2;
    //qw3 = (qc2 + qc4)*out1/callGeneratorSlope(actseq[tgf_weights.size()-ii],ww3*out1 + ww4*out2);
    //qw4 = (qc2 + qc4)*out2/callGeneratorSlope(actseq[tgf_weights.size()-ii],ww3*out1 + ww4*out2);
    ww1 -= eta*qw1; ww2 -= eta*qw2; ww3 -= eta*qw3; ww4 -= eta*qw4;
    // normalize weights to be a convex combination
    if(ww1 <= 0){
      ww1 = 0; ww2 = 1;
    }
    else if(ww2 <= 0){
      ww2 = 0; ww1 = 1;
    }
    else{
      tw = ww1 + ww2;
      ww1 /= tw;
      ww2 /= tw;
    }
    if(ww4 <= 0){
      ww4 = 0; ww3 = 1;
    }
    else if(ww3 <= 0){
      ww3 = 0; ww4 = 1;
    }
    else{
      tw = ww3 + ww4;
      ww3 /= tw;
      ww4 /= tw;
    }
    get<0>(tgf_weights.at(tgf_weights.size()-ii)) = ww1;
    get<1>(tgf_weights.at(tgf_weights.size()-ii)) = ww2;
    get<2>(tgf_weights.at(tgf_weights.size()-ii)) = ww3;
    get<3>(tgf_weights.at(tgf_weights.size()-ii)) = ww4;
    pc1 = qc1; pc2 = qc2; pc3 = qc3; pc4 = qc4;
  }
  ww1 = get<0>(tgf_weights.at(0));
  ww3 = get<2>(tgf_weights.at(0));
  qc1 = (pc1 + pc3)*ww1;
  out1 = get<0>(node_output.at(0));
  qc2 = (pc2 + pc4)*ww3;
  //qc3 = (pc2 + pc4)*callGenerator(actseq[0],ww3*out1 + ww4*out2)*ww3;
    
  qw1 = qc1*out1; qw3 = qc2*out1;
  ww1 -= eta*qw1; ww3 -= eta*qw3;
  if(ww3 <= 0){
    ww1 = 1; ww3 = 0;
  }
  else if(ww1 <= 0){
    ww1 = 0; ww3 = 1;
  }
  else{
    tw = ww1 + ww3;
    ww1 /= tw;
    ww3 /= tw;
  }
  get<0>(tgf_weights.at(0)) = ww1;
  get<2>(tgf_weights.at(0)) = ww3;

  return(1);
}

int GanitaZeroTGF::flipWeight(void)
{
  if(get<0>(tgf_weights.at(tgf_weights.size()-2)) == 0){
    get<0>(tgf_weights.at(tgf_weights.size()-2)) = 1;
    get<1>(tgf_weights.at(tgf_weights.size()-2)) = 0;
  }
  else{
    get<0>(tgf_weights.at(tgf_weights.size()-2)) = 0;
    get<1>(tgf_weights.at(tgf_weights.size()-2)) = 1;
  }
  if(get<2>(tgf_weights.at(tgf_weights.size()-2)) == 0){
    get<2>(tgf_weights.at(tgf_weights.size()-2)) = 1;
    get<3>(tgf_weights.at(tgf_weights.size()-2)) = 0;
  }
  else{
    get<2>(tgf_weights.at(tgf_weights.size()-2)) = 0;
    get<3>(tgf_weights.at(tgf_weights.size()-2)) = 1;
  }

  return(1);
}

