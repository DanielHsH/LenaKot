/****************************************************************************/
/****************************    demContour.cpp    **************************/
/****************************************************************************/

/****************************************************************************/
// Includes
#include "stdafx.h"
#include "demContour.h"
#include "sfCVextension.h"

using namespace demContourNS;

/****************************************************************************/
demContour::demContour(const char* path){

	if (path){ 
	   safeStrBufCpy(imagePath,path);
	}
	else {
	   safeStrBufCpy(imagePath,IPE_DEFAULT_IMAGE_FILE);
	}

	// Define the command
	safeStrBufCpy(command,"Detect contours.\tMove the slider to change the depth of the detected contour, i.e: Object, hole in object, hole in hole, etc.");

	// Initialize pointers to images
	srcImage = NULL;
	tmpImage = NULL;
	dstImage = NULL;
	storage  = NULL;
	contours = NULL;

	// Other
	_levels = 4;
}

/****************************************************************************/
// Destructor
demContour::~demContour(){

	cvDestroyWindow(winName);

    cvExtReleaseImageCheckNULL(&srcImage);
    cvExtReleaseImageCheckNULL(&tmpImage);
    cvExtReleaseImageCheckNULL(&dstImage);

    cvReleaseMemStorage( &storage );
}

/****************************************************************************/
int demContour::runDemo(void){
	 	
	// Allocate memory
    storage = cvCreateMemStorage(0);
    srcImage = cvCreateImage( cvSize(320,320), 8, 1 );
    dstImage = cvCreateImage( cvGetSize(srcImage), 8, 1 );

	// Clear src Image and paint faces on it
    cvZero(srcImage);
    for ( int i=0,j; i < 4; i++){
        int dx = (i%2)*250 - 30;
        int dy = (i/2)*150;
        CvScalar white = cvRealScalar(127);
        CvScalar black = cvRealScalar(0);

		// Draw hair of the first face
        if( i == 0 ){
            for( j = 0; j <= 10; j++ ){
                double angle = (j+5)*CV_PI/21;
                cvLine(srcImage, cvPoint(cvRound(dx+100+j*10-80*cos(angle)),
                    cvRound(dy+100-90*sin(angle))),
                    cvPoint(cvRound(dx+100+j*10-30*cos(angle)),
                    cvRound(dy+100-30*sin(angle))), white, 1, 8, 0);
            }
        }

		// Draw another face
        cvEllipse( srcImage, cvPoint(dx+150, dy+100), cvSize(100,70), 0, 0, 360, white, -1, 8, 0 );
        cvEllipse( srcImage, cvPoint(dx+115, dy+70), cvSize(30,20), 0, 0, 360, black, -1, 8, 0 );
        cvEllipse( srcImage, cvPoint(dx+185, dy+70), cvSize(30,20), 0, 0, 360, black, -1, 8, 0 );
        cvEllipse( srcImage, cvPoint(dx+115, dy+70), cvSize(15,15), 0, 0, 360, white, -1, 8, 0 );
        cvEllipse( srcImage, cvPoint(dx+185, dy+70), cvSize(15,15), 0, 0, 360, white, -1, 8, 0 );
        cvEllipse( srcImage, cvPoint(dx+115, dy+70), cvSize(5,5), 0, 0, 360, black, -1, 8, 0 );
        cvEllipse( srcImage, cvPoint(dx+185, dy+70), cvSize(5,5), 0, 0, 360, black, -1, 8, 0 );
        cvEllipse( srcImage, cvPoint(dx+150, dy+100), cvSize(10,5), 0, 0, 360, black, -1, 8, 0 );
        cvEllipse( srcImage, cvPoint(dx+150, dy+150), cvSize(40,10), 0, 0, 360, black, -1, 8, 0 );
        cvEllipse( srcImage, cvPoint(dx+27, dy+100), cvSize(20,35), 0, 0, 360, white, -1, 8, 0 );
        cvEllipse( srcImage, cvPoint(dx+273, dy+100), cvSize(20,35), 0, 0, 360, white, -1, 8, 0 );
    }

	// Open windows
    cvNamedWindow( winName, 1 );
    cvShowImage( winName, srcImage );

	// Add track bars.
    cvCreateTrackbar( "levels+3", winName, &_levels, 8, callBackFun );

	// Allocate memory
    tmpImage = cvCloneImage(srcImage);
	cvFindContours( tmpImage, storage, &contours, sizeof(CvContour),
                    CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0) );

    // comment this out if you do not want approximation
    contours = cvApproxPoly( contours, sizeof(CvContour), storage, CV_POLY_APPROX_DP, 3, 1 );


    callBackFun(0);
    cvWaitKey(0);    
    return 0;
}

/****************************************************************************/
// Return textual explanation of the commands
const char* demContour::getComDesc(void){
	return command;
}

/****************************************************************************/
// Call back

void demContourNS::callBackFun(int arg){

    int levels = _levels - 4;

	// If trackbar is in the middle delete all contours
	if (!levels){
		// Draw source image without contours
        cvShowImage( winName, srcImage );
		return;
	}

	// For inner counturs we need +1 level since they are all in a single face
	CvSeq* _contours = contours;
	if ( levels < 0 ){ 
	   levels += 1;
	   _contours = _contours->h_next->h_next->h_next; // Get to the face
	}

    cvZero(dstImage);
    cvDrawContours( dstImage, _contours, cvScalarAll(100), cvScalarAll(250), levels, 3, CV_AA, cvPoint(0,0) );
	cvAdd(dstImage,srcImage,dstImage);
    cvShowImage( winName, dstImage );
}

/****************************************************************************/
// EOF.



