/****************************************************************************/
/*****************************    demHist.cpp    ****************************/
/****************************************************************************/

/****************************************************************************/
// Includes
#include "stdafx.h"
#include "demHist.h"
#include "sfCVextension.h"

using namespace demHistNS;

/****************************************************************************/
demHist::demHist(const char* path){

	if (path){ 
	   safeStrBufCpy(imagePath,path);
	}
	else {
	   safeStrBufCpy(imagePath,IPE_DEFAULT_IMAGE_FILE);
	}

	// Define the command
	safeStrBufCpy(command,"Change image characteristics.\tBrightness and contrast ranges are [-100%..100%],\tGamma range is [0..2]");

	// Initialize pointers to images
	srcImage = NULL;
	dstImage = NULL;
	hisImage = NULL;
	lutImage = NULL;

	// Other
	_brightness = SIGN_PER_TRACK_BAR_RANGE/2;
	_contrast   = SIGN_PER_TRACK_BAR_RANGE/2;
	_gamma		= SIGN_PER_TRACK_BAR_RANGE/2;
	histSize    = 64;
	range_0[0]  = 0;
	range_0[1]  = 256;
	ranges      = range_0;
	hist        = NULL;
	lut_mat	    = cvMat(1, 256, CV_8U, lut);
}

/****************************************************************************/
// Destructor
demHist::~demHist(){

	cvDestroyWindow("Image");
	cvDestroyWindow("Histogram");
	cvDestroyWindow("Transformation LUT");

    cvExtReleaseImageCheckNULL(&srcImage);
    cvExtReleaseImageCheckNULL(&dstImage);
	cvExtReleaseImageCheckNULL(&hisImage);
	cvExtReleaseImageCheckNULL(&lutImage);

	if (hist) cvReleaseHist(&hist);
}

/****************************************************************************/
int demHist::runDemo(void){
	 	
	// Load image
    srcImage = cvLoadImage(imagePath,CV_LOAD_IMAGE_GRAYSCALE);
    if(!srcImage){
        fprintf(stderr,"Image was not loaded.\n");
        return -1;
    }

	// Allocate memory
    dstImage = cvCloneImage (srcImage);
    hisImage = cvCreateImage(cvSize(256,256), 8, 1);
    lutImage = cvCreateImage(cvSize(256,256), 8, 1);
    hist     = cvCreateHist(1, &histSize, CV_HIST_ARRAY, &ranges, 1);

	// Open windows
    cvNamedWindow("Image", 1);
    cvShowImage( "Image", dstImage );
    cvNamedWindow("Histogram", 1);
	cvNamedWindow("Transformation LUT",1);

	// Add track bars
	cvCreateTrackbar("Brightness", "Image", &_brightness, SIGN_PER_TRACK_BAR_RANGE, callBackFun);
    cvCreateTrackbar("Contrast",   "Image", &_contrast  , SIGN_PER_TRACK_BAR_RANGE, callBackFun);
	cvCreateTrackbar("Gamma",      "Image", &_gamma,      SIGN_PER_TRACK_BAR_RANGE, callBackFun);
    callBackFun(0);
    cvWaitKey(0);    
    return 0;
}

/****************************************************************************/
// Return textual explanation of the commands
const char* demHist::getComDesc(void){
	return command;
}

/****************************************************************************/
// Call back

void demHistNS::callBackFun(int arg){

	int brightness  = _brightness - SIGN_PER_TRACK_BAR_RANGE/2;
    int contrast    = _contrast   - SIGN_PER_TRACK_BAR_RANGE/2;   
    double gamma    = _gamma/100.;   
	
	// Calculate the linear transformation
	double delta  = (contrast > 0)? 127.*contrast/(SIGN_PER_TRACK_BAR_RANGE/2) : 
		                           -128.*contrast/(SIGN_PER_TRACK_BAR_RANGE/2);

	double amp    = (contrast > 0)? 255./(255. - delta*2)    : 
		                           (256.-delta*2)/255.;

	double bias   = (contrast > 0)? amp*(brightness - delta) : 
		                            amp*brightness + delta;

	// Turn transformation to lut
    for ( int i = 0; i < 256; i++ ){
        int v = cvRound(amp*i + bias);
 	    verifyCharRange(v);
		v = cvRound(pow(v/255.,gamma)*255);
        lut[i] = (uchar)v;
    }

	// Trasform the image and calculate histogram
    cvLUT( srcImage, dstImage, &lut_mat);
    cvShowImage( "Image", dstImage );
    cvCalcHist( &dstImage, hist, 0, NULL );

	// Normalize histogram by its max value to fit image.
	float max_value = 0;
	cvGetMinMaxHistValue( hist, 0, &max_value, 0, 0 );
    cvScale( hist->bins, hist->bins, ((double)hisImage->height)/max_value, 0 );


    // Transform histogram to image
	cvSet( hisImage, cvScalarAll(255), 0 );
    int binW = cvRound((double)hisImage->width/histSize);

    for ( int i = 0; i < histSize; i++ )
        cvRectangle( hisImage, 
		             cvPoint(i*binW     , hisImage->height),
                     cvPoint((i+1)*binW , hisImage->height - cvRound(cvGetReal1D(hist->bins,i))),
                     cvScalarAll(0), -1, 8, 0 );
   
    cvShowImage( "Histogram", hisImage );

	// Display the look up table
	cvSet( lutImage, cvScalarAll(255), 0 );
    for ( int i = 0; i < 256; i++ ){
		cvRectangle(lutImage, cvPoint(i, hisImage->height -1 - lut[i]), 
			                  cvPoint(i, hisImage->height -1 - lut[i]), 
							  cvScalarAll(0), -1, 8, 0 );
    }
    cvShowImage( "Transformation LUT", lutImage );
}

/****************************************************************************/
// EOF.



