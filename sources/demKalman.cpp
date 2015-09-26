/****************************************************************************/
/*****************************    demKalman.cpp    **************************/
/****************************************************************************/

/****************************************************************************/
// Includes
#include "stdafx.h"
#include "demKalman.h"
#include "sfCVextension.h"

using namespace demKalmanNS;

/****************************************************************************/
demKalman::demKalman(const char* path){

	if (path){ 
	   safeStrBufCpy(imagePath,path);
	}
	else {
	   safeStrBufCpy(imagePath,IPE_DEFAULT_IMAGE_FILE);
	}

	// Define the command
	safeStrBufCpy(command,"Enemy is circling around! Switch display of the radar.\t'M' - measured point,\t'P' - predicted,\t'T' - true enemy location. Press SPACE to relaung enemy");

	// Initialize pointers to images
	srcImage		= NULL;
	kalman			= NULL;
	state			= NULL;
	processNoise	= NULL;
	measurement		= NULL;

	// Other
	_displayType = 0;
}

/****************************************************************************/
// Destructor
demKalman::~demKalman(){

	cvDestroyWindow(winName);

	cvExtReleaseImageCheckNULL(&srcImage);
	cvReleaseKalman(&kalman);
	cvReleaseMat(&state);
	cvReleaseMat(&processNoise);
	cvReleaseMat(&measurement);
}

/****************************************************************************/
int demKalman::runDemo(void){
 
    
	// Target state is (phi, delta_phi)
	const float A[] = { 1, 1, 0, 1 };
	// State 'i' relates to state 'i+1' by matrix 'A'.
	// phi[i+1]       = phi[i] + delta_phi[i];
	// delta_phi[i+1] = delta_phi[i];

    // Allocate memory
	srcImage     = cvCreateImage( cvSize(500,350), IPL_DEPTH_8U, 3 );
    kalman       = cvCreateKalman  ( 2, 1, 0 );         // 2 state parameters, 1 measurment
    state        = cvCreateMat     ( 2, 1, CV_32FC1 );  // State is 2 scalars.
    processNoise = cvCreateMat     ( 2, 1, CV_32FC1 );  // 2 types of noise
    measurement  = cvCreateMat     ( 1, 1, CV_32FC1 );  // Phi
    CvRNG rng    = cvRNG(-1);



	// Open window and define symbols font
    cvNamedWindow( winName, 1 );
	cvCreateTrackbar("Display"  , winName, &_displayType     ,3, NULL);
	CvFont  font		= cvFont(1,2);

	char key = 0;
    while((key != -1)&&(key != 27)&&(key != 'q')&&(key != 'Q')){
	
		// Initialize moving object to random location
        cvRandArr( &rng, state, CV_RAND_NORMAL, cvRealScalar(0), cvRealScalar(0.1) );
        
		// Initialize Kalman at random location
        memcpy(        kalman->transition_matrix->data.fl, A, sizeof(A));
        cvSetIdentity( kalman->measurement_matrix        , cvRealScalar(1) );
        cvSetIdentity( kalman->process_noise_cov         , cvRealScalar(1e-5) );
        cvSetIdentity( kalman->measurement_noise_cov     , cvRealScalar(10e-1) );
        cvSetIdentity( kalman->error_cov_post            , cvRealScalar(1));
        cvRandArr( &rng, kalman->state_post              , CV_RAND_NORMAL, 
			             cvRealScalar(0), cvRealScalar(0.1) );
        
		// At each step predict with Kalman and update it
		key = 0;
        while((key != 27)&&(key != 'q')&&(key != 'Q')&&(key != ' ')){

			// Calculate state point
            float state_angle = state->data.fl[0];
            CvPoint state_pt  = calcPointAroundImageCenter(srcImage,state_angle);
            
			// Calculate predicted point
			const CvMat* prediction = cvKalmanPredict( kalman, 0 );
            CvPoint predict_pt      = calcPointAroundImageCenter(srcImage,prediction->data.fl[0]);

			// Generate measurement noise in 'measurment' variable
            cvRandArr( &rng, measurement, CV_RAND_NORMAL, cvRealScalar(0),
                       cvRealScalar(sqrt(kalman->measurement_noise_cov->data.fl[0])) );
			
			// Generate measurement point = (MeasureMatrix * State + MeasureNoise)
            cvMatMulAdd( kalman->measurement_matrix, state, measurement, measurement );
            CvPoint measurement_pt = calcPointAroundImageCenter(srcImage,measurement->data.fl[0]);

			// Correct Kalman with the measurment
            cvKalmanCorrect( kalman, measurement );


            cvZero( srcImage );

			if (_displayType != 2){
			   // Plot the measured place
		       cvPutText(srcImage, measuredSymbol, measurement_pt, &font,CV_RGB(255,0,0));
			}

			if (_displayType != 0){
			   // Plot the predicted place
		       cvPutText(srcImage, predictionSymbol, predict_pt, &font,CV_RGB(0,255,0));
			}

			if (_displayType > 1){
			   // Plot the true target place
		       cvPutText(srcImage, targetSymbol, state_pt, &font,CV_RGB(127,127,127));
			}

			CvPoint locator = cvPoint(cvRound(srcImage->width/2),cvRound(srcImage->height/2));
			if (_displayType == 0){
			   // Plot line towards the measurment
               cvLine( srcImage, locator, measurement_pt, CV_RGB(255,0,0), 1, CV_AA, 0 );
			}

			// Plot the center of the image 
			drawCrossOnImage( srcImage, locator,CV_RGB(0,255,0),5);

			cvShowImage( winName, srcImage );

			// Advance the state to the next step
            cvRandArr( &rng, processNoise, CV_RAND_NORMAL, cvRealScalar(0),
                       cvRealScalar(sqrt(kalman->process_noise_cov->data.fl[0])));
            cvMatMulAdd( kalman->transition_matrix, state, processNoise, state );

            key = (char) cvWaitKey(100);

			// Check if user closed the window while Kalman loop is running
			if (!cvGetWindowHandle(winName)){
			   // Avoid infinite loop
			   key = 27;   // Simulate 'Esc' button pressed
			}
        }
    }
    
    callBackFun(0);
    //cvWaitKey(0);    
    return 0;
}

/****************************************************************************/
// Return textual explanation of the commands
const char* demKalman::getComDesc(void){
	return command;
}

/****************************************************************************/
// Call back

void demKalmanNS::callBackFun(int arg){
}

/****************************************************************************/
// EOF.



