// Methods for GanitaZeroNeural class. 

GanitaZeroNeural::GanitaZeroNeural(void)
{
  name = "neural";
  type.push_back("repeat");
  type.push_back("recursive");
  type.push_back("fixed");
  keys.push_back("num-fixed-levels");
  keys.push_back("num-nodes");
  keys.push_back("edge-definition");
  keys.push_back("repeat-pattern-length");
  keys.push_back("repeat-nodes");
  keys.push_back("repeat-definition");
  keys.push_back("done");
  num_layers = 0;
  repeat_pattern_length = 0;
  layer_delim = ' ';
  node_delim = ' ';
  edge_delim = ',';
  weight_denom = 0x1<<31;
}

// Add blank layer
uint64_t GanitaZeroNeural::addLayer(void)
{
  GanitaGraph *ggs = new GanitaGraph();
  layer.push_back(std::make_shared<GanitaGraph>(*ggs));
  delete ggs;
  return(layer.size());
}

// Add layer and set id and value
uint64_t GanitaZeroNeural::addLayer(uint64_t ii, uint64_t val)
{
  GanitaGraph *ggs = new GanitaGraph();
  layer.push_back(std::make_shared<GanitaGraph>(*ggs));
  layer[layer.size()-1]->set(ii,val);
  delete ggs;
  return(layer.size());
}

int GanitaZeroNeural::readFixed(std::ifstream &gzt_file)
{
  string line, token;
  uint64_t ii, jj;
  if(!gzt_file.is_open()){
    return(-1);
  }
  if(!std::getline(gzt_file,line)){
    return(-1);
  }
  sscanf(line.c_str(),"num-fixed-levels %lld", (unsigned long long *) &num_layers);
  //cout<<"num-fixed-levels: "<<num_layers<<endl;
  // read in num-nodes line
  if(!std::getline(gzt_file,line)){
    return(-1);
  }
  for(ii=1; ii<num_layers; ii++){
    // read in each edge-definition line
    if(!std::getline(gzt_file,line)){
      return(-1);
    }
    //cout<<line<<endl;
    // Add layer
    addLayer();
    std::istringstream is(line);
    // first token is string edge-definition
    std::getline(is, token, layer_delim);
    //cout<<token<<endl;
    jj = 0;
    while(std::getline(is, token, layer_delim)) {
      //cout<<token<<endl;
      layer[layer.size()-1]->addNode(token);
      jj++;
    }
  }

  return(1);
}

int GanitaZeroNeural::readRepeat(std::ifstream &gzt_file)
{
  string line, token;
  uint64_t ii, jj;
  if(!gzt_file.is_open()){
    return(-1);
  }
  if(!std::getline(gzt_file,line)){
    return(-1);
  }
  // Read in length of repeating pattern
  sscanf(line.c_str(),"repeat-pattern-length %lld", (unsigned long long *) &repeat_pattern_length);
  //cout<<"repeat-pattern-length: "<<repeat_pattern_length<<endl;
  for(ii=0; ii<repeat_pattern_length; ii++){
    // read in repeat-nodes line
    if(!std::getline(gzt_file,line)){
      return(-1);
    }
    // read in each repeat-definition line
    if(!std::getline(gzt_file,line)){
      return(-1);
    }
    //cout<<line<<endl;
    // Add layer
    addLayer();
    std::istringstream is(line);
    // first token is string repeat-definition
    std::getline(is, token, layer_delim);
    //cout<<token<<endl;
    jj = 0;
    while(std::getline(is, token, layer_delim)) {
      //cout<<token<<endl;
      layer[layer.size()-1]->addNode(token);
      jj++;
    }
  }

  return(1);
}

uint64_t GanitaZeroNeural::dumpLayerSize(uint64_t ss)
{
  if(layer.size() == 0){
    // no layers added yet
    cout<<"No layers added yet."<<endl;
    return(0);
  }
  ss = ss % layer.size();
  cout<<"Number of nodes          "<<layer[ss]->returnNumNodes()<<endl;
  return(layer[ss]->returnNumNodes());
}

uint64_t GanitaZeroNeural::dumpLayer(uint64_t ss)
{
  uint64_t ii;
  if(layer.size() == 0){
    // no layers added yet
    cout<<"No layers added yet."<<endl;
    return(0);
  }
  ss = ss % layer.size();
  cout<<"Number of nodes          "<<layer[ss]->returnNumNodes()<<endl;
  for(ii=0; ii<layer[ss]->returnNumNodes(); ii++){
    layer[ss]->returnNode(ii)->dumpEdgeValues();
    cout<<" | ";
  }
  cout<<endl;
  return(layer[ss]->returnNumNodes());
}

