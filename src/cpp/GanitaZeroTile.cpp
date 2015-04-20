#include "ganita/zero/GanitaZeroTile.hpp"

GanitaZeroTile::GanitaZeroTile(void)
{
  tile = 0;
  size = 0;
}

GanitaZeroTile::GanitaZeroTile(int len)
{
  tile = 0;
  size = len;
}

GanitaZeroTile::GanitaZeroTile(uint64_t tt, int len)
{
  tile = tt;
  size = len;
}

int GanitaZeroTile::set(uint64_t tt, int len)
{
  tile = tt;
  size = len;
  return(len);
}

uint64_t GanitaZeroTile::getTile(void)
{
  return(tile);
}

int GanitaZeroTile::returnSize(void)
{
  return(size);
}

