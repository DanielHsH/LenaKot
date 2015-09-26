/* Coded By    : Daniel Herman Shmulyan
   Version     : 1.2
   Description : Swordfish multi tracker class. This class is responsible for
                 motion detection and tracking few objects in parallel.
*/
/****************************************************************************/
/************************    sfMultiTracker.cpp    **************************/
/****************************************************************************/

/****************************************************************************/
// Includes
#include "stdafx.h"
#include "sfTracker.h"

#define SF_MULTI_TRACKER_CAMERA_PATH    "camera" 


/****************************************************************************/
// Constructor

sfMultiTracker::sfMultiTracker(bool debugOn){

	// Initialize counters and constants
	trackStatus				= IDLE;
	motionThresh			= SF_INI_MOTIN_THRESH;
	minPixSizeObject		= SF_INI_MIN_SIZE_OBJ;
	minPixObjArea			= minPixSizeObject*minPixSizeObject;
	maxPixObjArea			= SF_INI_MAX_AREA_OBJ;

	frameNum				= 0;
	numDetectedMotions		= 0;
	numTrackedObjs			= 0;


	// Video Frames
	prevFrame				= NULL;
	curFrame				= NULL;
	curFrame32F				= NULL; 
	bgImage32F				= NULL;
	fgImage8U   			= NULL;
	mask8U   				= NULL;
	gradOrient32F		    = NULL;

	// Motion history image and matrix
	mhImage					= NULL;
	mhMatrix				= NULL;

	tmpImg32F				= NULL;  

	// Images for color tracking
	curFrameHSV_8U3         = NULL;
	curFrameHue_8U          = NULL;	
	backproject				= NULL;

	// Debug image
	mainDebugImage			= NULL;
	
	storage					= NULL;
	seq						= NULL;

	isColoredVideo			= false;

	hNumBins			    = SF_COLOR_HISTOGRAM_SIZE;
	hHueRangeArr[0]			= 0;  	// Min Hue is 0 degrees.
	hHueRangeArr[1]			= 180;  // Max Hue is 180 degrees.
	phHueRange				= hHueRangeArr;
	convergRule             = cvTermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 );

	ctVmin					= SF_INI_HSV_MIN_VALUE; 
	ctVmax					= SF_INI_HSV_MAX_VALUE; 
	ctSmin					= SF_INI_HSV_MIN_SATURATION;

	// Zero trackers memory
	for (int i = 0; i < SF_MAX_TRACK_OBJS; i++){
		tracksList[i].visualId  = NULL;	
		tracksList[i].hist = NULL;
	}			

	// -------------------------
	// Update debug flag and information
	debugFlag = debugOn;
	SF_DEBUG_CODE {
		safeStrBufCpy(logFilePath,LOG_FILE_NAME);
	}
	else{
		logFilePath[0] = '\0';
	}
	logFile = NULL;

}

/****************************************************************************/
// Destructor

sfMultiTracker::~sfMultiTracker(){
	trackStatus      = IDLE;

	// Clear allocated memory
	cvExtReleaseImageCheckNULL(&prevFrame);
	cvExtReleaseImageCheckNULL(&curFrame);
	cvExtReleaseImageCheckNULL(&curFrame32F);
	cvExtReleaseImageCheckNULL(&bgImage32F);
	cvExtReleaseImageCheckNULL(&fgImage8U);
	cvExtReleaseImageCheckNULL(&mask8U);
    cvExtReleaseImageCheckNULL(&gradOrient32F);

	cvExtReleaseImageCheckNULL(&mhImage);
	cvReleaseMat(&mhMatrix);    

	cvExtReleaseImageCheckNULL(&tmpImg32F);

	cvExtReleaseImageCheckNULL(&curFrameHSV_8U3);
	cvExtReleaseImageCheckNULL(&curFrameHue_8U);	
    cvExtReleaseImageCheckNULL(&backproject);

	cvExtReleaseImageCheckNULL(&mainDebugImage);

	cvReleaseMemStorage(&storage);

	// Release memory of color histograms of the tracked objects
	for (int i = 0; i < SF_MAX_TRACK_OBJS; i++){
		tracksList[i].objStatus = EMPTY;
		if (tracksList[i].hist){
		   cvReleaseHist(&(tracksList[i].hist));
		}
		cvExtReleaseImageCheckNULL(&(tracksList[i].visualId));
		tracksList[i].visualIdSize.height = 0;
		tracksList[i].visualIdSize.width  = 0;
	}

	// Close log file only if it was openned in debug mode
	if (logFile)  
	   fclose(logFile);

}

