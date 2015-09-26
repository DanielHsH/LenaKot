/****************************************************************************/
/***************************    demContour.h    *****************************/
/****************************************************************************/

/****************************************************************************/
// Includes

#include "globalPreDefines.h"

#if !defined(DEMCONTOUR_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC)
#define DEMCONTOUR_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/

namespace demContourNS{

// Pointers to images
static IplImage* srcImage;
static IplImage* tmpImage;
static IplImage* dstImage;

// Other
static int _levels;

static char winName[] = "Contour";

static CvSeq* contours;
static CvMemStorage* storage;

/****************************************************************************/

// Call back function
void callBackFun(int arg);

} // End of namespace

/****************************************************************************/
// demContour class
class demContour{

      public:
		    
            demContour(const char* path = NULL);

            ~demContour();			 

			int runDemo(void);

			const char* getComDesc(void);

      private:
	
		    // Path to image file that will be read
		    char imagePath[IPE_DEFAULT_PATH_LENGTH];
			char command  [IPE_DEFAULT_COMM_LENGTH];
};

#endif // #define DEMCONTOUR_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/
// EOF.
