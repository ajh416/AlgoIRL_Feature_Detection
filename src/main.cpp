#include "Image.hpp"
#include <iostream>

int main()
{
	Image raft("images/wog_raft.jpg");

	double ker[] = {1 / 16.0, 2 / 16.0, 1 / 16.0,
					2 / 16.0, 4 / 16.0, 2 / 16.0,
					1 / 16.0, 2 / 16.0, 1 / 16.0}; // gaussian blur

	double ridgeKer[] = {-1, -1, -1,
						 -1,  8, -1,
						 -1, -1, -1}; // ridge detection (8 can be 4)

	double sharpKer[] = {0, -1,  0,
						-1,  5, -1,
						 0, -1,  0}; // sharpen

	raft.ridge_detection_8();
	raft.write("modified_images/raft.png");
	return 0;
}
