#include "Image.hpp"
#include <iostream>

#include <cmath>

#define ASSERT(x, ...) if (!(x)) fprintf(stderr, __VA_ARGS__);

// used for calculating the sobel operator for convenience
template<typename T>
struct Vec3 {
	Vec3(T x, T y, T z) { this->x = x; this->y = y; this->z = z; }

	T x, y, z;

	Vec3 Pow(int p) { return Vec3(pow(x, p), pow(y, p), pow(z, p)); }

	Vec3 Sqrt() { return Vec3(sqrt(x), sqrt(y), sqrt(z)); }

	Vec3 operator+(Vec3 other) { return Vec3(x + other.x, y + other.y, z + other.z); }
};

// converts uint8_t to float bounded between 0 and 1
Vec3<float> vec3_uint8_to_float(const Vec3<uint8_t> conv) { return Vec3<float>(conv.x / 255.0f, conv.y / 255.0f, conv.z / 255.0f); }

// converts floats (bounded 0 to 1) to uint8_ts
Vec3<uint8_t> vec3_float_to_uint8(const Vec3<float> conv) { return Vec3<uint8_t>(conv.x * 255, conv.y * 255, conv.z * 255); }

// perform the sobel operator on the image
Image SobelOperator(const Image* img) {
	// make copies of the image for the calculation
	Image img_x = *img;
	Image img_y = *img;

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
	// the pixels are stored in a 1d array, with each element being
	// a singular value of a pixel
	// e.g., one pixel of RGBA format takes FOUR elements to represent it
	for (int i = 0; i < img->size; i += img->channels) {
		// get floats (0 --> 1) rather than uint8_ts to perform the calculation
		Vec3<float> x = vec3_uint8_to_float(Vec3<uint8_t>(img_x.data[i], img_x.data[i + 1], img_x.data[i + 2]));
		Vec3<float> y = vec3_uint8_to_float(Vec3<uint8_t>(img_y.data[i], img_y.data[i + 1], img_y.data[i + 2]));

		// calculate the gradient magnitude
		Vec3<float> Gf = (x.Pow(2) + y.Pow(2)).Sqrt();

		// convert back to a uint8_t for writing
		Vec3<uint8_t> G = vec3_float_to_uint8(Gf);

		result.data[i] = G.x; result.data[i + 1] = G.y; result.data[i + 2] = G.z;
	}

	// write the images for debugging
	img_x.write("modified_images/x.png");
	img_y.write("modified_images/y.png");

	return result;
}

int main()
{
	Image raft("images/sobel_op.png");

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
	ASSERT(sobel.write("modified_images/sobel_op.png"), "Image write failed!");

	return 0;
}
