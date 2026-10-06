#include "fractal_myflpt.h"
#include <swap.h>

//! \brief  Mandelbrot fractal point calculation function
//! \param  cx    x-coordinate
//! \param  cy    y-coordinate
//! \param  n_max maximum number of iterations
//! \return       number of performed iterations at coordinate (cx, cy)
uint16_t calc_mandelbrot_point_soft(float cx, float cy, uint16_t n_max) {
  float x = cx;
  float y = cy;
  uint16_t n = 0;
  float xx, yy, two_xy;
  do {
    xx = x * x;
    yy = y * y;
    two_xy = 2 * x * y;

    x = xx - yy + cx;
    y = two_xy + cy;
    ++n;
  } while (((xx + yy) < 4) && (n < n_max));
  return n;
}

uint16_t calc_mandelbrot_Myfloat_16(Myfloat_16 cx, Myfloat_16 cy, uint16_t n_max) {
  Myfloat_16 x = cx;
  Myfloat_16 y = cy;
  uint16_t n = 0;

  int32_t xx_mant, yy_mant, two_xy_mant;
  int16_t xx_exp, yy_exp, two_xy_exp;

  int32_t radius2_mant;
  int16_t radius2_exp;

  int32_t mant;
  int32_t cx_ext;
  int32_t cy_ext;
  uint32_t mag;
  int16_t exp;
  int16_t diff;
  int msb;
  int shift;

  do {
    xx_exp = x.exp + x.exp;
    xx_mant = (int32_t)x.mant * x.mant;

    yy_exp = y.exp + y.exp;
    yy_mant = (int32_t)y.mant * y.mant;

    // xx + yy
    diff = xx_exp - yy_exp;
    if (diff > 0) {
      radius2_exp = xx_exp;
      if (diff >= 31) radius2_mant = xx_mant;   
      else radius2_mant = (yy_mant >> diff) + xx_mant;    
    }
    else {
      radius2_exp = yy_exp;
      if (-diff >= 31) radius2_mant = yy_mant;  
      else radius2_mant = (xx_mant >> (-diff)) + yy_mant;  
    }

    // check break condition
    ++n;
    if (n >= n_max) break;

    if (radius2_mant > 0) {
      if (radius2_exp >= 32) break;
      if (radius2_exp >= 2) {
        if ((uint32_t)radius2_mant >= (1u << (32 - radius2_exp)))
          break;
      }
    }

    two_xy_exp = x.exp + y.exp;
    two_xy_mant = x.mant * y.mant;
    ++two_xy_exp;

    // x = xx - yy
    diff = xx_exp - yy_exp;
    if (diff > 0) {
      exp = xx_exp;
      if (diff >= 31) mant = xx_mant;
      else mant = xx_mant - (yy_mant >> diff); 
    }
    else {
      exp = yy_exp;
      if (-diff >= 31) mant = -yy_mant;   
      else mant = (xx_mant >> (-diff)) - yy_mant;   
    }

    // x = xx - yy + cx;
    cx_ext = cx.mant * (1 << 15);
    diff = exp - cx.exp;

    if (diff > 0) {
      if (diff < 31) mant = mant + (cx_ext >> diff);  
    }
    else {
      if (-diff >= 31) mant = cx_ext;
      else mant = (mant >> (-diff)) + cx_ext;
      exp = cx.exp;
    }

    if (mant == 0) {
      x.mant = 0;
      x.exp = 0;
    }
    else {
      mag = (mant < 0) ? (~(uint32_t)mant + 1u) : (uint32_t)mant;
      msb = 31 - __builtin_clz(mag);
      shift = msb - 14;

      if (shift > 0) mag >>= shift;
      else if (shift < 0) mag <<= -shift;
        
      x.mant = (mant < 0) ? -(int32_t)mag : (int32_t)mag;
      x.exp = exp - 15 + shift;
    }

    // y = two_xy + cy;
    cy_ext = cy.mant * (1 << 15);
    diff = two_xy_exp - cy.exp;

    if (diff > 0) {
      exp = two_xy_exp;
      if (diff >= 31) mant = two_xy_mant;
      else mant = (cy_ext >> diff) + two_xy_mant;
    }
    else {
      exp = cy.exp;
      if (-diff >= 31) mant = cy_ext;
      else mant = (two_xy_mant >> (-diff)) + cy_ext;
    }

    if (mant == 0) {
      y.mant = 0;
      y.exp = 0;
    }
    else {
      mag = (mant < 0) ? (~(uint32_t)mant + 1u) : (uint32_t)mant;
      msb = 31 - __builtin_clz(mag);
      shift = msb - 14;

      if (shift > 0) mag >>= shift;
      else if (shift < 0) mag <<= -shift;
        
      y.mant = (mant < 0) ? -(int32_t)mag : (int32_t)mag;
      y.exp = exp - 15 + shift;
    }

  } while (1);
  return n;
}

