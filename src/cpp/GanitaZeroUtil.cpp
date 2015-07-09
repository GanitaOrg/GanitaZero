#include "ganita/zero/GanitaZeroUtil.hpp"

namespace gzu
{
  bool gzCompareBlocks
  (const std::shared_ptr<GanitaZeroBlock> b1, 
   const std::shared_ptr<GanitaZeroBlock> b2)
  {
    return(b1->returnCount() < b2->returnCount());
  }
}

