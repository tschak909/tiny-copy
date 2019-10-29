/**
 * Tiny Copy - a Sector Copy in 8K Cart form
 *
 * Author: Thomas Cherryhomes
 *  <thom.cherryhomes@gmail.com>
 *
 * Sector copy functions
 */

#ifndef SECTOR_COPY_H
#define SECTOR_COPY_H

typedef struct _SectorCopyParams
{
  unsigned char drive_source;
  unsigned char drive_destination;
  unsigned short sector_size;
  unsigned short starting_read_sector;
  unsigned short ending_read_sector;
  unsigned short starting_write_sector;
} SectorCopyParams;

typedef struct _SectorPool
{
  unsigned short himem;
  unsigned short lomem;
  unsigned char sector_shift;
  unsigned char himem_banks;
} SectorPool;

/**
 * return number of passes needed
 */
unsigned char sector_copy_get_pass_info(unsigned short sectors_to_copy);

/**
 * Sector copy read pass
 */
unsigned char sector_copy_read(unsigned char pass);

/**
 * Sector copy write pass
 */
unsigned char sector_copy_write(unsigned char pass);

/**
 * Perform sector copy
 * s = source drive
 * d = dest drive
 * z = sector size
 * rb = beginning read sector
 * re = ending read sector
 * wb = beginning write sector
 */
unsigned char sector_copy(unsigned char s, unsigned char d, unsigned char z, unsigned short rb, unsigned short re, unsigned short wb);

#endif /* SECTOR_COPY_H */
