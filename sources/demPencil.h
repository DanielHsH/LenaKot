/****************************************************************************/
/**************************    demPencil.h    *******************************/
/****************************************************************************/

/****************************************************************************/
// Includes

#include "globalPreDefines.h"

#if !defined(DEMPENCIL_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC)
#define DEMPENCIL_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC


/****************************************************************************/

namespace demPencilNS{

// Main inner method which activates the demo
int demPencilLiveCam(int vidSource);

// Pointers to images
static IplImage* srcImage;
static IplImage* dstImage;

// Derivative arguments
static int _intensNormal;
static int _colorNormal;
static int _looseDitails;
static int _edgeSharpness;
static int _noiseRatio;
static int _colorBits;

static char winName1[] = "Original";
static char winName2[] = "Pencil sketch";

/****************************************************************************/

// Call back function
void callBackFun(int arg);

} // End of namespace

/****************************************************************************/
// demPencil class
class demPencil{

      public:
		    
            demPencil(const char* path = NULL);

            ~demPencil();			 

			int runDemo(void);

			const char* getComDesc(void);

      private:
	
		    // Path to image file that will be read
		    char imagePath[IPE_DEFAULT_PATH_LENGTH];
			char command  [IPE_DEFAULT_COMM_LENGTH];
};

#endif // #define DEMDERIVE_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/
// EOF.
