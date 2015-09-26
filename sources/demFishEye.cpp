/****************************************************************************/
/****************************    demFishEye.cpp    **************************/
/****************************************************************************/

/****************************************************************************/
// Includes
#include "stdafx.h"
#include "demFishEye.h"
#include "sfCVextension.h"

using namespace demFishEyeNS;

/****************************************************************************/
demFishEye::demFishEye(const char* path){

	if (path){ 
	   safeStrBufCpy(imagePath,path);
	}
	else {
	   safeStrBufCpy(imagePath,IPE_DEFAULT_IMAGE_FILE);
	}

	// Define the command
	//safeStrBufCpy(command,"Change Fish eye parameters: \tTry to set tangetial distortion to (tX,tY) = (15,12), \t set Radial distortion to 2");
	safeStrBufCpy(command,"Change Fish eye parameters: tangetial distortion and radial distortion");

	// Initialize pointers to images
	srcImage = NULL;
	dstImage = NULL;

	// Other
	allRange = 50;
	_r       = allRange;
	_tX      = allRange;
	_tY      = allRange;
}

/****************************************************************************/
// Destructor
demFishEye::~demFishEye(){

	cvDestroyWindow(winName);

    cvExtReleaseImageCheckNULL(&srcImage);
    cvExtReleaseImageCheckNULL(&dstImage);
}

/****************************************************************************/
int demFishEye::runDemo(void){
	 	
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
	
	// Add track bars.
	cvShowImage(winName, srcImage );
	cvCreateTrackbar("Radial", winName, &_r 		, 2*allRange, callBackFun);
	cvCreateTrackbar("Tang X", winName, &_tX		, 2*allRange, callBackFun);
	cvCreateTrackbar("Tang Y", winName, &_tY		, 2*allRange, callBackFun);
    //cvWaitKey(0);    
	
    callBackFun(0);
    cvWaitKey(0);    
    return 0;
}

/****************************************************************************/
// Return textual explanation of the commands
const char* demFishEye::getComDesc(void){
	return command;
}

/****************************************************************************/
// Call back

void demFishEyeNS::callBackFun(int arg){


	CvMat intrCamParamsMat = cvMat( 3, 3, CV_64F, _camera );
	CvMat dist4Coeff       = cvMat( 1, 4, CV_64F, _dist4Coeff );

	// Build distortion coefficients matrix.
	/*dist4Coeff.data.db[0] = -0.7673551364835269 +0.8+(_r-allRange*1.0)/allRange*1.0;
	dist4Coeff.data.db[1] = 0.4591356692276609  +0.8+(_r-allRange*1.0)/allRange*1.0;
	dist4Coeff.data.db[2] = -0.0133485248337560 -0.2+(_tY-allRange*1.0)/allRange*1.0;
	dist4Coeff.data.db[3] = 0.0304220253820542  -1.0+(_tX-allRange*1.0)/allRange*1.0;*/
	dist4Coeff.data.db[0] = (_r-allRange*1.0)/allRange*1.0;
	dist4Coeff.data.db[1] = (_r-allRange*1.0)/allRange*1.0;
	dist4Coeff.data.db[2] = (_tY-allRange*1.0)/allRange*1.0;
	dist4Coeff.data.db[3] = (_tX-allRange*1.0)/allRange*1.0;

	// Build intrinsic camera parameters matrix.
	intrCamParamsMat.data.db[0] = 587.1769751432448200/2.0;
	intrCamParamsMat.data.db[1] = 0.;
	intrCamParamsMat.data.db[2] = 319.5000000000000000/2.0+0;
	intrCamParamsMat.data.db[3] = 0.;
	intrCamParamsMat.data.db[4] = 591.3189722549362800/2.0;
	intrCamParamsMat.data.db[5] = 239.5000000000000000/2.0+0;
	intrCamParamsMat.data.db[6] = 0.;
	intrCamParamsMat.data.db[7] = 0.;
	intrCamParamsMat.data.db[8] = 1.;

	// Apply transformation
    cvUndistort2( srcImage, dstImage, &intrCamParamsMat, &dist4Coeff );
	cvShowImage( winName, dstImage );
}

/****************************************************************************/
// EOF.



