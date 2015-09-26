/****************************************************************************/
/****************************    demFill.cpp    ***************************/
/****************************************************************************/

/****************************************************************************/
// Includes
#include "stdafx.h"
#include "demFfill.h"
#include "sfCVextension.h"

using namespace demFfillNS;

/****************************************************************************/
demFfill::demFfill(const char* path){

	if (path){ 
	   safeStrBufCpy(imagePath,path);
	}
	else {
	   safeStrBufCpy(imagePath,IPE_DEFAULT_IMAGE_FILE);
	}

	// Define the command
	safeStrBufCpy(command,"Change flood types: null/fixed/relative range."
		    "\tc - switch color/gray mode.\tm - switch mask mode."
			"\t    R - restore the original image."
            "\t4 or 8 - connectivity");


	// Initialize pointers to images
	srcImage = NULL;
	gryImage = NULL;
	tmpImage = NULL;
	mskImage = NULL;
	dstImage = NULL;

	// Other variables
	_algType    = 1;     // 0 - Exact values fill, 1 - use [loDiff,upDiff], 
	                     // 2 - Gradient floating relative range
	_loDiff     = 20; 
	_upDiff     = 20;
	connectType = 4;  // 4,8 connectivity
	isColorFlag = 1;
	isMaskFlag  = 0;
	newMaskVal  = 255;
}

/****************************************************************************/
// Destructor
demFfill::~demFfill(){

	cvDestroyWindow(winName1);
	if (isMaskFlag){
       cvDestroyWindow(winName2);
	}

    cvExtReleaseImageCheckNULL(&srcImage);
	cvExtReleaseImageCheckNULL(&gryImage);	
	cvExtReleaseImageCheckNULL(&tmpImage);	
	cvExtReleaseImageCheckNULL(&mskImage);	
    cvExtReleaseImageCheckNULL(&dstImage);
}

/****************************************************************************/
int demFfill::runDemo(void){
	 	
	// Load image
    srcImage = cvLoadImage(imagePath,CV_LOAD_IMAGE_COLOR);
    if(!srcImage){
        fprintf(stderr,"Image was not loaded.\n");
        return -1;
    }

	// Allocate memory
    tmpImage = cvCloneImage(srcImage);
    gryImage = cvCreateImage(cvSize(tmpImage->width, tmpImage->height), 8, 1 );
    cvCvtColor( tmpImage, gryImage, CV_BGR2GRAY );
    dstImage = cvCloneImage(gryImage);
    mskImage = cvCreateImage(cvSize(tmpImage->width + 2, tmpImage->height + 2), 8, 1 );

	// Open windows
    cvNamedWindow(winName1, 1 );
    cvCreateTrackbar( "low Diff", winName1, &_loDiff, 255, NULL );
    cvCreateTrackbar( "up  Diff", winName1, &_upDiff, 255, NULL );
    cvCreateTrackbar( "FloodType ", winName1, &_algType, 2, NULL );
	cvSetTrackbarPos( "FloodType ", winName1, _algType);

    cvSetMouseCallback(winName1, callBackFun, NULL );

	// Show image
	cvShowImage(winName1, isColorFlag ? tmpImage: dstImage );

	char key = 0;
    while((key != -1)&&(key != 27)&&(key != 'q')&&(key != 'Q')){

        switch (key){

		// Change color mode
		case 'c':
            if (isColorFlag){
                // printf("Grayscale mode is set\n");
                cvCvtColor( tmpImage, dstImage, CV_BGR2GRAY );
                isColorFlag = 0;
            }
            else{
                // printf("Color mode is set\n");
                cvCopy(srcImage, tmpImage);
                cvZero(mskImage);
                isColorFlag = 1;
            }
            break;

		// Show/Hide mask image
        case 'm':
            if (isMaskFlag){
                cvDestroyWindow(winName2);
                isMaskFlag = 0;
            }
            else{
                cvZero(mskImage);
                cvNamedWindow(winName2,1);
                cvShowImage(winName2, mskImage );
                isMaskFlag = 1;
            }
            break;

		// Restore original image
        case 'r':
            // printf("Original image is restored\n");
            cvCopy( srcImage, tmpImage);
            cvCopy( gryImage, dstImage);
            cvZero(mskImage);
            if (isMaskFlag){
                cvShowImage(winName2, mskImage );
			}
            break;

        /* 
		case 's':
            printf("Simple floodfill mode is set\n");
            algType = 0;
            break;
        case 'f':
            printf("Fixed Range floodfill mode is set\n");
            algType = 1;
            break;
        case 'g':
            printf("Gradient (floating range) floodfill mode is set\n");
            algType = 2;
            break;*/

        case '4':
            // printf("4-connectType mode is set\n");
            connectType = 4;
            break;

        case '8':
            // printf("8-connectType mode is set\n");
            connectType = 8;
            break;

        } // Switch

		// Show image
		cvShowImage(winName1, isColorFlag ? tmpImage: dstImage );
		key = cvWaitKey(0);

    } // While
	        
	return 0;
}

/****************************************************************************/
// Return textual explanation of the commands
const char* demFfill::getComDesc(void){
	return command;
}

/****************************************************************************/
// Call back

void demFfillNS::callBackFun(int event, int x, int y, int flags, void* ){

    if (!tmpImage)
       return;

    switch(event){

	case CV_EVENT_RBUTTONDOWN:
    case CV_EVENT_LBUTTONDOWN:
         CvPoint seed = cvPoint(x,y);
         int lo = (_algType == 0) ? 0 : _loDiff;
         int up = (_algType == 0) ? 0 : _upDiff;
         int flags = connectType + (newMaskVal << 8) + (_algType == 1 ? CV_FLOODFILL_FIXED_RANGE : 0);

		 // Create random color
         int b = rand() & 255;
		 int g = rand() & 255;
		 int r = rand() & 255;

         CvConnectedComp comp;

         if (isMaskFlag)
            cvThreshold( mskImage, mskImage, 1, 128, CV_THRESH_BINARY );
            
         if (isColorFlag){

            CvScalar color = CV_RGB( r, g, b );
            cvFloodFill( tmpImage, seed, color, 
				         CV_RGB(lo,lo,lo ), CV_RGB(up,up,up), 
						 &comp, flags, isMaskFlag? mskImage : NULL );
            cvShowImage(winName1, tmpImage );
         }
         else {
			CvScalar brightness = cvRealScalar((r*2 + g*7 + b + 5)/10);
            cvFloodFill( dstImage, seed, brightness, 
						 cvRealScalar(lo), cvRealScalar(up), 
						 &comp, flags, isMaskFlag ? mskImage : NULL );
            cvShowImage(winName1, dstImage );
		 }

         //printf("%g pixels were repainted\n", comp.area );

		 if (isMaskFlag){
            cvShowImage(winName2, mskImage);
		 }
	     break;

    } // Switch
}

/****************************************************************************/
// EOF.