uint16_t calc_mandelbrot_Myfloat_32(Myfloat_32 cx, Myfloat_32 cy, uint16_t n_max) {
  Myfloat_32 x = cx;
  Myfloat_32 y = cy;
  uint16_t n = 0;

  int64_t xx_mant, yy_mant, two_xy_mant;
  int16_t xx_exp, yy_exp, two_xy_exp;

  int64_t radius2_mant;
  int16_t radius2_exp;

  int64_t mant;
  int64_t cx_ext;
  int64_t cy_ext;
  uint64_t mag;
  int16_t exp;
  int16_t diff;
  int msb;
  int shift;

  do {
    xx_exp = x.exp + x.exp;
    xx_mant = (int64_t)x.mant * x.mant;

    yy_exp = y.exp + y.exp;
    yy_mant = (int64_t)y.mant * y.mant;

    // xx + yy
    diff = xx_exp - yy_exp;
    if (diff > 0) {
      radius2_exp = xx_exp;
      if (diff >= 63) radius2_mant = xx_mant;
      else radius2_mant = (yy_mant >> diff) + xx_mant; 
    }
    else {
      radius2_exp = yy_exp;
      if (-diff >= 63) radius2_mant = yy_mant;
      else radius2_mant = (xx_mant >> (-diff)) + yy_mant;
    }

    // break condition
    ++n;
    if (n >= n_max) break;

    if (radius2_mant > 0) {
      if (radius2_exp >= 64) break;
      if (radius2_exp >= 2) {
        if ((uint64_t)radius2_mant >= (1ULL << (64 - radius2_exp)))
          break;
      }
    }


    two_xy_exp = x.exp + y.exp;
    two_xy_mant = (int64_t)x.mant * y.mant;
    ++two_xy_exp;

    // x = xx - yy
    diff = xx_exp - yy_exp;
    if (diff > 0) {
      exp = xx_exp;
      if (diff >= 63) mant = xx_mant;
      else mant = xx_mant - (yy_mant >> diff);
    }
    else {
      exp = yy_exp;
      if (-diff >= 63) mant = -yy_mant;
      else mant = (xx_mant >> (-diff)) - yy_mant;   
    }

    // x = xx - yy + cx;
    cx_ext = (int64_t)cx.mant * (1LL << 31);
    diff = exp - cx.exp;

    if (diff > 0) {
      if (diff < 63) mant = mant + (cx_ext >> diff); 
    }
    else {
      if (-diff >= 63) mant = cx_ext;
      else mant = (mant >> (-diff)) + cx_ext;
      exp = cx.exp;
    }

    if (mant == 0) {
      x.mant = 0;
      x.exp = 0;
    }
    else {
      mag = (mant < 0) ? (~(uint64_t)mant + 1ULL) : (uint64_t)mant;
      msb = 63 - __builtin_clzll(mag);
      shift = msb - 30;

      if (shift > 0) mag >>= shift;
      else if (shift < 0) mag <<= -shift;
        
      x.mant = (mant < 0) ? -(int32_t)mag : (int32_t)mag;
      x.exp = exp - 31 + shift;
    }

    // y = two_xy + cy;
    cy_ext = (int64_t)cy.mant * (1LL << 31);
    diff = two_xy_exp - cy.exp;

    if (diff > 0) {
      exp = two_xy_exp;
      if (diff >= 63) mant = two_xy_mant; 
      else mant = (cy_ext >> diff) + two_xy_mant;
    }
    else {
      exp = cy.exp;
      if (-diff >= 63) mant = cy_ext; 
      else mant = (two_xy_mant >> (-diff)) + cy_ext;
    }

    if (mant == 0) {
      y.mant = 0;
      y.exp = 0;
    }
    else {
      mag = (mant < 0) ? (~(uint64_t)mant + 1ULL) : (uint64_t)mant;
      msb = 63 - __builtin_clzll(mag);
      shift = msb - 30;

      if (shift > 0) mag >>= shift;
      else if (shift < 0) mag <<= -shift;

      y.mant = (mant < 0) ? -(int32_t)mag : (int32_t)mag;
      y.exp = exp - 31 + shift;
    }

  } while (1);
  return n;
}