/****************************************************************************/
// Update initial values that are set by constructor. Variables with 
// DEFAULT_SF_VALUE value will not be updated (remain default).
// Returns error if improper status for action
/*int sfMultiTracker::updateParams( int newMotionThresh, int newMinPixSizeObject, 
								  int newMaxPixSizeObject, 
								  int newCtVmin, int newCtVmax, int newCtSmin,
								  char *newLogFilePath){

	if (trackStatus != IDLE){
	   return SF_IMPROPER_STATUS_FOR_ACTION_ERR;
	}
	if (newMotionThresh!=DEFAULT_SF_VALUE)
	   motionThresh = newMotionThresh;

	if (newMinPixSizeObject!=DEFAULT_SF_VALUE){
	   minPixSizeObject = newMinPixSizeObject;
       minPixObjArea    = minPixSizeObject*minPixSizeObject;
	}

	if (newMaxPixSizeObject!=DEFAULT_SF_VALUE)
	   maxPixObjArea = newMaxPixSizeObject*newMaxPixSizeObject;

	if (newCtVmin!=DEFAULT_SF_VALUE)
	   ctVmin = newCtVmin;

	if (newCtVmax!=DEFAULT_SF_VALUE)
	   ctVmax = newCtVmax;

	if (newCtSmin!=DEFAULT_SF_VALUE)
	   ctSmin = newCtSmin;

	if (newLogFilePath)
	   safeStrBufCpy(logFilePath,newLogFilePath);

	return SF_SUCCESS;
}
/****************************************************************************/
// Main method. Called uppon each frame. The only method which should be 
// activated by user. Gets a new frame, detects new motions, tracks already 
// known motions, reports results in single message and writes debug 
// information to log file if needed.

int sfMultiTracker::proccessFrame(const IplImage* newFrame){

	// Store image, convert it to grayscal/HSV and more
	recieveImage(newFrame);

	// If not initialized then initialize
	if (trackStatus == IDLE){
	   initialize(newFrame);

	   // Open log file if debug flag is set
	   SF_DEBUG_CODE {
	  	   fopen_s(&logFile,logFilePath,"w");
	   }
	   return SF_SUCCESS;
	}

	// ---------------------------------------------------------------
	// Update the foreground back ground seperation and motion history
	updateBgFgCut();
	updateMotionHistory();

	// ---------------------------------------------------------------
	// Detect motion components
	detectMotion();
	
	// ---------------------------------------------------------------
    // Track already existing objects
    if (trackStatus == TRACKING){

       multiTrackToNewFrame();   	
	
   	   // If all trackers stopped then the whole system is not tracking
	   if (!numTrackedObjs){
	      trackStatus = INITIALIZED;
	   }

   	   // Debug Drawing
	   SF_DEBUG_CODE{} drawTrackedObjects();
	}

	// ---------------------------------------------------------------
	// Allocate free trackers slots to new detected motions

	int motionResidue = assignDetectedMotionToNonBusyTrackers();

	// If at least one tracker is active then the whole system is tracking
	if (numTrackedObjs){
	   trackStatus = TRACKING;
	}

	// In debug: Draw motions that will not be tracked
		drawUnassignedMotionDetections(motionResidue);		
	SF_DEBUG_CODE { 

		// Report full information to debug file
		fullFrameReport2LogFile(motionResidue);
	}

	forgetUnAssignedDetectedMotion();

	// ---------------------------------------------------------------
	// Prepare output images in debug mode

	SF_DEBUG_CODE {}
	{
 
		cvCvtScale(fgImage8U, fgImage8U,255,0);	
		double multF = (1.0/MOTION_MAX_TIME_DELTA);
		double a = 1.0*(-frameNum+MOTION_MAX_TIME_DELTA)*(1/(MOTION_MAX_TIME_DELTA));
		cvCvtScale(mhImage, mhImage,multF,a);	
	}

	return SF_SUCCESS;
}

/****************************************************************************/
// Called upon new arriving frame. Prepares the class for processing the new image. 

inline void sfMultiTracker::recieveImage(const IplImage* newFrame){
	
	// If first image
	if (!curFrame){
	   isColoredVideo = (newFrame->nChannels != 1);
	   if (!isColoredVideo)
	      curFrame   = cvCloneImage(newFrame);
	   else{
	      curFrame   = cvCreateImage( cvGetSize(newFrame), IPL_DEPTH_8U, 1 );
	      cvCvtColor(newFrame, curFrame, CV_BGR2GRAY);	
	   }	   

	   // Store frame size of the video
	   frameSize = cvGetSize(curFrame);

	   // Debug
       mainDebugImage   = cvCloneImage(newFrame);
	}

	// If not first image
	else {
	   // Copy curFrame to prevFrame.
	   cvCopy(curFrame, prevFrame);

	   // Store the new image in grayscale using preallocated memory
	   if (!isColoredVideo)
		  cvCopy(newFrame,curFrame);
	   else {
		  cvCvtColor(newFrame, curFrame, CV_BGR2GRAY);	
	   
		  // Store the image as HSV for color tracking, and split the hue plane
		  cvCvtColor( newFrame, curFrameHSV_8U3, CV_BGR2HSV );
          cvSplit( curFrameHSV_8U3, curFrameHue_8U, NULL, NULL, NULL );
	   }

	   // Update Debug image
	   cvCopy(newFrame,mainDebugImage);	  
	   SF_DEBUG_CODE {
		   // Frame caption
  		   fprintf(logFile,"--------------- Frame: %d ------------------------------------------\n",frameNum+1);
	   }
	}
	
	// Update frame counter
	frameNum++;
	return;
}

