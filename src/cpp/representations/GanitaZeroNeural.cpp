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
  cout<<".8751568"<<","<<convertFromWeight(.8751568)<<","
      <<convertToWeight(convertFromWeight(.8751568))<<endl;
  setBinaryWeights(); dumpWeights();
  feedForwardTGF(.50);
  writeNetwork();

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
  myran = arc4random_uniform(2);
  if(myran == 1){
    layer[0]->returnNode(0)->setEdgeValue(0, weight_denom);
  }
  else{
    layer[0]->returnNode(0)->setEdgeValue(1, weight_denom);
  }

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
  uint64_t ii, jj;
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

