// #include <stdio.h>
// #include <stdint.h>
// #include <math.h>
// #include <stdlib.h>

// typedef struct {
//   int32_t mant;
//   int16_t exp;
// } Myfloat_16;


// Myfloat_16 float_to_Myfloat_16(float value) {
//   Myfloat_16 result;

//   if (value == 0.0f) {
//     result.mant = 0;
//     result.exp = 0;
//     return result;
//   }

//   int exp;
//   float mant = frexpf(value, &exp);

//   result.mant = (int32_t)roundf(mant * (1 << 15));
//   result.exp = exp;

//   return result;
// }


// float Myfloat_16_to_float(Myfloat_16 value) {
//   return ((float)value.mant / (1 << 15)) * powf(2.0f, value.exp);
// }


// uint16_t calc_mandelbrot_point_soft(float cx, float cy, uint16_t n_max) {
//   float x = cx;
//   float y = cy;
//   uint16_t n = 0;
//   float xx, yy, two_xy;

//   do {
//     xx = x * x;
//     yy = y * y;
//     two_xy = 2 * x * y;

//     x = xx - yy + cx;
//     y = two_xy + cy;
//     ++n;
//   } while (((xx + yy) < 4) && (n < n_max));

//   return n;
// }


// uint16_t calc_mandelbrot_Myfloat_16(Myfloat_16 cx, Myfloat_16 cy, uint16_t n_max) {
//   Myfloat_16 x = cx;
//   Myfloat_16 y = cy;
//   uint16_t n = 0;
//   Myfloat_16 xx, yy, two_xy;
//   Myfloat_16 radius2;

//   int32_t mant;
//   int32_t cx_ext;
//   int32_t cy_ext;
//   uint32_t mag;
//   int16_t exp;
//   int16_t diff;
//   int msb;
//   int shift;

//   do {
//     xx.exp = x.exp + x.exp;
//     xx.mant = x.mant * x.mant;

//     yy.exp = y.exp + y.exp;
//     yy.mant = y.mant * y.mant;

//     // radius2
//     diff = xx.exp - yy.exp;
//     if (diff > 0) {
//       radius2.exp = xx.exp;
//       if (diff >= 31)
//         radius2.mant = xx.mant;
//       else
//         radius2.mant = (yy.mant >> diff) + xx.mant;
//     }
//     else {
//       radius2.exp = yy.exp;
//       if (-diff >= 31)
//         radius2.mant = yy.mant;
//       else
//         radius2.mant = (xx.mant >> (-diff)) + yy.mant;
//     }

//     ++n;

//     if (n >= n_max) break;
//     // check if xx +yy > 4
//     if (radius2.mant > 0) {
//       if (radius2.exp >= 32) break;

//       if (radius2.exp >= 2) {
//         if ((uint32_t)radius2.mant >= (1u << (32 - radius2.exp)))
//           break;
//       }
//     }

//     two_xy.exp = x.exp + y.exp;
//     two_xy.mant = x.mant * y.mant;
//     ++two_xy.exp;

//     // x = xx - yy
//     diff = xx.exp - yy.exp;
//     if (diff > 0) {
//       exp = xx.exp;
//       if (diff >= 31)
//         mant = xx.mant;
//       else
//         mant = xx.mant - (yy.mant >> diff);
//     }
//     else {
//       exp = yy.exp;
//       if (-diff >= 31)
//         mant = -yy.mant;
//       else
//         mant = (xx.mant >> (-diff)) - yy.mant;
//     }

//     // x = xx - yy + cx;
//     cx_ext = cx.mant * (1 << 15);
//     diff = exp - cx.exp;

//     if (diff > 0) {
//       if (diff < 31)
//         mant = mant + (cx_ext >> diff);
//     }
//     else {
//       if (-diff >= 31)
//         mant = cx_ext;
//       else
//         mant = (mant >> (-diff)) + cx_ext;

//       exp = cx.exp;
//     }

//     if (mant == 0) {
//       x.mant = 0;
//       x.exp = 0;
//     }
//     else {
//       mag = (mant < 0) ? (~(uint32_t)mant + 1u) : (uint32_t)mant;

//       msb = 31 - __builtin_clz(mag);
//       shift = msb - 14;

//       if (shift > 0)
//         mag >>= shift;
//       else if (shift < 0)
//         mag <<= -shift;

