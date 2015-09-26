/****************************************************************************/
/*&&*************************    demFfill.h    *******************************/
/****************************************************************************/

/****************************************************************************/
// Includes

#include "globalPreDefines.h"

#if !defined(DEMFFILL_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC)
#define DEMFFILL_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/

namespace demFfillNS{

// Pointers to images
static IplImage* srcImage;
static IplImage* gryImage;
static IplImage* tmpImage;
static IplImage* dstImage;
static IplImage* mskImage;

static int _algType;
static int _loDiff; 
static int _upDiff;
static int connectType;
static int isColorFlag;
static int isMaskFlag;
static int newMaskVal;


static char winName1[] = "Flood Fill Segments";
static char winName2[] = "Mask";

/****************************************************************************/

// Mouse Call back function
void callBackFun(int event, int x, int y, int flags, void* );

} // End of namespace

/****************************************************************************/
// demFfill class
class demFfill{

      public:
		    
            demFfill(const char* path = NULL);

            ~demFfill();			 

			int runDemo(void);

			const char* getComDesc(void);

      private:
	
		    // Path to image file that will be read
		    char imagePath[IPE_DEFAULT_PATH_LENGTH];
			char command  [IPE_DEFAULT_COMM_LENGTH];
};

#endif // #define DEMFFILL_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/
// EOF.
