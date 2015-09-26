/****************************************************************************/
/***************************    demFishEye.h    *****************************/
/****************************************************************************/

/****************************************************************************/
// Includes

#include "globalPreDefines.h"

#if !defined(DEMFISHEYE_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC)
#define DEMFISHEYE_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/

namespace demFishEyeNS{

// Pointers to images
static IplImage* srcImage;
static IplImage* dstImage;

// Other
static int _r;
static int _tX;
static int _tY;
static int allRange;

// Memory allocations for cooficient matrices.
static double _camera[9];
static double _dist4Coeff[4];

static char winName[] = "Fish Eye";

/****************************************************************************/

// Call back function
void callBackFun(int arg);

} // End of namespace

/****************************************************************************/
// demFishEye class
class demFishEye{

      public:
		    
            demFishEye(const char* path = NULL);

            ~demFishEye();			 

			int runDemo(void);

			const char* getComDesc(void);

      private:
	
		    // Path to image file that will be read
		    char imagePath[IPE_DEFAULT_PATH_LENGTH];
			char command  [IPE_DEFAULT_COMM_LENGTH];
};

#endif // #define DEMFISHEYE_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/
// EOF.
