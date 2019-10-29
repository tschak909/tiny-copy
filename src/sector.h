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
 * Return sector shift value given size
 */
void sector_set_shift(unsigned short z);

/**
 * Get sector, cram into sector_buffer
 * returns pointer to a percom block
 * d = Drive number.
 * s = sector #
 * z = sector size
 */
unsigned char sector_get(unsigned char d, unsigned short s, unsigned short z, unsigned char* buf);

/**
 * Put sector from sector_buffer
 * d = drive number
 * s = sector #
 * z = sector size
 */
unsigned char sector_put(unsigned char d, unsigned short s, unsigned short z, unsigned char* buf);

/**
 * Return max number of sectors available
 */
unsigned char sector_get_pass_info(unsigned short sectors_to_copy);

#endif /* PERCOM_H */ 
