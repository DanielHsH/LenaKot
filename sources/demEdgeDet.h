/****************************************************************************/
/**************************    demEdgeDet.h    ******************************/
/****************************************************************************/

/****************************************************************************/
// Includes

#include "globalPreDefines.h"

#if !defined(DEMDEDGEDET_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC)
#define DEMDEDGEDET_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/

namespace demEdgeDetNS{

// Pointers to images
static IplImage* srcImage;
static IplImage* gryImage;
static IplImage* tmpImage;
static IplImage* dstImage;

// Canny threshold
static int _edge_thresh;
static int _magnitude;

/****************************************************************************/

// Call back function
void callBackFun(int arg);

} // End of namespace

/****************************************************************************/
// demDerive class
class demEdgeDet{

      public:
		    
            demEdgeDet(const char* path = NULL);

            ~demEdgeDet();			 

			int runDemo(void);

			const char* getComDesc(void);

      private:
	
		    // Path to image file that will be read
		    char imagePath[IPE_DEFAULT_PATH_LENGTH];
			char command  [IPE_DEFAULT_COMM_LENGTH];
};

#endif // #define DEMDEDGEDET_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/
// EOF.
