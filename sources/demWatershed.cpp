/****************************************************************************/
/***************************    demWatershed.cpp    *************************/
/****************************************************************************/

/****************************************************************************/
// Includes
#include "stdafx.h"
#include "demWatershed.h"
#include "sfCVextension.h"

using namespace demWatershedNS;

/****************************************************************************/
demWatershed::demWatershed(const char* path){

	if (path){ 
	   safeStrBufCpy(imagePath,path);
	}
	else {
	   safeStrBufCpy(imagePath,IPE_DEFAULT_IMAGE_FILE);
	}

	// Define the command
	safeStrBufCpy(command,"Roughly mark the areas of objects in the image.\t\"r\" - restore original image,\t   SPACE - run watershed");

	// Initialize pointers to images
	srcImage    = NULL;
	gryImage	= NULL;
	watMask		= NULL;
	markMask	= NULL;
	tmpImage    = NULL;
	dstImage    = NULL;

	_lineWidth = 1;
}

/****************************************************************************/
// Destructor
demWatershed::~demWatershed(){

	cvDestroyWindow(winName1);
	cvDestroyWindow(winName2);

    cvExtReleaseImageCheckNULL(&srcImage);
    cvExtReleaseImageCheckNULL(&gryImage);
    cvExtReleaseImageCheckNULL(&watMask);
    cvExtReleaseImageCheckNULL(&markMask);
    cvExtReleaseImageCheckNULL(&tmpImage);
    cvExtReleaseImageCheckNULL(&dstImage);
}

/****************************************************************************/
int demWatershed::runDemo(void){
	 	
	// Load image
    srcImage = cvLoadImage(imagePath,CV_LOAD_IMAGE_COLOR);
    if(!srcImage){
        fprintf(stderr,"Image was not loaded.\n");
        return -1;
    }

	// Allocate memory
    tmpImage = cvCloneImage( srcImage );
    gryImage = cvCloneImage( srcImage );
    dstImage = cvCloneImage( srcImage );
    watMask  = cvCreateImage( cvGetSize(tmpImage), 8, 1 );               // Stores user painting
    markMask = cvCreateImage( cvGetSize(tmpImage), IPL_DEPTH_32S, 1 );
    cvCvtColor(tmpImage, watMask, CV_BGR2GRAY );
    cvCvtColor(watMask, gryImage, CV_GRAY2BGR );

    cvZero(watMask);
    cvZero(dstImage);

	// Initialize random generator for segments colors
    CvRNG rng = cvRNG(-1);

	// Open windows
    cvNamedWindow(winName1, 1);
    cvShowImage( winName1, tmpImage );
    cvNamedWindow( winName2, 1 );
    cvShowImage( winName2, dstImage );

	// Add track bars and callbacks
	cvCreateTrackbar("Line Width", winName1, &_lineWidth , 5 , NULL);
    cvSetMouseCallback(winName1, callBackFun, 0 );

	char key = 0;
    while((key != -1)&&(key != 27)&&(key != 'q')&&(key != 'Q')){

        // Restart watershed. Clear all previous results
		if (key == 'r'){
           cvZero(watMask);
           cvCopy(srcImage, tmpImage);
           cvShowImage(winName1, tmpImage );
        }

		// Run watershed
        if (key == ' '){

            CvMemStorage* storage = cvCreateMemStorage(0);
            CvSeq* contours = 0;

			// Find contours that user painted while defining areas
			cvFindContours( watMask, storage, &contours, sizeof(CvContour),
                            CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

			// Traverse all the contours and give each a unique color
            cvZero(markMask);
            int nComp = 0;
            for ( ; contours != NULL; contours = contours->h_next, nComp++ ){
                cvDrawContours( markMask, contours, 
					            cvScalarAll(nComp+1), cvScalarAll(nComp+1), 
								-1, -1, 8, cvPoint(0,0) );
            }

			// Segments source image using mark mask as seed 
            cvWatershed( srcImage, markMask );

			// Create color tab for painting each segment with unique color		
			CvMat* color_tab = cvCreateMat( 1, MAX(nComp,1), CV_8UC3 );
            int i;
            for( i = 0; i < nComp; i++ ){
                unsigned char* ptr = color_tab->data.ptr + i*3;
                ptr[0] = (uchar)(cvRandInt(&rng)%180 + 50);
                ptr[1] = (uchar)(cvRandInt(&rng)%180 + 50);
                ptr[2] = (uchar)(cvRandInt(&rng)%180 + 50);
            }

            // Paint the Watershed image
            int j;
            for (i = 0; i < markMask->height; i++ )
            for (j = 0; j < markMask->width ; j++ ){

				// Define destination pixel
                unsigned char* dst = &CV_IMAGE_ELEM( dstImage, uchar, i, j*3 );

				// Define source pixel color index
				int idx = CV_IMAGE_ELEM( markMask, int, i, j );
                
				if (idx == -1)
                   dst[0] = dst[1] = dst[2] = (uchar)255;
                else if( idx <= 0 || idx > nComp )
                   dst[0] = dst[1] = dst[2] = (uchar)0;
                else{
                   unsigned char* ptr = color_tab->data.ptr + (idx-1)*3;
                   dst[0] = ptr[0]; dst[1] = ptr[1]; dst[2] = ptr[2];
                }
			}

            cvAddWeighted( dstImage, 0.6, gryImage, 0.4, 0, dstImage );
            cvShowImage(winName2, dstImage );
            cvReleaseMemStorage(&storage);
            cvReleaseMat(&color_tab);
        }
        key = cvWaitKey(0);
    }

    return 0;
}

/****************************************************************************/
// Return textual explanation of the commands
const char* demWatershed::getComDesc(void){
	return command;
}

/****************************************************************************/
// Call back

void demWatershedNS::callBackFun(int event, int x, int y, int flags, void* ){

	// Draw lines where user moves its mouse
    if (event == CV_EVENT_LBUTTONUP||!(flags & CV_EVENT_FLAG_LBUTTON))
       prev_pt = cvPoint(-1,-1);
    else if(event == CV_EVENT_LBUTTONDOWN)
       prev_pt = cvPoint(x,y);
    else if(event == CV_EVENT_MOUSEMOVE&&(flags & CV_EVENT_FLAG_LBUTTON)){
        CvPoint pt = cvPoint(x,y);
        if( prev_pt.x < 0 )
            prev_pt = pt;
        cvLine( watMask, prev_pt, pt, cvScalarAll(255),  _lineWidth, 8, 0 );
        cvLine( tmpImage, prev_pt, pt, cvScalarAll(255), _lineWidth, 8, 0 );
        prev_pt = pt;
        cvShowImage(winName1, tmpImage );
    }
}

/****************************************************************************/
// EOF.



