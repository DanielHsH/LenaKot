/****************************************************************************/
/****************************    demPerspect.cpp    **************************/
/***************************************************************************/

/****************************************************************************/
// Includes
#include "stdafx.h"
#include "demPerspect.h"
#include "sfCVextension.h"

using namespace demPerspectNS;

/****************************************************************************/
demPerspect::demPerspect(const char* path){

	if (path){ 
	   safeStrBufCpy(imagePath,path);
	}
	else {
	   safeStrBufCpy(imagePath,IPE_DEFAULT_IMAGE_FILE);
	}

	// Define the command
	safeStrBufCpy(command,"Change Perspective parameters: \t(dX,dY) - translation, \t(pX,pY) - perspective. \tNote: image will be auto scaled to keep constant size.");

	// Initialize pointers to images
	srcImage = NULL;
	dstImage = NULL;

	// Other
	_dX      = 10;
	_dY      = 10;
	_pX      = 10;
	_pY      = 10;
}

/****************************************************************************/
// Destructor
demPerspect::~demPerspect(){

	cvDestroyWindow(winName);

    cvExtReleaseImageCheckNULL(&srcImage);
    cvExtReleaseImageCheckNULL(&dstImage);
}

/****************************************************************************/
int demPerspect::runDemo(void){
	 	
	// Load image
    srcImage = cvLoadImage(imagePath,CV_LOAD_IMAGE_COLOR);
    if(!srcImage){
        fprintf(stderr,"Image was not loaded.\n");
        return -1;
    }

	// Allocate memory
    dstImage = cvCloneImage(srcImage);

	// Open windows
	cvNamedWindow(winName, 1);
	cvShowImage(winName, srcImage );
	
	// Add track bars.
	cvCreateTrackbar("dX ", winName, &_dX		, 20, callBackFun);
	cvCreateTrackbar("dY ", winName, &_dY		, 20, callBackFun);
	cvCreateTrackbar("pX ", winName, &_pX		, 20, callBackFun);
	cvCreateTrackbar("pY ", winName, &_pY		, 20, callBackFun);
	
    callBackFun(0);
    cvWaitKey(0);    
    return 0;
}

/****************************************************************************/
// Return textual explanation of the commands
const char* demPerspect::getComDesc(void){
	return command;
}

/****************************************************************************/
// Call back

void demPerspectNS::callBackFun(int arg){


	// Get image size for perspective calculations
	CvSize imSize = cvGetSize(srcImage);

	// Dont let perspective be zero
	_pX = _pX?_pX:1;
	_pY = _pY?_pY:1;

	// Build transformation matrix.
    CvMat tMat = cvMat( 3, 3, CV_64F, _transMat );
	tMat.data.db[0] = 0.5*(1 + (_pX-10)/10.0);
	tMat.data.db[1] = 0;                    // Skew is zero
	tMat.data.db[2] = (_dX-10)*20;
	tMat.data.db[3] = 0;
	tMat.data.db[4] = 0.5*(1 + (_pY-10)/10.0);
	tMat.data.db[5] = (_dY-10)*20;          // Skew is zero
	tMat.data.db[6] = (_pX-10)/(10.0*imSize.width);
	tMat.data.db[7] = (_pY-10)/(10.0*imSize.height);
	tMat.data.db[8] = 1;

	// Apply transformation
	cvWarpPerspective(srcImage,dstImage,&tMat);
	cvShowImage( winName, dstImage );
}

/****************************************************************************/
// EOF.



