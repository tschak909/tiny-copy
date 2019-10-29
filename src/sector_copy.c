/**
 * Tiny Copy - a Sector Copy in 8K Cart form
 *
 * Author: Thomas Cherryhomes
 *  <thom.cherryhomes@gmail.com>
 *
 * Sector copy functions
 */

#include <atari.h>
#include "sector_copy.h"
#include "sector.h"
#include "bank.h"

static unsigned char dstats; // Status of last command.
static SectorPool pool;

/**
 * Initialize sector pool, called after UI and before sector copy start.
 * z = sector size.
 */
void sector_copy_init(unsigned short z)
{
  // sector shift (divisor of address to sector)
  switch(z)
    {
    case 128:
      pool.sector_shift=7;
      break;
    case 256:
      pool.sector_shift=8;
      break;
    case 512:
      pool.sector_shift=9;
      break;
    }

  // Fill in pool entries.
  pool.lomem=((unsigned short)OS.memtop - 0x1000) >> pool.sector_shift;
  pool.himem_banks=bank_detect();
  pool.himem=(pool.himem_banks*0x4000) >> pool.sector_shift;
}

/**
 * return number of passes needed
 */
unsigned char sector_copy_get_pass_info(unsigned short sectors_to_copy)
{
  unsigned short p;

  p=sectors_to_copy/(pool.himem+pool.lomem);

  if (p*(pool.himem+pool.lomem) < sectors_to_copy)
    p++;
  
  return p;
}

/**
 * Sector copy read pass
 */
unsigned char sector_copy_read(unsigned char pass)
{
  return dstats;
}

/**
 * Sector copy write pass
 */
unsigned char sector_copy_write(unsigned char pass)
{
  return dstats;
}

/**
 * Perform sector copy
 * s = source drive
 * d = dest drive
 * z = sector size
 * rb = beginning read sector
 * re = ending read sector
 * wb = beginning write sector
 */
unsigned char sector_copy(unsigned char s, unsigned char d, unsigned char z, unsigned short rb, unsigned short re, unsigned short wb)
{
  
  return dstats;
}
