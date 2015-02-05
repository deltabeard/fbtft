/*
 * FB driver for the R61581 LCD Controller
 *
 * Copyright (C) 2014 Noralf Tronnes
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include "fbtft.h"

#define DRVNAME        "fb_r61581"
#define WIDTH      320
#define HEIGHT     480


/* this init sequence matches R61581 */
static int default_init_sequence[] = {
   -1,0xB0, 0x00,
   -1,0x11,
   -2,250,
   -1,0xB3, 0x02, 0x00, 0x00, 0x00,
   -1,0xC0, 0x13, 0x3B, 0x00, 0x02, 0x00, 0x01, 0x00, 0x43,
   -1,0xC1, 0x08, 0x16, 0x08, 0x08,
   -1,0xC4, 0x11, 0x07, 0x03, 0x03,
   -1,0xC6, 0x00,
   -1,0xC8, 0x03, 0x03, 0x13, 0x5C, 0x03, 0x07, 0x14, 0x08,
            0x00, 0x21, 0x08, 0x14, 0x07, 0x53, 0x0C, 0x13,
            0x03, 0x03, 0x21, 0x00,
   -1,0x35, 0x00,
   -1,0x36, 0xa0,
   -1,0x3A, 0x55,
   -1,0x44, 0x00, 0x01,
   -1,0xD0, 0x07, 0x07, 0x1D, 0x03,
   -1,0xD1, 0x03, 0x30, 0x10,
   -1,0xD2, 0x03, 0x14, 0x04,
   -1,0x29,
   -1,0x2C,
   -3
};

static void set_addr_win(struct fbtft_par *par, int xs, int ys, int xe, int ye)
{
   fbtft_par_dbg(DEBUG_SET_ADDR_WIN, par,
       "%s(xs=%d, ys=%d, xe=%d, ye=%d)\n", __func__, xs, ys, xe, ye);

   /* Column address */
   write_reg(par, 0x2A, xs >> 8, xs & 0xFF, xe >> 8, xe & 0xFF);

   /* Row adress */
   write_reg(par, 0x2B, ys >> 8, ys & 0xFF, ye >> 8, ye & 0xFF);

   /* Memory write */
   write_reg(par, 0x2C);
}

static int set_var(struct fbtft_par *par)
{
   fbtft_par_dbg(DEBUG_INIT_DISPLAY, par, "%s()\n", __func__);

   switch (par->info->var.rotate) {
   case 0:
       write_reg(par, 0x36, 0x00 | (par->bgr << 3));
       break;
   case 90:
       write_reg(par, 0x36, 0x60 | (par->bgr << 3));
       break;
   case 180:
       write_reg(par, 0x36, 0xc0 | (par->bgr << 3));
       break;
   case 270:
       write_reg(par, 0x36, 0xA0 | (par->bgr << 3));
       break;
   default:
       break;
   }

   return 0;
}


static struct fbtft_display display = {
   .regwidth = 8,
   .width = WIDTH,
   .height = HEIGHT,
   .init_sequence = default_init_sequence,
   .fbtftops = {
       .set_addr_win = set_addr_win,
       .set_var = set_var,
   },
};
FBTFT_REGISTER_DRIVER(DRVNAME, "renesas,r61581", &display);

MODULE_ALIAS("spi:" DRVNAME);
MODULE_ALIAS("platform:" DRVNAME);
MODULE_ALIAS("spi:r61581");
MODULE_ALIAS("platform:r61581");

MODULE_DESCRIPTION("FB driver for the R61581 LCD Controller");
MODULE_AUTHOR("Noralf Tronnes");
MODULE_LICENSE("GPL");
