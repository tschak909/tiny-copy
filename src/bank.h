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

#endif /* BANK_H */
