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

uint32_t GanitaZeroTile::returnSize(void)
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
  uint32_t ii;
  fprintf(stdout, "Tile: ");
  for(ii=0; ii<size; ii++){
    fprintf(stdout, "%01lX", (tile>>ii) & 0x1);
  }
  fprintf(stdout, " %d, %ld", size,value);
  return(1);
}

int GanitaZeroTile::getBit(int loc)
{
  return((tile >> loc) & 0x1);
}

uint64_t GanitaZeroTile::setBit(int loc, int bit)
{
  uint64_t tb = ((tile >> loc) & 0x1);
  if((bit == 0) & (tb == 1)){
    tile = tile ^ (tb << loc);
  }
  else if((bit == 1) & (tb == 0)){
    tile = tile & (tb << loc);
  }

  return(tile);
}

