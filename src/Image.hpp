/********************************************************************************
-----------------------------------------------------------------------------
PROGRAM DEVELOPED BY ADAM H. USING STB_IMAGE AND STB_IMAGE_WRITE
-----------------------------------------------------------------------------
 * THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
********************************************************************************/

#pragma once
#include <stdint.h>
#include <stdio.h>
#include <string>

enum ImageType
{
	PNG,
	JPG
};

struct Image
{
	uint8_t *data = NULL;
	size_t size = 0;
	ImageType imageType;
	const char *imageExt;
	int w;
	int h;
	int channels;

	Image(const char *filename);
	Image(int w, int h, int channels);
	Image(const Image &img);
	~Image();

	bool read(const char *filename);
	bool write(const char *filename);

	ImageType getFileType(const char *filetype);

	Image &convolve_clamp_to_0(uint8_t channel, uint32_t ker_w, uint32_t ker_h, double ker[], uint32_t cr, uint32_t cc);
	Image &convolve_clamp_to_border(uint8_t channel, uint32_t ker_w, uint32_t ker_h, double ker[], uint32_t cr, uint32_t cc);

	Image &grayscale();
	Image &gaussian_blur();
	Image &ridge_detection_4();
	Image &ridge_detection_8();
	Image &sharpen();
	Image &diffmap(Image& img);
};
