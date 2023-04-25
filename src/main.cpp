#include "Image.hpp"
#include <iostream>

#include <cmath>

template<typename T>
struct Vec3 {
	Vec3(T x, T y, T z) { this->x = x; this->y = y; this->z = z; }

	T x, y, z;

	Vec3 Pow(int p) { return Vec3(pow(x, p), pow(y, p), pow(z, p)); }

	Vec3 Sqrt() { return Vec3(sqrt(x), sqrt(y), sqrt(z)); }

	Vec3 operator+(Vec3 other) { return Vec3(x + other.x, y + other.y, z + other.z); }
};

Vec3<float> vec3_uint8_to_float(const Vec3<uint8_t> conv) { return Vec3<float>(conv.x / 255.0f, conv.y / 255.0f, conv.z / 255.0f); }

Vec3<uint8_t> vec3_float_to_uint8(const Vec3<float> conv) { return Vec3<uint8_t>(conv.x * 255, conv.y * 255, conv.z * 255); }

// manually free the returned pointer
Image SobelOperator(const Image* img) {
	Image x = *img;
	Image y = *img;

	Image result = Image(img->w, img->h, img->channels);

	double Gx[9] = { 1, 0, -1,
			 2, 0, -2,
			 1, 0, -1 };
	double Gy[9] = {  1,  2,  1,
			  0,  0,  0,
			 -1, -2, -1 };

	for (int i = 0; i < 3; i++) {
		x = x.convolve_clamp_to_0(i, 3, 3, Gx, 1, 1);
		y = y.convolve_clamp_to_0(i, 3, 3, Gy, 1, 1);
	}
	
	for (int i = 0; i < img->size; i += img->channels) {
		Vec3<float> img_x = vec3_uint8_to_float(Vec3<uint8_t>(x.data[i], x.data[i + 1], x.data[i + 2]));
		Vec3<float> img_y = vec3_uint8_to_float(Vec3<uint8_t>(y.data[i], y.data[i + 1], y.data[i + 2]));

		Vec3<float> Gf = (img_x.Pow(2) + img_x.Pow(2)).Sqrt();
		Vec3<uint8_t> G = vec3_float_to_uint8(Gf);
		result.data[i] = G.x; result.data[i + 1] = G.y; result.data[i + 2] = G.z;
	}

	x.write("modified_images/x.png");
	y.write("modified_images/y.png");
	return result;
}

int main()
{
	Image raft("images/raft.jpg");

	double ker[] = {1 / 16.0, 2 / 16.0, 1 / 16.0,
					2 / 16.0, 4 / 16.0, 2 / 16.0,
					1 / 16.0, 2 / 16.0, 1 / 16.0}; // gaussian blur

	double ridgeKer[] = {-1, -1, -1,
						 -1,  8, -1,
						 -1, -1, -1}; // ridge detection (8 can be 4)

	double sharpKer[] = {0, -1,  0,
						-1,  5, -1,
						 0, -1,  0}; // sharpen

	Image sobel = SobelOperator(&raft);
	sobel.write("modified_images/raft.png");
	return 0;
}
