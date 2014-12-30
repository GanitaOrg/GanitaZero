// Methods for GanitaZeroVersion.
// Place to set version numbers. 

#include "GanitaZeroVersion.hpp"

GanitaZeroVersion::GanitaZeroVersion(void)
{
  // This is the main required place to set version numbers. 
  version = "0.1.1";
}

std::string GanitaZeroVersion::returnVersion(void)
{
  return(version);
}

