#ifndef FRACTAL_FXPT_H
#define FRACTAL_FXPT_H

#include <stdint.h>

//! scale factor for Q3.29 fixed point representation
#define SCALE_Q3_29 (1LL << 29) 

//! Colour type (5-bit red, 6-bit green, 5-bit blue)
typedef uint16_t rgb565;

//! Fixed point type for Q3.29 representation
typedef int32_t fxpt_t_Q3_29;

//! Float to fixed point conversion for Q3.29 
fxpt_t_Q3_29 f_to_fxpt_Q3_29(float x);

//! \brief Pointer to fractal point calculation function
typedef uint16_t (*calc_frac_point_p)(fxpt_t_Q3_29 cx, fxpt_t_Q3_29 cy, uint16_t n_max);

uint16_t calc_mandelbrot_point_soft(fxpt_t_Q3_29 cx, fxpt_t_Q3_29 cy, uint16_t n_max);

//! Pointer to function mapping iteration to colour value
typedef rgb565 (*iter_to_colour_p)(uint16_t iter, uint16_t n_max);

rgb565 iter_to_bw(uint16_t iter, uint16_t n_max);
rgb565 iter_to_grayscale(uint16_t iter, uint16_t n_max);
rgb565 iter_to_colour(uint16_t iter, uint16_t n_max);

void draw_fractal(rgb565 *fbuf, int width, int height,
                  calc_frac_point_p cfp_p, iter_to_colour_p i2c_p,
                  fxpt_t_Q3_29 cx_0, fxpt_t_Q3_29 cy_0, fxpt_t_Q3_29 delta, uint16_t n_max);

#endif // FRACTAL_FXPT_H
