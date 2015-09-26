/* Coded By    : Daniel Herman Shmulyan
   Version     : 1.3
   Description : Wrapper for cv grabbing functions. 
*/
/****************************************************************************/
/***********************    sfFrameGrabber.h    *****************************/
/****************************************************************************/
/* Wrapps in generic interface different video sources

   1. Online camera

   2. Video file, like "*.AVI"

   3. Text file with list of images in it.
      The list of views may look as following:
			img000.png
			img001.png
			#view002.png
			img003.png
			img010.png
			lastImg.jpg
       Each image file name in new line. Images marked with # will be discarded.

Use this library by calling by:
	sfFrameGrabber* Video  = new sfFrameGrabber(videoFilePath);
	IplImage *curImg = Video->grabFrame();
	while (curImg){
		 // Your code ...
		 cvReleaseImage(&curImg);
		 curImg = Video->grabFrame();
	}
	delete Video;
*/
/****************************************************************************/
// Includes

#include "globalPreDefines.h"
#include <string>
using namespace std;

#if !defined(SFFRAMEGRABBER_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC)
#define SFFRAMEGRABBER_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/******************************** Defines ***********************************/
// Define possible status of frame source.

typedef enum { ONLINE_CAMERA = 0, OFFLINE_VIDEO, FRAMES_TEXT_LIST, FAIL, DEPLETED } tFramesSource;

/****************************************************************************/
class sfFrameGrabber{

      public:
		    
			sfFrameGrabber();             // Constructor for online grabbing from camera
            sfFrameGrabber(string path);  // Constructor for offline grabbing from file

            ~sfFrameGrabber();			  // Destructor

			// Returns current frame. User must release it.
			IplImage* grabFrame(void);

			// Change vertical flipping flag. If flag is set then
			// All frames would be flipped vertically. 
			void setVerticalFlip(bool flag);

			// Change horizontal flipping flag. If flag is set then
			// All frames would be flipped horizontally. 
			void setHorizontalFlip(bool flag);

			// Alternate verical flipping flag
			//void switchVerticalFlip(void);

			// Returns the number of current frame
			int getCurFrameNumber(void);

			// Return true if current video source is online (real time)
			bool isRealTimeGrabbing(void);

			// Return true if current source is video and not sequence of images.
			bool isVideoStreamGrabbing(void);

      private:
	
		    // Source of frames.
		    tFramesSource framesSource;

		    // Path to video file that will be read (only offline grabbing)
		    string videoFilePath;

			// Text file with images list (only offline grabbing)
			FILE* textFile;
			char imageName[IPE_DEFAULT_PATH_LENGTH];

			// CV capture structure
		    CvCapture* capture;

			// Pointer to current frame data
			IplImage*  curFrame;

			// Frame grabber flags
			bool verticalFlipFlag;
			bool horizontalFlipFlag;

			// Frame number
            int frameNumber;
};

#endif // #define SFFRAMEGRABBER_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/
// EOF.
