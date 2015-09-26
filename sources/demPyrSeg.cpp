/****************************************************************************/
/****************************    demPyrSeg.cpp    ***************************/
/****************************************************************************/

/****************************************************************************/
// Includes
#include "stdafx.h"
#include "demPyrSeg.h"
#include "sfCVextension.h"

using namespace demPyrSegNS;

/****************************************************************************/
demPyrSeg::demPyrSeg(const char* path){

	if (path){ 
	   safeStrBufCpy(imagePath,path);
	}
	else {
	   safeStrBufCpy(imagePath,IPE_DEFAULT_IMAGE_FILE);
	}

	// Define the command
	safeStrBufCpy(command,"Change the level of the pyramid, threshold of fathe-son links and threshold of segments clustering");

	// Initialize pointers to images
	srcImage    = NULL;
	tmpImage    = NULL;
	dstImage    = NULL;

	comp    = NULL;
	storage = cvCreateMemStorage(1000);

	// Other
	resFactor = 16;
	_th1   = 256/resFactor;
	_th2   = 32/resFactor;
	_level = 2;
	maxLevel = 7;

}

/****************************************************************************/
// Destructor
demPyrSeg::~demPyrSeg(){

	cvDestroyWindow(winName1);
	cvDestroyWindow(winName2);

    cvExtReleaseImageCheckNULL(&srcImage);
    cvExtReleaseImageCheckNULL(&tmpImage);
    cvExtReleaseImageCheckNULL(&dstImage);

    cvReleaseMemStorage(&storage);

}

/****************************************************************************/
int demPyrSeg::runDemo(void){
	 	
	// Load image
    srcImage = cvLoadImage(imagePath,CV_LOAD_IMAGE_COLOR);
    if(!srcImage){
        fprintf(stderr,"Image was not loaded.\n");
        return -1;
    }

	// Allocate memory
    srcImage->width  &= -(1<<maxLevel);
    srcImage->height &= -(1<<maxLevel);
    tmpImage = cvCloneImage(srcImage);
    dstImage = cvCloneImage(srcImage);

	// Open windows
    cvNamedWindow(winName1, 1);
    cvNamedWindow(winName2, 1);

	// Add track bars.
    cvCreateTrackbar("Father Thr", winName2, &_th1,  256/resFactor, callBackFun);
    cvCreateTrackbar("Clust  Thr", winName2, &_th2, 256/resFactor, callBackFun);
    cvCreateTrackbar("Level", winName2, &_level, maxLevel, callBackFun);
    cvShowImage(winName1, srcImage);

	// Execute Demo
    callBackFun(0);
    cvWaitKey(0);    
    return 0;
}

/****************************************************************************/
// Return textual explanation of the commands
const char* demPyrSeg::getComDesc(void){
	return command;
}

/****************************************************************************/
// Call back

void demPyrSegNS::callBackFun(int arg){

	if (!_level){
	   _level = 1;
	}
	   
    cvPyrSegmentation(tmpImage, dstImage, storage, &comp, _level, (_th1+1)*resFactor, (_th2+1)*resFactor/2);

    cvShowImage(winName2, dstImage);	
}

/****************************************************************************/
// EOF.