/****************************************************************************/
// Allocate needed memory for multi-tracking. 

int sfMultiTracker::initialize(const IplImage* newFrame){

	if (trackStatus != IDLE){
		return SF_IMPROPER_STATUS_FOR_ACTION_ERR;
	}
	
	// Allocate memory for assist images.
	prevFrame		= cvCreateImage(frameSize, IPL_DEPTH_8U,  1);
	curFrame32F		= cvCreateImage(frameSize, IPL_DEPTH_32F, 1);
	curFrameHSV_8U3 = cvCreateImage(frameSize, IPL_DEPTH_8U,  3);
	curFrameHue_8U  = cvCreateImage(frameSize, IPL_DEPTH_8U,  1);

    bgImage32F		= cvCreateImage(frameSize, IPL_DEPTH_32F, 1);          
	fgImage8U		= cvCreateImage(frameSize, IPL_DEPTH_8U,  1);
	mask8U			= cvCreateImage(frameSize, IPL_DEPTH_8U,  1);

	gradOrient32F	= cvCreateImage(frameSize, IPL_DEPTH_32F, 1);

	tmpImg32F       = cvCreateImage(frameSize, IPL_DEPTH_32F, 1);        

	mhImage         = cvCreateImage(frameSize, IPL_DEPTH_32F, 1);
	mhMatrix        = cvCreateMat(curFrame->height, curFrame->width, CV_32FC1);
	cvSetZero(mhMatrix);


	// Approximate background by first frame
	cvConvertScale(curFrame, bgImage32F, 1.0/255, 0 );

	// Initialize storage for motion detection
	storage = cvCreateMemStorage(0);
	
	// -------------------------------------------
	// Tracking variables

	backproject		= cvCreateImage(frameSize, IPL_DEPTH_8U,  1);

	// Allocate memory for color histograms of the tracked objects and initialize
	// SF_MAX_TRACK_OBJS number of trackers
	for (int i = 0; i < SF_MAX_TRACK_OBJS; i++){
		tracksList[i].objStatus = EMPTY;
		tracksList[i].visualId  = 
			 cvCreateImage(cvSize(SF_VISUAL_ID_SIZE,SF_VISUAL_ID_SIZE),
			               curFrame->depth, curFrame->nChannels);
		tracksList[i].visualIdSize.height = 0;
		tracksList[i].visualIdSize.width  = 0;

		
		ClearTrackingBoxOf(i);
		tracksList[i].hist = cvCreateHist(1, &hNumBins, CV_HIST_ARRAY, &phHueRange, 1 );
	}			

	// Set status
	trackStatus = INITIALIZED;

	return SF_SUCCESS;
}

/****************************************************************************/
/***************************** Motion Detection *****************************/
/****************************************************************************/

/****************************************************************************/
// Seperate the foreground (new objects) from the background using running
// average technique

void sfMultiTracker::updateBgFgCut(void){

	// Calculate difference between curFrame and background
	cvConvertScale(curFrame, curFrame32F, 1.0/255, 0 );

	if (frameNum<=2){ // Some-times first frame is problematic. Overcame it
		cvCopy(curFrame32F,bgImage32F);
	}
	cvAbsDiff(curFrame32F, bgImage32F, tmpImg32F);  


	// Threshold the difference to create forground binary image
	cvThreshold(tmpImg32F, fgImage8U, (motionThresh)/255.0, 1.0, CV_THRESH_BINARY); 

	// Update the background only where motion did not occur, i.e
	// (Invert forground and use it as mask to update background)
	cvCopy(fgImage8U,mask8U);
	cvExtInvertBinaryImage(mask8U);
	cvRunningAvg(curFrame32F, bgImage32F, SF_MHI_NEW_IMG_WEIGHT, mask8U);         	
}

/****************************************************************************/
// Update motion history image. This image stores a concatenation of few 
// previous forgrounds and the new one

inline void sfMultiTracker::updateMotionHistory(void){

	// Remove noise and small objects using morphological operations
	cvExtMorphologicalNoiseRemove(fgImage8U,minPixSizeObject,0);

	// Update motion history image with the calculated forground image
	// (fgImage8U). Motion history together with forground image are the basis
	// For motion detection.
	cvUpdateMotionHistory(fgImage8U,mhMatrix, frameNum, MOTION_MAX_TIME_DELTA);
	cvConvert(mhMatrix,mhImage);
}

