#include "Image.hpp"
#include "FeatureDetection.hpp"

#include <iostream>
#include <cmath>

#define ASSERT(x, ...) if (!(x)) fprintf(stderr, __VA_ARGS__);

#ifndef _WIN32

int main() {
	std::cout << "Image filename > ";
	std::string input_image;
	std::getline(std::cin, input_image);
	std::cout << '\n';

	Image image(input_image.c_str());

	double kernel[9] = { 1, 0, -1,
		2, 0, -2,
		1, 0, -1 };
	image.convolve_clamp_to_border(0, 3, 3, kernel, 1, 1);
	
	// Perform the operator on an image
	//Image sobel = FeatureDetection::SobelOperator(&raft);

	// this is unique to how we store and read our images, without finding a slash
	// in the input this would probably cause our output to be quite wonky
	// e.g. "modified_imagessobel_op.png"
	char buf[128];
	snprintf(buf, 128, "modified_images%s", strrchr(input_image.c_str(), '/'));

	// Write the image
	ASSERT(image.write(buf), "Image write failed!");
	printf("Wrote image '%s' to '%s'!\n", input_image.c_str(), buf);

	return 0;
}

#else

int main() {
	std::cout << "This program only supports unix systems for now!\n";
	return 1;
}

#endif