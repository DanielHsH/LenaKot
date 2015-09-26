/****************************************************************************/
/**************************    demDistTr.h    *******************************/
/****************************************************************************/

/****************************************************************************/
// Includes

#include "globalPreDefines.h"

#if !defined(DEMDISTTR_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC)
#define DEMDISTTR_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/

namespace demDistTrNS{

// Pointers to images
static IplImage* srcImage;
static IplImage* tmpImage;
static IplImage* dstImage;

// Strel arguments
static IplConvKernel* element;
static int elementShape;

// The address of variable which receives trackbar position update 
static int _objTresh;
static int _SwitchObjBack;
static int _distType;
static int _maskSize;

/****************************************************************************/

// Call back function
void callBackFun(int arg);

} // End of namespace

/****************************************************************************/
// demMorpho class
class demDistTr{

      public:
		    
            demDistTr(const char* path = NULL);

            ~demDistTr();			 

			int runDemo(void);

			const char* getComDesc(void);

      private:
	
		    // Path to image file that will be read
		    char imagePath[IPE_DEFAULT_PATH_LENGTH];
			char command  [IPE_DEFAULT_COMM_LENGTH];
};

#endif // #define DEMDISTTR_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/
// EOF.
