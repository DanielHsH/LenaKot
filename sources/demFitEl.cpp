/****************************************************************************/
/*****************************    demFitEl.cpp    ***************************/
/****************************************************************************/

/****************************************************************************/
// Includes
#include "stdafx.h"
#include "demFitEl.h"
#include "sfCVextension.h"

using namespace demFitElNS;

/****************************************************************************/
demFitEl::demFitEl(const char* path){

	if (path){ 
	   safeStrBufCpy(imagePath,path);
	}
	else {
	   safeStrBufCpy(imagePath,IPE_DEFAULT_IMAGE_FILE);
	}

	// Define the command
	safeStrBufCpy(command,"Move the treshold bar and fit ellipse to detected contours");

	// Initialize pointers to images
	srcImage = NULL;
	gryImage = NULL;
	tmpImage = NULL;
	dstImage = NULL;

	storage			= cvCreateMemStorage(0);
	cont			= cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint) , storage);
	box				= NULL;
	PointArray		= NULL;
	PointArray2D32f	= NULL;

	// Other
	_thres = 255;
}

/****************************************************************************/
// Destructor
demFitEl::~demFitEl(){

	cvDestroyWindow(winName1);
	//cvDestroyWindow(winName2);

    cvExtReleaseImageCheckNULL(&srcImage);
	cvExtReleaseImageCheckNULL(&gryImage);   
	cvExtReleaseImageCheckNULL(&tmpImage);
    cvExtReleaseImageCheckNULL(&dstImage);

    cvReleaseMemStorage( &storage );
}

/****************************************************************************/
int demFitEl::runDemo(void){
	 	
	// Load image
    srcImage = cvLoadImage(imagePath,CV_LOAD_IMAGE_COLOR);
    if(!srcImage){
        fprintf(stderr,"Image was not loaded.\n");
        return -1;
    }

	// Allocate memory
	dstImage	   = cvCloneImage(srcImage);
	tmpImage	   = cvCreateImage( cvGetSize(srcImage), IPL_DEPTH_8U, 1);
	gryImage	   = cvCreateImage( cvGetSize(srcImage), IPL_DEPTH_8U, 1);
	cvCvtColor(srcImage, gryImage, CV_BGR2GRAY);    

	// Open windows
    cvNamedWindow(winName1, 1);
    cvShowImage(winName1, srcImage);
    //cvNamedWindow(winName2, 1);

	// Add track bars.
	cvCreateTrackbar("Thres", winName1, &_thres, 255, callBackFun);

	// Execute demo
    callBackFun(0);
    cvWaitKey(0);    
    return 0;
}

/****************************************************************************/
// Return textual explanation of the commands
const char* demFitEl::getComDesc(void){
	return command;
}

/****************************************************************************/
// Call back

void demFitElNS::callBackFun(int arg){

    // Threshold the source imageand find contours.
    cvThreshold( gryImage, tmpImage, _thres, 255, CV_THRESH_BINARY );
    cvFindContours( tmpImage, storage, &cont, sizeof(CvContour), 
                    CV_RETR_LIST, CV_CHAIN_APPROX_NONE, cvPoint(0,0));
    
    // Clear dstImage
	cvCopy( srcImage, dstImage);
    
    // Iterate over all contours. Draw them + approximate by ellipses.
    for ( ; cont; cont = cont->h_next){   

        int nPoints = cont->total;   // Number point in contour
                
		if (nPoints < 6){
		   // There must be at least 6 points for cvFitEllipse_32f() to work.        
            continue;
		}
        
        // Alloc memory for contour point set and Elipse box.    
        PointArray      = (CvPoint*)     malloc(nPoints*sizeof(CvPoint)      );
        PointArray2D32f = (CvPoint2D32f*)malloc(nPoints*sizeof(CvPoint2D32f) );
        box             = (CvBox2D32f*)  malloc(sizeof(CvBox2D32f));
        
        // -----------------------------------
		// Fits ellipse to current contour.

        // Convert contour point set to CvBox2D32f set.
        cvCvtSeqToArray(cont, PointArray, CV_WHOLE_SEQ);
        for (int i=0; i<nPoints; i++){
            PointArray2D32f[i].x = (float)PointArray[i].x;
            PointArray2D32f[i].y = (float)PointArray[i].y;
        }       
        cvFitEllipse(PointArray2D32f, nPoints, box);
        
        // Convert ellipse data from float to integer representation.
        CvPoint center;
        center.x    = cvRound(box->center.x);
        center.y    = cvRound(box->center.y);

        CvSize size;
        size.width  = cvRound(box->size.width*0.5);
        size.height = cvRound(box->size.height*0.5);
		
		if ((size.height < 3)||(size.width < 3)){	   
		   // Do not draw Elipses with small size
		}
		else {
			// Draw current contour.
			cvDrawContours(dstImage,cont,CV_RGB(255,0,0),CV_RGB(255,0,0),0,2,8,cvPoint(0,0));        
      
			// Draw ellipse.
			cvEllipse(dstImage, center, size, -box->angle, 0, 360, cvScalarAll(255), 1, CV_AA, 0);
		}
        
        // Free memory.          
        free(PointArray);
        free(PointArray2D32f);
        free(box);
    }
	
	cvClearMemStorage(storage);
    cvShowImage(winName1, dstImage );
}

/****************************************************************************/
// EOF.



