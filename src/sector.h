/**
 * Tiny Copy - a Sector Copy in 8K Cart form
 *
 * Author: Thomas Cherryhomes
 *  <thom.cherryhomes@gmail.com>
 *
 * Sector get/put functions
 */

#ifndef SECTOR_H
#define SECTOR_H

/**
 * Get sector, cram into sector_buffer
 * returns pointer to a percom block
 * d = Drive number.
 * s = sector #
 * z = sector size
 */
unsigned char sector_get(unsigned char d, unsigned short s, unsigned short z);

/**
 * Put sector from sector_buffer
 * d = drive number
 * s = sector #
 * z = sector size
 */
unsigned char sector_put(unsigned char d, unsigned short s, unsigned short z);

#endif /* PERCOM_H */ 
