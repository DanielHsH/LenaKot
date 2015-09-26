#include "stdafx.h"
#include "sfFrameGrabber.h"
#include "demPencil.h"
#include "sfCVextension.h"

namespace demPencilNS{

/****************************************************************************/
// Defines 

// main debug windows command
#define MAIN_SF_WINDOW_COMMAND_OPEN   (0)
#define MAIN_SF_WINDOW_COMMAND_UPDATE (1)
#define MAIN_SF_WINDOW_COMMAND_CLOSE  (2)

/****************************************************************************/
// Inner variables

sfFrameGrabber*	Video                        = NULL;		// Video source 

int intensNormal  = 7;
int colorNormal   = 5;
int looseDitails  = 1;
int edgeSharpness = 8;
int noiseRatio    = 2;
int colorBits     = 8;

// File path's
char videoFilePath   [SF_DEFAULT_PATH_LENGTH];


IplImage *curImg;                        // Current video frame
int keyPressed;                          // Current keyboard command

/****************************************************************************/
/*************************    Assist methods    *****************************/
/****************************************************************************/

/****************************************************************************/
// Methods which manages swordfish tracker windows. Executes command given by
// main()
void updateWindows(int action){

	switch (action){

	case MAIN_SF_WINDOW_COMMAND_OPEN:

		  // Open source video window
		  cvNamedWindow("Source", CV_WINDOW_AUTOSIZE );
		  cvCreateTrackbar("Background" ,"Source",&intensNormal  ,10, NULL);
		  cvCreateTrackbar("Color     " ,"Source",&colorNormal   ,20, NULL);
		  cvCreateTrackbar("No ditails" ,"Source",&looseDitails  ,2 , NULL);
		  cvCreateTrackbar("Bold Strok" ,"Source",&edgeSharpness ,20, NULL);
		  cvCreateTrackbar("Thin Srtok" ,"Source",&noiseRatio    ,10, NULL);
		  cvNamedWindow("Pencil"     , CV_WINDOW_AUTOSIZE);
		break;
	
	case MAIN_SF_WINDOW_COMMAND_UPDATE:
		   cvShowImage("Pencil",curImg);
		   cvExtPhoto2Pencil(curImg,curImg,intensNormal/10.,colorNormal,looseDitails,
			                               edgeSharpness,noiseRatio/10.,colorBits);
		   cvShowImage("Source",curImg);
		break;

	case MAIN_SF_WINDOW_COMMAND_CLOSE:
			cvDestroyWindow("Source");
			cvDestroyWindow("Pencil");
		break;
	}
}

/****************************************************************************/
/***************************    Main method    ******************************/
/****************************************************************************/

int demPencilLiveCam(int isCamera){

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
	
	char frameName[30];
	curImg = Video->grabFrame();
	if (!curImg){
	   // Retry movie from hard-disk
	   delete Video;
	   safeStrBufCpy(videoFilePath,"TrackVideo.avi");
//	   safeStrBufCpy(videoFilePath,"a.txt");
	   Video      = new sfFrameGrabber(videoFilePath);
	   curImg = Video->grabFrame();

	   if (!curImg){
	      fprintf(stderr,"Error: Unable to acquire video frames!\n");
		  delete Video;
	      return -1;
	   }
	}

	// ---------------------------------------
    // Open program windows and prepare to the main loop.
	updateWindows(MAIN_SF_WINDOW_COMMAND_OPEN);
	// CvVideoWriter *writer=cvCreateVideoWriter("out2.avi",CV_FOURCC('D', 'I', 'V', 'X'),29.97,cvSize(320,240),1);
	// Grab next frame

	// ---------------------------------------
	// main loop
	while (curImg){
		updateWindows(MAIN_SF_WINDOW_COMMAND_UPDATE);
	    sprintf_s(frameName,"Frame%0.5d.jpg",Video->getCurFrameNumber());
		// cvSaveImage(frameName,curImg);
		// cvWriteFrame(writer, curImg);
		
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
		case 32 :  // space
	  			// pause
			    cvWaitKey(0);
				break;
		}
	}

	// End of main loop
	// ---------------------------------------
	
	// Program has to terminate
	updateWindows(MAIN_SF_WINDOW_COMMAND_CLOSE);
	//cvReleaseVideoWriter(&writer);
    // Delete swordfish classes
    delete Video;

	return 0;
}

} // End of NameSpace

/****************************************************************************/
// EOF.
