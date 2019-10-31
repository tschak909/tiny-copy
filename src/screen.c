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
#include <stdbool.h>
#include <stdlib.h>
#include <conio.h>
#include "screen.h"
#include "sector.h"
#include "percom.h"
#include "drive_detect.h"

unsigned char source_drive=1;
unsigned char destination_drive=1;
unsigned short sector_size=128;
unsigned short starting_read_sector=1;
unsigned short ending_read_sector=720;
unsigned short starting_write_sector=1;

extern unsigned char sector_buffer[512];

#define DISPLAY_LIST 0x600
#define SetChar(v,x,a) v[x]=a;
#define CURSOR_BEGIN_X 23
#define STATUS_NUM_X   10

// TINY COPY V0.2 (mode 6)
static unsigned char screen_status[16]=
  {0,52,41,46,57,0,35,47,48,57,0,54,16,14,17,0};

/* // READING: # */
/* static unsigned char screen_status_reading[16]= */
/*   {0,50,37,34,36,41,46,39,0,3,0,0,0,0,0,0}; */

/* // WRITING: # */
/* static unsigned char screen_status_writing[16]= */
/*   {0,55,50,41,52,41,46,39,0,3,0,0,0,0,0,0}; */

/* // FORMATTING */
/* static unsigned char screen_status_formatting[16]= */
/*   {0,0,0,38,47,50,45,33,52,52,41,46,39,0,0,0}; */

/* // ERROR */
/* static unsigned char screen_status_error[16]= */
/*   {0,0,0,37,50,50,47,50,0,0,0,0,0,0,0,0}; */

// SOURCE DRIVE: 1 (mode 2)
static unsigned char screen_source_drive[32]=
  {0,0,0,0,0,0,0,0,0,51,47,53,50,35,37,0,36,50,41,54,37,26,0,17,0,0,0,0,0,0,0,0};

// DESTINATION DRIVE: 1 (mode 2)
static unsigned char screen_destination_drive[32]=
  {0,0,0,0,36,37,51,52,41,46,33,52,41,47,46,0,36,50,41,54,37,26,0,17,0,0,0,0,0,0,0,0};

// SECTOR SIZE: 128
static unsigned char screen_sector_size[32]=
  {0,0,0,0,0,0,0,0,0,0,51,37,35,52,47,50,0,51,41,58,37,26,0,17,18,24,0,0,0,0,0,0};

// STARTING READ SECTOR: _____
static unsigned char screen_starting_read_sector[32]=
  {0,51,52,33,50,52,41,46,39,0,50,37,33,36,0,51,37,35,52,47,50,26,0,63,63,63,63,63,0,0,0,0};

// ENDING READ SECTOR: _____
static unsigned char screen_ending_read_sector[32]=
  {0,0,0,37,46,36,41,46,39,0,50,37,33,36,0,51,37,35,52,47,50,26,0,63,63,63,63,63,0,0,0,0};

// STARTING WRITE SECTOR: _____
static unsigned char screen_starting_write_sector[32]=
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

// Cursor position on line.
unsigned char cx,ox;

/**
 * Hilight/unhilight a line
 * v = pointer to 32 char video line
 * h = highlight or no.
 */
void screen_hilight(unsigned char* v, bool h)
{
  unsigned char i;
  for (i=0;i<32;i++)
    {
      if (h)
	v[i]|=0x80; // Enable inverse.
      else
	v[i]&=0x7F; // disable inverse.
    }
}

/**
 * screen_num(v,n)
 * update screen with new number
 */
void screen_num(unsigned char* v, long n, unsigned char pos)
{
  char tmp[6];
  unsigned char i;

  // Convert to string.
  utoa(n,tmp,10);

  // Convert string to screen code
  for (i=0;i<6;i++)
    {
      if (tmp[i]==0)
	break;
      else
	v[pos+i]=tmp[i]-32;
    }
}

/**
 * Get screen input, with default value
 * v = which field to set
 * i = which var to set
 * l = length of field
 * d = default value
 * ret = arrow key, return, or esc code
 */
unsigned char screen_input_byte(unsigned char* v, unsigned char* i, unsigned char l, unsigned short d)
{
  unsigned char pos;
  unsigned char tmp[6];
  unsigned char k;
  bool done;
  
  // Set up default value
  utoa(d,tmp,10);
  screen_num(v,d,CURSOR_BEGIN_X);
  
  // first highlight the field
  screen_hilight(v,1);

  // Set cursor to beginning of field.
  cx=ox=CURSOR_BEGIN_X;
  screen_cursor(v);

  pos=0;
  done=false;
  
  while (done==false)
    {
      k=cgetc();
      switch(k)
	{
	case 0x1C: // Arrow keys.
	case 0x1D:
	case 0x1E:
	case 0x1F:
	  done=true;
	  break;
	case 0x9B:  // RETURN
	  done=true;
	  break;
	case 0x7E:  // BS
	  if (l>0)
	    {
	      tmp[pos]=0;
	      SetChar(v,CURSOR_BEGIN_X+pos,0);
	      pos--;
	    }
	case 0x30:
	case 0x31:
	case 0x32:
	case 0x33:
	case 0x34:
	case 0x35:
	case 0x36:
	case 0x37:
	case 0x38:
	case 0x39:
	  tmp[pos]=k;
	  SetChar(v,CURSOR_BEGIN_X+pos,k-32);
	  pos++;
	}
      if (pos>=l)
	done=true;
    }
  
  // Convert string to target int
  (unsigned char)i=atoi(tmp);
  
  // then unhighlight the field
  screen_hilight(v,0);

  // Return last key pressed.
  return k;
}

/**
 * screen_cursor(v,cx,ox)
 * set cursor position
 * v = which field to change
 * cx = current cursor position
 * ox = old character position
 */
void screen_cursor(unsigned char* v)
{
  if (ox!=cx)
    v[ox]^=0x80;
  v[cx]^=0x80;
}

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

/**
 * Update status bar
 * msg = pointer to msg to copy into status bar
 * num = number value to copy into status bar.
 */
void screen_status_set(unsigned char* msg, unsigned short num)
{
  memcpy(screen_status,msg,16);
  screen_num(screen_status,num,STATUS_NUM_X);
}

/**
 * Display output from percom block
 */
void screen_percom_block(PercomBlock* pb)
{
  screen_num(screen_sector_size,pb->sector_size, CURSOR_BEGIN_X);
  screen_num(screen_starting_read_sector,1, CURSOR_BEGIN_X);
  screen_num(screen_ending_read_sector,ending_read_sector, CURSOR_BEGIN_X);
  screen_num(screen_starting_write_sector,1, CURSOR_BEGIN_X);
}

/**
 * screen_run(void)
 * Run the screen UI
 */
void screen_run(void)
{
  unsigned char cf=0;
  unsigned char ready=false;
  unsigned char k;
  
  while (ready==false)
    {
      switch(cf)
	{
	case 0:
	  k=screen_input_byte(screen_source_drive,&source_drive,1,1);
	  break;
	case 1:
	  k=screen_input_byte(screen_destination_drive,&destination_drive,1,1);
	  break;
	}
      
      switch(k)
	{
	case 0x1C:
	case 0x1E:
	  if (cf==1)
	    cf--;
	  break;
	case 0x1D:
	case 0x1F:
	  if (cf==0)
	    cf++;
	  break;
	default:
	  if (cf==0)
	    cf++;
	  else
	    ready=true;
	}
      
    }
  
  for (;;) {}
  
}
