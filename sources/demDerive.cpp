/****************************************************************************/
/****************************    demDerive.cpp    ***************************/
/****************************************************************************/

/****************************************************************************/
// Includes
#include "stdafx.h"
#include "demDerive.h"
#include "sfCVextension.h"

using namespace demDeriveNS;

/****************************************************************************/
demDerive::demDerive(const char* path){

	if (path){ 
	   safeStrBufCpy(imagePath,path);
	}
	else {
	   safeStrBufCpy(imagePath,IPE_DEFAULT_IMAGE_FILE);
	}

	// Define the command
	safeStrBufCpy(command,"Change derivative angle and filter");

	// Initialize pointers to images
	srcImage = NULL;
	tmpImage = NULL;
	dstImage = NULL;

	// Other
	_orientation = 9;
	_magnitude   = 0;
}

/****************************************************************************/
// Destructor
demDerive::~demDerive(){

	cvDestroyWindow("Derivative");
	cvDestroyWindow("Original");

    cvExtReleaseImageCheckNULL(&srcImage);
    cvExtReleaseImageCheckNULL(&tmpImage);
    cvExtReleaseImageCheckNULL(&dstImage);
}

/****************************************************************************/
int demDerive::runDemo(void){
	 	
	// Load image
    srcImage = cvLoadImage(imagePath,CV_LOAD_IMAGE_COLOR);
    if(!srcImage){
        fprintf(stderr,"Image was not loaded.\n");
        return -1;
    }

	// Allocate memory
    dstImage = cvCloneImage(srcImage);
    tmpImage = cvCloneImage(srcImage);

	// Open windows
	cvNamedWindow("Original", 1);
	cvShowImage( "Original", srcImage );
    cvNamedWindow("Derivative", 1);
	
	// Add track bars. Orientation is in units of 5[deg]. magnitude is in pixels
	cvCreateTrackbar("Angle  5x" , "Original", &_orientation, 18, callBackFun);
	cvCreateTrackbar("Smooth   ", "Original", &_magnitude  , 4 , callBackFun);
    callBackFun(0);
    cvWaitKey(0);    
    return 0;
}

/****************************************************************************/
// Return textual explanation of the commands
const char* demDerive::getComDesc(void){
	return command;
}

/****************************************************************************/
// Call back

void demDeriveNS::callBackFun(int arg){

	// Smooth the image according to the filter
	cvSmooth( srcImage, tmpImage, CV_BLUR, (2*_magnitude)+1, (2*_magnitude)+1, 0, 0 );

	// Calculate derivatives dX, dY
	IplImageIterator<unsigned char> src(tmpImage,1,1,srcImage->width-2,srcImage->height-2);
	cvExtImageIterator dst(dstImage,1,1,srcImage->width-2,srcImage->height-2);

	double cos_alpha = cos(_orientation*5*CV_PI/180);
	double sin_alpha = sin(_orientation*5*CV_PI/180);

	int dX,dY;

	while (!src){

		dX = abs(  src.neighbor(-1,-1) -   src.neighbor(1,-1)+
	             2*src.neighbor(-1,0)  - 2*src.neighbor(1,0) +
		           src.neighbor(-1,1)  -   src.neighbor(1,1));
		dX = dX > 255 ? 255 : dX;

		dY = abs(  src.neighbor(-1,-1) -   src.neighbor(-1,1)+
	             2*src.neighbor(0,-1)  - 2*src.neighbor(0,1) +
		           src.neighbor(1,-1)  -   src.neighbor(1,1));
		dY = dY > 255 ? 255 : dY;

		*dst = cvRound(sqrt(pow(sin_alpha*(dX),2) + 
			                pow(cos_alpha*(dY),2)));			 
		++src;
	    ++dst;
	}
	cvShowImage( "Derivative", dstImage );
}

/****************************************************************************/
// EOF.