//! \brief  Map number of performed iterations to black and white
//! \param  iter  performed number of iterations
//! \param  n_max maximum number of iterations
//! \return       colour
rgb565 iter_to_bw(uint16_t iter, uint16_t n_max) {
  if (iter == n_max) {
    return 0x0000;
  }
  return 0xffff;
}


//! \brief  Map number of performed iterations to grayscale
//! \param  iter  performed number of iterations
//! \param  n_max maximum number of iterations
//! \return       colour
rgb565 iter_to_grayscale(uint16_t iter, uint16_t n_max) {
  if (iter == n_max) {
    return 0x0000;
  }
  uint16_t brightness = iter & 0xf;
  return swap_u16(((brightness << 12) | ((brightness << 7) | brightness<<1)));
}


//! \brief Calculate binary logarithm for unsigned integer argument x
//! \note  For x equal 0, the function returns -1.
int ilog2(unsigned x) {
  if (x == 0) return -1;
  int n = 1;
  if ((x >> 16) == 0) { n += 16; x <<= 16; }
  if ((x >> 24) == 0) { n += 8; x <<= 8; }
  if ((x >> 28) == 0) { n += 4; x <<= 4; }
  if ((x >> 30) == 0) { n += 2; x <<= 2; }
  n -= x >> 31;
  return 31 - n;
}


//! \brief  Map number of performed iterations to a colour
//! \param  iter  performed number of iterations
//! \param  n_max maximum number of iterations
//! \return colour in rgb565 format little Endian (big Endian for openrisc)
rgb565 iter_to_colour(uint16_t iter, uint16_t n_max) {
  if (iter == n_max) {
    return 0x0000;
  }
  uint16_t brightness = (iter&1)<<4|0xF;
  uint16_t r = (iter & (1 << 3)) ? brightness : 0x0;
  uint16_t g = (iter & (1 << 2)) ? brightness : 0x0;
  uint16_t b = (iter & (1 << 1)) ? brightness : 0x0;
  return swap_u16(((r & 0x1f) << 11) | ((g & 0x1f) << 6) | ((b & 0x1f)));
}

rgb565 iter_to_colour1(uint16_t iter, uint16_t n_max) {
  if (iter == n_max) {
    return 0x0000;
  }
  uint16_t brightness = ((iter&0x78)>>2)^0x1F;
  uint16_t r = (iter & (1 << 2)) ? brightness : 0x0;
  uint16_t g = (iter & (1 << 1)) ? brightness : 0x0;
  uint16_t b = (iter & (1 << 0)) ? brightness : 0x0;
  return swap_u16(((r & 0xf) << 12) | ((g & 0xf) << 7) | ((b & 0xf)<<1));
}

//! \brief  Draw fractal into frame buffer
//! \param  width  width of frame buffer
//! \param  height height of frame buffer
//! \param  cfp_p  pointer to fractal function
//! \param  i2c_p  pointer to function mapping number of iterations to colour
//! \param  cx_0   start x-coordinate
//! \param  cy_0   start y-coordinate
//! \param  delta  increment for x- and y-coordinate
//! \param  n_max  maximum number of iterations
void draw_fractal_16(rgb565 *fbuf, int width, int height,
                  calc_frac_point_16_p cfp_p, iter_to_colour_p i2c_p,
                  Myfloat_16 cx_0, Myfloat_16 cy_0, Myfloat_16 delta, uint16_t n_max) {
  rgb565 *pixel = fbuf;
  Myfloat_16 cy = cy_0;
  int16_t diff;
  int32_t mant;

  for (int k = 0; k < height; ++k) {
    Myfloat_16 cx = cx_0;
    for(int i = 0; i < width; ++i) {
      uint16_t n_iter = (*cfp_p)(cx, cy, n_max);
      rgb565 colour = (*i2c_p)(n_iter, n_max);
      *(pixel++) = colour;

      // cx += delta;
      diff = cx.exp - delta.exp;
      if (diff > 0) {
          if (diff >= 15) mant = cx.mant;   
          else mant = (int32_t)cx.mant + ((int32_t)delta.mant >> diff);   
        }
        else {
          cx.exp = delta.exp;
          if (-diff >= 15) mant = delta.mant;  
          else mant = ((int32_t)cx.mant >> (-diff)) + (int32_t)delta.mant;
        }

      if (mant == 0) {
        cx.mant = 0;
        cx.exp = 0;
      }
      else {
        uint32_t mag;
        int msb;
        int shift;
        mag = (mant < 0) ? (~(uint32_t)mant + 1u) : (uint32_t)mant;
        msb = 31 - __builtin_clz(mag);
        shift = msb - 14;

        if (shift > 0) mag >>= shift;
        else if (shift < 0) mag <<= -shift;

        cx.mant = (mant < 0) ? -(int32_t)mag : (int32_t)mag;
        cx.exp = cx.exp + shift;
      }
    }

    // cy += delta;
    diff = cy.exp - delta.exp;
    if (diff > 0) {
      if (diff >= 15) mant = cy.mant;   
      else mant = (int32_t)cy.mant + ((int32_t)delta.mant >> diff);  
    }
    else {
      cy.exp = delta.exp;
      if (-diff >= 15) mant = delta.mant;  
      else mant = ((int32_t)cy.mant >> (-diff)) + (int32_t)delta.mant;
    }

    if (mant == 0) {
      cy.exp = 0;
      cy.mant = 0;
    }
    else {
      uint32_t mag;
      int msb;
      int shift;
      mag = (mant < 0) ? (~(uint32_t)mant + 1u) : (uint32_t)mant;
      msb = 31 - __builtin_clz(mag);
      shift = msb - 14;

      if (shift > 0) mag >>= shift;
      else if (shift < 0) mag <<= -shift;

      cy.mant = (mant < 0) ? -(int32_t)mag : (int32_t)mag;
      cy.exp = cy.exp  + shift;
    }
  }
}

