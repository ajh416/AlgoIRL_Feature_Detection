#pragma once

#include "Image.hpp"

namespace FeatureDetection {
	// not currently implemented
	Image Convolve(const Image* img);
	
	Image SobelOperatorGrayscale(const Image* img);
	
	// Performs the sobel operator on the specified image
	// Direction specifies whether or not to find the direction
	// of the magnitude and overlay it onto the image
	Image SobelOperator(const Image* img, const bool direction = false);
}
