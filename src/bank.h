/**
 * Tiny Copy - a Sector Copy in 8K Cart form
 *
 * Author: Thomas Cherryhomes
 *  <thom.cherryhomes@gmail.com>
 *
 * Bank switch function
 */

#ifndef BANK_H
#define BANK_H

/**
 * Select desired bank.
 * b = bank # to swap into $4000-$7FFF
 */
void bank_select(unsigned char b);

/**
 * Get # of sectors available in lower 64K
 * z = sector size
 */
unsigned short lomem_num_sectors(unsigned short z);

#endif /* BANK_H */