void draw_fractal_32(rgb565 *fbuf, int width, int height,
                  calc_frac_point_32_p cfp_p, iter_to_colour_p i2c_p,
                  Myfloat_32 cx_0, Myfloat_32 cy_0, Myfloat_32 delta, uint16_t n_max) {
  rgb565 *pixel = fbuf;
  Myfloat_32 cy = cy_0;
  int16_t diff;
  int64_t mant;

  for (int k = 0; k < height; ++k) {
    Myfloat_32 cx = cx_0;
    for(int i = 0; i < width; ++i) {
      uint16_t n_iter = (*cfp_p)(cx, cy, n_max);
      rgb565 colour = (*i2c_p)(n_iter, n_max);
      *(pixel++) = colour;

      // cx += delta;
      diff = cx.exp - delta.exp;
      if (diff > 0) {
        if (diff >= 31) mant = cx.mant;   
        else mant = (int64_t)cx.mant + ((int64_t)delta.mant >> diff);   
      }
      else {
        cx.exp = delta.exp;
        if (-diff >= 31) mant = delta.mant;  
        else mant = ((int64_t)cx.mant >> (-diff)) + (int64_t)delta.mant;
      }

      if (mant == 0) {
        cx.mant = 0;
        cx.exp = 0;
      }
      else {
        uint64_t mag;
        int msb;
        int shift;
        mag = (mant < 0) ? (~(uint64_t)mant + 1ULL) : (uint64_t)mant;
        msb = 63 - __builtin_clzll(mag);
        shift = msb - 30;

        if (shift > 0) mag >>= shift;
        else if (shift < 0) mag <<= -shift;

        cx.mant = (mant < 0) ? -(int32_t)mag : (int32_t)mag;
        cx.exp = cx.exp + shift;
      }
    }

    // cy += delta;
    diff = cy.exp - delta.exp;
    if (diff > 0) {
      if (diff >= 31) mant = cy.mant;   
      else mant = (int64_t)cy.mant + ((int64_t)delta.mant >> diff);  
    }
    else {
      cy.exp = delta.exp;
      if (-diff >= 31) mant = delta.mant;  
      else mant = ((int64_t)cy.mant >> (-diff)) + (int64_t)delta.mant;
    }

    if (mant == 0) {
      cy.mant = 0;
      cy.exp = 0;
    }
    else {
      uint64_t mag;
      int msb;
      int shift;
      mag = (mant < 0) ? (~(uint64_t)mant + 1ULL) : (uint64_t)mant;
      msb = 63 - __builtin_clzll(mag);
      shift = msb - 30;

      if (shift > 0) mag >>= shift;
      else if (shift < 0) mag <<= -shift;

      cy.mant = (mant < 0) ? -(int32_t)mag : (int32_t)mag;
      cy.exp += shift;
    }
  }
}

