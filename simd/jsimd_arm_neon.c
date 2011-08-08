/*
 * jsimd_arm_neon.c
 *
 * Copyright 2009 Pierre Ossman <ossman@cendio.se> for Cendio AB
 * Copyright 2009 D. R. Commander
 * Copyright 2011 Mandeep Kumar <mandeep.kumar@linaro.org> 
 * 
 * Based on the x86 SIMD extension for IJG JPEG library,
 * Copyright (C) 1999-2006, MIYASAKA Masaru.
 * For conditions of distribution and use, see copyright notice in jsimdext.inc
 *
 * This file contain ARM NEON optimized routines. 
 */

#define JPEG_INTERNALS
#include "../jinclude.h"
#include "../jpeglib.h"
#include "../jsimd.h"
#include "../jdct.h"
#include "../jsimddct.h"


/* Private subobject */

typedef struct {
  struct jpeg_color_deconverter pub; /* public fields */

  /* Private state for YCC->RGB conversion */
  int * Cr_r_tab;		/* => table for Cr to R conversion */
  int * Cb_b_tab;		/* => table for Cb to B conversion */
  INT32 * Cr_g_tab;		/* => table for Cr to G conversion */
  INT32 * Cb_g_tab;		/* => table for Cb to G conversion */
} my_color_deconverter;

typedef my_color_deconverter * my_cconvert_ptr;


#define DEQUANTIZE(coef,quantval)  ((coef) * ((INT16)quantval))

/* IDCT routines */
EXTERN (void) idct_1x1_venum (INT16 * coeffPtr, INT16 * samplePtr, INT32 stride);
EXTERN (void) idct_2x2_venum (INT16 * coeffPtr, INT16 * samplePtr, INT32 stride);
EXTERN (void) idct_4x4_venum (INT16 * coeffPtr, INT16 * samplePtr, INT32 stride);
EXTERN (void) idct_8x8_venum (INT16 * coeffPtr, UINT8 **samplePtr, INT32 col, INT16 *qtab);

/* Color conversion routines */
EXTERN (void) yvup2rgb565_venum (UINT8 *pLumaLine,
                UINT8 *pCrLine,
                UINT8 *pCbLine,
                UINT8 *pRGB565Line,
                JDIMENSION nLineWidth);
EXTERN (void) yyvup2rgb565_venum (UINT8 * pLumaLine,
                UINT8 *pCrLine,
                UINT8 *pCbLine,
                UINT8 * pRGB565Line,
                JDIMENSION nLineWidth);
EXTERN (void) yvup2bgr888_venum (UINT8 * pLumaLine,
                UINT8 *pCrLine,
                UINT8 *pCbLine,
                UINT8 * pBGR888Line,
                JDIMENSION nLineWidth);
EXTERN (void) yyvup2bgr888_venum (UINT8 * pLumaLine,
                UINT8 *pCrLine,
                UINT8 *pCbLine,
                UINT8 * pBGR888Line,
                JDIMENSION nLineWidth);
EXTERN (void) yvup2abgr8888_venum (UINT8 * pLumaLine,
                UINT8 *pCrLine,
                UINT8 *pCbLine,
                UINT8 * pABGR888Line,
                JDIMENSION nLineWidth);
EXTERN (void) yyvup2abgr8888_venum (UINT8 * pLumaLine,
                UINT8 *pCrLine,
                UINT8 *pCbLine,
                UINT8 * pABGR888Line,
                JDIMENSION nLineWidth);


GLOBAL(int)
jsimd_can_rgb_ycc (void)
{
  return 0;
}

GLOBAL(int)
jsimd_can_ycc_rgb (void)
{
  return 1;
}

GLOBAL(int)
jsimd_can_idct_islow (void)
{
  return 1;
}

GLOBAL(int)
jsimd_can_idct_ifast (void)
{
  return 1;
}

GLOBAL(int)
jsimd_can_idct_float (void)
{
  return 0;
}

GLOBAL(int)
jsimd_can_h2v2_downsample (void)
{
  return 0;
}

GLOBAL(int)
jsimd_can_h2v1_downsample (void)
{
  return 0;
}
GLOBAL(int)
jsimd_can_h2v2_upsample (void)
{
  return 0;
}

GLOBAL(int)
jsimd_can_h2v1_upsample (void)
{
  return 0;
}
GLOBAL(int)
jsimd_can_h2v2_fancy_upsample (void)
{
  return 0;
}

GLOBAL(int)
jsimd_can_h2v1_fancy_upsample (void)
{
  return 0;
}
GLOBAL(int)
jsimd_can_h2v2_merged_upsample (void)
{
  return 0;
}

GLOBAL(int)
jsimd_can_h2v1_merged_upsample (void)
{
  return 0;
}
GLOBAL(int)
jsimd_can_convsamp (void)
{
  return 0;
}

