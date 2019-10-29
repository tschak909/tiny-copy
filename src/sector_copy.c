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
static SectorCopyParams params;
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
      pool.bank_shift=7;
      pool.sector_shift=7;
      break;
    case 256:
      pool.bank_shift=6;
      pool.sector_shift=8;
      break;
    case 512:
      pool.bank_shift=5;
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
 * Return a pointer to the buffer for a given sector
 * within a copying pass.
 * s = sector
 * ret = pointer to buffer
 */
unsigned char* sector_to_buffer(unsigned short s)
{
  unsigned char* r;
  
  if (s<=pool.lomem)
    r=(unsigned char *)(0x1000 + (s << pool.sector_shift));
  else
    {
      bank_select(s >> pool.bank_shift);
      r=(unsigned char *)(0x4000 + ((s << pool.sector_shift)&0x7FFFF));
    }

  return r;
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
  params.drive_source=s;
  params.drive_destination=d;
  params.sector_size=z;
  params.starting_read_sector=rb;
  params.ending_read_sector=re;
  params.starting_write_sector=wb;
  sector_copy_init(z);
  
  return dstats;
}
