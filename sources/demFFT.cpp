/****************************************************************************/
/******************************    demFFT.cpp    ****************************/
/****************************************************************************/

/****************************************************************************/
// Includes
#include "stdafx.h"
#include "demFFT.h"
#include "sfCVextension.h"

using namespace demFFTNS;

/****************************************************************************/
demFFT::demFFT(const char* path){

	if (path){ 
	   safeStrBufCpy(imagePath,path);
	}
	else {
	   safeStrBufCpy(imagePath,IPE_DEFAULT_IMAGE_FILE);
	}

	// Define the command
	safeStrBufCpy(command,"Manipulate the image and watch how Furier transform changes.\n\tAchieve better visibility by changing log scale value");

	// Initialize pointers to images
	srcImage = NULL;
	tmpImage = NULL;
	noiImage = NULL;
	realInput = NULL;
	imaginaryInput = NULL;
	complexInput = NULL;
	image_Re = NULL;
	image_Im = NULL;
	dft_A = NULL;

	// Other
	_smooth   = 0;
	_rnNoise  = 0;
	_logScale = 3;
	rng = cvRNG(-1);
}

/****************************************************************************/
// Destructor
demFFT::~demFFT(){

	cvDestroyWindow(winName1);
	cvDestroyWindow(winName2);

    cvExtReleaseImageCheckNULL(&srcImage);
    cvExtReleaseImageCheckNULL(&tmpImage);
    cvExtReleaseImageCheckNULL(&noiImage);
    cvExtReleaseImageCheckNULL(&realInput);
    cvExtReleaseImageCheckNULL(&imaginaryInput);
    cvExtReleaseImageCheckNULL(&complexInput);
    cvExtReleaseImageCheckNULL(&image_Re);
    cvExtReleaseImageCheckNULL(&image_Im);

	cvReleaseMat(&dft_A);
}

/****************************************************************************/
int demFFT::runDemo(void){
	 	
	// Load image
    srcImage = cvLoadImage(imagePath,CV_LOAD_IMAGE_GRAYSCALE);
    if(!srcImage){
        fprintf(stderr,"Image was not loaded.\n");
        return -1;
    }

	// Allocate memory
	realInput      = cvCreateImage( cvGetSize(srcImage), IPL_DEPTH_64F, 1);
    imaginaryInput = cvCreateImage( cvGetSize(srcImage), IPL_DEPTH_64F, 1);
    complexInput   = cvCreateImage( cvGetSize(srcImage), IPL_DEPTH_64F, 2);
    tmpImage       = cvCloneImage(srcImage);
	noiImage       = cvCreateImage( cvGetSize(srcImage), IPL_DEPTH_32F, 1 );

	// Open windows
    cvNamedWindow(winName1, 1);
    cvNamedWindow(winName2, 1);
    cvShowImage(winName1, srcImage);

	// Add track bars.
	cvCreateTrackbar("Smooth", winName1, &_smooth   , 20, callBackFun);
	cvCreateTrackbar("Noise",  winName1, &_rnNoise  , 20, callBackFun);
	cvCreateTrackbar("Log Scale",  winName1, &_logScale , 5, callBackFun);


	// -------------------------------------------------
	// Start FFT

	// Calculate the best matrix size for DFT.
    int dft_M = cvGetOptimalDFTSize(srcImage->height - 1);
    int dft_N = cvGetOptimalDFTSize(srcImage->width  - 1);

    // Allocate memory according to dft best values
	dft_A    = cvCreateMat( dft_M, dft_N, CV_64FC2 );
    image_Re = cvCreateImage( cvSize(dft_N, dft_M), IPL_DEPTH_64F, 1);
    image_Im = cvCreateImage( cvSize(dft_N, dft_M), IPL_DEPTH_64F, 1);

    callBackFun(0);
    cvWaitKey(0);    
    return 0;
}

/****************************************************************************/
// Return textual explanation of the commands
const char* demFFT::getComDesc(void){
	return command;
}

/****************************************************************************/
// Call back

