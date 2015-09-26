/****************************************************************************/
/***************************    demPyrCre.h    ******************************/
/****************************************************************************/

/****************************************************************************/
// Includes

#include "globalPreDefines.h"

#if !defined(DEMPYRCRE_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC)
#define DEMPYRCRE_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/

namespace demPyrCreNS{

// Pointers to images
static IplImage* srcImage;
static IplImage* dstImage;

// Other
static int _dGL;

static char winName[] = "Pyramids";

/****************************************************************************/

// Call back function
void callBackFun(int arg);

} // End of namespace

/****************************************************************************/
// demPyrCre class
class demPyrCre{

      public:
		    
            demPyrCre(const char* path = NULL);

            ~demPyrCre();			 

			int runDemo(void);

			const char* getComDesc(void);

      private:
	
		    // Path to image file that will be read
		    char imagePath[IPE_DEFAULT_PATH_LENGTH];
			char command  [IPE_DEFAULT_COMM_LENGTH];
};

#endif // #define DEMPYRCRE_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/
// EOF.