/****************************************************************************/
// Sets minimal.
// If called with no arguments then resets the threshold to default

void sfMultiTracker::setMotionDetectionThreshold(int newThresh){
	// Update threshold if needed.
	motionThresh = (newThresh < 0)? SF_INI_MOTIN_THRESH : newThresh;
}

/****************************************************************************/
// Extract connected component from motion history and store them in array
// 'detMotionsList'. Each moving suspect is stored in 'tDetectedMotion' struct
// and will be passed 

void sfMultiTracker::detectMotion(void){

	// Calc gradients of motion history
	cvCalcMotionGradient( mhMatrix, mask8U, gradOrient32F, 
                           MOTION_MAX_TIME_DELTA, MOTION_MIN_TIME_DELTA, SOBEL_SIZE );
    
    // Segment motion to different objects. Reuse curFrame32F variable
	seq = cvSegmentMotion(mhMatrix, curFrame32F, storage, frameNum, MOTION_MAX_TIME_DELTA );
	
	CvRect objRect; // Will store the bounding box of current motion componnent.

	time_t t;
	time(&t);       // Will store the detection time

	// Iterate through the detected objects (Motion components)
	numDetectedMotions = 0;
	int curObjArea;

    for (int i = 0; i < seq->total; i++) {

		// Maintain maximal detections.
		if (numDetectedMotions >= SF_MAX_DETEC_OBJS){
		   break;
		}

		// Retrieve objects rectangle
        objRect = ((CvConnectedComp*)cvGetSeqElem(seq,i))->rect;
        
		// Reject objects with size out of bounds
		curObjArea = objRect.width * objRect.height;
		if (curObjArea < minPixObjArea){
           continue;
		}
		if (curObjArea > maxPixObjArea){
           continue;
		}		
        		
        // Select current object ROI
        cvSetImageROI(mhImage       , objRect);
        cvSetImageROI(gradOrient32F , objRect);
        cvSetImageROI(mask8U        , objRect);

        // Calculate orientation of current object (in the ROI)
        double motionAngle = 
			   cvCalcGlobalOrientation(gradOrient32F , mask8U , mhImage, 
			                           frameNum, MOTION_MAX_TIME_DELTA);
		// Adjust for images with top-left origin
        motionAngle = 360.0 - motionAngle;  

		// Add objects to motion detection array.
		detMotionsList[numDetectedMotions].loc			= objRect;
		detMotionsList[numDetectedMotions].area			= curObjArea;
		detMotionsList[numDetectedMotions].velAngle		= motionAngle;
		detMotionsList[numDetectedMotions].velMagnitude = 0; // Unknown yet

		// Use current time tag, and store it as detection time				
		detMotionsList[numDetectedMotions].detectTime  = t;
		detMotionsList[numDetectedMotions].detectFrame = frameNum;
		
		// New motion was succesfully detected 
		numDetectedMotions++;
	    
    } // Loop for each motion object.

    // Reset the ROI
    cvResetImageROI(mask8U);
	cvResetImageROI(mhImage);
    cvResetImageROI(gradOrient32F);

	// Clear temporary sequence and memory storage.
	cvClearSeq(seq);
	cvClearMemStorage(storage);	
}

/****************************************************************************/
// Allocate free tracker slots to new detected motions. If not enough free
// trackers exist then some detected motions will not be tracked.
// Each tracker is implemented by 'tTrackedObject' structure and array of
// trackers is called 'tracksList'
// Returns first motion component that were not assigned.

int sfMultiTracker::assignDetectedMotionToNonBusyTrackers(void){
	
	int t = 0, m = 0;

	// Do while new detected motion exists and free trackers exist
	while ((t<SF_MAX_TRACK_OBJS)&&(m<numDetectedMotions)){

		// If current tracker is busy then skip to next one
		if ((tracksList[t]).objStatus != EMPTY){
		   t++;
		   continue;
		}

		// Assign motion 'm' to tracker 't'. 
		tracksList[t].detObj        = detMotionsList[m];
		tracksList[t].objStatus     = SUSPECT;
		tracksList[t].lastSeenTime  = tracksList[t].detObj.detectTime;
		tracksList[t].lastSeenFrame = tracksList[t].detObj.detectFrame;

		// Check if current motion component is indeed a new object
		if (isReallyNewObject(t)){

			calcObjectColorHistogram(t);
			calcVisualID(t);

			// Generate Pseudorandom ID for the object
			generatePseudomRandID(t);

			// Skip to next tracker.
			SF_DEBUG_CODE fprintf(logFile,"-->Motion %d assigned to tracker %d\n",m,t);
			t++;
			numTrackedObjs++;
		}
		else{
			// Motion component represents already tracked object. 
			SF_DEBUG_CODE fprintf(logFile,"-->Motion %d is dropped (already tracked)\n",m);
			tracksList[t].objStatus = EMPTY;
		}

		// Skip to next motion
		m++;
	}

	return m;  // residul motions that were not assigned
}

