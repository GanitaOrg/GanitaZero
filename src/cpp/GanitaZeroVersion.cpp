// Methods for GanitaZeroVersion.
// Place to set version numbers. 

#include "ganita/zero/GanitaZeroVersion.hpp"

GanitaZeroVersion::GanitaZeroVersion(void)
{
  // This is the main required place to set version numbers. 
  version = "0.35.1";
}

std::string GanitaZeroVersion::returnVersion(void)
{
  return(version);
}

