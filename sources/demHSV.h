/****************************************************************************/
/****************************    demHSV.h    ********************************/
/****************************************************************************/

/****************************************************************************/
// Includes

#include "globalPreDefines.h"

#if !defined(DEMHSV_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC)
#define DEMHSV_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/

namespace demHSVNS{

// Pointers to images
static IplImage* srcImage;
static IplImage* hImage;
static IplImage* sImage;
static IplImage* vImage;
static IplImage* dstImage;

// Other
static int maxHue;
static int maxSat;
static int maxVal;
static int _hue;
static int _sat;
static int _val;

/****************************************************************************/

// Call back function
void callBackFun(int arg);

} // End of namespace

/****************************************************************************/
// demHSV class
class demHSV{

      public:
		    
            demHSV(const char* path = NULL);

            ~demHSV();			 

			int runDemo(void);

			const char* getComDesc(void);

      private:
	
		    // Path to image file that will be read
		    char imagePath[IPE_DEFAULT_PATH_LENGTH];
			char command  [IPE_DEFAULT_COMM_LENGTH];
};

#endif // #define DEMHSV_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/
// EOF.
