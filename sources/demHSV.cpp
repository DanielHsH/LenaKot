/****************************************************************************/
/*****************************    demHSV.cpp    *****************************/
/****************************************************************************/

/****************************************************************************/
// Includes
#include "stdafx.h"
#include "demHSV.h"
#include "sfCVextension.h"

using namespace demHSVNS;

/****************************************************************************/
demHSV::demHSV(const char* path){

	if (path){ 
	   safeStrBufCpy(imagePath,path);
	}
	else {
	   safeStrBufCpy(imagePath,IPE_DEFAULT_IMAGE_FILE);
	}

	// Define the command
	safeStrBufCpy(command,"Change HSV parameters.\tHue is added in portions of 10 degrees");

	// Initialize pointers to images
	srcImage = NULL;
	hImage   = NULL;
	sImage   = NULL;
	vImage   = NULL;
	dstImage = NULL;

	// Other
	maxHue = 36;
	maxSat = 100;
	maxVal = 100;
	_hue   = 0;
	_sat = maxSat/2;
	_val = maxVal/2;
}

/****************************************************************************/
// Destructor
demHSV::~demHSV(){

	cvDestroyWindow("HSV parameters");

    cvExtReleaseImageCheckNULL(&srcImage);
    cvExtReleaseImageCheckNULL(&hImage);
    cvExtReleaseImageCheckNULL(&sImage);
    cvExtReleaseImageCheckNULL(&vImage);	
    cvExtReleaseImageCheckNULL(&dstImage);
}

/****************************************************************************/
int demHSV::runDemo(void){
	 	
	// Load image
    srcImage = cvLoadImage(imagePath,CV_LOAD_IMAGE_COLOR);
    if(!srcImage){
        fprintf(stderr,"Image was not loaded.\n");
        return -1;
    }

	// Allocate memory
    hImage   = cvCreateImage( cvGetSize(srcImage), IPL_DEPTH_8U,  1 );
    sImage   = cvCreateImage( cvGetSize(srcImage), IPL_DEPTH_8U,  1 );
    vImage   = cvCreateImage( cvGetSize(srcImage), IPL_DEPTH_8U,  1 );
    dstImage = cvCloneImage(srcImage);

	// Open windows
	cvNamedWindow("HSV parameters", 1);
	cvShowImage( "HSV parameters", srcImage );
	
	// Add track bars.
	cvCreateTrackbar("Add Hue", "HSV parameters", &_hue, maxHue, callBackFun);
	cvCreateTrackbar("Satur %", "HSV parameters", &_sat, maxSat, callBackFun);
	cvCreateTrackbar("Value %", "HSV parameters", &_val, maxVal, callBackFun);

	cvCvtColor( srcImage, srcImage, CV_BGR2HSV );
    callBackFun(0);
    cvWaitKey(0);    
    return 0;
}

/****************************************************************************/
// Return textual explanation of the commands
const char* demHSV::getComDesc(void){
	return command;
}

/****************************************************************************/
// Call back

void demHSVNS::callBackFun(int arg){

	
    cvSplit( srcImage, hImage, sImage, vImage, 0);
	cvExtImageIterator hI(hImage);

	// Change the hue
	while (!hI){
		*hI = (*hI +_hue*5)%180;
		++hI;
	}

	// Change value & saturation
	cvAddS(sImage,cvScalarAll((_sat-(maxSat/2))*5.12),sImage);
	cvAddS(vImage,cvScalarAll((_val-(maxVal/2))*5.12),vImage);

	// Incorporate back into image
    cvMerge(hImage, sImage, vImage, 0, dstImage );
    cvCvtColor(dstImage, dstImage, CV_HSV2BGR);
	cvShowImage( "HSV parameters", dstImage );
}

/****************************************************************************/
// EOF.