/****************************************************************************/
// Delete unassigned detected motions. In future can use them for testing
// time consistency

void sfMultiTracker::forgetUnAssignedDetectedMotion(void){
	// Just forget all the motions that were not assigned to trackers.
	numDetectedMotions = 0;
}

/****************************************************************************/
// Calculate color histogram of the tracked object
void sfMultiTracker::calcObjectColorHistogram(int trackerID){

    // Update histogram for tracking.
	cvSetImageROI(curFrameHue_8U, tracksList[trackerID].detObj.loc);
    cvSetImageROI(fgImage8U     , tracksList[trackerID].detObj.loc);
    cvCalcHist(&curFrameHue_8U  , tracksList[trackerID].hist, 0, fgImage8U );
	   
	// Reset ROI
    cvResetImageROI(curFrameHue_8U);
    cvResetImageROI(fgImage8U);	
	
	// Normalize histogram to 0..255 range.
	// float maxHistVal = 0.f;
    // cvGetMinMaxHistValue(tracksList[trackerID].hist, 0, &maxHistVal, 0, 0 );
    // cvConvertScale( (tracksList[trackerID].hist)->bins, (tracksList[trackerID].hist)->bins, 
	//	            maxHistVal ? 255./maxHistVal : 0., 0 );
}

/****************************************************************************/
// Calculate the visual specifications of the object
void sfMultiTracker::calcVisualID(int trackerID){

	CvRect idRect = tracksList[trackerID].detObj.loc;

	// Dont let visual ID ve greater then maximal allocated size
	idRect.height = idRect.height < SF_VISUAL_ID_SIZE ? idRect.height: SF_VISUAL_ID_SIZE;
	idRect.width  = idRect.width  < SF_VISUAL_ID_SIZE ? idRect.width : SF_VISUAL_ID_SIZE;

	// Copy the area of the object pixels from curFrame
	cvZero(tracksList[trackerID].visualId);
	cvSetImageROI(curFrame                      , idRect);	
	cvSetImageROI(tracksList[trackerID].visualId, cvRect(0,0,idRect.width,idRect.height));	
	cvCopy(curFrame,tracksList[trackerID].visualId);
	tracksList[trackerID].visualIdSize = cvSize(idRect.width,idRect.height);

	cvResetImageROI(tracksList[trackerID].visualId);
	cvResetImageROI(curFrame);
}

/****************************************************************************/
// Generate psudo random to new motion that will betracked 
void sfMultiTracker::generatePseudomRandID(int trackerID){
	
	char* visualSeed = (char*)tracksList[trackerID].visualId->imageData;

	int ind = tracksList[trackerID].visualIdSize.height*
		      tracksList[trackerID].visualIdSize.width;

	tracksList[trackerID].prID = (PR_ID_TYPE)tracksList[trackerID].detObj.area * 
	                             (PR_ID_TYPE)tracksList[trackerID].detObj.velAngle +
								 (PR_ID_TYPE)visualSeed[ind/2]+
								 (PR_ID_TYPE)visualSeed[ind  ];
}

/****************************************************************************/
// zero tracking box of tracker by its ID.
inline void sfMultiTracker::ClearTrackingBoxOf(int trackerID){
	tracksList[trackerID].track_box.angle		= 0;
	tracksList[trackerID].track_box.center.x	= 0;
	tracksList[trackerID].track_box.center.y	= 0;
	tracksList[trackerID].track_box.size.width	= 0;
	tracksList[trackerID].track_box.size.height = 0;
}

/****************************************************************************/
/***************************** Tracking issues ******************************/
/****************************************************************************/

