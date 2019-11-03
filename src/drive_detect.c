/**
 * Routine to detect drive density/sector size
 */

#include <atari.h>
#include <6502.h>
#include <stddef.h>
#include "drive_detect.h"
#include "sector.h"

#define DETECT_READ_SECTOR 4

extern unsigned char sector_buffer;

/**
 * Detect drive sector size/density
 * by reading a non-boot sector
 * Returns total # of sectors
 * s = src drive number
 * d = dest drive number
 * pb = The percom block to return
 */
unsigned short drive_detect(unsigned char s, unsigned char d, PercomBlock* pb)
{
  unsigned char status;
  struct regs r;
  
  OS.dcb.ddevic=0x31; // disk drives
  OS.dcb.dunit=s;     // unit #
  OS.dcb.dcomnd='R';  // Read Sector
  OS.dcb.dbuf=NULL;   // Point to sector buffer
  OS.dcb.dtimlo=0x0F; // Timeout
  OS.dcb.dstats=0x00; // tell SIO it's null op
  OS.dcb.dbyt=0;     // no buffer needed.
  OS.dcb.daux=4;     // Read first non-boot sector.

  r.pc=0xE459;
  _sys(&r);
  
  status=percom_get(s,pb);

  if (pb->reserved1==0xFF)
    {
      // If we get a default percom block, let's try
      // to read sector 1040 to detect a medium
      // density disk.

      status=sector_get(s,1040,128,&sector_buffer);

      if (status==0x01) // read successful.
	{
	  pb->sectors_per_track=26;
	  pb->density=4; // mfm
	}
    }

  if (status=0x01 && (s!=d))
    percom_set(d,pb); // TODO: get status?

  return (pb->num_tracks*pb->num_sides*pb->sectors_per_track);
}
