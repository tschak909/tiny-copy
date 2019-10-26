/**
 * Tiny Copy - a Sector Copy in 8K Cart form
 *
 * Author: Thomas Cherryhomes
 *  <thom.cherryhomes@gmail.com>
 *
 * Percom block functions
 */

#include <atari.h>
#include <6502.h>
#include "percom.h"

extern unsigned char sector_buffer[512]; // Re-use sector buffer

/**
 * Get percom block
 * d = Drive number.
 * p = pointer to percom block.
 */
unsigned char percom_get(unsigned char d, PercomBlock* p)
{
  struct regs r; // For system call.

  OS.dcb.ddevic=0x31; // disk drives
  OS.dcb.dunit=d;     // unit #
  OS.dcb.dcomnd=0x4E; // Get PERCOM Block command.
  OS.dcb.dbuf=&sector_buffer;   // Point to sector buffer
  OS.dcb.dtimlo=0x0F; // Timeout
  OS.dcb.dstats=0x40; // tell SIO it's a read.
  OS.dcb.dbyt=12;     // Percom blocks are 12 bytes in length.

  // Call SIOV
  r.pc=0xE459;
  _sys(&r);

  // The data is now in the buffer, however, it needs to be massaged into the
  // data structure, because 16-bit quantities are in big-endian order,
  // and must be swapped.

  if (OS.dcb.dstats==0x01)
    {
      // Received a valid percom block, fill data structure.
      p->num_tracks=sector_buffer[0];
      p->step_rate=sector_buffer[1];
      p->sectors_per_track=(sector_buffer[2]<<8)+sector_buffer[3];
      p->num_sides=sector_buffer[4]+1;
      p->density=sector_buffer[5];
      p->sector_size=(sector_buffer[6]<<8)+sector_buffer[7];
      p->drive_present=sector_buffer[8];
    }
  else
    {
      percom_get_default_parameters(p);
    }

  return OS.dcb.dstats;
}

/**
 * Set percom block
 * d = drive number
 * p = pointer to percom block struct
 */
unsigned char percom_set(unsigned char d, PercomBlock* p)
{
  struct regs r;

  OS.dcb.ddevic=0x31; // disk drives
  OS.dcb.dunit=d;     // unit #
  OS.dcb.dcomnd=0x4F; // Get PERCOM Block command.
  OS.dcb.dbuf=&sector_buffer;   // Point to sector buffer
  OS.dcb.dtimlo=0x0F; // Timeout
  OS.dcb.dstats=0x80; // tell SIO it's a write.
  OS.dcb.dbyt=12;     // Percom blocks are 12 bytes in length.

  // take the percom data and swap 16 bit quantities.
  sector_buffer[0]=p->num_tracks;
  sector_buffer[1]=p->step_rate;
  sector_buffer[2]=p->sectors_per_track>>8;
  sector_buffer[3]=p->sectors_per_track&0xFF;
  sector_buffer[4]=p->num_sides-1;
  sector_buffer[5]=p->density;
  sector_buffer[6]=p->sector_size>>8;
  sector_buffer[7]=p->sector_size&0xFF;
  sector_buffer[8]=p->drive_present;

  // and send it to the drive.
  r.pc=0xE459;
  _sys(&r);

  return OS.dcb.dstats;
}

/**
 * Get default parameters
 * Returns a dummy percom block if drive does not
 * support the command, e.g. Atari 810/1050
 */
void percom_get_default_parameters(PercomBlock* pb)
{
  pb->num_tracks=40;
  pb->step_rate=1;
  pb->sectors_per_track=18;
  pb->num_sides=1;
  pb->density=0;
  pb->sector_size=128;
  pb->drive_present=0xFF; // really don't care if it is or not.
}
