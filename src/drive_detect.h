/**
 * Routine to detect drive density/sector size
 */

#ifndef DRIVE_DETECT_H
#define DRIVE_DETECT_H

#include "percom.h"

/**
 * Detect drive sector size/density
 * by reading a non-boot sector
 * s = src drive number
 * d = dst drive number
 * ret = total # of sectors.
 */
unsigned short drive_detect(unsigned char s, unsigned char d, PercomBlock* pb);

#endif /* DRIVE_DETECT_H */
