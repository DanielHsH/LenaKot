/****************************************************************************/
/****************************    demKalman.h    *****************************/
/****************************************************************************/
/*  Tracking of rotating point.
    Rotation speed is constant.
    Both state and measurements vectors are 1D (a point angle),
    Measurement is the real point angle + gaussian noise.
    Pressing any key (except ESC) will reset the tracking with a different speed.
    Pressing ESC will stop the program.

/****************************************************************************/
// Includes

#include "globalPreDefines.h"

#if !defined(DEMKALMAN_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC)
#define DEMKALMAN_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/

namespace demKalmanNS{

// Pointers to images
static IplImage* srcImage;

// Other
static int _displayType;
static CvKalman* kalman;
static CvMat* state;
static CvMat* processNoise;
static CvMat* measurement;

static char winName[] = "Kalman (Enemy bomber is circling your base!)";
static char targetSymbol[]     = "T";
static char predictionSymbol[] = "P";
static char measuredSymbol[]   = "M";

/****************************************************************************/

// Call back function
void callBackFun(int arg);

} // End of namespace

/****************************************************************************/
// demKalman class
class demKalman{

      public:
		    
            demKalman(const char* path = NULL);

            ~demKalman();			 

			int runDemo(void);

			const char* getComDesc(void);

      private:
	
		    // Path to image file that will be read
		    char imagePath[IPE_DEFAULT_PATH_LENGTH];
			char command  [IPE_DEFAULT_COMM_LENGTH];
};

#endif // #define DEMKALMAN_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/
// EOF.
