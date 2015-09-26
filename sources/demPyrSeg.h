/****************************************************************************/
/***************************    demPyrSeg.h    ******************************/
/****************************************************************************/

/****************************************************************************/
// Includes

#include "globalPreDefines.h"

#if !defined(DEMPYRSEG_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC)
#define DEMPYRSEG_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/

namespace demPyrSegNS{

// Pointers to images
static IplImage* srcImage;
static IplImage* tmpImage;
static IplImage* dstImage;

static int _th1;
static int _th2;
static int _level;
static int maxLevel;
static int resFactor;

static char winName1[] = "Original";
static char winName2[] = "Segmentation";

static CvSeq* comp;
static CvMemStorage* storage;


/****************************************************************************/

// Mouse Call back function
void callBackFun(int arg);

} // End of namespace

/****************************************************************************/
// demDerive class
class demPyrSeg{

      public:
		    
            demPyrSeg(const char* path = NULL);

            ~demPyrSeg();			 

			int runDemo(void);

			const char* getComDesc(void);

      private:
	
		    // Path to image file that will be read
		    char imagePath[IPE_DEFAULT_PATH_LENGTH];
			char command  [IPE_DEFAULT_COMM_LENGTH];
};

#endif // #define DEMPYRSEG_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/
// EOF.