GLOBAL(int)
jsimd_can_convsamp_float (void)
{
  return 0;
}
GLOBAL(int)
jsimd_can_fdct_islow (void)
{
  return 0;
}

GLOBAL(int)
jsimd_can_fdct_ifast (void)
{
  return 0;
}

GLOBAL(int)
jsimd_can_fdct_float (void)
{
  return 0;
}
GLOBAL(int)
jsimd_can_quantize (void)
{
  return 0;
}

GLOBAL(int)
jsimd_can_quantize_float (void)
{
  return 0;
}
GLOBAL(int)
jsimd_can_idct_2x2 (void)
{
  return 1;
}

GLOBAL(int)
jsimd_can_idct_4x4 (void)
{
  return 1;
}




/* Function Implementation */

GLOBAL(void)
jsimd_rgb_ycc_convert (j_compress_ptr cinfo,
                       JSAMPARRAY input_buf, JSAMPIMAGE output_buf,
                       JDIMENSION output_row, int num_rows)
{
}

GLOBAL(void)
jsimd_ycc_rgb_convert (j_decompress_ptr cinfo,
                       JSAMPIMAGE input_buf, JDIMENSION input_row,
                       JSAMPARRAY output_buf, int num_rows)
{
  my_cconvert_ptr cconvert = (my_cconvert_ptr) cinfo->cconvert;
  JSAMPROW inptr0, inptr1, inptr2;
  JSAMPROW outptr;
  JDIMENSION row;

  for (row = 0; row < (JDIMENSION)num_rows; row++)
  {
    inptr0     = input_buf[0][input_row];
    inptr1     = input_buf[1][input_row];
    inptr2     = input_buf[2][input_row];

    input_row++;
    outptr = *output_buf++;

    yvup2bgr888_venum((UINT8*) inptr0,
                      (UINT8*) inptr2,
                      (UINT8*) inptr1,
                      (UINT8*) outptr,
                      cinfo->output_width);
  }
}



GLOBAL(void)
jsimd_h2v2_downsample (j_compress_ptr cinfo, jpeg_component_info * compptr,
                       JSAMPARRAY input_data, JSAMPARRAY output_data)
{
}

GLOBAL(void)
jsimd_h2v1_downsample (j_compress_ptr cinfo, jpeg_component_info * compptr,
                       JSAMPARRAY input_data, JSAMPARRAY output_data)
{
}


GLOBAL(void)
jsimd_h2v2_upsample (j_decompress_ptr cinfo,
                     jpeg_component_info * compptr, 
                     JSAMPARRAY input_data,
                     JSAMPARRAY * output_data_ptr)
{
}

GLOBAL(void)
jsimd_h2v1_upsample (j_decompress_ptr cinfo,
                     jpeg_component_info * compptr, 
                     JSAMPARRAY input_data,
                     JSAMPARRAY * output_data_ptr)
{
}


GLOBAL(void)
jsimd_h2v2_fancy_upsample (j_decompress_ptr cinfo,
                           jpeg_component_info * compptr, 
                           JSAMPARRAY input_data,
                           JSAMPARRAY * output_data_ptr)
{
}

GLOBAL(void)
jsimd_h2v1_fancy_upsample (j_decompress_ptr cinfo,
                           jpeg_component_info * compptr, 
                           JSAMPARRAY input_data,
                           JSAMPARRAY * output_data_ptr)
{
}


GLOBAL(void)
jsimd_h2v2_merged_upsample (j_decompress_ptr cinfo,
                            JSAMPIMAGE input_buf,
                            JDIMENSION in_row_group_ctr,
                            JSAMPARRAY output_buf)
{
}

GLOBAL(void)
jsimd_h2v1_merged_upsample (j_decompress_ptr cinfo,
                            JSAMPIMAGE input_buf,
                            JDIMENSION in_row_group_ctr,
                            JSAMPARRAY output_buf)
{
}


GLOBAL(void)
jsimd_convsamp (JSAMPARRAY sample_data, JDIMENSION start_col,
                DCTELEM * workspace)
{
}

GLOBAL(void)
jsimd_convsamp_float (JSAMPARRAY sample_data, JDIMENSION start_col,
                      FAST_FLOAT * workspace)
{
}


GLOBAL(void)
jsimd_fdct_islow (DCTELEM * data)
{
}

GLOBAL(void)
jsimd_fdct_ifast (DCTELEM * data)
{
}

GLOBAL(void)
jsimd_fdct_float (FAST_FLOAT * data)
{
}


GLOBAL(void)
jsimd_quantize (JCOEFPTR coef_block, DCTELEM * divisors,
                DCTELEM * workspace)
{
}

GLOBAL(void)
jsimd_quantize_float (JCOEFPTR coef_block, FAST_FLOAT * divisors,
                      FAST_FLOAT * workspace)
{
}


