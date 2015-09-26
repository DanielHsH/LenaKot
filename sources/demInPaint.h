/****************************************************************************/
/**************************    demInpaint.h    ******************************/
/****************************************************************************/

/****************************************************************************/
// Includes

#include "globalPreDefines.h"

#if !defined(DEMDINPAINT_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC)
#define DEMDINPAINT_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/

namespace demInpaintNS{

// Pointers to images
static IplImage* srcImage;
static IplImage* inpaintMask;
static IplImage* tmpImage;
static IplImage* dstImage;

static int _lineWidth;

// Derivative arguments
static CvPoint prev_pt = {-1,-1};

/****************************************************************************/

// Mouse Call back function
void callBackFun(int event, int x, int y, int flags, void* );

} // End of namespace

/****************************************************************************/
// demInpaint class
class demInpaint{

      public:
		    
            demInpaint(const char* path = NULL);

            ~demInpaint();			 

			int runDemo(void);

			const char* getComDesc(void);

      private:
	
		    // Path to image file that will be read
		    char imagePath[IPE_DEFAULT_PATH_LENGTH];
			char command  [IPE_DEFAULT_COMM_LENGTH];
};

#endif // #define DEMDINPAINT_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/
// EOF.
