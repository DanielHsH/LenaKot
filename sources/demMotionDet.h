/****************************************************************************/
/***************************    demMotionDet.h    ***************************/
/****************************************************************************/

/****************************************************************************/
// Includes

#include "globalPreDefines.h"
#include "sfCVextension.h"

#if !defined(DEMMOTIONDET_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC)
#define DEMMOTIONDET_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/
// Main inner method which activates the demo
int demMotionDetMain(int vidSource);

/****************************************************************************/
// Dummy Class. Just for compatibility with LenaKot package
class demMotionDet{
      public:		    
            demMotionDet(const char* path = NULL){

				imagePath[0] = 0;
				if (path){
				   imagePath[0] = 1;
				}
				
				// Define the command
				safeStrBufCpy(command,"Motion Detection and tracking");
			}

			~demMotionDet(){			 
			}

			int runDemo(void){
				demMotionDetMain(1);//imagePath[0]);
				return 0;
			}

			const char* getComDesc(void){
				return command;
			}

      private:
	
		    // Path to image file that will be read
		    char imagePath[IPE_DEFAULT_PATH_LENGTH];
			char command  [IPE_DEFAULT_COMM_LENGTH];
};


#endif // #define DEMMOTIONDET_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/
// EOF.
