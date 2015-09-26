/****************************************************************************/
/****************************    demRes.h    ********************************/
/****************************************************************************/

/****************************************************************************/
// Includes

#include "globalPreDefines.h"

#if !defined(DEMRES_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC)
#define DEMRES_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/

namespace demResNS{

// Pointers to images
static IplImage* srcImage;
static IplImage* tmpImage;
static IplImage* dstImage;

// histogram parameters
static int _spacial;
static int _depth;
static int _freq;

static int interpolationType;

/****************************************************************************/

// Call back function
void callBackFun(int arg);

} // End of namespace

/****************************************************************************/
// demRes class
class demRes{

      public:
		    
            demRes(const char* path = NULL);

            ~demRes();			 

			int runDemo(void);

			const char* getComDesc(void);

      private:
	
		    // Path to image file that will be read
		    char imagePath[IPE_DEFAULT_PATH_LENGTH];
			char command  [IPE_DEFAULT_COMM_LENGTH];
};

#endif // #define DEMRES_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/
// EOF.
