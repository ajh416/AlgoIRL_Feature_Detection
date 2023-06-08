/*
 * How to compile with gcc: g++ src/main.cpp src/Image.cpp
 * src/FeatureDetection.cpp -O2 -o sobel When using Windows, you must also link
 * against comdlg32.lib for some reason MSVC links against it automagically, but
 * clang must be manually specified with "-l comdlg32.lib" Mingw does not seem
 * to work! Command for Windows: clang++ src/main.cpp src/Image.cpp
 * src/FeatureDetection.cpp -l comdlg32.lib -O2 -o sobel.exe
 * For filesystem support, we use the standard filesystem which requires at minimum c++17.
 * As such, add the flag -std=c++17
 *
 * Authors: Adam Henry, Shane Ludwig
 */

#include "FeatureDetection.hpp"
#include "Image.hpp"
#include "Timer.hpp"

#include <cmath>
#include <cstring>
#include <filesystem>

#ifdef _WIN32
// Needed for file dialog
#include <Windows.h>
#endif

#define ASSERT(x, ...)                                                         \
  if (!(x))                                                                    \
    fprintf(stderr,                                                            \
            __VA_ARGS__); // allows for printf formatting into this assert

// open_path is only used on linux, filter is only used on windows
std::string OpenFileDialog(const std::string &open_path = std::string(),
                           const char *filter = "All\0*.*\0");

int main() {
  const std::string current_path = std::filesystem::current_path().string();

  // open a dialog window
  std::string input_image = OpenFileDialog(current_path);
  if (input_image == std::string()) {
    printf("No file selected!\n");
    return 1;
  }

  // aims to remove some of the noise, not sure how much it really does
  printf("Apply gaussian blur first? (y/n): ");
  char gaussian[10];
  scanf("%s", gaussian);

  int gaus = 0;
  if (strcmp(gaussian, "y") == 0)
    gaus = 1;

  // beware, extremely noisy
  printf("Direction of gradient? (y/n): ");
  char direction[10];
  scanf("%s", direction);

  int dir = 0;
  if (strcmp(direction, "y") == 0)
    dir = 1;

  // load the image into memory
  Image image(input_image.c_str());

  // gaussian blur makes "noise" in the image less visible
  if (gaus)
    image.gaussian_blur();

  /* DOING THIS CAUSES A SEGFAULT (temp retval dies after exiting the scope)
  Image sobel = Image(0, 0, 0);
  {
          sobel = Image(FeatureDetection::SobelOperator(&image, dir));
  }
  */

  // for reference, really
  printf("Beginning convolution!\n");

  // do this to use timer, make image with the same size as the one we are
  // convolving, and memcpy the data in from the temp variable probably not the
  // best solution for this silly problem
  image = image.grayscale_avg();
  image.write("grayscale.png");
  Image sobel = Image(image.w, image.h, image.channels);
  {
    // Create a timer, stops when it dies (end of scope)
    Timer t("SobelOperator(+memcpy)");
    Image temp = FeatureDetection::SobelOperator(&image);

    // memcpy needed to avoid the temp object dying and causing a segfault
    memcpy(sobel.data, temp.data, temp.size);
  }

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
std::string OpenFileDialog(const std::string &open_path, const char *filter) {
#ifdef _WIN32
  OPENFILENAMEA ofn;      // common dialog box structure
  CHAR szFile[260] = {0}; // if using TCHAR macros

  // Initialize OPENFILENAME (memset to zero)
  ZeroMemory(&ofn, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = NULL;
  ofn.lpstrFile = szFile;
  ofn.nMaxFile = sizeof(szFile);
  ofn.lpstrFilter = filter;
  ofn.nFilterIndex = 1;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

  if (GetOpenFileNameA(&ofn) == TRUE) {
    std::string fp = ofn.lpstrFile;
    for (size_t i = 0; i < fp.size(); i++) {
      if (fp[i] == '\\')
        fp[i] = '/';
    }
    // std::replace(fp.begin(), fp.end(), '\\', '/');
    return fp;
  }

  return std::string();
#else

  // Set zenity to open in our current directory
  char buf[256];
  snprintf(buf, 256, "zenity --file-selection --filename=%s/",
           open_path.c_str());
  char filename[1024];

  // open the zenity window
  FILE *f = popen(buf, "r");

  // get filename from zenity
  fgets(filename, 1024, f);

  // remove any newlines
  filename[strcspn(filename, "\n")] = 0;

  // if no file selected, return blank path
  if (strcmp(filename, "") == 0)
    return std::string();

  // else, return the string
  return std::string(filename);
#endif
}