/****************************************************************************/
// Activate non empty trackers to find the object in the new frame.
// Uses as sub routines:
//          trackNewObjectLocation          - for each active tracker
//          hasTrackingFailed               - to check trackers failure 
//          extrapolateMovementTrajectory   - If tracker failed
//          updateMovementTrajectory        - If tracker succeded
//          resolveTrackersInterferenceWith - Solve multitracking collisions
void sfMultiTracker::multiTrackToNewFrame(void){

	CvConnectedComp newLocation;      // New object location

	time_t timeTag; time(&timeTag);   // Generate time tag

	// Go over all the trackers
	for (int i = 0; i < SF_MAX_TRACK_OBJS; i++){

		if ((tracksList[i]).objStatus == EMPTY){
		   // Current tracker is idle nothing to update
			continue;
		}

		// Track current object from the previous frame to the new one
		trackNewObjectLocation(i, &newLocation, &tracksList[i].track_box);

		// Tracking Results:
		//  0. tracksList[i].detObj.loc still holds the previous location
		//  1. 'newLocation' sotres the updated object location
		//  2. tracksList[i].track_box is updated to new bounding box

		// ----------------------------------------------------------
  	    // Check if tracker failed
		if (hasTrackingFailed(i,&newLocation)){

		   // If tracking failed only on motion suspect then abort tracker.
		   if (tracksList[i].objStatus <= SUSPECT){
			  SF_DEBUG_CODE fprintf(logFile,"-->Tracker %d failed on suspect\n",i);
			  stopTracker(i);
			  continue;
		   }

		   // It was already known object then try not to loose it.
		   // Update new location using previous velocity
		   extrapolateMovementTrajectory(i);
		   tracksList[i].objStatus = OCCLUDED_OBJ;

		   // If object was occluded too much time, stop the tracker
		   if (frameNum - tracksList[i].lastSeenFrame > SF_MIN_FRAMES_OCCLUDED_OBJ){
			  SF_DEBUG_CODE fprintf(logFile,"-->Tracker %d occlusion time expired\n",i);
		      stopTracker(i);
		   }		   
		   continue;
		}

		// ----------------------------------------------------------
		// Object was successfully tracked to new frame

		updateMovementTrajectory(i,newLocation);

		// Update time tags				
		tracksList[i].lastSeenTime  = timeTag;
		tracksList[i].lastSeenFrame = frameNum;

		// If suspect constantly exists few frames then it becomes object.
		tracksList[i].objStatus = (getObjAge(i) <= SF_MIN_FRAMES_MOTION_SUSPECT)?
								  SUSPECT:OBJECT;

		// Check If two trackers converged to the same object. 
		// If so then one of them must be stopped or maybe an object became
		// ocluded by other object and their trackers collided.
		resolveTrackersInterferenceWith(i);

		// If current tracker is was stopped then dont continue with updates.
		if ((tracksList[i]).objStatus == EMPTY){
		   continue;
		}

		// Update visual ID (only if current object is not occluded)
		if ((tracksList[i]).objStatus != OCCLUDED_OBJ){
		   calcVisualID(i);
		   // calcObjectColorHistogram(i);
		}
	}
}

/****************************************************************************/
// Calculate new location of object tracked by 'trackerId'.
// Algorithm: Color histogram tracking (camShift).
// You can change the algorithem by changing the code of this method.
void sfMultiTracker::trackNewObjectLocation(int trackerID, CvConnectedComp* newLoc, 
						                    CvBox2D* newBoundingBox){

    // Calculate color probability distribution
	cvCalcBackProject( &curFrameHue_8U, backproject, tracksList[trackerID].hist );
		
	// Search only in areas of forground (at least some motion occured)
	cvAnd( backproject, fgImage8U, backproject, 0 );

	// Reuse mask8U variable in order to remove low saturated pixels
	cvInRangeS( curFrameHSV_8U3, cvScalar(0,ctSmin,MIN(ctVmin,ctVmax),0),
                                 cvScalar(180,256 ,MAX(ctVmin,ctVmax),0), mask8U );
	cvAnd( backproject, mask8U, backproject, 0 );
	
	cvCamShift( backproject, tracksList[trackerID].detObj.loc, convergRule,
				newLoc, newBoundingBox );

	// Adjust for images with top-left origin
    newBoundingBox->angle = newBoundingBox->angle;
}

/****************************************************************************/
// Check if tracking in current frame by trackerId failed
bool sfMultiTracker::hasTrackingFailed(int trackerID, const CvConnectedComp* newLoc){

	double objSize = ((tracksList[trackerID].track_box).size.height)*
	   			    ((tracksList[trackerID].track_box).size.width);

	// If track_box has zero size then tracker surelly failed. If not then, check
	// object accurate area.
	if (objSize){
	   objSize = newLoc->area;
	}
	return ((objSize < minPixObjArea)||(objSize > maxPixObjArea));
}

/****************************************************************************/
// When tracking failed, extrapolate objects trajectory movement.
// Very simple Kalman Filter Here.
void sfMultiTracker::extrapolateMovementTrajectory(int trackerID){
    
	double length = tracksList[trackerID].detObj.velMagnitude;
    double angle  = tracksList[trackerID].detObj.velAngle*CV_PI/180.0;
    int newX      = cvRound(tracksList[trackerID].detObj.loc.x + length*cos(angle));
    int newY      = cvRound(tracksList[trackerID].detObj.loc.y + length*sin(angle));

    // Check image borders
	newX = (newX<0)?0:newX;
    newY = (newY<0)?0:newY;

	// Check (width,height) out of bounds
    if (newX < frameSize.width-tracksList[trackerID].detObj.loc.width){
	    tracksList[trackerID].detObj.loc.x = newX;
    }
    if (newY < frameSize.height-tracksList[trackerID].detObj.loc.height){
 	   tracksList[trackerID].detObj.loc.y = newY;
    }
}

