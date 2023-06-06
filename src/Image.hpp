#pragma once
#include <stdint.h>
#include <stdio.h>
#include <string>

struct Image {
  uint8_t *data = NULL;
  size_t size = 0;
  int w, h, channels;

  Image(const char *filename);
  Image(int w, int h, int channels);
  Image(const Image &img);
  ~Image();

  // read an image, true if successful
  bool read(const char *filename);

  // write the image data, true if successfull
  bool write(const char *filename);

  // convolves the image and clamps the border to zero
  // this function from https://github.com/Code-Break0/Image-Processing
  Image &convolve_clamp_to_0(uint8_t channel, uint32_t ker_w, uint32_t ker_h,
                             double ker[], uint32_t cr, uint32_t cc);

  // convolves the image and clamps the border to tile the current image
  // this function from https://github.com/Code-Break0/Image-Processing
  Image &convolve_clamp_to_border(uint8_t channel, uint32_t ker_w,
                                  uint32_t ker_h, double ker[], uint32_t cr,
                                  uint32_t cc);

  // grayscale an image
  Image &grayscale_avg();

  Image &grayscale_lum();

  // apply gaussian blur to an image
  Image &gaussian_blur();

  // ridge detect an image, 4 means the center
  // kernel value is 4, less ridges are detected
  Image &ridge_detection_4();

  // ridge detect an image, 8 means the center
  // kernel value is 8, more ridges are detected
  Image &ridge_detection_8();

  // sharpen an image
  Image &sharpen();

  // create a diffmap of one image onto another
  // useful for testing small differences
  Image &diffmap(Image &img);
};
