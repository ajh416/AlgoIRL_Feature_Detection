#include "FeatureDetection.hpp"

#include <cmath>

namespace FeatureDetection {

// not currently implemented
Image Convolve(const Image *img) { return *img; }

Image SobelOperatorGrayscale(const Image *img) {
  // make copies of the image for the calculation
  Image img_x = Image(*img);
  Image img_y = Image(*img);

  // create a placeholder image for the final result
  Image result = Image(img->w, img->h, img->channels);

  // the "x" kernel
  double Gx[9] = {1, 0, -1, 2, 0, -2, 1, 0, -1};
  // the "y" kernel
  double Gy[9] = {1, 2, 1, 0, 0, 0, -1, -2, -1};

  // convolve to get the partial derivative of the image with respect to x and y
  // perform on the color channels only, no alpha channels, if they exist
  img_x = img_x.convolve_clamp_to_border(0, 3, 3, Gx, 1, 1);
  img_y = img_y.convolve_clamp_to_border(0, 3, 3, Gy, 1, 1);

  printf("input width, height, size: %i, %i, %i, %lu\n", img->w, img->h, img->channels, img->size);
  printf("img_x width, height, size: %i, %i, %i, %lu\n", img_x.w, img_x.h, img_x.channels, img_x.size);
  printf("result width, height, size: %i, %i, %i, %lu\n", result.w, result.h, result.channels, result.size);

  // loop through the pixels in the image
  // the pixels are stored in an array, with each element being
  // a singular value of a pixel
  // e.g., one pixel of RGBA format takes FOUR elements to represent it
  for (int i = 0; i < img_x.size; i+=img_x.channels) {

    uint8_t G;

    // convert uint8_ts to floats bounded between 0 -> 1
    float X;
    X = img_x.data[i] / 255.0f;

    float Y;
    Y = img_y.data[i] / 255.0f;

    // Gx^2
    X *= X;

    // Gy^2
    Y *= Y;

    // sqrt(Gx + Gy) and convert to uint8
    G = std::sqrt(X + Y) * 255;

    // set our result image to the gradient magnitude
    // the for loop iterates at the number of channels
    // which means that i is the red pixel, i + 1 is green
    // and i + 3 is blue (i + 3 is alpha if applicable)
    result.data[i] = G;
    result.data[i + 1] = G;
    result.data[i + 2] = G;
  }

#ifdef DEBUG_SOBEL
  // write the x and y derivatives for debugging
  img_x.write("modified_images/sobel_x.png");
  img_y.write("modified_images/sobel_y.png");
#endif

  return result;
}

Image SobelOperator(const Image *img, const bool direction) {
  // make copies of the image for the calculation
  Image img_x = Image(*img);
  Image img_y = Image(*img);

  // create a placeholder image for the final result
  Image result = Image(img->w, img->h, img->channels);

  // the "x" kernel
  double Gx[9] = {1, 0, -1, 2, 0, -2, 1, 0, -1};
  // the "y" kernel
  double Gy[9] = {1, 2, 1, 0, 0, 0, -1, -2, -1};

  // convolve to get the partial derivative of the image with respect to x and y
  // perform on the color channels only, no alpha channels, if they exist
  for (int i = 0; i < 3; i++) {
    img_x = img_x.convolve_clamp_to_border(i, 3, 3, Gx, 1, 1);
    img_y = img_y.convolve_clamp_to_border(i, 3, 3, Gy, 1, 1);
  }

  // loop through the pixels in the image
  // the pixels are stored in an array, with each element being
  // a singular value of a pixel
  // e.g., one pixel of RGBA format takes FOUR elements to represent it
  for (int i = 0; i < img->size; i += img->channels) {

    uint8_t Gr, Gg, Gb;

    // convert uint8_ts to floats bounded between 0 -> 1
    float Xr, Xg, Xb;
    Xr = img_x.data[i] / 255.0f;
    Xg = img_x.data[i + 1] / 255.0f;
    Xb = img_x.data[i + 2] / 255.0f;

    float Yr, Yg, Yb;
    Yr = img_y.data[i] / 255.0f;
    Yg = img_y.data[i + 1] / 255.0f;
    Yb = img_y.data[i + 2] / 255.0f;

    if (!direction) {
      // obtain the gradient magnitude
      // gradient magnitude = sqrt(Gx^2 + Gy^2)

      // Gx^2
      Xr *= Xr;
      Xg *= Xg;
      Xb *= Xb;

      // Gy^2
      Yr *= Yr;
      Yg *= Yg;
      Yb *= Yb;

      // sqrt(Gx + Gy) and convert to uint8
      Gr = std::sqrt(Xr + Yr) * 255;
      Gg = std::sqrt(Xg + Yg) * 255;
      Gb = std::sqrt(Xb + Yb) * 255;

    } else {
      // gradient direction = atan2(Gy, Gx);
      Gr = std::atan2(Yr, Xr) * 255;
      Gg = std::atan2(Yg, Xg) * 255;
      Gb = std::atan2(Yb, Xb) * 255;
    }
    // set our result image to the gradient magnitude
    // the for loop iterates at the number of channels
    // which means that i is the red pixel, i + 1 is green
    // and i + 3 is blue (i + 3 is alpha if applicable)
    result.data[i] = Gr;
    result.data[i + 1] = Gg;
    result.data[i + 2] = Gb;
    if (img->channels == 4)
      result.data[i + 3] = img->data[i + 3];
  }

#ifdef DEBUG_SOBEL
  // write the x and y derivatives for debugging
  img_x.write("modified_images/sobel_x.png");
  img_y.write("modified_images/sobel_y.png");
#endif

  return result;
}

#ifdef REALTIME_DEMO
Image SobelOperator(const cv::Mat& matrix) {
  uint8_t* arr = (uint8_t*)malloc(sizeof(uint8_t) * (matrix.rows * matrix.cols));
  for (int i = 0; i < matrix.cols; i++) {
		for (int j = 0; j < matrix.rows; i++) {
			auto pixel = matrix.ptr<cv::Point3_<uint8_t>>(j + i * matrix.rows, 0);
		}
	}
}
#endif
} // namespace FeatureDetection
