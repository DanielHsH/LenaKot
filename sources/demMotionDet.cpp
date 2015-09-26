#include "stdafx.h"
#include "sfFrameGrabber.h"
#include "sfTracker.h"


/****************************************************************************/
// Defines 

// main debug windows command
#define MAIN_SF_WINDOW_COMMAND_OPEN   (0)
#define MAIN_SF_WINDOW_COMMAND_UPDATE (1)
#define MAIN_SF_WINDOW_COMMAND_CLOSE  (2)

// Macro for deleting non NULL pointers on objects
#define sfDelete(ptr) if (ptr!=NULL) {delete ptr;}

// Macro for silent returning main value
#define sfDeleteAll_Return(val)							 \
							sfDelete(sfTrackers);		 \
							sfDelete(Video);			 \
							return val; 

/****************************************************************************/
// Inner variables

// ---------------------------------------
// Swordfish classes
sfFrameGrabber*	Video                        = NULL;		// Video source 
sfMultiTracker* sfTrackers					 = NULL;		// Tracking class

// ---------------------------------------
// Multitracker default parameters override.
int mdThreshold         = SF_INI_MOTIN_THRESH;          // motion detection threshold 
														// trackbar update.
int newMinPixSizeObject = DEFAULT_SF_VALUE; 
int newMaxPixSizeObject = DEFAULT_SF_VALUE; 

int newCtVmin           = DEFAULT_SF_VALUE; 
int newCtVmax           = DEFAULT_SF_VALUE; 
int newCtSmin		    = DEFAULT_SF_VALUE;

// File path's
char videoFilePath   [SF_DEFAULT_PATH_LENGTH];
char camParamFilePath[SF_DEFAULT_PATH_LENGTH];

// Reporting socket information is not needed by main()

// Debug information
bool debugFlag          = false;
char newLogFilePath  [SF_DEFAULT_PATH_LENGTH];


// ---------------------------------------
// Other variables

IplImage *curImg;                        // Current video frame
int keyPressed;                          // Current keyboard command

/****************************************************************************/
/*************************    Assist methods    *****************************/
/****************************************************************************/

/****************************************************************************/
// Callback function for motion detection treshold slider 
void onMDthreshChange(int mdThreshold){
	sfTrackers->setMotionDetectionThreshold(mdThreshold);
}

/****************************************************************************/
// Methods which manages swordfish tracker windows. Executes command given by
// main()
void updateWindows(int action){

	switch (action){

	case MAIN_SF_WINDOW_COMMAND_OPEN:

		  // Open source video window
		  cvNamedWindow("source", CV_WINDOW_AUTOSIZE );
		  cvCreateTrackbar("MD Thres.","source",&mdThreshold,80,onMDthreshChange);
		  // cvMoveWindow("source", 690, 0);

		  // Open debug windows
		  cvNamedWindow("backGrnd", CV_WINDOW_AUTOSIZE);
		  cvNamedWindow("forGrnd" , CV_WINDOW_AUTOSIZE);
		  cvNamedWindow("MHI"     , CV_WINDOW_AUTOSIZE);
		break;
	
	case MAIN_SF_WINDOW_COMMAND_UPDATE:
		   cvShowImage("forGrnd", sfTrackers->getForGroundImage());
		   cvShowImage("backGrnd",sfTrackers->getBackGroundImage());
		   cvShowImage("MHI",sfTrackers->getMotionHistoryImage());		   
		   cvShowImage("source",sfTrackers->getMainDebugImage());
		break;

	case MAIN_SF_WINDOW_COMMAND_CLOSE:
			cvDestroyWindow("source");
			cvDestroyWindow("forGrnd");
			cvDestroyWindow("backGrnd");
			cvDestroyWindow("MHI");
		break;
	}
}

/****************************************************************************/
/***************************    Main method    ******************************/
/****************************************************************************/

int demMotionDetMain(int isCamera){

	// ---------------------------------------
	// Initialize video source (if no source then use online camera)
	if (!isCamera){
	   safeStrBufCpy(videoFilePath,"TrackVideo.avi");
	   Video      = new sfFrameGrabber(videoFilePath);
	}
	else{
	   videoFilePath[0] = 0;
	   Video      = new sfFrameGrabber();
	}
	
	curImg = Video->grabFrame();
	if (!curImg){ 
	   // Retry movie from hard-disk
	   sfDelete(Video);
	   safeStrBufCpy(videoFilePath,"TrackVideo.avi");
	   Video      = new sfFrameGrabber(videoFilePath);
	   curImg = Video->grabFrame();

	   if (!curImg){
	      fprintf(stderr,"Error: Unable to acquire video frames!\n");
	      sfDeleteAll_Return(SF_MULTI_TRACKER_ERR_VIDEO_FILE);
	   }
	}


	// ---------------------------------------
	// Initialize trackers
	sfTrackers = new sfMultiTracker(debugFlag);
	sfTrackers->proccessFrame(curImg);
	
	// ---------------------------------------
    // Open program windows and prepare to the main loop.
	updateWindows(MAIN_SF_WINDOW_COMMAND_OPEN);

	// Grab next frame
	cvReleaseImage(&curImg);
	curImg = Video->grabFrame();

	// ---------------------------------------
	// main loop
	while (curImg){

		// For debug only: Skip N first frames
		if (Video->getCurFrameNumber() < 0){ 
			// Get the next frame.
			cvReleaseImage(&curImg);
			curImg = Video->grabFrame();			
			continue;
		}

		// Proccess new frame
		sfTrackers->proccessFrame(curImg);
		updateWindows(MAIN_SF_WINDOW_COMMAND_UPDATE);
		
		// Get the next frame.
		cvReleaseImage(&curImg);
		curImg = Video->grabFrame();

		// Proccess user commands
		keyPressed = cvWaitKey(1);
 		switch (keyPressed){

		case -1: 
	  			// No key 
				break;

		case 27: 
	  			// Quit
				if (curImg){
				   cvReleaseImage( &curImg );
				   curImg = NULL;
				}
				break;

		case 'p': 
		case 'P':
		case 32 :          // space
	  			// pause
			    cvWaitKey(0);
				break;
		}
	}

	// End of main loop
	// ---------------------------------------
	
	// Program has to terminate
	updateWindows(MAIN_SF_WINDOW_COMMAND_CLOSE);

    // Delete swordfish classes
	sfDeleteAll_Return(SF_SUCCESS);
}

/****************************************************************************/
// EOF.
