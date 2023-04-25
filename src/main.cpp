#include "Image.hpp"
#include <iostream>

#include <cmath>

#define ASSERT(x, ...) if (!(x)) fprintf(stderr, __VA_ARGS__);

// perform the sobel operator on the image
Image SobelOperator(const Image* img) {
	// make copies of the image for the calculation
	Image img_x = Image(*img);
	Image img_y = Image(*img);

	// create a placeholder image for the final result
	Image result = Image(img->w, img->h, img->channels);

	// the "x" kernel
	double Gx[9] = { 1, 0, -1,
			 2, 0, -2,
			 1, 0, -1 };
	// the "y" kernel
	double Gy[9] = {  1,  2,  1,
			  0,  0,  0,
			 -1, -2, -1 };

	// convolve to get the partial derivative of the image with respect to x and y
	// perform on the color channels only, no alpha channels, if they exist
	for (int i = 0; i < 3; i++) {
		img_x = img_x.convolve_clamp_to_0(i, 3, 3, Gx, 1, 1);
		img_y = img_y.convolve_clamp_to_0(i, 3, 3, Gy, 1, 1);
	}

	// loop through the pixels in the image
	// the pixels are stored in an array, with each element being
	// a singular value of a pixel
	// e.g., one pixel of RGBA format takes FOUR elements to represent it
	for (int i = 0; i < img->size; i += img->channels) {
		float Xr, Xg, Xb;
		Xr = img_x.data[i] / 255.0f;
		Xg = img_x.data[i + 1] / 255.0f;
		Xb = img_x.data[i + 2] / 255.0f;

		float Yr, Yg, Yb;
		Yr = img_y.data[i] / 255.0f;
		Yg = img_y.data[i + 1] / 255.0f;
		Yb = img_y.data[i + 2] / 255.0f;

		Xr *= Xr;
		Xg *= Xg;
		Xb *= Xb;

		Yr *= Yr;
		Yg *= Yg;
		Yb *= Yb;

		uint8_t Gr, Gg, Gb;
		Gr = std::sqrt(Xr + Yr) * 255;
		Gg = std::sqrt(Xg + Yg) * 255;
		Gb = std::sqrt(Xb + Yb) * 255;

		result.data[i] = Gr; result.data[i + 1] = Gg; result.data[i + 2] = Gb;
		if (img->channels == 4)
			result.data[i + 3] = 255;
	}

	// write the images for debugging
	img_x.write("modified_images/x.png");
	img_y.write("modified_images/y.png");

	return result;
}

int main()
{
	std::cout << "Input an image filename > ";
	std::string input_image;
	std::getline(std::cin, input_image);
	std::cout << '\n';

	std::cout << "Input the output filename > ";
	std::string output_image;
	std::getline(std::cin, output_image);
	std::cout << '\n';

	Image raft(input_image.c_str());

	double ker[] = {1 / 16.0, 2 / 16.0, 1 / 16.0,
					2 / 16.0, 4 / 16.0, 2 / 16.0,
					1 / 16.0, 2 / 16.0, 1 / 16.0}; // gaussian blur

	double ridgeKer[] = {-1, -1, -1,
						 -1,  8, -1,
						 -1, -1, -1}; // ridge detection (8 can be 4)

	double sharpKer[] = {0, -1,  0,
						-1,  5, -1,
						 0, -1,  0}; // sharpen

	// Perform the operator on an image
	Image sobel = SobelOperator(&raft);

	// Write the image
	ASSERT(sobel.write(output_image.c_str()), "Image write failed!");
	printf("Wrote image %s to %s!\n", input_image.c_str(), output_image.c_str());

	return 0;
}