//       x.mant = (mant < 0) ? -(int32_t)mag : (int32_t)mag;
//       x.exp = exp - 15 + shift;
//     }

//     // y = two_xy + cy;
//     cy_ext = cy.mant * (1 << 15);
//     diff = two_xy.exp - cy.exp;

//     if (diff > 0) {
//       exp = two_xy.exp;

//       if (diff >= 31)
//         mant = two_xy.mant;
//       else
//         mant = (cy_ext >> diff) + two_xy.mant;
//     }
//     else {
//       exp = cy.exp;

//       if (-diff >= 31)
//         mant = cy_ext;
//       else
//         mant = (two_xy.mant >> (-diff)) + cy_ext;
//     }

//     if (mant == 0) {
//       y.mant = 0;
//       y.exp = 0;
//     }
//     else {
//       mag = (mant < 0) ? (~(uint32_t)mant + 1u) : (uint32_t)mant;

//       msb = 31 - __builtin_clz(mag);
//       shift = msb - 14;

//       if (shift > 0)
//         mag >>= shift;
//       else if (shift < 0)
//         mag <<= -shift;

//       y.mant = (mant < 0) ? -(int32_t)mag : (int32_t)mag;
//       y.exp = exp - 15 + shift;
//     }

//   } while (1);

//   return n;
// }


// int main(void) {

//   const uint16_t n_max = 100;

//   float test_points[][2] = {
//     { 0.0f,       0.0f      },
//     {-1.0f,       0.0f      },
//     { 0.5f,       0.5f      },
//     { 1.0f,       1.0f      },
//     {-2.0f,       0.0f      },
//     {-0.75f,      0.1f      },
//     {-0.1f,       0.651f    },
//     {-0.7436439f, 0.1318259f}
//   };

//   int nb_tests = sizeof(test_points) / sizeof(test_points[0]);

//   printf("Tests individuels\n");
//   printf("=================\n\n");

//   for (int i = 0; i < nb_tests; ++i) {

//     float cx = test_points[i][0];
//     float cy = test_points[i][1];

//     Myfloat_16 mx = float_to_Myfloat_16(cx);
//     Myfloat_16 my = float_to_Myfloat_16(cy);

//     uint16_t n_float =
//         calc_mandelbrot_point_soft(cx, cy, n_max);

//     uint16_t n_myfloat =
//         calc_mandelbrot_Myfloat_16(mx, my, n_max);

//     printf("c = % .8f %+.8fi\n", cx, cy);

//     printf("  cx : mant = %d, exp = %d, reconstructed = %.10f\n",
//            mx.mant, mx.exp, Myfloat_16_to_float(mx));

//     printf("  cy : mant = %d, exp = %d, reconstructed = %.10f\n",
//            my.mant, my.exp, Myfloat_16_to_float(my));

//     printf("  float      : %u\n", n_float);
//     printf("  Myfloat_16 : %u\n", n_myfloat);

//     if (n_float == n_myfloat)
//       printf("  OK\n");
//     else
//       printf("  DIFFERENCE : %d\n", (int)n_myfloat - (int)n_float);

//     printf("\n");
//   }


//   printf("\nTest sur grille\n");
//   printf("=================\n\n");

//   int total = 0;
//   int identical = 0;
//   int different = 0;
//   int max_difference = 0;

//   for (float cy = -1.5f; cy <= 1.5f; cy += 0.01f) {

//     for (float cx = -2.0f; cx <= 1.0f; cx += 0.01f) {

//       Myfloat_16 mx = float_to_Myfloat_16(cx);
//       Myfloat_16 my = float_to_Myfloat_16(cy);

//       uint16_t n_float =
//           calc_mandelbrot_point_soft(cx, cy, n_max);

//       uint16_t n_myfloat =
//           calc_mandelbrot_Myfloat_16(mx, my, n_max);

//       int difference =
//           abs((int)n_float - (int)n_myfloat);

//       ++total;

//       if (difference == 0) {
//         ++identical;
//       }
//       else {
//         ++different;

//         if (difference > max_difference)
//           max_difference = difference;
//       }
//     }
//   }

//   printf("Nombre de points testes : %d\n", total);
//   printf("Resultats identiques    : %d\n", identical);
//   printf("Resultats differents    : %d\n", different);

//   printf("Pourcentage identique   : %.2f %%\n",
//          100.0 * identical / total);

//   printf("Difference maximale     : %d iterations\n",
//          max_difference);

//   return 0;
// }