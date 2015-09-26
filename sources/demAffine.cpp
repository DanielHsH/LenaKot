/****************************************************************************/
/****************************    demAffine.cpp    ***************************/
/****************************************************************************/

/****************************************************************************/
// Includes
#include "stdafx.h"
#include "demAffine.h"
#include "sfCVextension.h"

using namespace demAffineNS;

/****************************************************************************/
demAffine::demAffine(const char* path){

	if (path){ 
	   safeStrBufCpy(imagePath,path);
	}
	else {
	   safeStrBufCpy(imagePath,IPE_DEFAULT_IMAGE_FILE);
	}

	// Define the command
	safeStrBufCpy(command,"Change Affine parameters: \t(dX,dY) - translation, \t(sX,sY) - scale, \t(qX,qY) - skew");

	// Initialize pointers to images
	srcImage = NULL;
	dstImage = NULL;

	// Other
	_dX      = 10;
	_dY      = 10;
	_skX     = 10;
	_skY     = 10;
	_scaleX	 = 10;
	_scaleY	 = 10;
}

/****************************************************************************/
// Destructor
demAffine::~demAffine(){

	cvDestroyWindow(winName);

    cvExtReleaseImageCheckNULL(&srcImage);
    cvExtReleaseImageCheckNULL(&dstImage);
}

/****************************************************************************/
int demAffine::runDemo(void){
	 	
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
	cvCreateTrackbar("sX ", winName, &_scaleX	, 20, callBackFun);
	cvCreateTrackbar("sY ", winName, &_scaleY	, 20, callBackFun);
	cvCreateTrackbar("qX ", winName, &_skX		, 20, callBackFun);
	cvCreateTrackbar("qY ", winName, &_skY		, 20, callBackFun);
		
    callBackFun(0);
    cvWaitKey(0);    
    return 0;
}

/****************************************************************************/
// Return textual explanation of the commands
const char* demAffine::getComDesc(void){
	return command;
}

/****************************************************************************/
// Call back

void demAffineNS::callBackFun(int arg){


	// Build transformation matrix.
    CvMat tMat = cvMat( 2, 3, CV_64F, _transMat );
	tMat.data.db[0] = _scaleX/10.0;
	tMat.data.db[1] = (_skX-10)/10.0;
	tMat.data.db[2] = (_dX-10)*10*2;
	tMat.data.db[3] = (_skY-10)/10.0;
	tMat.data.db[4] = _scaleY/10.0;
	tMat.data.db[5] = (_dY-10)*10*2;

	// Apply transformation
	cvWarpAffine(srcImage,dstImage,&tMat);
	cvShowImage( winName, dstImage );
}

/****************************************************************************/
// EOF.

