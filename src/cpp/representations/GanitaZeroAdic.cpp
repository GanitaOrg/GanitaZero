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
}

int GanitaZeroAdic::read(std::ifstream &gzt_file)
{
  if(!gzt_file.is_open()){
    return(-1);
  }
  return(1);
}

