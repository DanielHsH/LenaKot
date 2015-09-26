/****************************************************************************/
/**************************    demWatershed.h    ****************************/
/****************************************************************************/

/****************************************************************************/
// Includes

#include "globalPreDefines.h"

#if !defined(DEMWATERSHED_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC)
#define DEMWATERSHED_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/

namespace demWatershedNS{

// Pointers to images
static IplImage* srcImage;
static IplImage* gryImage;
static IplImage* watMask;
static IplImage* markMask;
static IplImage* tmpImage;
static IplImage* dstImage;

static int _lineWidth;

// Derivative arguments
static CvPoint prev_pt = {-1,-1};

static char winName1[] = "Original";
static char winName2[] = "Watershed Segments";


/****************************************************************************/

// Mouse Call back function
void callBackFun(int event, int x, int y, int flags, void* );

} // End of namespace

/****************************************************************************/
// demWatershed class
class demWatershed{

      public:
		    
            demWatershed(const char* path = NULL);

            ~demWatershed();			 

			int runDemo(void);

			const char* getComDesc(void);

      private:
	
		    // Path to image file that will be read
		    char imagePath[IPE_DEFAULT_PATH_LENGTH];
			char command  [IPE_DEFAULT_COMM_LENGTH];
};

#endif // #define DEMWATERSHED_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/
// EOF.
