/****************************************************************************/
/***************************    demInpaint.cpp    ***************************/
/****************************************************************************/

/****************************************************************************/
// Includes
#include "stdafx.h"
#include "demInpaint.h"
#include "sfCVextension.h"

using namespace demInpaintNS;

/****************************************************************************/
demInpaint::demInpaint(const char* path){

	if (path){ 
	   safeStrBufCpy(imagePath,path);
	}
	else {
	   safeStrBufCpy(imagePath,IPE_DEFAULT_IMAGE_FILE);
	}

	// Define the command
	safeStrBufCpy(command,"Paint on the image with the mouse, then run impaint.\t\"r\" - restore original image,\t   SPACE - run inpaint");

	// Initialize pointers to images
	srcImage    = NULL;
	inpaintMask = NULL;
	tmpImage    = NULL;
	dstImage    = NULL;

	_lineWidth = 1;
}

/****************************************************************************/
// Destructor
demInpaint::~demInpaint(){

	cvDestroyWindow("Original");
	cvDestroyWindow("Inpainted");

    cvExtReleaseImageCheckNULL(&srcImage);
    cvExtReleaseImageCheckNULL(&tmpImage);
    cvExtReleaseImageCheckNULL(&inpaintMask);
    cvExtReleaseImageCheckNULL(&dstImage);
}

/****************************************************************************/
int demInpaint::runDemo(void){
	 	
	// Load image
    srcImage = cvLoadImage(imagePath,CV_LOAD_IMAGE_COLOR);
    if(!srcImage){
        fprintf(stderr,"Image was not loaded.\n");
        return -1;
    }

	// Allocate memory
    tmpImage    = cvCloneImage(srcImage);
    dstImage    = cvCloneImage(srcImage);
	inpaintMask = cvCreateImage( cvGetSize(tmpImage), 8, 1 );
    cvZero(inpaintMask);
    cvZero(dstImage);

	// Open windows
    cvNamedWindow("Original", 1);
    cvShowImage( "Original", tmpImage );
		// Add track bars.
	cvCreateTrackbar("Line Width", "Original", &_lineWidth , 5 , NULL);

    cvSetMouseCallback( "Original", callBackFun, 0 );

	char key = 0;
    while((key != -1)&&(key != 27)&&(key != 'q')&&(key != 'Q')){

        // Restart inpaint. Clear all previous results
		if (key == 'r'){
           cvZero(inpaintMask);
           cvCopy(srcImage, tmpImage);
           cvShowImage( "Original", tmpImage );
        }

		// Run inpaint
        if (key == ' '){
           cvNamedWindow( "Inpainted", 1 );
           cvInpaint( tmpImage, inpaintMask, dstImage, 3, CV_INPAINT_TELEA );
           cvShowImage( "Inpainted", dstImage );
        }
        key = cvWaitKey(0);
    }

    return 0;
}

/****************************************************************************/
// Return textual explanation of the commands
const char* demInpaint::getComDesc(void){
	return command;
}

/****************************************************************************/
// Call back

void demInpaintNS::callBackFun(int event, int x, int y, int flags, void* ){

	// Draw lines where user moves its mouse
    if (event == CV_EVENT_LBUTTONUP||!(flags & CV_EVENT_FLAG_LBUTTON))
       prev_pt = cvPoint(-1,-1);
    else if(event == CV_EVENT_LBUTTONDOWN)
       prev_pt = cvPoint(x,y);
    else if(event == CV_EVENT_MOUSEMOVE&&(flags & CV_EVENT_FLAG_LBUTTON)){
        CvPoint pt = cvPoint(x,y);
        if( prev_pt.x < 0 )
            prev_pt = pt;
        cvLine( inpaintMask, prev_pt, pt, cvScalarAll(255), _lineWidth, 8, 0 );
        cvLine( tmpImage,    prev_pt, pt, cvScalarAll(255), _lineWidth, 8, 0 );
        prev_pt = pt;
        cvShowImage( "Original", tmpImage );
    }
}

/****************************************************************************/
// EOF.



