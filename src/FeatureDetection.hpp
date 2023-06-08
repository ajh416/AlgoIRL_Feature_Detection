#pragma once

#include "Image.hpp"

#ifdef REALTIME_DEMO
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#endif

namespace FeatureDetection {
	// not currently implemented
	Image Convolve(const Image* img);
	
	Image SobelOperatorGrayscale(const Image* img);
	
	// Performs the sobel operator on the specified image
	// Direction specifies whether or not to find the direction
	// of the magnitude and overlay it onto the image
	Image SobelOperator(const Image* img, const bool direction = false);

#ifdef REALTIME_DEMO
	Image SobelOperator(const cv::Mat& matrix);
#endif
}
