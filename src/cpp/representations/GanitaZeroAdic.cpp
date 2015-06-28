// Methods for GanitaZeroAdic class. 

GanitaZeroAdic::GanitaZeroAdic(void)
{
  name = "adic";
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
  num_fixed_levels = 0;
  repeat_pattern_length = 0;
  stage_delim = ' ';
  node_delim = ' ';
  edge_delim = ',';
}

// Add blank stage
uint64_t GanitaZeroAdic::addStage(void)
{
  GanitaGraph *ggs = new GanitaGraph();
  stage.push_back(std::make_shared<GanitaGraph>(*ggs));
  delete ggs;
  return(stage.size());
}

// Add stage and set id and value
uint64_t GanitaZeroAdic::addStage(uint64_t ii, uint64_t val)
{
  GanitaGraph *ggs = new GanitaGraph();
  stage.push_back(std::make_shared<GanitaGraph>(*ggs));
  stage[stage.size()-1]->set(ii,val);
  delete ggs;
  return(stage.size());
}

int GanitaZeroAdic::readFixed(std::ifstream &gzt_file)
{
  string line, token;
  uint64_t ii, jj;
  if(!gzt_file.is_open()){
    return(-1);
  }
  if(!std::getline(gzt_file,line)){
    return(-1);
  }
  sscanf(line.c_str(),"num-fixed-levels %ld", &num_fixed_levels);
  //cout<<"num-fixed-levels: "<<num_fixed_levels<<endl;
  // read in num-nodes line
  if(!std::getline(gzt_file,line)){
    return(-1);
  }
  for(ii=1; ii<num_fixed_levels; ii++){
    // read in each edge-definition line
    if(!std::getline(gzt_file,line)){
      return(-1);
    }
    //cout<<line<<endl;
    // Add stage
    addStage();
    std::istringstream is(line);
    // first token is string edge-definition
    std::getline(is, token, stage_delim);
    //cout<<token<<endl;
    jj = 0;
    while(std::getline(is, token, stage_delim)) {
      //cout<<token<<endl;
      stage[stage.size()-1]->addNode(token);
      jj++;
    }
  }

  return(1);
}

int GanitaZeroAdic::readRepeat(std::ifstream &gzt_file)
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
  sscanf(line.c_str(),"repeat-pattern-length %ld", &repeat_pattern_length);
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
    // Add stage
    addStage();
    std::istringstream is(line);
    // first token is string repeat-definition
    std::getline(is, token, stage_delim);
    //cout<<token<<endl;
    jj = 0;
    while(std::getline(is, token, stage_delim)) {
      //cout<<token<<endl;
      stage[stage.size()-1]->addNode(token);
      jj++;
    }
  }

  return(1);
}

uint64_t GanitaZeroAdic::dumpStageSize(uint64_t ss)
{
  if(stage.size() == 0){
    // no stages added yet
    cout<<"No stages added yet."<<endl;
    return(0);
  }
  ss = ss % stage.size();
  cout<<"Number of nodes          "<<stage[ss]->returnNumNodes()<<endl;
  return(stage[ss]->returnNumNodes());
}

uint64_t GanitaZeroAdic::dumpStage(uint64_t ss)
{
  uint64_t ii;
  if(stage.size() == 0){
    // no stages added yet
    cout<<"No stages added yet."<<endl;
    return(0);
  }
  ss = ss % stage.size();
  cout<<"Number of nodes          "<<stage[ss]->returnNumNodes()<<endl;
  for(ii=0; ii<stage[ss]->returnNumNodes(); ii++){
    stage[ss]->returnNode(ii)->dumpEdgeValues();
    cout<<" | ";
  }
  cout<<endl;
  return(stage[ss]->returnNumNodes());
}

GanitaGraph *GanitaZeroAdic::returnStage(uint64_t ss)
{
  if(stage.size() <= 0){
    // no stages yet
    return(NULL);
  }

  return(stage[ss % stage.size()].get());
}

uint64_t GanitaZeroAdic::returnNumStages(void)
{
  return((uint64_t)stage.size());
}

int GanitaZeroAdic::buildBase(void)
{
  string label;
  if(stage.size() > 0){
    cout<<"Already added graph."<<endl;
    return(-1);
  }
  addStage();
  cout<<"Number of stages: "<<stage.size()<<endl;
  label.assign("root");
  cout<<label<<endl;
  stage[0]->addNode1(0, label);
  label.assign("obs 0");
  stage[0]->addNode1(0, label);
  label.assign("obs 1");
  stage[0]->addNode1(1, label);
  GanitaNode *gn;
  gn = stage[0]->returnNode(0);
  gn->addEdge(2);
  gn->addEdge(3);

  return(3);
}

int GanitaZeroAdic::extendStage(GanitaZeroTile *mytile)
{
  uint64_t num = stage[0]->returnNumNodes();
  if(num < 3){
    cout<<"Did not add base nodes."<<endl;
    return(-1);
  }

  stage[0]->addNode1(num, "hidden");
  stage[0]->addNode1(num + 1, "hidden");
  cout<<"Number of nodes: "<<stage[0]->returnNumNodes()<<endl;

  uint64_t pat = mytile->getTile();
  int len = mytile->returnSize();
  GanitaNode *gn1, *gn2;
  gn1 = stage[0]->returnNode(num-2);
  gn2 = stage[0]->returnNode(num-1);
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

