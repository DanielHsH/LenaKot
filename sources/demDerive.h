/****************************************************************************/
/**************************    demDerive.h    *******************************/
/****************************************************************************/

/****************************************************************************/
// Includes

#include "globalPreDefines.h"

#if !defined(DEMDERIVE_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC)
#define DEMDERIVE_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/

namespace demDeriveNS{

// Pointers to images
static IplImage* srcImage;
static IplImage* tmpImage;
static IplImage* dstImage;

// Derivative arguments
static int _orientation;
static int _magnitude;

/****************************************************************************/

// Call back function
void callBackFun(int arg);

} // End of namespace

/****************************************************************************/
// demDerive class
class demDerive{

      public:
		    
            demDerive(const char* path = NULL);

            ~demDerive();			 

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
