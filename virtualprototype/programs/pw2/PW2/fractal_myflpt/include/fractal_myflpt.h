#ifndef FRACTAL_MYFLPT_H
#define FRACTAL_MYFLPT_H

#include <stdint.h>
#include <string.h>

//! Colour type (5-bit red, 6-bit green, 5-bit blue)
typedef uint16_t rgb565;

//! \brief Pointer to fractal point calculation function
typedef uint16_t (*calc_frac_point_p)(float cx, float cy, uint16_t n_max);

uint16_t calc_mandelbrot_point_soft(float cx, float cy, uint16_t n_max);

//! Pointer to function mapping iteration to colour value
typedef rgb565 (*iter_to_colour_p)(uint16_t iter, uint16_t n_max);

rgb565 iter_to_bw(uint16_t iter, uint16_t n_max);
rgb565 iter_to_grayscale(uint16_t iter, uint16_t n_max);
rgb565 iter_to_colour(uint16_t iter, uint16_t n_max);

void draw_fractal(rgb565 *fbuf, int width, int height,
                  calc_frac_point_p cfp_p, iter_to_colour_p i2c_p,
                  float cx_0, float cy_0, float delta, uint16_t n_max);


//=================== My function ==================//

typedef struct {
    int16_t mant;
    int16_t exp;
} Myfloat_16;

typedef struct {
    int32_t mant;
    int16_t exp;
} Myfloat_32;

typedef uint16_t (*calc_frac_point_16_p)(
    Myfloat_16 cx,
    Myfloat_16 cy,
    uint16_t n_max
);

typedef uint16_t (*calc_frac_point_32_p)(
    Myfloat_32 cx,
    Myfloat_32 cy,
    uint16_t n_max
);

uint16_t calc_mandelbrot_Myfloat_16(Myfloat_16 cx, Myfloat_16 cy, uint16_t n_max);
uint16_t calc_mandelbrot_Myfloat_32(Myfloat_32 cx, Myfloat_32 cy, uint16_t n_max);



void draw_fractal_16(rgb565 *fbuf, int width, int height,
                  calc_frac_point_16_p cfp_p, iter_to_colour_p i2c_p,
                  Myfloat_16 cx_0, Myfloat_16 cy_0, Myfloat_16 delta, uint16_t n_max);

void draw_fractal_32(rgb565 *fbuf, int width, int height,
                  calc_frac_point_32_p cfp_p, iter_to_colour_p i2c_p,
                  Myfloat_32 cx_0, Myfloat_32 cy_0, Myfloat_32 delta, uint16_t n_max);

Myfloat_16 div_Myfloat_16(Myfloat_16 a, Myfloat_16 b);
Myfloat_32 div_Myfloat_32(Myfloat_32 a, Myfloat_32 b);
Myfloat_16 float_to_Myfloat_16(float value);
float Myfloat_16_to_float(Myfloat_16 value);
Myfloat_32 float_to_Myfloat_32(float value);
float Myfloat_32_to_float(Myfloat_32 value);

#endif // FRACTAL_MYFLPT_H