GLOBAL(void)
jsimd_idct_2x2 (j_decompress_ptr cinfo, jpeg_component_info * compptr,
                JCOEFPTR coef_block, JSAMPARRAY output_buf,
                JDIMENSION output_col)
{
  ISLOW_MULT_TYPE * quantptr;
  JSAMPROW outptr;

  /* Note: Must allocate 8x2 even though only 2x2 is used because
   * IDCT function expects stride of 8. Stride input to function is ignored.
   * There is also a hw limitation requiring input size to be 8x2.
   */
  INT16    idct_out[DCTSIZE * (DCTSIZE>>2)];  /* buffers data between passes */
  INT16*   idctptr;
  JCOEFPTR coefptr;
  int ctr;

  coefptr  = coef_block;
  quantptr = (ISLOW_MULT_TYPE *) compptr->dct_table;

  /* Dequantize the coeff buffer and write it back to the same location */
  for (ctr = (DCTSIZE>>2); ctr > 0; ctr--) {
    coefptr[0]         = DEQUANTIZE(coefptr[0]        , quantptr[0]        );
    coefptr[DCTSIZE*1] = DEQUANTIZE(coefptr[DCTSIZE*1], quantptr[DCTSIZE*1]);

    /* advance pointers to next column */
    quantptr++;
    coefptr++;
  }

  idct_2x2_venum((INT16*)coef_block,
                 (INT16*)idct_out,
                  DCTSIZE * sizeof(INT16));

  idctptr = idct_out;
  for (ctr = 0; ctr < (DCTSIZE>>2); ctr++) {
    outptr = output_buf[ctr] + output_col;

    /* outptr sample size is 1 bytes, idctptr sample size is 2 bytes */
    outptr[0] = idctptr[0];
    outptr[1] = idctptr[1];

    /* IDCT function assumes stride of 8 units */
    idctptr += (DCTSIZE);    /* advance pointers to next row */
  }
}

GLOBAL(void)
jsimd_idct_4x4 (j_decompress_ptr cinfo, jpeg_component_info * compptr,
                JCOEFPTR coef_block, JSAMPARRAY output_buf,
                JDIMENSION output_col)
{
  ISLOW_MULT_TYPE * quantptr;
  JSAMPROW outptr;

  /* Note: Must allocate 8x4 even though only 4x4 is used because
   * IDCT function expects stride of 8. Stride input to function is ignored.
   */
  INT16    idct_out[DCTSIZE * (DCTSIZE>>1)];  /* buffers data between passes */
  INT16*   idctptr;
  JCOEFPTR coefptr;
  int ctr;

  coefptr  = coef_block;
  quantptr = (ISLOW_MULT_TYPE *) compptr->dct_table;

  /* Dequantize the coeff buffer and write it back to the same location */
  for (ctr = (DCTSIZE>>1); ctr > 0; ctr--) {
    coefptr[0]         = DEQUANTIZE(coefptr[0]        , quantptr[0]        );
    coefptr[DCTSIZE*1] = DEQUANTIZE(coefptr[DCTSIZE*1], quantptr[DCTSIZE*1]);
    coefptr[DCTSIZE*2] = DEQUANTIZE(coefptr[DCTSIZE*2], quantptr[DCTSIZE*2]);
    coefptr[DCTSIZE*3] = DEQUANTIZE(coefptr[DCTSIZE*3], quantptr[DCTSIZE*3]);

    /* advance pointers to next column */
    quantptr++;
    coefptr++;
  }

  idct_4x4_venum((INT16*)coef_block,
                 (INT16*)idct_out,
                  DCTSIZE * sizeof(INT16));

  idctptr = idct_out;
  for (ctr = 0; ctr < (DCTSIZE>>1); ctr++) {
    outptr = output_buf[ctr] + output_col;

    /* outptr sample size is 1 byte while idctptr sample size is 2 bytes */
    outptr[0] = idctptr[0];
    outptr[1] = idctptr[1];
    outptr[2] = idctptr[2];
    outptr[3] = idctptr[3];
    /* IDCT function assumes stride of 8 units */
    idctptr += (DCTSIZE);    /* advance pointers to next row */
  }
}


GLOBAL(void)
jsimd_idct_islow (j_decompress_ptr cinfo, jpeg_component_info * compptr,
                JCOEFPTR coef_block, JSAMPARRAY output_buf,
                JDIMENSION output_col)
{
  idct_8x8_venum((INT16*)coef_block,
                 output_buf,
                 output_col,
                 compptr->dct_table);
}

GLOBAL(void)
jsimd_idct_ifast (j_decompress_ptr cinfo, jpeg_component_info * compptr,
                JCOEFPTR coef_block, JSAMPARRAY output_buf,
                JDIMENSION output_col)
{
  idct_8x8_venum((INT16*)coef_block,
                 output_buf,
                 output_col,
                 compptr->dct_table);
}

GLOBAL(void)
jsimd_idct_float (j_decompress_ptr cinfo, jpeg_component_info * compptr,
                JCOEFPTR coef_block, JSAMPARRAY output_buf,
                JDIMENSION output_col)
{
}

