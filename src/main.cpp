/*
 * How to compile with gcc: g++ src/main.cpp src/Image.cpp src/FeatureDetection.cpp -std=c++20 -O2 -o main
 * When using Windows, you must also link against comdlg32.lib for some reason
 * MSVC links against it automagically, but clang/gcc must be manually specified with "-l comdlg32.lib"
 *
 * Authors: Adam Henry, Shane Ludwig
 */

#include "Image.hpp"
#include "FeatureDetection.hpp"

#include <cmath>
#include <cstring>
#include <filesystem>

#ifdef _WIN32
// Needed for file dialog
#include <Windows.h>
#else
// Needed for popen
#include <unistd.h>
#endif

#define ASSERT(x, ...) if (!(x)) fprintf(stderr, __VA_ARGS__); // allows for printf formatting into this assert

std::filesystem::path OpenFileDialog(const std::filesystem::path open_path = std::filesystem::current_path(), const char* filter = "All\0*.*\0");

int main() {
	// open a dialog window
	std::string input_image = OpenFileDialog().string();
	if (input_image == std::string()) {
		printf("No file selected!\n");
		return 1;
	}

	printf("Direction of gradient? (y/n): ");
	char direction[10];
	scanf("%s", direction);

	int dir = 0;
	if (strcmp(direction, "y") == 0)
		dir = 1;

	// load the image into memory
	Image image(input_image.c_str());

	// Perform the operator on an image
	Image sobel = FeatureDetection::SobelOperator(&image, dir);

	// this is unique to how we store and read our images, without finding a slash
	// in the input this would probably cause our output to be quite wonky
	// e.g. "modified_imagessobel_op.png", or maybe a null char?
	char buf[128];
	snprintf(buf, 128, "modified_images%s", strrchr(input_image.c_str(), '/'));

	// Write the image to specified filepath
	ASSERT(sobel.write(buf), "Image write failed!");
	printf("Wrote image '%s' to '%s'!\n", input_image.c_str(), buf);

	return 0;
}

// Opens a file dialog on either Windows or Linux
std::filesystem::path OpenFileDialog(const std::filesystem::path open_path, const char* filter)
{
#ifdef _WIN32
        OPENFILENAMEA ofn;       // common dialog box structure
        CHAR szFile[260] = { 0 };       // if using TCHAR macros

        // Initialize OPENFILENAME (memset to zero)
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = NULL;
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        if (GetOpenFileNameA(&ofn) == TRUE)
        {
                std::string fp = ofn.lpstrFile;
                std::replace(fp.begin(), fp.end(), '\\', '/');
                return std::filesystem::path(fp);
        }

	return std::filesystem::path();
#else

	// Set zenity to open in our current directory
	char buf[256];
	snprintf(buf, 256, "zenity --file-selection --filename=%s/", open_path.c_str());  
        char filename[1024];

	// open the zenity window
        FILE *f = popen(buf, "r");

	// get filename from zenity
        fgets(filename, 1024, f);

	// remove any newlines
        filename[strcspn(filename, "\n")] = 0;

	// if no file selected, return blank path
        if (strcmp(filename, "") == 0)
                return std::filesystem::path();

	// else, return the const char as a path
        return std::filesystem::path(std::string(filename));
#endif
}
