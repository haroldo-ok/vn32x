// PNG decoding for SEGA 32X
// Mic, 2009

#include "dpng32x.h"
#include "png.h"


char *g_baseAddr;
int g_imageSize;
int g_pos;

 /* The png_jmpbuf() macro, used in error handling, became available in
  * libpng version 1.0.6.  If you want to be able to run your code with older
  * versions of libpng, you must define the macro yourself (but only if it
  * is not already defined by libpng!).
  */

#ifndef png_jmpbuf
#  define png_jmpbuf(png_ptr) ((png_ptr)->jmpbuf)
#endif


typedef struct {
  char * baseAddr;		/* source stream */
  unsigned int imageSize,pos;
} my_source_mgr;

typedef my_source_mgr * my_src_ptr;


static void
pngtest_read_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
   png_size_t check;

   /* fread() returns 0 on error, so it is OK to store this in a png_size_t
    * instead of an int, which is what fread() actually returns.
    */
   //check = fread(data, 1, length, (png_FILE_p)png_ptr->io_ptr);
   	int i;
   	check = length;
   	if ((g_pos + length) > g_imageSize)
   	{
	   check = g_imageSize - g_pos;
   	}

   	for (i = 0; i < length; i++)
	{
		data[i] = g_baseAddr[g_pos++];
	}

    if (check != length)
    {
       png_error(png_ptr, "Read Error!");
    }
}



int read_png(char *baseAddr, unsigned int imageSize, vu16 *dest)
{
   png_structp png_ptr;
   png_infop info_ptr;
   png_uint_32 width, height;
   int bit_depth, color_type, interlace_type;
   static u16 scanlineBuffer[320*3];
   int pass,row,x,y;
   vu16 *dest2;

	g_baseAddr = baseAddr;
	g_imageSize = imageSize;
	g_pos = 0;

   /* Create and initialize the png_struct with the desired error handler
    * functions.  If you want to use the default stderr and longjump method,
    * you can supply NULL for the last three parameters.  We also supply the
    * the compiler header file version, so that we know if the application
    * was compiled with a compatible version of the library.  REQUIRED
    */
   png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
      NULL, NULL, NULL);

   if (png_ptr == NULL)
   {
      return (1);
   }

   /* Allocate/initialize the memory for image information.  REQUIRED. */
   info_ptr = png_create_info_struct(png_ptr);
   if (info_ptr == NULL)
   {
      //fclose(fp);
      png_destroy_read_struct(&png_ptr, NULL, NULL);
      return (1);
   }

   /* Set error handling if you are using the setjmp/longjmp method (this is
    * the normal method of doing things with libpng).  REQUIRED unless you
    * set up your own error handlers in the png_create_read_struct() earlier.
    */

   if (setjmp(png_jmpbuf(png_ptr)))
   {
      /* Free all of the memory associated with the png_ptr and info_ptr */
      png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
      /* If we get here, we had a problem reading the file */
      return (1);
   }

   png_set_read_fn(png_ptr, (void *)NULL, pngtest_read_data);


   /* The call to png_read_info() gives us all of the information from the
    * PNG file before the first IDAT (image data chunk).  REQUIRED
    */
   png_read_info(png_ptr, info_ptr);

   png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
       &interlace_type, NULL, NULL);


   /* Tell libpng to strip 16 bit/color files down to 8 bits/color */
   png_set_strip_16(png_ptr);

   /* Strip alpha bytes from the input data without combining with the
    * background (not recommended).
    */
   png_set_strip_alpha(png_ptr);

   /* Extract multiple pixels with bit depths of 1, 2, and 4 from a single
    * byte into separate bytes (useful for paletted and grayscale images).
    */
   png_set_packing(png_ptr);

   /* Expand paletted colors into true RGB triplets */
   if (color_type == PNG_COLOR_TYPE_PALETTE)
      png_set_palette_to_rgb(png_ptr);

   /* Expand grayscale images to the full 8 bits from 1, 2, or 4 bits/pixel */
   if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
      png_set_expand_gray_1_2_4_to_8(png_ptr);


   /* Turn on interlace handling.  REQUIRED if you are not using
    * png_read_image().  To see how to handle interlacing passes,
    * see the png_read_row() method below:
    */
   int number_passes = png_set_interlace_handling(png_ptr);

   png_bytep row_pointer = (png_bytep)(&scanlineBuffer[0]);


   for (pass = 0; pass < number_passes; pass++)
   {
      for (y = 0; y < height; y++)
      {
         png_read_rows(png_ptr, &row_pointer, NULL, 1);

         dest2 = dest;
	     for (x = 0; x < width; x++)
	     {
		 	*dest2++ = (row_pointer[x+x+x] >> 3) + ((row_pointer[x+x+x+1] >> 3)<<5) + ((row_pointer[x+x+x+2] >> 3)<<10);
		 }
		 dest += 320;

      }

   }

   /* Read rest of file, and get additional chunks in info_ptr - REQUIRED */
   png_read_end(png_ptr, info_ptr);


   /* Clean up after the read, and free any memory allocated - REQUIRED */
   png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

   /* That's it */
   return (0);
}

