/****************************************************************************/
/***************************    demAffine.h    ******************************/
/****************************************************************************/

/****************************************************************************/
// Includes

#include "globalPreDefines.h"

#if !defined(DEMAFFINE_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC)
#define DEMAFFINE_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/

namespace demAffineNS{

// Pointers to images
static IplImage* srcImage;
static IplImage* dstImage;

// Other
static int _dX;
static int _dY;
static int _skX;
static int _skY;
static int _scaleX;
static int _scaleY;

static double _transMat[6];

static char winName[] = "Affine";

/****************************************************************************/

// Call back function
void callBackFun(int arg);

} // End of namespace

/****************************************************************************/
// demAffine class
class demAffine{

      public:
		    
            demAffine(const char* path = NULL);

            ~demAffine();			 

			int runDemo(void);

			const char* getComDesc(void);

      private:
	
		    // Path to image file that will be read
		    char imagePath[IPE_DEFAULT_PATH_LENGTH];
			char command  [IPE_DEFAULT_COMM_LENGTH];
};

#endif // #define DEMAFFINE_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/
// EOF.
