/****************************************************************************/
/******************************    demFFT.cpp    ****************************/
/****************************************************************************/

/****************************************************************************/
// Includes
#include "stdafx.h"
#include "demHough.h"
#include "sfCVextension.h"

using namespace demHoughNS;

/****************************************************************************/
demHough::demHough(const char* path){

	if (path){ 
	   safeStrBufCpy(imagePath,path);
	}
	else {
	   safeStrBufCpy(imagePath,IPE_DEFAULT_IMAGE_FILE);
	}

	// Define the command
	safeStrBufCpy(command,"Change minimal circle radius and watch how Hough finds the circles.\tRise threshold to remove false circles");

	// Initialize pointers to images
	srcImage = NULL;
	gryImage = NULL;
	dstImage = NULL;

	circles  = NULL;
	lines    = NULL;
	storage = cvCreateMemStorage(0);

	// Other
	_maxR = 0;
	_accT = 0;
}

/****************************************************************************/
// Destructor
demHough::~demHough(){

	cvDestroyWindow(winName1);

    cvExtReleaseImageCheckNULL(&srcImage);
    cvExtReleaseImageCheckNULL(&gryImage);
    cvExtReleaseImageCheckNULL(&dstImage);

    cvReleaseMemStorage( &storage );
}

/****************************************************************************/
int demHough::runDemo(void){
	 	
	// Load image
    srcImage = cvLoadImage(imagePath,CV_LOAD_IMAGE_COLOR);
    if(!srcImage){
        fprintf(stderr,"Image was not loaded.\n");
        return -1;
    }

	// Allocate memory
	dstImage	   = cvCloneImage(srcImage);
	gryImage	   = cvCreateImage( cvGetSize(srcImage), IPL_DEPTH_8U, 1);
	cvCvtColor(srcImage, gryImage, CV_BGR2GRAY);    
	cvSmooth(gryImage, gryImage, CV_GAUSSIAN, 9, 9); 

	// Open windows
    cvNamedWindow(winName1, 1);
    cvShowImage(winName1, srcImage);

	// Add track bars.
	cvCreateTrackbar("Radius", winName1, &_maxR		, 10, callBackFun);
	cvCreateTrackbar("Thresh", winName1, &_accT		, 10, callBackFun);

	// Execute Hough
    callBackFun(0);
    cvWaitKey(0);    
    return 0;
}

/****************************************************************************/
// Return textual explanation of the commands
const char* demHough::getComDesc(void){
	return command;
}

/****************************************************************************/
// Call back

void demHoughNS::callBackFun(int arg){

	cvCopy(srcImage,dstImage);
	if (!_maxR){
  	   cvShowImage( winName1, dstImage );    
	   return;
	}

	// -------------------------------------------------
	// Start Hough circles detection

	int accomulatorResolution  = 1;  // for each pixel
	int minDistBetweenCircles  = 10; // In pixels
	int cannyThresh            = 20;
	int accomulatorThresh      = 5*_accT+1;
	int minCircleRadius        = 0;
	int maxCircleRadius        = _maxR*10;
	cvClearMemStorage(storage);
	circles = cvHoughCircles( gryImage, storage,
		                      CV_HOUGH_GRADIENT, accomulatorResolution, 
							  minDistBetweenCircles,
							  cannyThresh , accomulatorThresh,
							  minCircleRadius,maxCircleRadius );    
	// Draw circles
	for (int i = 0; i < circles->total; i++){
		float* p = (float*)cvGetSeqElem(circles,i);
		// Draw center
		cvCircle(dstImage, cvPoint(cvRound(p[0]),cvRound(p[1])),
			               1, CV_RGB(0,255,0), -1, 8, 0 );
		// Draw circle
		cvCircle(dstImage, cvPoint(cvRound(p[0]),cvRound(p[1])),
			               cvRound(p[2]),CV_RGB(255,0,0), 1, 8, 0 );
	}    


	// -------------------------------------------------
	// Start Hough lines detection

/*	double thetaResolution  = CV_PI/180;  // In radians
	lines = cvHoughLines2( gryImage, storage, 
		                   CV_HOUGH_PROBABILISTIC, 
						   accomulatorResolution, thetaResolution, 
						   50, 10, 10 );

	// Draw lines
	for (int i = 0; i < lines->total; i++){
		CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
		cvLine( dstImage, line[0], line[1], CV_RGB(0,255,0), 1, CV_AA, 0 );
	}    */
	cvShowImage( winName1, dstImage );    
}

/****************************************************************************/
// EOF.



