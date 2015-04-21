#include "ganita/zero/GanitaZeroTile.hpp"

GanitaZeroTile::GanitaZeroTile(void)
{
  tile = 0;
  size = 0;
  value = 0;
  next = 0;
}

GanitaZeroTile::GanitaZeroTile(int len)
{
  tile = 0;
  size = len;
  value = 0;
  next = 0;
}

GanitaZeroTile::GanitaZeroTile(uint64_t tt, int len)
{
  tile = tt;
  size = len;
  value = 0;
  next = 0;
}

int GanitaZeroTile::set(uint64_t tt, int len)
{
  tile = tt;
  size = len;
  return(len);
}

int GanitaZeroTile::set(uint64_t tt, int len, uint64_t val)
{
  tile = tt;
  size = len;
  value = val;
  return(len);
}

int GanitaZeroTile::setValue(uint64_t val)
{
  value = val;
  return(size);
}

int GanitaZeroTile::setNext(uint32_t nn)
{
  next = nn;
  return(size);
}

uint64_t GanitaZeroTile::getTile(void)
{
  return(tile);
}

int GanitaZeroTile::returnSize(void)
{
  return(size);
}

uint32_t GanitaZeroTile::returnNext(void)
{
  return(next);
}

uint64_t GanitaZeroTile::getValue(void)
{
  return(value);
}

uint64_t GanitaZeroTile::returnValue(void)
{
  return(value);
}

int GanitaZeroTile::dumpTile(void)
{
  int ii;
  fprintf(stdout, "Tile: ");
  for(ii=0; ii<size; ii++){
    fprintf(stdout, "%01lX", (tile>>ii) & 0x1);
  }
  fprintf(stdout, " %d, %ld", size,value);
  return(1);
}

