#include "fractal_fxpt.h"
#include "swap.h"
#include "vga.h"
#include "cache.h"

#include <stddef.h>
#include <stdio.h>

// Constants describing the output device
const int SCREEN_WIDTH = 512;   //!< screen width
const int SCREEN_HEIGHT = 512;  //!< screen height

// Constants describing the initial view port on the fractal function
const uint16_t N_MAX = 64;    //!< maximum number of iterations
const   fract_t_Q3_29 frac_width_fxpt, cx_0_fxpt, cy_0_fxpt;
frac_width_fxpt = f_to_fxpt_Q3_29(3.0);
cx_0_fxpt = f_to_fxpt_Q3_29(-2.0);
cy_0_fxpt = f_to_fxpt_Q3_29(-1.5);
int main() {
   volatile unsigned int *vga = (unsigned int *) 0x50000020;
   volatile unsigned int reg, hi;
 
   fxpt_t_Q3_29 delta_fxpt = frac_width_fxpt / SCREEN_WIDTH;
   rgb565 frameBuffer[SCREEN_WIDTH*SCREEN_HEIGHT];
   int i;
   vga_clear();
   printf("Starting drawing a fractal\n");

#ifdef __OR1300__   
   /* enable the caches */
   icache_write_cfg( CACHE_DIRECT_MAPPED | CACHE_SIZE_8K | CACHE_REPLACE_FIFO );
   dcache_write_cfg( CACHE_FOUR_WAY | CACHE_SIZE_8K | CACHE_REPLACE_LRU | CACHE_WRITE_BACK );
   icache_enable(1);
   dcache_enable(1);
#endif
   /* Enable the vga-controller's graphic mode */
   vga[0] = swap_u32(SCREEN_WIDTH);
   vga[1] = swap_u32(SCREEN_HEIGHT);
   vga[2] = swap_u32(1);
   vga[3] = swap_u32((unsigned int)&frameBuffer[0]);
   /* Clear screen */
   for (i = 0 ; i < SCREEN_WIDTH*SCREEN_HEIGHT ; i++) frameBuffer[i]=0;

   draw_fractal(frameBuffer,SCREEN_WIDTH,SCREEN_HEIGHT,&calc_mandelbrot_point_soft, &iter_to_colour,cx_0_fxpt,cy_0_fxpt,delta_fxpt,N_MAX);
#ifdef __OR1300__
   dcache_flush();
#endif
   printf("Done\n");
}
