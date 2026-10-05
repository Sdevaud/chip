#ifndef FRACTAL_FXPT_H
#define FRACTAL_FXPT_H

#include <stdint.h>

//! Colour type (5-bit red, 6-bit green, 5-bit blue)
typedef uint16_t rgb565;

typedef int32_t fxpt_t_Q2_30;

static inline fxpt_t_Q2_30 f_to_fxpt_Q2_30(float x) {
    return (fxpt_t_Q2_30)(x * 1073741824.0f);
}
//! \brief Pointer to fractal point calculation function
typedef uint16_t (*calc_frac_point_p)(fxpt_t_Q2_30 cx, fxpt_t_Q2_30 cy, uint16_t n_max);

uint16_t calc_mandelbrot_point_soft(fxpt_t_Q2_30 cx, fxpt_t_Q2_30 cy, uint16_t n_max);

//! Pointer to function mapping iteration to colour value
typedef rgb565 (*iter_to_colour_p)(uint16_t iter, uint16_t n_max);

rgb565 iter_to_bw(uint16_t iter, uint16_t n_max);
rgb565 iter_to_grayscale(uint16_t iter, uint16_t n_max);
rgb565 iter_to_colour(uint16_t iter, uint16_t n_max);

void draw_fractal(rgb565 *fbuf, int width, int height,
                  calc_frac_point_p cfp_p, iter_to_colour_p i2c_p,
                  fxpt_t_Q2_30 cx_0, fxpt_t_Q2_30 cy_0, fxpt_t_Q2_30 delta, uint16_t n_max);

#endif // FRACTAL_FXPT_H
