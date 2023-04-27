#pragma once

#include "Image.hpp"

namespace FeatureDetection {
	Image Convolve(const Image* img);
	
	// Performs the sobel operator on the specified image
	// When true, direction calculates the gradient direction rather
	// than calculating the magnitude
	Image SobelOperator(const Image* img, const bool direction = false);
}
