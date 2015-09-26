/****************************************************************************/
/****************************    demPyrCre.cpp    ***************************/
/****************************************************************************/

/****************************************************************************/
// Includes
#include "stdafx.h"
#include "demPyrCre.h"
#include "sfCVextension.h"

using namespace demPyrCreNS;

/****************************************************************************/
demPyrCre::demPyrCre(const char* path){

	if (path){ 
	   safeStrBufCpy(imagePath,path);
	}
	else {
	   safeStrBufCpy(imagePath,IPE_DEFAULT_IMAGE_FILE);
	}

	// Define the command
	safeStrBufCpy(command,"Move the slider to the right to create gaussian pyramid,\t then to the left to create Laplacian pyramid");

	// Initialize pointers to images
	srcImage = NULL;
	dstImage = NULL;

	// Other
	_dGL      = 7;
}

/****************************************************************************/
// Destructor
demPyrCre::~demPyrCre(){

	cvDestroyWindow(winName);

    cvExtReleaseImageCheckNULL(&srcImage);
    cvExtReleaseImageCheckNULL(&dstImage);
}

/****************************************************************************/
int demPyrCre::runDemo(void){
	 	
	// Load image
    srcImage = cvLoadImage(imagePath,CV_LOAD_IMAGE_COLOR);
    if(!srcImage){
        fprintf(stderr,"Image was not loaded.\n");
        return -1;
    }

	// Allocate memory
	CvSize imSize = cvGetSize(srcImage);
	dstImage = cvCreateImage( cvSize(imSize.width*2,imSize.height), 
		                      IPL_DEPTH_8U,  srcImage->nChannels );
	// Open windows
	cvNamedWindow(winName, 1);
	cvShowImage( winName, dstImage );
	
	// Add track bars.
	cvCreateTrackbar("Depth  ", winName, &_dGL		, _dGL*2, callBackFun);
	
	// cvShowImage(winName, srcImage );
    callBackFun(0);
    cvWaitKey(0);    
    return 0;
}

/****************************************************************************/
// Return textual explanation of the commands
const char* demPyrCre::getComDesc(void){
	return command;
}

/****************************************************************************/
// Call back

void demPyrCreNS::callBackFun(int arg){
	
	// Define auxillary variables
	IplImage* prevLevel = cvCloneImage(srcImage);
	IplImage* curLevel  = NULL;
   	CvSize imSize = cvGetSize(srcImage);
	
	// Copy first level into dest image initial coordinate.
	cvZero(dstImage);
	CvRect rect   = cvRect(0,0,imSize.width,imSize.height);
	cvSetImageROI(dstImage,rect);
	cvCopy(prevLevel,dstImage);
	cvResetImageROI(dstImage);

	// Determine which pyramid to do
	int  dGL = (_dGL - 7);
	bool doLaplacian = (dGL < 0);
		 dGL = abs(dGL);

	// -------------------------------------------------
	// Create gaussian/ laplacian pyramid in a loop

	for (int i = 0; i < dGL; i++){

		// Create next level (cerLevel from prevLevel)
		int newWidth = (int)floor(rect.width /2.0);
		int newHeight= (int)floor(rect.height/2.0);
		curLevel  = cvCreateImage( cvSize(newWidth,newHeight), 
 								   IPL_DEPTH_8U,  srcImage->nChannels );
		cvPyrDown(prevLevel, curLevel,CV_GAUSSIAN_5x5);

		// Update Previous level if laplacian is built
		if (doLaplacian){
			IplImage* recLevel = cvCreateImage( cvSize(curLevel->width*2,curLevel->height*2), 
								  IPL_DEPTH_8U,  srcImage->nChannels );
			cvPyrUp(curLevel, recLevel,CV_GAUSSIAN_5x5);
			cvSetImageROI(prevLevel,cvRect(0,0,recLevel->width,recLevel->height));
			cvSub(prevLevel,recLevel,prevLevel);
			cvResetImageROI(prevLevel);
			// Copy laplace
			cvSetImageROI(dstImage,rect);
			cvCopy(prevLevel,dstImage);
			cvResetImageROI(dstImage);
			cvExtReleaseImageCheckNULL(&recLevel);
		}

		// Define area in dstImage where to copy the new level
		rect.x     += rect.width;
		rect.width  = newWidth;
		rect.height = newHeight;

		// Copy current level into dest image.
		cvSetImageROI(dstImage,rect);
		cvCopy(curLevel,dstImage);
		cvResetImageROI(dstImage);

		// move curLevel to prevLevel
		cvExtReleaseImageCheckNULL(&prevLevel);
		prevLevel = curLevel;
	}
	cvExtReleaseImageCheckNULL(&prevLevel);

	// Show the image
	cvShowImage( winName, dstImage );
}

/****************************************************************************/
// EOF.



