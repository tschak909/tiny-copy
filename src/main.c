/**
 * Tiny Copy - a Sector Copy in 8K Cart form
 *
 * Author: Thomas Cherryhomes
 *  <thom.cherryhomes@gmail.com>
 *
 * Main Functions
 */

unsigned char sector_buffer[512];

#include "screen.h"

void main(void)
{
  screen_setup();
  screen_run();
}
