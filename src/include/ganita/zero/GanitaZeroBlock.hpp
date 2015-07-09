#ifndef _GANITAZEROBLOCK_
#define _GANITAZEROBLOCK_

#include <cstdint>

#ifndef __APPLE__
#include <bsd/stdlib.h>
#endif

using namespace std;

class GanitaZeroBlock
{
private:
  uint64_t id;
  uint64_t start;
  uint64_t end;
  uint64_t total;
  uint64_t count;
public:
  uint64_t returnId(void);
  uint64_t returnStart(void);
  uint64_t returnEnd(void);
  uint64_t returnTotal(void);
  uint64_t returnCount(void);
  uint64_t setId(uint64_t);
  uint64_t setStart(uint64_t);
  uint64_t setEnd(uint64_t);
  uint64_t setTotal(uint64_t);
  uint64_t setCount(uint64_t);
};

#endif
