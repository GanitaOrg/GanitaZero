#include <cstdio>
#include <cstdint>

class GanitaZeroTile
{
private:
  uint64_t tile;
  int size;
public:
  GanitaZeroTile(void);
  GanitaZeroTile(int len);
  GanitaZeroTile(uint64_t tt, int len);
  int set(uint64_t tt, int len);
  uint64_t getTile(void);
  int returnSize(void);
};

