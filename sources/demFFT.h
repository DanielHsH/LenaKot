/****************************************************************************/
/******(**********************    demFFT.h    *******************************/
/****************************************************************************/

/****************************************************************************/
// Includes

#include "globalPreDefines.h"

#if !defined(DEMFFT_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC)
#define DEMFFT_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/

namespace demFFTNS{

// Pointers to images
static IplImage* srcImage;
static IplImage* tmpImage;
static IplImage* noiImage;
static IplImage* realInput;
static IplImage* imaginaryInput;
static IplImage* complexInput;
static IplImage* image_Re;
static IplImage* image_Im;

static CvMat* dft_A;

// Derivative arguments
static int _smooth;
static int _rnNoise;
static int _logScale;
static CvRNG rng;

static char winName1[] = "Original";
static char winName2[] = "FFT";

/****************************************************************************/

// Call back function
void callBackFun(int arg);

// Implement FFT shift (copy central freq to the center of the image)
void cvShiftDFT(CvArr* srcArr, CvArr* dstArr);


} // End of namespace

/****************************************************************************/
// demFFT class
class demFFT{

      public:
		    
            demFFT(const char* path = NULL);

            ~demFFT();			 

			int runDemo(void);

			const char* getComDesc(void);

      private:
	
		    // Path to image file that will be read
		    char imagePath[IPE_DEFAULT_PATH_LENGTH];
			char command  [IPE_DEFAULT_COMM_LENGTH];
};

#endif // #define DEMFFT_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/
// EOF.
