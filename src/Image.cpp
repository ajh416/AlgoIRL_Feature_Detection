#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#define BYTE_BOUND(x) (uint8_t)(x < 0 ? 0 : (x > 255 ? 255 : x))

#include "Image.hpp"

#include "stb_image/stb_image.h"
#include "stb_image/stb_image_write.h"

#include <iostream>
#include <string>

Image::Image(const char *filename) {
  if (read(filename)) {
    printf("Read %s\n", filename);
    printf("Width: %i; Height: %i; Channels: %i\n", w, h, channels);
    size = w * h * channels;
  } else {
    printf("Failed to read image %s!\n", filename);
  }
}

Image::Image(int w, int h, int channels) : w(w), h(h), channels(channels) {
  size = w * h * channels;
  data = new uint8_t[size];
}

Image::Image(const Image &img) : Image(img.w, img.h, img.channels) {
  memcpy(data, img.data, size);
}

Image::~Image() { stbi_image_free(data); }

bool Image::read(const char *filename) {
  data = stbi_load(filename, &w, &h, &channels, 0);
  return data != NULL;
}

bool Image::write(const char *filename) {
  const char *ext = strrchr(filename, '.');
  int success = 0;

  if (strcmp(ext, ".png") == 0)
    success = stbi_write_png(filename, w, h, channels, data, w * channels);
  else
    success = stbi_write_jpg(filename, w, h, channels, data, 100);

  return success != 0;
}

Image &Image::convolve_clamp_to_0(uint8_t channel, uint32_t ker_w,
                                  uint32_t ker_h, double ker[], uint32_t cr,
                                  uint32_t cc) {
  uint8_t *new_data = new uint8_t[w * h];
  uint64_t center = cr * ker_w + cc;
  for (uint64_t k = channel; k < size; k += channels) {
    double c = 0;
    for (long i = -((long)cr); i < (long)ker_h - cr; ++i) {
      long row = ((long)k / channels) / w - i;
      if (row < 0 || row > h - 1) {
        continue;
      }
      for (long j = -((long)cc); j < (long)ker_w - cc; ++j) {
        long col = ((long)k / channels) % w - j;
        if (col < 0 || col > w - 1) {
          continue;
        }
        c += ker[center + i * (long)ker_w + j] *
             data[(row * w + col) * channels + channel];
      }
    }
    new_data[k / channels] = BYTE_BOUND(c);
  }
  for (uint64_t k = channel; k < size; k += channels) {
    data[k] = new_data[k / channels];
  }
  delete[] new_data;
  return *this;
}

Image &Image::convolve_clamp_to_border(uint8_t channel, uint32_t ker_w,
                                       uint32_t ker_h, double ker[],
                                       uint32_t cr, uint32_t cc) {
  uint8_t *new_data = new uint8_t[w * h];
  uint64_t center = (size_t)cr * ker_w + cc;
  for (uint64_t k = channel; k < size; k += channels) {
    double c = 0;
    for (long i = -((long)cr); i < (long)ker_h - (long)cr; ++i) {
      long row = ((long)k / channels) / w - i;
      if (row < 0) {
        row = 0;
      } else if (row > h - 1) {
        row = h - 1;
      }
      for (long j = -((long)cc); j < (long)ker_w - (long)cc; ++j) {
        long col = ((long)k / channels) % w - j;
        if (col < 0) {
          col = 0;
        } else if (col > w - 1) {
          col = w - 1;
        }
        c += ker[center + (long long)i * (long)ker_w + j] *
             data[(row * w + col) * channels + channel];
      }
    }
    new_data[k / channels] = BYTE_BOUND(c);
  }
  for (uint64_t k = channel; k < size; k += channels) {
    data[k] = new_data[k / channels];
  }
  delete[] new_data;
  return *this;
}

Image& Image::grayscale_avg() {
	if(channels < 3) {
		printf("Image %p has less than 3 channels, it is assumed to already be grayscale.", (void*)this);
	}
	else {
		for(int i = 0; i < size; i+=channels) {
			//(r+g+b)/3
			int gray = (data[i] + data[i+1] + data[i+2])/3;
			memset(data+i, gray, 3);
		}
	}
	return *this;
}

Image& Image::grayscale_lum() {
	if(channels < 3) {
		printf("Image %p has less than 3 channels, it is assumed to already be grayscale.", (void*)this);
	}
	else {
		for(int i = 0; i < size; i+=channels) {
			int gray = 0.2126*data[i] + 0.7152*data[i+1] + 0.0722*data[i+2];
			memset(data+i, gray, 3);
		}
	}
	return *this;
}

// this may actually be some modified form of box blur
Image &Image::gaussian_blur() {
  double ker[] = {1 / 16.0, 2 / 16.0, 1 / 16.0, 2 / 16.0, 4 / 16.0,
                  2 / 16.0, 1 / 16.0, 2 / 16.0, 1 / 16.0}; // gaussian blur

  // Convolve on all three channels (avoid alpha channel even if there is one)
  this->convolve_clamp_to_border(0, 3, 3, ker, 1, 1);
  this->convolve_clamp_to_border(1, 3, 3, ker, 1, 1);
  this->convolve_clamp_to_border(2, 3, 3, ker, 1, 1);

  return *this;
}

Image &Image::ridge_detection_4() {
  double ker[] = {-1, -1, -1, -1, 4,
                  -1, -1, -1, -1}; // ridge detection (4 can be 8)

  // Convolve on all 3 channels (avoid alpha channel even if there is one)
  this->convolve_clamp_to_border(0, 3, 3, ker, 1, 1);
  this->convolve_clamp_to_border(1, 3, 3, ker, 1, 1);
  this->convolve_clamp_to_border(2, 3, 3, ker, 1, 1);

  return *this;
}

Image &Image::ridge_detection_8() {
  double ker[] = {-1, -1, -1, -1, 8,
                  -1, -1, -1, -1}; // ridge detection (8 can be 4)

  // Convolve on all three channels (avoid alpha channel even if there is one)
  this->convolve_clamp_to_border(0, 3, 3, ker, 1, 1);
  this->convolve_clamp_to_border(1, 3, 3, ker, 1, 1);
  this->convolve_clamp_to_border(2, 3, 3, ker, 1, 1);

  return *this;
}

Image &Image::sharpen() {
  double ker[] = {0, -1, 0, -1, 5, -1, 0, -1, 0}; // sharpen

  // Convolve on all three channels (avoid alpha channel even if there is one)
  this->convolve_clamp_to_border(0, 3, 3, ker, 1, 1);
  this->convolve_clamp_to_border(1, 3, 3, ker, 1, 1);
  this->convolve_clamp_to_border(2, 3, 3, ker, 1, 1);

  return *this;
}

// not 100% sure how this works, stolen from github
Image &Image::diffmap(Image &img) {
  int compare_width = fmin(w, img.w);
  int compare_height = fmin(h, img.h);
  int compare_channels = fmin(channels, img.channels);

  for (uint32_t i = 0; i < compare_height; i++) {
    for (uint32_t j = 0; j < compare_width; j++) {
      for (uint8_t k = 0; k < compare_channels; k++) {
        data[(i * w + j) * channels + k] =
            BYTE_BOUND(img.data[(i * img.w + j) * img.channels + k]);
      }
    }
  }
  return *this;
}
