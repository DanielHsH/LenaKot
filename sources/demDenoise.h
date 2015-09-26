/****************************************************************************/
/**************************    demDenoise.h    ******************************/
/****************************************************************************/

/****************************************************************************/
// Includes

#include "globalPreDefines.h"

#if !defined(DEMDENOISE_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC)
#define DEMDENOISE_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/

namespace demDenoiseNS{

// Pointers to images
static IplImage* srcImage;
static IplImage* yImage;
static IplImage* tstImage;
static IplImage* noiImage;
static IplImage* dstImage;

// Noise/Denoising arguments
static int _rnNoise;
static int _spNoise;
static int _prev_rnNoise;
static int _prev_spNoise;
static int _avgDeNoise;
static int _medDeNoise;
static CvRNG rng;


/****************************************************************************/

// Call back function
void callBackFun(int arg);

} // End of namespace

/****************************************************************************/
// demDenoise class
class demDenoise{

      public:
		    
            demDenoise(const char* path = NULL);

            ~demDenoise();			 

			int runDemo(void);

			const char* getComDesc(void);

      private:
	
		    // Path to image file that will be read
		    char imagePath[IPE_DEFAULT_PATH_LENGTH];
			char command  [IPE_DEFAULT_COMM_LENGTH];
};

#endif // #define DEMDENOISE_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/
// EOF.
