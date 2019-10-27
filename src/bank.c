/**
 * Tiny Copy - a Sector Copy in 8K Cart form
 *
 * Author: Thomas Cherryhomes
 *  <thom.cherryhomes@gmail.com>
 *
 * Bank switch function
 */

#include <atari.h>
#include "bank.h"

extern unsigned char banks[64];

/**
 * Select desired bank.
 * b = bank # to swap into $4000-$7FFF
 */
void bank_select(unsigned char b)
{
  PIA.portb=banks[b];
}

/**
 * Get # of sectors available in lower 64K
 * z = sector size
 */
unsigned short lomem_num_sectors(unsigned short z)
{
  return ((OS.memtop - 0x1000)/z);
}
