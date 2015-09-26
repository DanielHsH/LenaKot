/****************************************************************************/
/**************************    demPerspect.h    *****************************/
/****************************************************************************/

/****************************************************************************/
// Includes

#include "globalPreDefines.h"

#if !defined(DEMPERSPECT_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC)
#define DEMPERSPECT_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/

namespace demPerspectNS{

// Pointers to images
static IplImage* srcImage;
static IplImage* dstImage;

// Other
static int _dX;
static int _dY;
static int _pX;
static int _pY;

static double _transMat[9];

static char winName[] = "Perspective";

/****************************************************************************/

// Call back function
void callBackFun(int arg);

} // End of namespace

/****************************************************************************/
// demPerspect class
class demPerspect{

      public:
		    
            demPerspect(const char* path = NULL);

            ~demPerspect();			 

			int runDemo(void);

			const char* getComDesc(void);

      private:
	
		    // Path to image file that will be read
		    char imagePath[IPE_DEFAULT_PATH_LENGTH];
			char command  [IPE_DEFAULT_COMM_LENGTH];
};

#endif // #define DEMPERSPECT_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/
// EOF.
