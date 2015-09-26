/****************************************************************************/
/***************************    demDenoise.cpp    ***************************/
/****************************************************************************/

/****************************************************************************/
// Includes
#include "stdafx.h"
#include "demDenoise.h"
#include "sfCVextension.h"

using namespace demDenoiseNS;

/****************************************************************************/
demDenoise::demDenoise(const char* path){

	if (path){ 
	   safeStrBufCpy(imagePath,path);
	}
	else {
	   safeStrBufCpy(imagePath,IPE_DEFAULT_IMAGE_FILE);
	}

	// Define the command
	safeStrBufCpy(command,"Add noise and then try to filter it using median and gaussian filters.");

	// Initialize pointers to images
	srcImage = NULL;
	yImage   = NULL;
	noiImage = NULL;
	tstImage = NULL;
	dstImage = NULL;

	// Other
	_rnNoise      = 0;
	_prev_rnNoise = -1;
    _spNoise      = 0;
	_prev_spNoise = -1;
    _avgDeNoise   = 0;
    _medDeNoise   = 0;

	rng = cvRNG(-1);
}

/****************************************************************************/
// Destructor
demDenoise::~demDenoise(){

	cvDestroyWindow("Original");

    cvExtReleaseImageCheckNULL(&srcImage);
    cvExtReleaseImageCheckNULL(&yImage);
    cvExtReleaseImageCheckNULL(&noiImage);
    cvExtReleaseImageCheckNULL(&tstImage);	
    cvExtReleaseImageCheckNULL(&dstImage);
}

/****************************************************************************/
int demDenoise::runDemo(void){
	 	
	// Load image
    srcImage = cvLoadImage(imagePath,CV_LOAD_IMAGE_COLOR);
    if(!srcImage){
        fprintf(stderr,"Image was not loaded.\n");
        return -1;
    }

	// Allocate memory
    yImage   = cvCreateImage( cvGetSize(srcImage), IPL_DEPTH_8U,  1 );
	noiImage = cvCreateImage( cvGetSize(srcImage), IPL_DEPTH_32F, 1 );
	tstImage = cvCreateImage( cvGetSize(srcImage), IPL_DEPTH_8U,  1 );
    dstImage = cvCloneImage(srcImage);

	// Open windows
	cvNamedWindow("Original", 1);
	cvShowImage( "Original", srcImage );
	
	// Add track bars.
	cvCreateTrackbar("Rand Noise", "Original", &_rnNoise   , 20 , callBackFun);
	cvCreateTrackbar("Salt/Pep %", "Original", &_spNoise   , 100, callBackFun);
	cvCreateTrackbar("Gaus Mask ", "Original", &_avgDeNoise, 5  , callBackFun);
	cvCreateTrackbar("Med  Mask ", "Original", &_medDeNoise, 5  , callBackFun);
	cvCvtColor( srcImage, srcImage, CV_BGR2YCrCb );

	callBackFun(0);
    cvWaitKey(0);    
    return 0;
}

/****************************************************************************/
// Return textual explanation of the commands
const char* demDenoise::getComDesc(void){
	return command;
}

/****************************************************************************/
// Call back

void demDenoiseNS::callBackFun(int arg){

	bool genNoise = false;

	// If Noise parameters change, add noise
	if ((_prev_rnNoise-_rnNoise)||(_prev_spNoise-_spNoise)){
       _prev_rnNoise = _rnNoise;
	   _prev_spNoise = _spNoise;
	   genNoise = true;
	}

	if (genNoise){
        cvSplit( srcImage, yImage, 0, 0, 0 );

	    // Add random noise
		cvRandArr( &rng, noiImage, CV_RAND_NORMAL, cvScalarAll(0), cvScalarAll(4*_rnNoise));
		cvAcc( yImage, noiImage );
		cvConvert( noiImage, yImage );

		// Add Salt & Pepper noise to yImage
		cvExtImageIterator src(yImage);
		int x;
		while (!src){
			x = rand()%100;
			if (x < _spNoise){
			   *src = 255*(x%2);
			}
			++src;
		}
	}


	// Denoise using median filter and blurr filter
	cvCopy(yImage,tstImage);
	for (int i=0; i<_medDeNoise;i++){
	    cvSmooth( tstImage, tstImage, CV_MEDIAN,  3, 0, 0, 0 );
	}
	cvSmooth( tstImage, tstImage, CV_GAUSSIAN,(2*_avgDeNoise)+1, (2*_avgDeNoise)+1, 0, 0 );

	// Incorporate noise into image
	cvCopy(srcImage,dstImage);
    cvMerge(tstImage, 0, 0, 0, dstImage );
    cvCvtColor(dstImage, dstImage, CV_YCrCb2BGR );
	cvShowImage( "Original", dstImage );
}

/****************************************************************************/
// EOF.