void demFFTNS::callBackFun(int arg){

	// Add noise (Not part of FFT)
	cvRandArr( &rng, noiImage, CV_RAND_NORMAL, cvScalarAll(0), cvScalarAll(4*_rnNoise));
	cvAcc(srcImage, noiImage);
	cvConvert(noiImage, tmpImage);

	// Smooth the original image (Not part of FFT)
	cvSmooth(tmpImage,tmpImage,CV_BLUR,((_smooth)*2+1),((_smooth)*2+1));

    cvShowImage(winName1, tmpImage);

	// Convert real image to complex
    cvScale(tmpImage, realInput, 1.0, 0.0);
    cvZero(imaginaryInput);
    cvMerge(realInput, imaginaryInput, NULL, NULL, complexInput);

    // Copy A to dft_A and pad dft_A with zeros
	CvMat tmp;
    cvGetSubRect( dft_A, &tmp, cvRect(0,0, srcImage->width, srcImage->height));
    cvCopy( complexInput, &tmp, NULL );
    if( dft_A->cols > srcImage->width ){
        cvGetSubRect( dft_A, &tmp, cvRect(srcImage->width,0, 
			          dft_A->cols - srcImage->width, srcImage->height));
        cvZero( &tmp );
    }
    // No need to pad bottom part of dft_A with zeros because of use nonzero_rows 
	// parameter in cvDFT() call below

    cvDFT( dft_A, dft_A, CV_DXT_FORWARD, complexInput->height );


    // Split Fourier in real and imaginary parts
    cvSplit( dft_A, image_Re, image_Im, 0, 0 );

    // Compute the magnitude of the spectrum Mag = sqrt(Re^2 + Im^2)
    cvPow( image_Re, image_Re, 2.0);
    cvPow( image_Im, image_Im, 2.0);
    cvAdd( image_Re, image_Im, image_Re, NULL);
    cvPow( image_Re, image_Re, 0.5 );

    // Compute log(1 + Mag). Use _logScale instead of 1 for getting well spread values
	// After normalization	
	
    cvAddS( image_Re, cvScalarAll(pow((double)10,(int)_logScale)), image_Re, NULL ); // 1 + Mag
    cvLog( image_Re, image_Re ); // log(1 + Mag)


    // Rearrange the quadrants of Fourier image so that the origin is at
    // the image center
    cvShiftDFT( image_Re, image_Re );

	// Normalize the results and show
	double m, M;
    cvMinMaxLoc(image_Re, &m, &M, NULL, NULL, NULL);
    cvScale(image_Re, image_Re, 1.0/(M-m), 1.0*(-m)/(M-m));
    cvShowImage(winName2, image_Re);
}

/****************************************************************************/

// Rearrange the quadrants of Fourier image so that the origin is at
// the image center
// src & dst arrays of equal size & type
void demFFTNS::cvShiftDFT(CvArr * srcArr, CvArr * dstArr ){
    CvMat * tmp;
    CvMat q1stub, q2stub;
    CvMat q3stub, q4stub;
    CvMat d1stub, d2stub;
    CvMat d3stub, d4stub;
    CvMat * q1, * q2, * q3, * q4;
    CvMat * d1, * d2, * d3, * d4;

    CvSize size     = cvGetSize(srcArr);
    CvSize dst_size = cvGetSize(dstArr);
    int cx, cy;

    if(dst_size.width != size.width || 
       dst_size.height != size.height){
        cvError( CV_StsUnmatchedSizes, "cvShiftDFT", "Source and Destination arrays must have equal sizes", __FILE__, __LINE__ );   
    }

    if(srcArr==dstArr){
        tmp = cvCreateMat(size.height/2, size.width/2, cvGetElemType(srcArr));
    }
    
    cx = size.width/2;
    cy = size.height/2; // image center

    q1 = cvGetSubRect( srcArr, &q1stub, cvRect(0,0,cx, cy) );
    q2 = cvGetSubRect( srcArr, &q2stub, cvRect(cx,0,cx,cy) );
    q3 = cvGetSubRect( srcArr, &q3stub, cvRect(cx,cy,cx,cy) );
    q4 = cvGetSubRect( srcArr, &q4stub, cvRect(0,cy,cx,cy) );
    d1 = cvGetSubRect( srcArr, &d1stub, cvRect(0,0,cx,cy) );
    d2 = cvGetSubRect( srcArr, &d2stub, cvRect(cx,0,cx,cy) );
    d3 = cvGetSubRect( srcArr, &d3stub, cvRect(cx,cy,cx,cy) );
    d4 = cvGetSubRect( srcArr, &d4stub, cvRect(0,cy,cx,cy) );

    if(srcArr!=dstArr){
        if( !CV_ARE_TYPES_EQ( q1, d1 )){
            cvError( CV_StsUnmatchedFormats, "cvShiftDFT", "Source and Destination arrays must have the same format", __FILE__, __LINE__ ); 
        }
        cvCopy(q3, d1, 0);
        cvCopy(q4, d2, 0);
        cvCopy(q1, d3, 0);
        cvCopy(q2, d4, 0);
    }
    else{
        cvCopy(q3, tmp, 0);
        cvCopy(q1, q3, 0);
        cvCopy(tmp, q1, 0);
        cvCopy(q4, tmp, 0);
        cvCopy(q2, q4, 0);
        cvCopy(tmp, q2, 0);
    }

	cvReleaseMat(&tmp);
}

/****************************************************************************/
// EOF.



