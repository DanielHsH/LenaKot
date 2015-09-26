/* Coded By    : Daniel Herman Shmulyan
   Version     : 1.3
   Description : Wrapper for cv grabbing functions. 
*/
/****************************************************************************/
/**********************    sfFrameGrabber.cpp    ****************************/
/****************************************************************************/

/****************************************************************************/
// Includes
#include "stdafx.h"
#include "sfFrameGrabber.h"

/****************************************************************************/
// Empty Constructor for online grabbing (from camera)
sfFrameGrabber::sfFrameGrabber(){

	 framesSource	  = ONLINE_CAMERA;
	 frameNumber	  = 0;
	 verticalFlipFlag   = false;
	 horizontalFlipFlag = false;
	 textFile		  = NULL;
	 curFrame		  = NULL;

	 capture          = cvCreateCameraCapture(CV_CAP_ANY);
	 if (!capture){
		framesSource = FAIL;
     }
}

/****************************************************************************/
// Constructor with file path for offline grabbing
sfFrameGrabber::sfFrameGrabber(string path){

	 videoFilePath    = path;
	 frameNumber      = 0;
	 verticalFlipFlag   = false;
	 horizontalFlipFlag = false;
	 textFile		  = NULL;
	 curFrame		  = NULL;

	 capture          = cvCaptureFromFile(videoFilePath.c_str());

     if (capture){
   	    framesSource = OFFLINE_VIDEO;
	 }
	 else {
		  fopen_s(&textFile,videoFilePath.c_str(), "rt" );
		  if (textFile){
		 	 framesSource = FRAMES_TEXT_LIST;
		  }
		  else{
             // Error: Frame grabber cannot find a source of frames
 		 	 framesSource = FAIL;
		  }
     }
}

/****************************************************************************/
// Destructor
sfFrameGrabber::~sfFrameGrabber(){

	 // Close the grabber
	 if (capture){
	    cvReleaseCapture( &capture ); 
	 }

	 // Close the text file if needed
	 if (textFile){
        fclose(textFile);
     }
}

/****************************************************************************/
// Grabbing action. Returns a new allocated frame. 
// User has responsibility to release the grabbed frame.
// User must not modify or release the grabbed frame.
IplImage* sfFrameGrabber::grabFrame(){

	IplImage* tmpFrame = NULL; 

	switch (framesSource){

	case ONLINE_CAMERA:
	case OFFLINE_VIDEO:

		 tmpFrame = cvQueryFrame(capture);
		 if (tmpFrame){
  			curFrame = cvCreateImage( cvGetSize(tmpFrame), IPL_DEPTH_8U, tmpFrame->nChannels);
			if (tmpFrame->origin == IPL_ORIGIN_BL)
  				// Flip Horizontally
				cvFlip(tmpFrame, curFrame, 0);
			else
				cvCopy(tmpFrame, curFrame);
		 }
		 else {
		    // Error loading image
			curFrame = tmpFrame;
		 }
  
		 break;

	case FRAMES_TEXT_LIST:

		 curFrame = NULL;

		 while (fgets(imageName, sizeof(imageName)-2, textFile )){

  			   int l = (int)strlen(imageName);
			   if (!l){
			      // empty line occured
			      continue;
			   }
			   if (imageName[0] == '#' ){
			      // Comment
				  continue;
			   }

		 	   //imageName[l-1] == '\n')
 			   imageName[--l] = '\0';

			   // Check valid extension
			   int extension = (int)strlen(imageName)-3;
			   if ((!_stricmp(&imageName[extension],"jpg"))||
			       (!_stricmp(&imageName[extension],"tif"))||
				   (!_stricmp(&imageName[extension],"png"))||
				   (!_stricmp(&imageName[extension],"bmp"))||
				   (!_stricmp(&imageName[extension],"gif"))){

			      curFrame = cvLoadImage( imageName, 1 );
				  break;
			   }		      			   
		 }       // End of While
  	     break;

	case FAIL:
	case DEPLETED:
		 // Do nothing.
		 break;

	} //End switch

	if (curFrame){
	   // If image loaded
	   frameNumber++;

	   // Flip if needed 
	   if (verticalFlipFlag)
 	      if (horizontalFlipFlag)
	         cvFlip( curFrame, curFrame, -1 );
		  else
	         cvFlip( curFrame, curFrame,  1 );
	   else
 	      if (horizontalFlipFlag)
	         cvFlip( curFrame, curFrame,  0 );
	   
	}
	else {
	   framesSource = DEPLETED;
	}

	return curFrame;
}

/****************************************************************************/
// Returns the number of current frame
int sfFrameGrabber::getCurFrameNumber(){
	return frameNumber;
}

/****************************************************************************/
// Change vertical flipping flag. If flag is set then
// All frames would be flipped vertically. 
void sfFrameGrabber::setVerticalFlip(bool flag){
	verticalFlipFlag = flag;
}

/****************************************************************************/
// Change vertical flipping flag. If flag is set then
// All frames would be flipped vertically. 
void sfFrameGrabber::setHorizontalFlip(bool flag){
	horizontalFlipFlag = flag;
}

/****************************************************************************/
// Return true if current video source is online (realtime)
bool sfFrameGrabber::isRealTimeGrabbing(void){
	return (framesSource == ONLINE_CAMERA);
}

/****************************************************************************/
// Return true if current source is video and not sequence of images.
bool sfFrameGrabber::isVideoStreamGrabbing(void){
	return ((framesSource == ONLINE_CAMERA)||(framesSource == OFFLINE_VIDEO));
}

/****************************************************************************/
// EOF.