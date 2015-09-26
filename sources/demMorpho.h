/****************************************************************************/
/**************************    demMorpho.h    *******************************/
/****************************************************************************/

/****************************************************************************/
// Includes

#include "globalPreDefines.h"

#if !defined(DEMMORPHO_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC)
#define DEMMORPHO_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/

namespace demMorphoNS{

// Pointers to images
static IplImage* srcImage;
static IplImage* dstImage;

// Strel arguments
static IplConvKernel* element;
static int elementShape;

// The address of variable which receives trackbar position update 
#define MAX_ITER   (10)
static int _openClose;
static int _erodeDilate;

/****************************************************************************/

// Call back function
void callBackFun(int arg);

} // End of namespace

/****************************************************************************/
// demMorpho class
class demMorpho{

      public:
		    
            demMorpho(const char* path = NULL);

            ~demMorpho();			 

			int runDemo(void);

			const char* getComDesc(void);

      private:
	
		    // Path to image file that will be read
		    char imagePath[IPE_DEFAULT_PATH_LENGTH];
			char command  [IPE_DEFAULT_COMM_LENGTH];
};

#endif // #define DEMMORPHO_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/
// EOF.
