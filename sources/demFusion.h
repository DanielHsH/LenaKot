/****************************************************************************/
/****************************    demFusion.h    *****************************/
/****************************************************************************/

/****************************************************************************/
// Includes

#include "globalPreDefines.h"

#if !defined(DEMFUSION_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC)
#define DEMFUSION_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/

namespace demFusionNS{

// Pointers to images
static IplImage* srcImage1;
static IplImage* srcImage2;
static IplImage* dstImage;

// Other
static int _alpha;
static int _set;
static int _prevSet;

static char winName[] = "IR/ICCD Fusion";
static char imgName[] = "Fuse0.jpg";

/****************************************************************************/

// Call back function
void callBackFun(int arg);

} // End of namespace

/****************************************************************************/
// demFusion class
class demFusion{

      public:
		    
            demFusion(const char* path = NULL);

            ~demFusion();			 

			int runDemo(void);

			const char* getComDesc(void);

      private:
	
		    // Path to image file that will be read
		    char imagePath[IPE_DEFAULT_PATH_LENGTH];
			char command  [IPE_DEFAULT_COMM_LENGTH];
};

#endif // #define DEMFUSION_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/
// EOF.