GanitaGraph *GanitaZeroNeural::returnLayer(uint64_t ss)
{
  if(layer.size() <= 0){
    // no layers yet
    return(NULL);
  }

  return(layer[ss % layer.size()].get());
}

uint64_t GanitaZeroNeural::returnNumLayers(void)
{
  return((uint64_t)layer.size());
}

int GanitaZeroNeural::buildBase(void)
{
  string label;
  if(layer.size() > 0){
    cout<<"Already added graph."<<endl;
    return(-1);
  }
  addLayer();
  cout<<"Number of layers: "<<layer.size()<<endl;
  label.assign("root");
  cout<<label<<endl;
  layer[0]->addNode1(0, label);
  label.assign("obs 0");
  layer[0]->addNode1(0, label);
  label.assign("obs 1");
  layer[0]->addNode1(1, label);
  GanitaNode *gn;
  gn = layer[0]->returnNode(0);
  gn->addEdge(2);
  gn->addEdge(3);

  return(3);
}

int GanitaZeroNeural::extendLayer(GanitaZeroTile *mytile)
{
  uint64_t num = layer[0]->returnNumNodes();
  if(num < 3){
    cout<<"Did not add base nodes."<<endl;
    return(-1);
  }

  layer[0]->addNode1(num, "hidden");
  layer[0]->addNode1(num + 1, "hidden");
  cout<<"Number of nodes: "<<layer[0]->returnNumNodes()<<endl;

  uint64_t pat = mytile->getTile();
  int len = mytile->returnSize();
  GanitaNode *gn1, *gn2;
  gn1 = layer[0]->returnNode(num-2);
  gn2 = layer[0]->returnNode(num-1);
  cout<<"Adding edges to nodes: "<<num-2<<", "<<num-1<<endl;
  for(int ii=0; ii<len; ii++){
    if(((pat >> ii) & 0x1) == 0){
      gn1->addEdge(num+1);
    }
    else gn2->addEdge(num+1);
  }
  gn2->addEdge(num+2);

  return(1);
}

int GanitaZeroNeural::createTGF(uint64_t numlayers)
{
  uint64_t ii;
  double myran;

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
  std::cout << tgf_generator_bkpts[0][1] << std::endl;
  std::cout << callGenerator(2,.3) << std::endl;
  generateRandActSeq(num_layers+2);
  dumpActSeq();
  cout<<composeTGF(0.30)<<endl;
  setBinaryWeights(); dumpWeights();
  copyInternalTGF(.50);
  for(ii=0; ii<1000; ii++){
    myran = ((double) arc4random_uniform(100000))/100000.0;
    cout<<"ii="<<ii<<"|";
    backPropTGF(myran, callGenerator(0,myran));
    feedForwardInternalTGF(myran);
    backPropTGF(myran, callGenerator(0,myran));
    feedForwardInternalTGF(myran);
  }
  //writeNetwork();

  return(1);
}

// Break points on [0,1] for two generators 
// of the Thompson Group F and their inverses.
// First two arrays are for generators a and b.
// Second two arrays are for inverses a^{-1}, b^{-1}. 
const double GanitaZeroNeural::tgf_generator_bkpts[][6] = 
  {{0, .25, .50, 1, 2, 3}, {0, .50, .625, .75, 1, 2}, 
   {0, .50, .75, 1, 2, 3}, {0, .50, .75, .875, 1, 2}};

// Slopes of piecewise linear generator functions for 
// the Thompson Group F that will be used as activation 
// functions for a deep neural network. 
const double GanitaZeroNeural::tgf_generator_fun[4][4][2] = {
  {{2,0},{1,.25}, {.50,.50},{-1,-1}}, 
  {{1,0},{2,-.50},{1,.125}, {.50,.50}},
  {{.50,0},{1,-.25},{2,-1},{-1,-1}},
  {{1,0},{.50,.25},{1,-.125},{2,-1}}
};

double GanitaZeroNeural::callGenerator(int fn, double input)
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

double GanitaZeroNeural::callGeneratorSlope(int fn, double input)
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

int GanitaZeroNeural::generateRandActSeq(uint64_t len)
{
  uint32_t myran, skip;
  uint64_t ii;

  if(len < 1){
    return(-1);
  }

  myran = arc4random_uniform(4);
  actseq.push_back(myran);
  for(ii=1; ii<len; ii++){
    skip = (myran + 2) % 4;
    myran = arc4random_uniform(3);
    if(skip <= myran){
      myran++;
    }
    actseq.push_back(myran);
  }

  return(1);
}

