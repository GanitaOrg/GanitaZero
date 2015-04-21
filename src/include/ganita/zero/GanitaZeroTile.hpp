#include <cstdio>
#include <cstdint>

class GanitaZeroTile
{
private:
  uint64_t tile;
  int size;
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
  int returnSize(void);
  uint32_t returnNext(void);
  uint64_t getValue(void);
  uint64_t returnValue(void);
  int dumpTile(void);
};

