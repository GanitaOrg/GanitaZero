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

