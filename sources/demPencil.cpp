/****************************************************************************/
/****************************    demPencil.cpp    ***************************/
/****************************************************************************/

/****************************************************************************/
// Includes
#include "stdafx.h"
#include "demPencil.h"
#include "sfCVextension.h"

using namespace demPencilNS;

/****************************************************************************/
demPencil::demPencil(const char* path){

	if (path){ 
	   safeStrBufCpy(imagePath,path);
	}
	else {
	   safeStrBufCpy(imagePath,IPE_DEFAULT_IMAGE_FILE);
	}

	// Define the command
	safeStrBufCpy(command,"Convert image to pencil sketch");

	// Initialize pointers to images
	srcImage  = NULL;
	dstImage  = NULL;

	// Other
	_intensNormal  = 7;
	_colorNormal   = 5;
	_looseDitails  = 1;
	_edgeSharpness = 8;
	_noiseRatio    = 2;
	_colorBits     = 8;

}

/****************************************************************************/
// Destructor
demPencil::~demPencil(){

	cvDestroyWindow(winName1);
	cvDestroyWindow(winName2);

	if (dstImage){
	   cvSaveImage("PencilPhoto.jpg",dstImage);
	}
    cvExtReleaseImageCheckNULL(&srcImage);
    cvExtReleaseImageCheckNULL(&dstImage);
}

/****************************************************************************/
int demPencil::runDemo(void){

	// Check if we should run it on live video
	if (!strcmp(imagePath,IPE_DEFAULT_ONLINE_CAMERA)){
	   demPencilLiveCam(1);
	   return 0;
	}
	// Load image
    srcImage = cvLoadImage(imagePath,CV_LOAD_IMAGE_COLOR);
    if(!srcImage){
        fprintf(stderr,"Image was not loaded.\n");
        return -1;
    }

	// Allocate memory
	dstImage  = cvCreateImage( cvGetSize(srcImage), IPL_DEPTH_8U , srcImage->nChannels );

	// Open windows
	cvNamedWindow(winName1, 1);
	cvShowImage(winName1, srcImage );
    cvNamedWindow(winName2, 1);
	
	// Add track bars. Orientation is in units of 5[deg]. magnitude is in pixels
	cvCreateTrackbar("Background" ,winName1, &_intensNormal  , 10, callBackFun);
	cvCreateTrackbar("Color     " ,winName1, &_colorNormal   , 20, callBackFun);
	cvCreateTrackbar("No ditails" ,winName1, &_looseDitails , 2,  callBackFun);
	cvCreateTrackbar("Bold Strok" ,winName1, &_edgeSharpness , 20, callBackFun);
	cvCreateTrackbar("Thin Srtok" ,winName1, &_noiseRatio    , 10, callBackFun);
	cvCreateTrackbar("Color Bit " ,winName1, &_colorBits     , 8 , callBackFun);

    callBackFun(0);
    cvWaitKey(0);    
    return 0;
}

/****************************************************************************/
// Return textual explanation of the commands
const char* demPencil::getComDesc(void){
	return command;
}

/****************************************************************************/
// Call back

void demPencilNS::callBackFun(int arg){

	cvExtPhoto2Pencil(srcImage,dstImage,_intensNormal/10.,_colorNormal,_looseDitails,
		                                _edgeSharpness,_noiseRatio/10.,_colorBits);
	cvShowImage( winName2, dstImage );
}

/****************************************************************************/
// EOF.



