/****************************************************************************/
/****************************    demMorpho.cpp    ***************************/
/****************************************************************************/

/****************************************************************************/
// Includes
#include "stdafx.h"
#include "demMorpho.h"
#include "sfCVextension.h"

using namespace demMorphoNS;

/****************************************************************************/
demMorpho::demMorpho(const char* path){

	if (path){ 
	   safeStrBufCpy(imagePath,path);
	}
	else {
	   safeStrBufCpy(imagePath,IPE_DEFAULT_IMAGE_FILE);
	}

	// Define the command
	safeStrBufCpy(command,"Apply morphological operations. Press SPACE - for looping through different shapes of stractural elements");

	// Initialize pointers to images
	srcImage = NULL;
	dstImage = NULL;

	// Other
	element      = NULL;
    elementShape = CV_SHAPE_RECT;

	_openClose   = MAX_ITER;
	_erodeDilate = MAX_ITER;
}

/****************************************************************************/
// Destructor
demMorpho::~demMorpho(){

	cvDestroyWindow("Proccessed");
	cvDestroyWindow("Original");

    cvExtReleaseImageCheckNULL(&srcImage);
    cvExtReleaseImageCheckNULL(&dstImage);
}

/****************************************************************************/
int demMorpho::runDemo(void){
	 	
	// Load image
    srcImage = cvLoadImage(imagePath,CV_LOAD_IMAGE_COLOR);
    if(!srcImage){
        fprintf(stderr,"Image was not loaded.\n");
        return -1;
    }

	// Allocate memory
    dstImage = cvCloneImage(srcImage);

	// Open windows
	cvNamedWindow("Original", 1);
	cvShowImage( "Original", srcImage );
    cvNamedWindow("Proccessed", 1);
	
	// Add track bars. Orientation is in units of 5[deg]. magnitude is in pixels
	cvCreateTrackbar("Open/Close", "Original", &_openClose   , MAX_ITER*2+1, callBackFun);
	cvCreateTrackbar("Erod/Dilat", "Original", &_erodeDilate , MAX_ITER*2+1, callBackFun);
    callBackFun(0);

	char key = 0;
    while((key != -1)&&(key != 27)&&(key != 'q')&&(key != 'Q')){

        // Restart inpaint. Clear all previous results
		if (key == ' '){
		   elementShape = (elementShape + 1) % 3;
		   callBackFun(0);
        }
        key = cvWaitKey(0);
    }
	
	return 0;
}

/****************************************************************************/
// Return textual explanation of the commands
const char* demMorpho::getComDesc(void){
	return command;
}

/****************************************************************************/
// Call back

void demMorphoNS::callBackFun(int arg){

    int oc  = _openClose   - MAX_ITER;
	int aoc = oc>0 ? oc:-oc;

	int ed  = _erodeDilate - MAX_ITER;
	int aed = ed>0 ? ed:-ed;

	// Perform Openning & Closing
    element = cvCreateStructuringElementEx( aoc*2+1, aoc*2+1, aoc, aoc, elementShape, 0 );
    if ( oc < 0 ){
        cvErode (srcImage,dstImage,element,1);
        cvDilate(dstImage,dstImage,element,1);
    }
    else{
        cvDilate(srcImage,dstImage,element,1);
        cvErode (dstImage,dstImage,element,1);
    }
    cvReleaseStructuringElement(&element);

	// Perform Erosion and Delation
    element = cvCreateStructuringElementEx( aed*2+1, aed*2+1, aed, aed, elementShape, 0 );
    if ( ed < 0 ){
        cvErode (dstImage,dstImage,element,1);
    }
    else{
        cvDilate(dstImage,dstImage,element,1);
    }
    cvReleaseStructuringElement(&element);

    cvShowImage("Proccessed",dstImage);
}

/****************************************************************************/
// EOF.
