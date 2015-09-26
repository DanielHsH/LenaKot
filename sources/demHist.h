/****************************************************************************/
/***************************    demHist.h    ********************************/
/****************************************************************************/

/****************************************************************************/
// Includes

#include "globalPreDefines.h"

#if !defined(DEMHIST_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC)
#define DEMHIST_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/

namespace demHistNS{

// Pointers to images
static IplImage* srcImage;
static IplImage* dstImage;
static IplImage* hisImage;
static IplImage* lutImage;

// histogram parameters
static int _brightness;
static int _contrast;
static int _gamma;
static int histSize;
static float range_0[2];
static float* ranges;
static CvHistogram *hist;
static uchar lut[256];
static CvMat lut_mat;

/****************************************************************************/

// Call back function
void callBackFun(int arg);

} // End of namespace

/****************************************************************************/
// demHist class
class demHist{

      public:
		    
            demHist(const char* path = NULL);

            ~demHist();			 

			int runDemo(void);

			const char* getComDesc(void);

      private:
	
		    // Path to image file that will be read
		    char imagePath[IPE_DEFAULT_PATH_LENGTH];
			char command  [IPE_DEFAULT_COMM_LENGTH];
};

#endif // #define DEMHIST_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/
// EOF.