/****************************************************************************/
// When tracking was successful, update object's movements parameters
// (i.e. location, velocity, velocity angle).
void sfMultiTracker::updateMovementTrajectory(int trackerID, const CvConnectedComp newLoc){
	
	double newVelMag;
	double newVelAng = cvExtAngBetweenRectCenters(tracksList[trackerID].detObj.loc,
		                                          newLoc.rect,&newVelMag);

	// Update velocity
	cvExtAddVector(&tracksList[trackerID].detObj.velMagnitude, 
		           &tracksList[trackerID].detObj.velAngle,
		           newVelMag, newVelAng);

	tracksList[trackerID].detObj.velMagnitude = newVelMag;

	// Update location
	tracksList[trackerID].detObj.loc      = newLoc.rect;
	tracksList[trackerID].detObj.area     = cvRound(newLoc.area);
}

/****************************************************************************/
// Stop tracker by its Id.
void sfMultiTracker::stopTracker(int trackerID){
	if ((tracksList[trackerID]).objStatus != EMPTY){
	    (tracksList[trackerID]).objStatus = EMPTY;
	   numTrackedObjs--;
	}
	ClearTrackingBoxOf(trackerID);
}

/****************************************************************************/
// Calculate how much time object exists in the system.
inline int sfMultiTracker::getObjAge(int trackerID){
	return tracksList[trackerID].lastSeenFrame - 
		   tracksList[trackerID].detObj.detectFrame;
}

/****************************************************************************/
// Check if at least one other slot is tracking the same object as 'i' wants.
// If so then this object is not new.
bool sfMultiTracker::isReallyNewObject(int trackerID){

	// Go over all trackers
	for (int j = 0; j < SF_MAX_TRACK_OBJS; j++){

		if ((tracksList[j]).objStatus == EMPTY){
	       // 'j' tracker is idle, it cannot interfer with 'i' trackers.
		   continue;
		}

		if (j == trackerID){		   
		   continue;
		}

		// Compare the objects
		if (hasConvergedToSameObject(trackerID,j) > 0){
		   // indeed object already exists
		   return false;
		}
	}
	return true;
}

/****************************************************************************/
/************************** Multi Tracking issues ***************************/
/****************************************************************************/

/****************************************************************************/
// Check if trackers lower the 'i' tracker are tracking the same object.
// If so one must be stopped
void sfMultiTracker::resolveTrackersInterferenceWith(int trackerID){

	// Go over all lower trackers
	for (int j = 0; j < trackerID; j++){
		if ((tracksList[j]).objStatus == EMPTY){
	       // 'j' tracker is idle, it cannot interfer with 'trackerID'.
		   continue;
		}

		// Compare the objects
		int whoToStop = hasConvergedToSameObject(trackerID,j);

		if (whoToStop >= 0){
		   // Check motion angle. If two object has different motion then
		   // maybe one of them just became occluded by the other
		
		   double angDiff = cvExtAngAbsDiff(tracksList[trackerID].detObj.velAngle,
			                                tracksList[j        ].detObj.velAngle);
		   
		   // If angle of motion is more then constant then objects are assumed not
		   // to be the same.
		   if ((angDiff > SF_MIN_VEL_OCCLUD_ANG)&&
			   (tracksList[whoToStop].objStatus >= OCCLUDED_OBJ)){
			   SF_DEBUG_CODE fprintf(logFile,"-->Tracker %d is occluded by tracker %d\n",whoToStop,trackerID+j-whoToStop);
			   tracksList[whoToStop].objStatus = OCCLUDED_OBJ;
		   }
		   else{
		       // Stop tracker
			   SF_DEBUG_CODE fprintf(logFile,"-->Tracker %d stopped by tracker %d\n",whoToStop,trackerID+j-whoToStop);
			   stopTracker(whoToStop);
		   }
		}
		if (whoToStop == trackerID){
		   // Exit the for loop since current tracker is stopped
	       break;
		}
	}
}

/****************************************************************************/
// Has two trackers converged to the same object?
// Returns the tracker which should be stopped.
int sfMultiTracker::hasConvergedToSameObject(int i, int j){
	CvRect iRect, jRect;
	int iAge, jAge;

	iRect = (tracksList[i].detObj).loc;
	jRect = (tracksList[j].detObj).loc;

	iAge  = getObjAge(i);
	jAge  = getObjAge(j);

	// Check if rectangles are equal.
	int inside = cvExtIsRectInside(iRect,jRect);

	// If there is some kind of inclusion.
	if (inside != -2){

		// First judge upon age. Older object is better
		if (iAge > jAge){
		   return j;
		}
		if (iAge < jAge){
		   return i;
		}

		// If objects have same age then sacrifice the smaller one
		if (inside == -1)
		   return i;

		if (inside == +1)
		   return j;

	    // If also same size then stop the tracker with higher id.
	    return MAX(i,j);
	}

	// There is no inclusion but check for large intersection
	if (cvExtIsRectLargeIntersection(iRect,jRect)){
		// First deside uppon priority.
		if (iAge > jAge){
		   return j;
		}
		if (iAge < jAge){
		   return i;
		}

		if (inside == -1)
		   return i;

		if (inside == +1)
		   return j;

	   return MAX(i,j);
	}

	return -1;
}

