// Methods for GanitaZeroSchema

#include "GanitaZeroSchema.hpp"

GanitaZeroSchema::GanitaZeroSchema(void)
{
  gzst.push_back("ganitazero");
  gzst.push_back("transformation");
  gzst.push_back("name");
  gzst.push_back("representation");
  gzstr.push_back("adic");
  gzstr.push_back("symbolic");
  gzstr.push_back("tower");
  gzstr.push_back("map");
}

std::string GanitaZeroSchema::returnSchema(int index)
{
  return(gzst[index % gzst.size()]);
}

