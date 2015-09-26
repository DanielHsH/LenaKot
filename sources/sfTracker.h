/* Coded By    : Daniel Herman Shmulyan
   Version     : 1.2
   Description : Swordfish multi tracker class. This class is responsible for
                 motion detection and tracking few objects in parallel.
*/
/****************************************************************************/
/*************************    sfMultiTracker.h    ***************************/
/****************************************************************************/

/****************************************************************************/
// Includes

#include <time.h>
#include <stdio.h>
#include "globalPreDefines.h"
#include "sfCVextension.h"

#if !defined(SFMULTITRACKER_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC)
#define SFMULTITRACKER_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/******************************** Defines ***********************************/
// Define possible status of multi tracker class.
// Multitracker class can handle few object trackers.

// Defines possible status of the system.
typedef enum { IDLE = 0, INITIALIZED = 1, TRACKING } tTrackStatus;

// define possible status of each tracker (tracked object).
typedef enum { EMPTY = 0, SUSPECT = 1, OCCLUDED_OBJ, OBJECT } tObjTrackStatus;

// Define default value tag for inner variables.
#define DEFAULT_SF_VALUE   (-1)

// Define debug line
#define SF_DEBUG_CODE if(debugFlag)
#define SF_LOG_FILE_MESSAGE(mes)  fprintf(logFile,mes);

#define LOG_FILE_NAME "SwordFish.log.txt"  // Default log file path. Can be changed

// ----------------------------------------------------------
// Define Error and action result codes of swordfish tracker and calibrator

#define SF_WRONG_PROGRAM_ARGUMENTS        (-3)  // Swordfish tracker only
#define SF_FILE_CORRUPT_OR_MISSING_ERR    (-2)  // Swordfish calibrator only
#define SF_IMPROPER_STATUS_FOR_ACTION_ERR (-1)  // Both packages
#define SF_GENERAL_ACTION_FAILURE_ERR	   (0)  // Both packages 
#define SF_SUCCESS						   (1)  // Both packages
#define SF_USER_CANCELED_ACTION		       (2)  // Swordfish calibrator only
#define SF_MOVIE_TOO_SHORT_FOR_CALIB	   (3)  // Swordfish calibrator only
#define SF_MULTI_TRACKER_ERR_VIDEO_FILE	   (11) // Swordfish tracker only
#define SF_MULTI_TRACKER_ERR_FISHEYE_FILE  (12) // Swordfish tracker only
#define SF_CLIENT_CONNECTION_ERROR         (21) // Swordfish tracker only
#define SF_CLIENT_MESSAGE_SND_ERROR        (22) // Swordfish tracker only

/******************************** Type Defines ***********************************/

// Struct which represents detected motion
typedef struct{
	 CvRect loc;
	 int    area;
	 double velAngle;
	 double velMagnitude;
	 time_t detectTime;
	 int    detectFrame;
} tDetectedMotion;


// ---------------------------------------
// Struct of single tracker (tracked object)
#define PR_ID_TYPE    int        // Pseudo random object id type
typedef struct{

	 tObjTrackStatus objStatus;  // Status of tracker 

	 tDetectedMotion detObj;     // Motion which represents the object

	 PR_ID_TYPE prID;            // pseudo random id of an object, id is unique 
	                             // per object, not per tracker.

	 IplImage* visualId;         // Pixels of the object
	 CvSize    visualIdSize;

	 CvHistogram *hist;          // Visual histogram of the object. Used for tracking

	 CvBox2D track_box;          // Object exact bounding box inside its bounding
	                             // Rectangle (x,y,x+width,y+height).

	 time_t lastSeenTime;		 // Time in msec
	 int	lastSeenFrame;		 // Inner count frame number

} tTrackedObject;

/***************************** Defines of constants ********************************/

// ----------------------------------------------------------
// Define initial values 
#define SF_INI_MOTIN_THRESH          (15)  // Minimal change in pixel brightness that will
                                           // identified as change in the image

#define SF_INI_MIN_SIZE_OBJ           (5)  // Minimum object size: 5x5 pixels
#define SF_INI_MAX_AREA_OBJ      (100000)  // 100*100 pixels

// ----------------------------------------------------------
// Define algorithmic constants
#define MOTION_MAX_TIME_DELTA		(5.0)  // Number of consecutive frames to use when 
										   // calculating motion history of the video

#define MOTION_MIN_TIME_DELTA		(1.0)  // Minimal number of frames

#define SOBEL_SIZE					  (3)  // Size of sobel mask to use when extracting 
										   // image gradients

#define SF_MHI_NEW_IMG_WEIGHT       (0.1)  // The weight of background extracted from
                                           // current frame when compared to the 
                                           // weight to the history backround.

#define SF_MIN_FRAMES_MOTION_SUSPECT (15)  // Minimal number of frames that motion 
										   // must occure before it is classified as 
										   // valid object

#define SF_MIN_FRAMES_OCCLUDED_OBJ    (7)  // Maximal number of frames that occluded 
										   // object exists in the system untill it is
                                           // permanently disregarded

#define SF_MIN_VEL_OCCLUD_ANG        (45)  // Minimal relative velocity angle between 
                                           // two colliding objects which will result in
                                           // occlusion of one of them and not just 
										   // stopping a tracker

#define SF_MAX_DETEC_OBJS            (10)  // Maximal number of object that multi 
                                           // motion detector will handle

#define SF_MAX_TRACK_OBJS             (3)  // Maximal number of parallel trackers

