#include "Image.hpp"
#include "FeatureDetection.hpp"

#include <iostream>
#include <cmath>

#define ASSERT(x, ...) if (!(x)) fprintf(stderr, __VA_ARGS__);

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
	Image sobel = FeatureDetection::SobelOperator(&raft);

	// Write the image
	ASSERT(sobel.write(output_image.c_str()), "Image write failed!");
	printf("Wrote image %s to %s!\n", input_image.c_str(), output_image.c_str());

	return 0;
}