int GanitaZeroNeural::dumpActSeq(void)
{
  uint64_t ii;
  for(ii=0; ii<actseq.size(); ii++){
    cout<<actseq[ii]<<",";
  }
  cout<<endl;

  return(1);
}

double GanitaZeroNeural::composeTGF(double input)
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

double GanitaZeroNeural::convertToWeight(uint64_t val)
{
  long double tstore;
  tstore = val;
  tstore /= weight_denom;

  return((double) tstore);
}

uint64_t GanitaZeroNeural::convertFromWeight(double ww)
{
  return((uint64_t) (weight_denom*ww));
}

int GanitaZeroNeural::setBinaryWeights(void)
{
  uint32_t myran, ii;
  
  if(layer.size() < 3){
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

  for(ii=1; ii<layer.size()-1; ii++){
    myran = arc4random_uniform(2);
    if(myran == 1){
      layer[ii]->returnNode(0)->setEdgeValue(0, weight_denom);
    }
    else{
      layer[ii]->returnNode(1)->setEdgeValue(0, weight_denom);
    }
    myran = arc4random_uniform(2);
    if(myran == 1){
      layer[ii]->returnNode(0)->setEdgeValue(1, weight_denom);
    }
    else{
      layer[ii]->returnNode(1)->setEdgeValue(1, weight_denom);
    }
  }
  myran = arc4random_uniform(2);
  if(myran == 1){
    layer[layer.size()-1]->returnNode(0)->setEdgeValue(0, weight_denom);
  }
  else{
    layer[layer.size()-1]->returnNode(1)->setEdgeValue(0, weight_denom);
  }

  return(1);
}

int GanitaZeroNeural::dumpWeights(void)
{
  uint64_t ii, jj;
  
  for(ii=0; ii<layer.size(); ii++){
    for(jj=0; jj<layer[ii]->returnNumNodes(); jj++){
      cout<<"Layer "<<ii<<" Node "<<jj<<" Edges ";
      layer[ii]->returnNode(jj)->dumpEdgeAll();
    }
  }

  return(1);
}

int GanitaZeroNeural::feedForwardTGF(double input)
{
  uint64_t ii;
  double ww1,ww2;
  GanitaEdge ed;
  double xx, xx1, xx2;
  
  if(layer.size() < 2){
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
  for(ii=1; ii<layer.size()-1; ii++){
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

int GanitaZeroNeural::copyInternalTGF(double input)
{
  uint64_t ii;
  double ww1,ww2, ww3, ww4;
  GanitaEdge ed;
  double xx1, xx2, xx3, xx4;
  double ss1, ss2;
  
  if(layer.size() < 2){
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
  for(ii=1; ii<layer.size()-1; ii++){
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

int GanitaZeroNeural::feedForwardInternalTGF(double input)
{
  uint64_t ii;
  double ww1,ww2, ww3, ww4;
  GanitaEdge ed;
  double xx1, xx2;
  double ss1, ss2;
  
  if(layer.size() < 2){
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
  for(ii=1; ii<layer.size()-1; ii++){
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
    //cout<<"(ww1,ww2,xx1,xx2)="<<"("<<ww1<<","<<ww2<<","
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
  cout<<"output "<<ww1*ss1+ww2*ss2<<endl;
  //cout<<"Weights 2 "<<ww1<<","<<get<0>(tgf_weights.at(tgf_weights.size()-1))<<endl;

  return(1);
}

int GanitaZeroNeural::writeNetwork(void)
{
  GanitaGraphFile ggf;
  GanitaGraph gg;
  uint64_t ii;
  
  gg.addNode1(0,"input");
  gg.returnNode(0)->addEdge(0,2);
  gg.returnNode(0)->addEdge(1,3);
  for(ii=1; ii<layer.size()-1; ii++){
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

int GanitaZeroNeural::backPropTGF(double xx, double yy)
{
  double out1, out2;
  double ww1, ww2, ww3, ww4;
  GanitaEdge ed;
  double eta, diff, tw;
  double pc1, pc2, pc3, pc4;
  double qc1, qc2, qc3, qc4;
  double qw1, qw2, qw3, qw4;
  eta = 5.0;
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
    
    qw1 = (qc1 + qc3)*out1; qw2 = (qc1 + qc3)*out2; 
    qw3 = (qc2 + qc4)*callGeneratorSlope(actseq[tgf_weights.size()-ii],ww3*out1 + ww4*out2)*out1;
    qw4 = (qc2 + qc4)*callGeneratorSlope(actseq[tgf_weights.size()-ii],ww3*out1 + ww4*out2)*out2;
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

