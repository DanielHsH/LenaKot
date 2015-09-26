/****************************************************************************/
/***************************    demEdgeDet.cpp    ***************************/
/****************************************************************************/

/****************************************************************************/
// Includes
#include "stdafx.h"
#include "demEdgeDet.h"
#include "sfCVextension.h"

using namespace demEdgeDetNS;

/****************************************************************************/
demEdgeDet::demEdgeDet(const char* path){

	if (path){ 
	   safeStrBufCpy(imagePath,path);
	}
	else {
	   safeStrBufCpy(imagePath,IPE_DEFAULT_IMAGE_FILE);
	}

	// Define the command
	safeStrBufCpy(command,"Detect edges contours using Canny algorithm.\tChange threshold and filter of the edges");

	// Initialize pointers to images
	srcImage = NULL;
	gryImage = NULL;
	tmpImage = NULL;
	dstImage = NULL;

	// Other
	_edge_thresh = 1;
	_magnitude   = 0;
}

/****************************************************************************/
// Destructor
demEdgeDet::~demEdgeDet(){

	cvDestroyWindow("Edge Threshold");

    cvExtReleaseImageCheckNULL(&srcImage);
    cvExtReleaseImageCheckNULL(&gryImage);
    cvExtReleaseImageCheckNULL(&tmpImage);
    cvExtReleaseImageCheckNULL(&dstImage);
}

/****************************************************************************/
int demEdgeDet::runDemo(void){
	 	
	// Load image
    srcImage = cvLoadImage(imagePath,CV_LOAD_IMAGE_COLOR);
    if(!srcImage){
        fprintf(stderr,"Image was not loaded.\n");
        return -1;
    }

	// Allocate memory
    dstImage = cvCloneImage(srcImage);
    gryImage = cvCreateImage(cvSize(srcImage->width,srcImage->height), 
		                     IPL_DEPTH_8U, 1);
    tmpImage = cvCreateImage(cvSize(srcImage->width,srcImage->height), 
		                     IPL_DEPTH_8U, 1);

    cvCvtColor(srcImage, gryImage, CV_BGR2GRAY);

	// Open windows
    cvNamedWindow("Edge Threshold", 1);
    cvShowImage("Edge Threshold", srcImage);
	cvCreateTrackbar("Threshold" , "Edge Threshold", &_edge_thresh, 100, callBackFun);
	cvCreateTrackbar("Smooth"    , "Edge Threshold", &_magnitude,     5, callBackFun);

    callBackFun(0);
    cvWaitKey(0);    
    return 0;
}

/****************************************************************************/
// Return textual explanation of the commands
const char* demEdgeDet::getComDesc(void){
	return command;
}

/****************************************************************************/
// Call back

void demEdgeDetNS::callBackFun(int arg){

    // Run the edge detector on grayscale
    cvSmooth( gryImage, tmpImage, CV_BLUR, (2*_magnitude)+1, 2*(_magnitude)+1, 0, 0 );
    cvCanny(tmpImage, tmpImage, (float)_edge_thresh, (float)_edge_thresh*3, 3);
    cvZero(dstImage);

    // Copy edge points
    cvCopy( srcImage, dstImage, tmpImage );
    cvShowImage("Edge Threshold", dstImage);
}

/****************************************************************************/
// EOF.