/****************************************************************************/
/********************************** Debug ***********************************/
/****************************************************************************/

/****************************************************************************/
// Print detected motion struct to log file
inline void sfMultiTracker::printMotionStruct2LogFile(const tDetectedMotion* motion){
	fprintf(logFile,
		    "Loc:(%d,%d,%d,%d),\t V(%0.1f,%0.1f),\t DetectTime:(%d)\n",
			motion->loc.x,
			motion->loc.y, 
			motion->loc.width,
			motion->loc.height, 
			motion->velMagnitude, 
			motion->velAngle,
			motion->detectFrame);
}

/****************************************************************************/
// Print total information (motion detections + trackers status) to log file
inline void sfMultiTracker::fullFrameReport2LogFile(int motionResidue){

	int m;

	// Print motions to log file
	fprintf(logFile,"\nTotal %d Motion Detections:\n",numDetectedMotions);
	for (m = 0; m<numDetectedMotions; m++){
		if (m >= motionResidue){
		   fprintf(logFile,"drop ");
		}
		fprintf(logFile,"%d.\t ",m);
		printMotionStruct2LogFile(detMotionsList+m);
	}
	fprintf(logFile,"\n");

	// Print trackers to log file
	for (int m = 0; m < SF_MAX_TRACK_OBJS; m++){
	    fprintf(logFile,"T%d.",m);
		switch (tracksList[m].objStatus){
		case EMPTY:
					fprintf(logFile,"-------\n");
					continue;
					break;
		case SUSPECT:
					fprintf(logFile,"Suspect\t");
					break;
					
		case OCCLUDED_OBJ:
					fprintf(logFile,"Occluded\t");
					break;

		case OBJECT: 
					fprintf(logFile,"Active\t");
					break;
		}

		fprintf(logFile,"(%10d),\tTrackBox(%0.1f,%0.1f,%0.1f,%0.1f,%0.0f),\tLastSeen: %d\n\t\t",
			    tracksList[m].prID, 
				tracksList[m].track_box.center.x,
				tracksList[m].track_box.center.y,
				tracksList[m].track_box.size.width,
				tracksList[m].track_box.size.height,
				tracksList[m].track_box.angle,
				tracksList[m].lastSeenFrame);
		printMotionStruct2LogFile(&(tracksList[m].detObj));
	}

	// Seperate frames
	fprintf(logFile,"\n\n");
}


/****************************************************************************/
// Draw Motion Detection that were not assigned to trackers
inline void sfMultiTracker::drawUnassignedMotionDetections(int motionResidue){
	CvScalar color = CV_RGB(255,000,000); // Red

	int m = motionResidue;

	while (m<numDetectedMotions){
		cvExtDrawObj(mainDebugImage, detMotionsList[m].loc, detMotionsList[m].velAngle, detMotionsList[m].velMagnitude*5,color);
		m++;
	}
}

/****************************************************************************/
// Draw Tracked objects
inline void sfMultiTracker::drawTrackedObjects(void){
		
	CvScalar color;
	char s[20];
	CvFont  font = cvFont(1,2);

	// Go over all the trackers
	for (int i = 0; i < SF_MAX_TRACK_OBJS; i++){

		if (tracksList[i].objStatus == EMPTY){
		   continue;
		}

		// Define color according to object status.
		switch (tracksList[i].objStatus){

		case SUSPECT:
					// Yellow
					color = CV_RGB(255,255,0);
					break;
					
		case OCCLUDED_OBJ:
					// Gray
					color = CV_RGB(122,122,122);
					break;

		case OBJECT: 
					// Green
					color = CV_RGB(0,255,0);
					break;
		}

		// Draw
		//cvEllipseBox( mainDebugImage, tracksList[i].track_box, color, 1, CV_AA, 0 );
		cvExtDrawObj(mainDebugImage, tracksList[i].detObj.loc, 
			         tracksList[i].detObj.velAngle, 
					 tracksList[i].detObj.velMagnitude*5, color);
		sprintf_s( s, "%ld",tracksList[i].prID);
		//sprintf_s( s, "%d",i);
		//cvPutText(mainDebugImage, s, cvPointFrom32f(tracksList[i].track_box.center), &font, color); 
	}	
}

/****************************************************************************/
// Output image for showing in window. Used for debugging the code

const IplImage* sfMultiTracker::getBackGroundImage(void){
	return bgImage32F;
}

const IplImage* sfMultiTracker::getForGroundImage(void){	
	return fgImage8U;
}

const IplImage* sfMultiTracker::getMotionHistoryImage(void){
	return mhImage;
}
const IplImage* sfMultiTracker::getMainDebugImage(void){
	return mainDebugImage;
}

/****************************************************************************/
// EOF.