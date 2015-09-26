/****************************************************************************/
/*****(**********************    demFitEl.h    ******************************/
/****************************************************************************/

/****************************************************************************/
// Includes

#include "globalPreDefines.h"

#if !defined(DEMFITEL_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC)
#define DEMFITEL_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/

namespace demFitElNS{

// Pointers to images
static IplImage* srcImage;
static IplImage* gryImage;
static IplImage* tmpImage;
static IplImage* dstImage;

// Derivative arguments
static int _thres;

static char winName1[] = "Ellips Fitting";
//static char winName2[] = "Result";

static CvMemStorage* storage;
static CvSeq* cont;
static CvBox2D32f* box;
static CvPoint* PointArray;
static CvPoint2D32f* PointArray2D32f;
 

/****************************************************************************/

// Call back function
void callBackFun(int arg);

} // End of namespace

/****************************************************************************/
// demFitEl class
class demFitEl{

      public:
		    
            demFitEl(const char* path = NULL);

            ~demFitEl();			 

			int runDemo(void);

			const char* getComDesc(void);

      private:
	
		    // Path to image file that will be read
		    char imagePath[IPE_DEFAULT_PATH_LENGTH];
			char command  [IPE_DEFAULT_COMM_LENGTH];
};

#endif // #define DEMFITEL_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/
// EOF.
