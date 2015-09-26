/****************************************************************************/
/****************************    demDistTr.cpp    ***************************/
/****************************************************************************/

/****************************************************************************/
// Includes
#include "stdafx.h"
#include "demDistTr.h"
#include "sfCVextension.h"

using namespace demDistTrNS;

/****************************************************************************/
demDistTr::demDistTr(const char* path){

	if (path){ 
	   safeStrBufCpy(imagePath,path);
	}
	else {
	   safeStrBufCpy(imagePath,IPE_DEFAULT_IMAGE_FILE);
	}

	// Define the command
	safeStrBufCpy(command,"0 - use precise distance\t\t1 - use L1 metric\t\t2 - use L2 metric\n3 - use 3x3 mask\t\t5 - use 5x5 mask\n");

	// Initialize pointers to images
	srcImage = NULL;
	tmpImage = NULL;
	dstImage = NULL;

	// Other variables
	_objTresh      = 100;
	_SwitchObjBack = 0;
	_distType      = CV_DIST_L2;
	_maskSize      = CV_DIST_MASK_PRECISE;
}

/****************************************************************************/
// Destructor
demDistTr::~demDistTr(){

	cvDestroyWindow("Proccessed");
	cvDestroyWindow("Original");

    cvExtReleaseImageCheckNULL(&srcImage);
	cvExtReleaseImageCheckNULL(&tmpImage);	
    cvExtReleaseImageCheckNULL(&dstImage);
}

/****************************************************************************/
int demDistTr::runDemo(void){
	 	
	// Load image
    srcImage = cvLoadImage(imagePath,CV_LOAD_IMAGE_GRAYSCALE);
    if(!srcImage){
        fprintf(stderr,"Image was not loaded.\n");
        return -1;
    }

	// Allocate memory
    tmpImage = cvCloneImage(srcImage);
	dstImage = cvCreateImage( cvGetSize(srcImage), IPL_DEPTH_32F, 1 );

	// Open windows
	cvNamedWindow("Original", 1);
	cvShowImage( "Original", srcImage );
    cvNamedWindow("Proccessed", 1);
	
	cvCreateTrackbar("Thresh"  , "Original", &_objTresh     ,254, callBackFun);
	cvCreateTrackbar("Obj/Hole", "Original", &_SwitchObjBack,1, callBackFun);
    callBackFun(0);

	char key = 0;
    while((key != -1)&&(key != 27)&&(key != 'q')&&(key != 'Q')){
      
		switch (key){
		case '1' : _distType = CV_DIST_L1;
				   break;
		case '2' : _distType = CV_DIST_L2;
				   break;
		case '3' : _maskSize = CV_DIST_MASK_3;
				   break;
		case '5' : _maskSize = CV_DIST_MASK_5;
				   break;
		case '0' : _maskSize = CV_DIST_MASK_PRECISE;
				   break;

		}
		callBackFun(0);
        key = cvWaitKey(0);
    }
	
	return 0;
}

/****************************************************************************/
// Return textual explanation of the commands
const char* demDistTr::getComDesc(void){
	return command;
}

/****************************************************************************/
// Call back

void demDistTrNS::callBackFun(int arg){
    int mSize    = _maskSize;
    int distType = _distType;

	cvThreshold( srcImage, tmpImage, _objTresh,255, CV_THRESH_BINARY );
	if (_SwitchObjBack){
	   cvNot(tmpImage,tmpImage);
	}
	if( distType == CV_DIST_L1 ){
        cvDistTransform( tmpImage, tmpImage, _distType, mSize, NULL, NULL );
        cvConvert( tmpImage, dstImage );
    }
	else{
        cvDistTransform( tmpImage, dstImage, _distType, mSize, NULL, NULL );
	}

	// Normalize image
	double max;
	cvMinMaxLoc(dstImage,NULL,&max);
	cvConvertScale( dstImage, dstImage, 1./max, 0);
	
	cvShowImage("Proccessed",dstImage);
}

/****************************************************************************/
// EOF.
