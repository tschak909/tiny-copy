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

extern unsigned char sector_buffer[512];

/**
 * Get sector, cram into sector_buffer
 * returns pointer to a percom block
 * d = Drive number.
 * s = sector #
 * z = sector size
 */
unsigned char sector_get(unsigned char d, unsigned short s, unsigned short z)
{
  struct regs r;

  OS.dcb.ddevic=0x31; // disk drives
  OS.dcb.dunit=d;     // unit #
  OS.dcb.dcomnd='R';  // Read Sector
  OS.dcb.dbuf=&sector_buffer;   // Point to sector buffer
  OS.dcb.dtimlo=0x0F; // Timeout
  OS.dcb.dstats=0x40; // tell SIO it's a read.
  OS.dcb.dbyt=z;     // grab a sector's worth
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
unsigned char sector_put(unsigned char d, unsigned short s, unsigned short z)
{
  struct regs r;

  OS.dcb.ddevic=0x31; // disk drives
  OS.dcb.dunit=d;     // unit #
  OS.dcb.dcomnd='P';  // Put Sector
  OS.dcb.dbuf=&sector_buffer;   // Point to sector buffer
  OS.dcb.dtimlo=0x0F; // Timeout
  OS.dcb.dstats=0x80; // tell SIO it's a write.
  OS.dcb.dbyt=z;     // grab a sector's worth
  OS.dcb.daux=s;      // Sector #
  
  // Call SIOV
  r.pc=0xE459;
  _sys(&r);

  return OS.dcb.dstats;
}
