#ifndef _GANITAZEROUTIL_
#define _GANITAZEROUTIL_

#include <boost/shared_ptr.hpp>
#include "ganita/zero/GanitaZeroBlock.hpp"

namespace gzu
{
  bool gzCompareBlocks
  (const std::shared_ptr<GanitaZeroBlock> b1, const std::shared_ptr<GanitaZeroBlock> b2);
}

#endif
