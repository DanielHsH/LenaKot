/****************************************************************************/
/*****************************    demFusion.cpp    **************************/
/****************************************************************************/

/****************************************************************************/
// Includes
#include "stdafx.h"
#include "demFusion.h"
#include "sfCVextension.h"

using namespace demFusionNS;

/****************************************************************************/
demFusion::demFusion(const char* path){

	if (path){ 
	   safeStrBufCpy(imagePath,path);
	}
	else {
	   safeStrBufCpy(imagePath,IPE_DEFAULT_IMAGE_FILE);
	}

	// Define the command
	safeStrBufCpy(command,"Change transparancy to fuse IR and ICCD images. Change images sets");

	// Initialize pointers to images
	srcImage1 = NULL;
	srcImage2 = NULL;
	dstImage  = NULL;

	// Other
	_alpha   = 0;
	_set     = 0;
	_prevSet = 0;
}

/****************************************************************************/
// Destructor
demFusion::~demFusion(){

	cvDestroyWindow(winName);

    cvExtReleaseImageCheckNULL(&srcImage1);
    cvExtReleaseImageCheckNULL(&srcImage2);
    cvExtReleaseImageCheckNULL(&dstImage);
}

/****************************************************************************/
int demFusion::runDemo(void){
	 	
	// Load image
    srcImage1 = cvLoadImage("Fuse0.jpg",CV_LOAD_IMAGE_COLOR);
    srcImage2 = cvLoadImage("Fuse1.jpg",CV_LOAD_IMAGE_COLOR);
    if((!srcImage1)||(!srcImage1)){
        fprintf(stderr,"Image was not loaded.\n");
        return -1;
    }

	// Allocate memory
    dstImage = cvCloneImage(srcImage1);

	// Open windows
	cvNamedWindow(winName, 1);
	
	// Add track bars.
	cvShowImage(winName, srcImage1 );
	cvCreateTrackbar("Transp"   , winName, &_alpha		, UNSIGN_PER_TRACK_BAR_RANGE, callBackFun);
	cvCreateTrackbar("Img sets" , winName, &_set		, 2                         , callBackFun);
	
    callBackFun(0);
    cvWaitKey(0);    
    return 0;
}

/****************************************************************************/
// Return textual explanation of the commands
const char* demFusion::getComDesc(void){
	return command;
}

/****************************************************************************/
// Call back

void demFusionNS::callBackFun(int arg){

	// Check if image set was changed
	if (_set-_prevSet){
	   // Image set was changed. Load new images.
	   _prevSet = _set;
    
	   // Release allocated images
	   cvExtReleaseImageCheckNULL(&srcImage1);
       cvExtReleaseImageCheckNULL(&srcImage2);
	   cvExtReleaseImageCheckNULL(&dstImage);

	   // Reload new images
	   imgName[4] = (char)(_set*2) + '0';
       srcImage1 = cvLoadImage(imgName,CV_LOAD_IMAGE_COLOR);
	   imgName[4] = (char)(_set*2) + '1';   
	   srcImage2 = cvLoadImage(imgName,CV_LOAD_IMAGE_COLOR);
	   dstImage = cvCloneImage(srcImage1);
	}

	double alpha = _alpha/100.0;
	// Apply transformation
	cvAddWeighted(srcImage2,alpha,srcImage1,1-alpha,0,dstImage);

	cvShowImage( winName, dstImage );
}

/****************************************************************************/
// EOF.



