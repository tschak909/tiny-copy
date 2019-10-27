/**
 * Tiny Copy - a Sector Copy in 8K Cart form
 *
 * Author: Thomas Cherryhomes
 *  <thom.cherryhomes@gmail.com>
 *
 * Sector get/put functions
 */

#include <atari.h>
#include <6502.h>
#include "sector.h"
#include "bank.h"

extern unsigned char sector_buffer[512];

unsigned char sector_shift; // Sector shift value.

/**
 * Return sector shift value given size
 */
void sector_set_shift(unsigned short z)
{
  switch(z)
    {
    case 128:
      sector_shift=7;
      break;
    case 256:
      sector_shift=8;
      break;
    case 512:
      sector_shift=9;
      break;
    }
}

/**
 * Get sector, cram into sector_buffer
 * returns pointer to a percom block
 * d = Drive number.
 * s = sector #
 * z = sector size
 */
unsigned char sector_get(unsigned char d, unsigned short s, unsigned short z, unsigned char* buf)
{
  struct regs r;

  OS.dcb.ddevic=0x31; // disk drives
  OS.dcb.dunit=d;     // unit #
  OS.dcb.dcomnd='R';  // Read Sector
  OS.dcb.dbuf=buf;   // Point to sector buffer
  OS.dcb.dtimlo=0x0F; // Timeout
  OS.dcb.dstats=0x40; // tell SIO it's a read.

  if (s<4)
    OS.dcb.dbyt=128;  // First 3 sectors are always 128 bytes
  else
    OS.dcb.dbyt=z;    // grab a sector's worth

  OS.dcb.daux=s;      // Sector #
  
  // Call SIOV
  r.pc=0xE459;
  _sys(&r);

  return OS.dcb.dstats;  
}

/**
 * Put sector from sector_buffer
 * d = drive number
 * s = sector #
 * z = sector size
 */
unsigned char sector_put(unsigned char d, unsigned short s, unsigned short z, unsigned char* buf)
{
  struct regs r;

  OS.dcb.ddevic=0x31; // disk drives
  OS.dcb.dunit=d;     // unit #
  OS.dcb.dcomnd='P';  // Put Sector
  OS.dcb.dbuf=buf;   // Point to sector buffer
  OS.dcb.dtimlo=0x0F; // Timeout
  OS.dcb.dstats=0x80; // tell SIO it's a write.
  OS.dcb.dbyt=z;     // grab a sector's worth
  OS.dcb.daux=s;      // Sector #
  
  // Call SIOV
  r.pc=0xE459;
  _sys(&r);

  return OS.dcb.dstats;
}

