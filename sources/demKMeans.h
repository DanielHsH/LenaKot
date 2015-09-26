/****************************************************************************/
/**************************    demKMeans.h    *******************************/
/****************************************************************************/

/****************************************************************************/
// Includes

#include "globalPreDefines.h"

#if !defined(DEMKMEANS_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC)
#define DEMKMEANS_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/
// demKMeans class
class demKMeans{

      public:
		    
            demKMeans(const char* path = NULL);

            ~demKMeans();			 

			int runDemo(void);

			const char* getComDesc(void);

      private:
	
		    // Path to image file that will be read
		    char imagePath[IPE_DEFAULT_PATH_LENGTH];
			char command  [IPE_DEFAULT_COMM_LENGTH];

			// Private variables 
			#define MAX_KMEANS_CLUSTERS 5
		    CvScalar color_tab[MAX_KMEANS_CLUSTERS];
			IplImage* srcImage;
			CvRNG rng;
			CvPoint ipt;
};

#endif // #define DEMKMEANS_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/
// EOF.
