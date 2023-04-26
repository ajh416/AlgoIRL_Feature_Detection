#pragma once

#include "Image.hpp"

namespace FeatureDetection {
	Image Convolve(const Image* img);
	Image SobelOperator(const Image* img);
}
