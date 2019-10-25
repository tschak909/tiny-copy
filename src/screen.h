/**
 * Tiny Copy - a Sector Copy in 8K Cart form
 *
 * Author: Thomas Cherryhomes
 *  <thom.cherryhomes@gmail.com>
 *
 * Screen functions
 */

#ifndef SCREEN_H
#define SCREEN_H

#include <stdbool.h>

/**
 * Hilight/unhilight a line
 * v = pointer to 32 char video line
 * h = highlight or no.
 */
void screen_hilight(unsigned char* v, bool h);

/**
 * screen_cursor(v,cx,ox)
 * set cursor position
 * v = which field to change
 * cx = current cursor position
 * ox = old character position
 */
void screen_cursor(unsigned char* v, unsigned char cx, unsigned char ox);

/**
 * screen_setup(void)
 * Set up atari screen for program
 */
void screen_setup(void);

/**
 * screen_run(void)
 * Run the screen UI
 */
void screen_run(void);

#endif /* SCREEN_H */
