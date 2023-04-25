#pragma once
#include <stdint.h>
#include <stdio.h>
#include <string>

struct Image
{
	uint8_t *data = NULL;
	size_t size = 0;
	int w, h, channels;

	Image(const char *filename);
	Image(int w, int h, int channels);
	Image(const Image &img);
	~Image();

	bool read(const char *filename);
	bool write(const char *filename);

	Image &convolve_clamp_to_0(uint8_t channel, uint32_t ker_w, uint32_t ker_h, double ker[], uint32_t cr, uint32_t cc);
	Image &convolve_clamp_to_border(uint8_t channel, uint32_t ker_w, uint32_t ker_h, double ker[], uint32_t cr, uint32_t cc);

	Image &grayscale();
	Image &gaussian_blur();
	Image &ridge_detection_4();
	Image &ridge_detection_8();
	Image &sharpen();
	Image &diffmap(Image& img);
};
