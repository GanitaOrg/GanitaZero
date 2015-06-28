#ifndef _GANITAZEROTILE_
#define _GANITAZEROTILE_

#include <cstdio>
#include <cstdint>

class GanitaZeroTile
{
private:
  uint64_t tile;
  uint32_t size;
  uint64_t value;
  uint32_t next;
public:
  GanitaZeroTile(void);
  GanitaZeroTile(int len);
  GanitaZeroTile(uint64_t tt, int len);
  int set(uint64_t tt, int len);
  int set(uint64_t tt, int len, uint64_t val);
  int setValue(uint64_t val);
  int setNext(uint32_t nn);
  uint64_t getTile(void);
  uint32_t returnSize(void);
  uint32_t returnNext(void);
  uint64_t getValue(void);
  uint64_t returnValue(void);
  int dumpTile(void);
  int getBit(int loc);
  uint64_t setBit(int loc, int bit);
};

#endif
