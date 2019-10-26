/**
 * Routine to detect drive density/sector size
 */

#include "drive_detect.h"
#include "sector.h"

#define DETECT_READ_SECTOR 4

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

  status=sector_get(s,DETECT_READ_SECTOR,128);
  status=percom_get(s,pb);

  if (status=0x01)
    percom_set(d,pb); // TODO: get status?

  return (pb->num_tracks*pb->num_sides*pb->sectors_per_track);
}