#define SF_VISUAL_ID_SIZE            (32)  // Maximal size of visual ID of an object.
										   // For example 32x32 pixels

// ----------------------------------------------------------
// Color tracking initial restrictions in HSV image
#define SF_INI_HSV_MIN_VALUE          (0)	   
#define SF_INI_HSV_MAX_VALUE        (255)	   
#define SF_INI_HSV_MIN_SATURATION   (100)	   

// Color tracking constants
#define SF_COLOR_HISTOGRAM_SIZE      (32)  // Number of bins in the histogram

/****************************************************************************/
// Main multi tracker class

class sfMultiTracker{  

	  public:

			// Constructor. Can start in debug mode or regular 
			sfMultiTracker(bool debugOn = false);
			// Destrucot
			~sfMultiTracker();

			// ---------------------------------------
			// Activation method. Gets a new frame, detects new motions,
			// tracks already known motions, reports results in single message
			// and writes debug information to log file if needed.
			int proccessFrame(const IplImage* newFrame);

      private:

			// Initial memory allocation
			inline int initialize(const IplImage* newFrame);

			// ---------------------------------------
			// Image preporcessing			
			inline void recieveImage(const IplImage* newFrame);
            inline void updateBgFgCut(void);

			// ---------------------------------------
			// Motion Detection and track initialization methods			
			inline void updateMotionHistory(void);

	public:
			void setMotionDetectionThreshold(int newThresh = -1); 

	private:
			void detectMotion(void);
            int  assignDetectedMotionToNonBusyTrackers(void);
			void forgetUnAssignedDetectedMotion(void);
			void calcObjectColorHistogram(int trackerID);
			void calcVisualID            (int trackerID);
			void generatePseudomRandID   (int trackerID);
			inline void ClearTrackingBoxOf(int trackerID);

			// ---------------------------------------
			// Tracking issues
			void multiTrackToNewFrame(void);
			void trackNewObjectLocation(int trackerID, CvConnectedComp* newLoc, 
				                                       CvBox2D* newBoundingBox);
			bool hasTrackingFailed            (int trackerID, const CvConnectedComp* newLoc);
			void extrapolateMovementTrajectory(int trackerID);
			void updateMovementTrajectory     (int trackerID, const CvConnectedComp newLoc);
			void stopTracker                  (int trackerID);
			inline int getObjAge			  (int trackerID);
			bool isReallyNewObject            (int trackerID);

			// ---------------------------------------
			// Multi Tracking issues
			void resolveTrackersInterferenceWith(int trackerID);
			int hasConvergedToSameObject(int i, int j);

			// ---------------------------------------
			// Debug methods
			inline void fullFrameReport2LogFile(int motionResidue);
			inline void printMotionStruct2LogFile(const tDetectedMotion* motion);
			inline void drawUnassignedMotionDetections(int motionResidue);
			inline void drawTrackedObjects(void);

      public:
			const IplImage* getBackGroundImage   (void);
			const IplImage* getForGroundImage    (void);
			const IplImage* getMotionHistoryImage(void);
			const IplImage* getMainDebugImage    (void);

      private:
	
			// ---------------------------------------
			// General variables

			bool debugFlag; 
			char logFilePath[IPE_DEFAULT_PATH_LENGTH];
			FILE *logFile;

			bool isColoredVideo;

			// Sequence number of the current frame
			int frameNum;

			CvSize frameSize; // Size (x,y) of video frames

			// ---------------------------------------
			// Inner image variables

			// Previous frame of the video
			IplImage* prevFrame;

			// Current frame of the video
			IplImage* curFrame;
		    IplImage* curFrame32F;

 	        // Background image of the movie history (static objects)
		    IplImage* bgImage32F;          

			// Forground motion detection of the curFrame (binary mask)
 	        IplImage* fgImage8U;
 	        IplImage* mask8U;

			// Orientation of the gradients
			IplImage *gradOrient32F;

			// Motion history variables
			IplImage * mhImage;
 	        CvMat*     mhMatrix;

			// Temprary variable
			IplImage* tmpImg32F; 

			// Images for color tracking
			IplImage* curFrameHSV_8U3;
			IplImage* curFrameHue_8U;
			IplImage* backproject;

			IplImage* mainDebugImage;		

			// ---------------------------------------
			// Motion detection variables

			// Threshold for motion detection 
            int motionThresh;

			// Detect and track only motions with area bounded by constants 
			int minPixSizeObject;
			int minPixObjArea;
			int maxPixObjArea;

			// Array of detected motions, and its size variable.
			tDetectedMotion detMotionsList[SF_MAX_DETEC_OBJS];
			int numDetectedMotions;

			// Storage for motion components
			CvMemStorage *storage;
			CvSeq        *seq;

			// ---------------------------------------
			// Tracking Variables

			int hNumBins;                // Number of color histogram bins
			float hHueRangeArr[2];       // Ranges of Hue [0..180]
			float* phHueRange;           // Pointer to hue ranges array
			CvTermCriteria convergRule;  // Trackering convergence rule.

			// Color tracking restrictions in HSV image
			int ctVmin, ctVmax, ctSmin;

			// Array of tracked objects, and its size variable.
			tTrackedObject tracksList[SF_MAX_TRACK_OBJS];
			int numTrackedObjs;

			// Current status of multi-tracker
			tTrackStatus trackStatus;

};

#endif // #define SFMULTITRACKER_09FJ17FB3N59SGFOL6H7H7KJD3487_IINC

/****************************************************************************/
// EOF.
