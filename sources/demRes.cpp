/****************************************************************************/
/******************************    demRes.cpp    ****************************/
/****************************************************************************/

/****************************************************************************/
// Includes
#include "stdafx.h"
#include "demRes.h"
#include "sfCVextension.h"

using namespace demResNS;

/****************************************************************************/
demRes::demRes(const char* path){

	if (path){ 
	   safeStrBufCpy(imagePath,path);
	}
	else {
	   safeStrBufCpy(imagePath,IPE_DEFAULT_IMAGE_FILE);
	}

	// Define the command
	safeStrBufCpy(command,"Change image resolutions.\tPress SPACE - for looping through interpolations: Neares Pixel, Bi-Linear, Bi-Quibic\n");

	// Initialize pointers to images
	srcImage = NULL;
	tmpImage = NULL;
	dstImage = NULL;

	// Other
	_spacial = UNSIGN_PER_TRACK_BAR_RANGE;
	_depth   = 8;
    _freq    = UNSIGN_PER_TRACK_BAR_RANGE;

	interpolationType = CV_INTER_NN;
}

/****************************************************************************/
// Destructor
demRes::~demRes(){

	cvDestroyWindow("Image");

    cvExtReleaseImageCheckNULL(&srcImage);
    cvExtReleaseImageCheckNULL(&tmpImage);
    cvExtReleaseImageCheckNULL(&dstImage);
}

/****************************************************************************/
int demRes::runDemo(void){
	 	
	// Load image
    srcImage = cvLoadImage(imagePath,CV_LOAD_IMAGE_COLOR);
    if(!srcImage){
        fprintf(stderr,"Image was not loaded.\n");
        return -1;
    }

	// Allocate memory
    tmpImage = cvCloneImage (srcImage);
    dstImage = cvCloneImage (srcImage);

	// Open windows
    cvNamedWindow("Image", 1);
	cvShowImage( "Image", dstImage );

	// Add track bars
	cvCreateTrackbar("spacial", "Image", &_spacial, UNSIGN_PER_TRACK_BAR_RANGE, callBackFun);
    cvCreateTrackbar("depth  ", "Image", &_depth  , 8, callBackFun);
	cvCreateTrackbar("freq   ", "Image", &_freq   , UNSIGN_PER_TRACK_BAR_RANGE, callBackFun);
    callBackFun(0);

	char key = 0;
    while((key != -1)&&(key != 27)&&(key != 'q')&&(key != 'Q')){

        // Change interpolation type
		if (key == ' '){
		   interpolationType = (interpolationType + 1) % 3;
		   callBackFun(0);
        }
        key = cvWaitKey(0);
    }
    return 0;
}

/****************************************************************************/
// Return textual explanation of the commands
const char* demRes::getComDesc(void){
	return command;
}

/****************************************************************************/
// Call back

void demResNS::callBackFun(int arg){

	// Calculate new spacial resolution
	CvSize newSize = cvGetSize(srcImage);
	newSize.height = cvRound((_spacial/100.0)*newSize.height);
	newSize.width  = cvRound((_spacial/100.0)*newSize.width );

	// Reduce resolution
	if (_spacial != 0){
		cvExtReleaseImageCheckNULL(&tmpImage);
		tmpImage = cvCreateImage( newSize, srcImage->depth,  srcImage->nChannels);
		cvResize(srcImage,tmpImage,interpolationType);
		cvResize(tmpImage,dstImage,interpolationType);
	}
	else{
		cvSet(dstImage,cvScalarAll(128));
	}

	// Remove high frequencies
	int smoothFactor = (100-_freq)*2+1;
	cvSmooth(dstImage,dstImage,CV_BLUR,smoothFactor,smoothFactor);

	// Reduce Bit depth. Use smart reduction (values are renormalized to [0..255]).
	// For disabling smart reduction use multFactor = depthRatio.
	int depthRatio = cvRound(256/pow(2.0,_depth));
	int multFactor = (depthRatio!=256)?255/(256/depthRatio -1):0;
	cvExtImageIterator it(dstImage);
	while (!it){

		*it = cvRound(*it/depthRatio)*multFactor;
		++it;
	}

	// Show the image
	cvShowImage( "Image", dstImage );
}

/****************************************************************************/
// EOF.



