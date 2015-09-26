/****************************************************************************/
/****************************    demFindObj.h    *****************************/
/****************************************************************************/

/****************************************************************************/
// Includes

#include "globalPreDefines.h"
#include "sfCVextension.h"

#include <vector>

#if !defined(DEMFINDOBJ_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC)
#define DEMFINDOBJ_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/
// Main inner method which activates the demo
int demFindObjMain(void);

/****************************************************************************/
// Dummy Class. Just for compatibility with LenaKot package
class demFindObj{
      public:		    
            demFindObj(const char* path = NULL){
				// Define the command
				safeStrBufCpy(command,"Surf - Speed up robust feature example. Implemented by: Liu Liu, 1987");
			}

			~demFindObj(){			 
			}

			int runDemo(void){
				demFindObjMain();
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


#endif // #define DEMFINDOBJ_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/
// EOF.
