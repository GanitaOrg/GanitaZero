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
unsigned long GanitaZeroAdic::addStage(void)
{
  GanitaGraphStage *ggs = new GanitaGraphStage();
  stage.push_back(std::make_shared<GanitaGraphStage>(*ggs));
  delete ggs;
  return(stage.size());
}

// Add stage and set id and value
unsigned long GanitaZeroAdic::addStage(unsigned long ii, unsigned long val)
{
  GanitaGraphStage *ggs = new GanitaGraphStage();
  stage.push_back(std::make_shared<GanitaGraphStage>(*ggs));
  stage[stage.size()-1]->set(ii,val);
  delete ggs;
  return(stage.size());
}

int GanitaZeroAdic::readFixed(std::ifstream &gzt_file)
{
  string line, token;
  unsigned long ii, jj;
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
  unsigned long ii, jj;
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

unsigned long GanitaZeroAdic::dumpStageSize(unsigned long ss)
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

unsigned long GanitaZeroAdic::dumpStage(unsigned long ss)
{
  unsigned long ii;
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

GanitaGraphStage *GanitaZeroAdic::returnStage(unsigned long ss)
{
  if(stage.size() <= 0){
    // no stages yet
    return(NULL);
  }

  return(stage[ss % stage.size()].get());
}

unsigned long GanitaZeroAdic::returnNumStages(void)
{
  return((unsigned long)stage.size());
}