void draw_fractal(rgb565 *fbuf, int width, int height,
                  calc_frac_point_p cfp_p, iter_to_colour_p i2c_p,
                  float cx_0, float cy_0, float delta, uint16_t n_max) {
  rgb565 *pixel = fbuf;
  float cy = cy_0;
  for (int k = 0; k < height; ++k) {
    float cx = cx_0;
    for(int i = 0; i < width; ++i) {
      uint16_t n_iter = (*cfp_p)(cx, cy, n_max);
      rgb565 colour = (*i2c_p)(n_iter, n_max);
      *(pixel++) = colour;
      cx += delta;
    }
    cy += delta;
  }
}


Myfloat_16 div_Myfloat_16(Myfloat_16 a, Myfloat_16 b) {
  Myfloat_16 result;
  int32_t mant;
  uint32_t mag;

  if (a.mant == 0) {
    result.mant = 0;
    result.exp = 0;
    return result;
  }

  if (b.mant == 0) {
    result.mant = 0;
    result.exp = 0;
    return result;
  }

  mant = ((int32_t)a.mant * (1 << 15)) / b.mant;
  result.exp = a.exp - b.exp;

  mag = (mant < 0) ? (~(uint32_t)mant + 1u) : (uint32_t)mant;

  if (mag >= (1u << 15)) {
    mag >>= 1;
    ++result.exp;
  }

  result.mant = (mant < 0) ? -(int16_t)mag : (int16_t)mag;

  return result;
}

Myfloat_32 div_Myfloat_32(Myfloat_32 a, Myfloat_32 b) {
  Myfloat_32 result;
  int64_t mant;
  uint64_t mag;

  if (a.mant == 0) {
    result.mant = 0;
    result.exp = 0;
    return result;
  }

  if (b.mant == 0) {
    result.mant = 0;
    result.exp = 0;
    return result;
  }

  mant = ((int64_t)a.mant * (1LL << 31)) / b.mant;
  result.exp = a.exp - b.exp;

  mag = (mant < 0) ? (~(uint64_t)mant + 1ULL) : (uint64_t)mant;

  if (mag >= (1ULL << 31)) {
    mag >>= 1;
    ++result.exp;
  }

  result.mant = (mant < 0) ? -(int32_t)mag : (int32_t)mag;

  return result;
}

Myfloat_16 float_to_Myfloat_16(float value) {
  Myfloat_16 result;

  union {
    float f;
    uint32_t u;
  } conv;

  conv.f = value;

  if ((conv.u & 0x7FFFFFFF) == 0) {
    result.mant = 0;
    result.exp = 0;
    return result;
  }

  uint32_t sign = conv.u >> 31;
  uint32_t frac = conv.u & 0x7FFFFF;
  int16_t exp = ((conv.u >> 23) & 0xFF) - 127;

  uint32_t mant = (1u << 23) | frac;

  mant >>= 9;

  result.mant = sign ? -(int16_t)mant : (int16_t)mant;
  result.exp = exp + 1;

  return result;
}

float Myfloat_16_to_float(Myfloat_16 value) {
  if (value.mant == 0)
    return 0.0f;

  uint32_t sign = value.mant < 0;
  uint32_t mant = value.mant < 0 ? -(int32_t)value.mant : value.mant;

  int16_t exp = value.exp - 1;

  uint32_t frac = (mant << 9) & 0x7FFFFF;
  uint32_t E = exp + 127;

  union {
    float f;
    uint32_t u;
  } conv;

  conv.u = (sign << 31) | (E << 23) | frac;

  return conv.f;
}

Myfloat_32 float_to_Myfloat_32(float value) {
  Myfloat_32 result;

  union {
    float f;
    uint32_t u;
  } conv;

  conv.f = value;

  if ((conv.u & 0x7FFFFFFF) == 0) {
    result.mant = 0;
    result.exp = 0;
    return result;
  }

  uint32_t sign = conv.u >> 31;
  uint32_t frac = conv.u & 0x7FFFFF;
  int16_t exp = ((conv.u >> 23) & 0xFF) - 127;

  uint32_t mant = (1u << 23) | frac;

  mant <<= 7;

  result.mant = sign ? -(int32_t)mant : (int32_t)mant;
  result.exp = exp + 1;

  return result;
}

float Myfloat_32_to_float(Myfloat_32 value) {
  if (value.mant == 0)
    return 0.0f;

  uint32_t sign = value.mant < 0;
  uint32_t mant = value.mant < 0 ? -(uint32_t)value.mant : (uint32_t)value.mant;

  int16_t exp = value.exp - 1;

  uint32_t frac = (mant >> 7) & 0x7FFFFF;
  uint32_t E = exp + 127;

  union {
    float f;
    uint32_t u;
  } conv;

  conv.u = (sign << 31) | (E << 23) | frac;

  return conv.f;
}