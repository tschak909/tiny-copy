/**
 * Tiny Copy - a Sector Copy in 8K Cart form
 *
 * Author: Thomas Cherryhomes
 *  <thom.cherryhomes@gmail.com>
 *
 * Screen functions
 */

#include <atari.h>
#include <string.h>
#include "screen.h"

extern unsigned char sector_buffer[512];

#define DISPLAY_LIST 0x600

// TINY COPY V0.2 (mode 6)
static char screen_status[16]=
  {0,52,41,46,57,0,35,47,48,57,0,54,16,14,17,0};

// SOURCE DRIVE: 1 (mode 2)
static char screen_source_drive[32]=
  {0,0,0,0,0,0,0,0,0,51,47,53,50,35,37,0,36,50,41,54,37,26,0,17,0,0,0,0,0,0,0,0};

// DESTINATION DRIVE: 1 (mode 2)
static char screen_destination_drive[32]=
  {0,0,0,0,36,37,51,52,41,46,33,52,41,47,46,0,36,50,41,54,37,26,0,17,0,0,0,0,0,0,0,0};

// SECTOR SIZE: 128
static char screen_sector_size[32]=
  {0,0,0,0,0,0,0,0,0,0,51,37,35,52,47,50,0,51,41,58,37,26,0,17,18,24,0,0,0,0,0,0};

// STARTING READ SECTOR: _____
static char screen_starting_read_sector[32]=
  {0,51,52,33,50,52,41,46,39,0,50,37,33,36,0,51,37,35,52,47,50,26,0,63,63,63,63,63,0,0,0,0};

// ENDING READ SECTOR: _____
static char screen_ending_read_sector[32]=
  {0,0,0,37,46,36,41,46,39,0,50,37,33,36,0,51,37,35,52,47,50,26,0,63,63,63,63,63,0,0,0,0};

// STARTING WRITE SECTOR: _____
static char screen_starting_write_sector[32]=
  {51,52,33,50,52,41,46,39,0,55,50,41,52,37,0,51,37,35,52,47,50,26,0,63,63,63,63,63,0,0,0,0};

const void dlist=
  {
   DL_BLK8,
   DL_BLK8,
   DL_BLK8,
   DL_LMS(DL_CHR20x8x2),
   screen_status,
   DL_LMS(DL_CHR40x8x1),
   sector_buffer,
   DL_CHR40x8x1,
   DL_CHR40x8x1,
   DL_CHR40x8x1,
   DL_CHR40x8x1,
   DL_CHR40x8x1,
   DL_CHR40x8x1,
   DL_CHR40x8x1,
   DL_CHR40x8x1,
   DL_CHR40x8x1,
   DL_CHR40x8x1,
   DL_CHR40x8x1,
   DL_CHR40x8x1,
   DL_CHR40x8x1,
   DL_CHR40x8x1,
   DL_CHR40x8x1,
   DL_BLK3,
   DL_LMS(DL_CHR40x8x1),
   screen_source_drive,
   DL_CHR40x8x1,
   DL_BLK2,
   DL_CHR40x8x1,
   DL_BLK2,
   DL_CHR40x8x1,
   DL_CHR40x8x1,
   DL_CHR40x8x1,
   DL_JVB,
   DISPLAY_LIST
  };

/**
 * screen_setup(void)
 * Set up atari screen for program
 */
void screen_setup(void)
{
  OS.sdmctl=0x00; // Disable screen DMA while we set up dlist
  memcpy((void *)DISPLAY_LIST, &dlist, sizeof(dlist));
  OS.sdlst=(void *)DISPLAY_LIST;
  OS.sdmctl=0x21; // Enable display list dma; narrow playfield
}
