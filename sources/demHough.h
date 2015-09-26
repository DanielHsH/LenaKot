/****************************************************************************/
/******(**********************    demHough.h    *******************************/
/****************************************************************************/

/****************************************************************************/
// Includes

#include "globalPreDefines.h"

#if !defined(DEMHOUGH_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC)
#define DEMHOUGH_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/

namespace demHoughNS{

// Pointers to images
static IplImage* srcImage;
static IplImage* gryImage;
static IplImage* dstImage;

// Derivative arguments
static int _maxR;
static int _accT;

static char winName1[] = "Hough Detected";

static CvSeq* circles;
static CvSeq* lines;
static CvMemStorage* storage;


/****************************************************************************/

// Call back function
void callBackFun(int arg);

} // End of namespace

/****************************************************************************/
// demHough class
class demHough{

      public:
		    
            demHough(const char* path = NULL);

            ~demHough();			 

			int runDemo(void);

			const char* getComDesc(void);

      private:
	
		    // Path to image file that will be read
		    char imagePath[IPE_DEFAULT_PATH_LENGTH];
			char command  [IPE_DEFAULT_COMM_LENGTH];
};

#endif // #define DEMHOUGH_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/
// EOF.
